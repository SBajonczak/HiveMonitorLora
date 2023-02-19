#include <SPI.h>
#include <LoRa.h>
#include <SPI.h>              // include libraries
#include <LoRa.h>
#include "LoraMessage.h"
#include "WeightProcessor.h"
#include "LoraTool.h"
#include <Adafruit_SleepyDog.h>

WeightProcessor scaledevice(GPIO_HX711_DT,GPIO_HX711_SCK);


LoraTool lora(GPIO_LORA_SC,GPIO_LORA_RST,GPIO_LORA_IRQ);

// #define DONE_PIN 12
// #define SETUP_TARE 0
#define WEIGHT_OFFSET 114340
#define KG_DIVIDER 12.7064
void setup() {

    pinMode(DONE_PIN, OUTPUT);
    digitalWrite(DONE_PIN, LOW);
    Serial.begin(115200);
    // while(!Serial); // wait for Arduino Serial Monitor (native USB boards)
    //     Serial.println("Start");
    

    scaledevice.setup(KG_DIVIDER,WEIGHT_OFFSET,0,0);
    lora.Setup();
    if (scaledevice.DeviceReady())
    {
        Serial.println("Scaledevice found!");
        
    }

    while(!scaledevice.DeviceReady()){
        Serial.println("Wait on scale");
        yield;
    }


       
}

void loop() {
    

#if SETUP_TARE ==1
  Serial.println("Remove all weight. Waiting 5 seconds");
  delay(5000);
  float tareOffset = scaledevice.getRawWeight();
  Serial.printf("Got %f", tareOffset);
  Serial1.println();
  
  Serial.println("Place 1 kg weight. Waiting 5 seconds");
  delay(5000);
  Serial.println("Start Measure");
  float onekgweight  = scaledevice.getRawWeight();
  Serial.printf("Got %f", onekgweight);
  Serial.println();

  Serial.println("Place 2 kg weight. Waiting 5 seconds");
  delay(5000);
  Serial.println("Start Measure");
  float twokgweight = scaledevice.getRawWeight();
  Serial.printf("Got %f", twokgweight);
  Serial.println();

  float loadkg= SETUP_TARE_UPPERLOAD; // For 2kilos
  float loadCellKgDivider = (twokgweight - onekgweight) / ((float) loadkg / 1000.0f);

  Serial.printf("loadCellKgDivider %f", loadCellKgDivider);
  Serial.println();
  Serial.printf("tareOffset %f", tareOffset);
  Serial.println();
  delay(100000);
#else
   
    Serial.println("Getting measurements");
    float weight= scaledevice.getWeight();
    float kgWeight= scaledevice.toKilogram(weight);
    Serial.printf("Got %f\tin KG %f", weight, kgWeight);
    Serial.println();

    Serial.println("Send data to lora");

    String message= "weight:";
    message.concat(weight);
    Serial.print("Message: ");
    Serial.println(message);
    lora.Send(message);
    
    // lora.Sleep();
    // while (1) {
        digitalWrite(DONE_PIN, HIGH);
        delay(10);
        digitalWrite(DONE_PIN, LOW);
        delay(1);
    delay(5000);
    // }
#endif
}
