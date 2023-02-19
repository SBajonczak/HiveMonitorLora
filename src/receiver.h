#include <SPI.h>
#include <LoRa.h>
#include <LoraTool.h>
LoraTool lora(GPIO_LORA_SC, GPIO_LORA_RST, GPIO_LORA_IRQ);

void setup()
{
  Serial.begin(115200);
  lora.Setup();
}

void loop()
{
  if (lora.Receive())
  {
    Serial.println(lora.lastMessage);
  }
}