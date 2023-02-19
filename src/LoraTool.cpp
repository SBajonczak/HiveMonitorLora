#include <SPI.h> // include libraries
#include <LoRa.h>
#include <LoraTool.h>
#include <RH_RF95.h>

const long frequency = RFM_FREQUENCY;
RH_RF95 rf95(GPIO_LORA_SC, GPIO_LORA_IRQ);

/*




const int csPin =           GPIO_LORA_SC;          // LoRa radio chip select
const int resetPin =        GPIO_LORA_RST;        // LoRa radio reset
const int irqPin =          GPIO_LORA_IRQ;
*/
LoraTool::LoraTool(int CS, int RST, int IRQ)
{
    this->lastMessage = nullptr;
    this->_cs = CS;
    this->_rst = RST;
    this->_irq = IRQ;
}
LoraTool::~LoraTool() {}

void LoraTool::Setup()
{
    pinMode(this->_rst, OUTPUT);
    digitalWrite(this->_rst, HIGH);
    delay(100);
    digitalWrite(this->_rst, LOW);
    delay(10);
    digitalWrite(this->_rst, HIGH);
    delay(10);
    while (!rf95.init())
    {
        Serial.println("LoRa radio init failed");
        Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
        while (1)
            ;
    }
    Serial.println("LoRa radio init OK!");

    // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
    if (!rf95.setFrequency(frequency))
    {
        Serial.println("setFrequency failed");
        while (1)
            ;
    }
    Serial.print("Set Freq to: ");
    Serial.println(frequency);

    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

    // The default transmitter power is 13dBm, using PA_BOOST.
    // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
    // you can set transmitter powers from 5 to 23 dBm:
    rf95.setTxPower(23, false);
};

void LoraTool::Send(String message)
{
    this->Setup();
    int16_t packetnum = 0; // packet counter, we increment per xmission
                           // this->Setup();
    char radiopacket[message.length() + 1];
    message.toCharArray(radiopacket, message.length());

    Serial.print("Sending ");
    Serial.println(radiopacket);
    rf95.send((uint8_t *)radiopacket, message.length());

    delay(10);
    rf95.waitPacketSent();
};

void LoraTool::Sleep()
{
    rf95.sleep();
};

bool LoraTool::Receive()
{
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
        String s = String((char *)buf);
        int i = String(this->lastMessage).compareTo((char *)buf);
        if (i != 0)
        {
            this->lastMessage = (char *)buf;
            return true;
        }
    }
    return false;
}
void LoraTool::Cycle()
{
    yield;
};
