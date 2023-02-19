#include <SPI.h>
#include <LoRa.h>

const long frequency = 866E6;  // LoRa Frequency
void onReceive(int packetSize) {
  String message = "";

  while (LoRa.available()) {
    message += (char)LoRa.read();
  }

  Serial.print("Gateway Receive: ");
  Serial.println(message);
}

void setup() {
  Serial.begin(9600);
  while(!Serial); // wait for Arduino Serial Monitor (native USB boards)
    Serial.println("Start");
  

  LoRa.setPins(GPIO_LORA_SC,GPIO_LORA_RST,GPIO_LORA_IRQ);
  LoRa.onReceive(onReceive);
  if (!LoRa.begin(frequency)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }else{
    Serial.println("LORA LISTENING");
  }
}

void loop() {
  // try to parse packet
  
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}