/*
  Weight.h - Lib for gtting weights
*/
#include "Arduino.h"
#include "WeightProcessor.h"
#include "HX711.h"
#include <RunningMedian.h>
// Avoiding compile issues
// WeightProcessor *WeightProcessor::instance = nullptr;

int _dtPin;
int _scPin;
float _kilogramDivider;
float _weightOffset;
float _calibrationTemperature;
float _calibrationFactor;

HX711 scale;

WeightProcessor::WeightProcessor(int dtPin, int scPin)
{
  this->_calibrationTemperature = _calibrationTemperature;
  this->_dtPin = dtPin;
  this->_scPin = scPin;
  scale.begin(_dtPin, _scPin);
}

void WeightProcessor::SetScale()
{
  scale.set_scale();
}

void WeightProcessor::Tare()
{
  scale.tare();
}

float WeightProcessor::GetUnits()
{
  return scale.get_units(10);
}

void WeightProcessor::setup(float kilogramDivider, float weightOffset, float calibrationTemperature, float calibrationFactor)
{
#if SETUP_TARE == 0
  this->_calibrationFactor = calibrationFactor;
  this->_calibrationTemperature = calibrationTemperature;
  this->_kilogramDivider = kilogramDivider;
  this->_weightOffset = weightOffset;
  if (this->_weightOffset > 0)
    scale.set_offset(this->_weightOffset);
  if (this->_kilogramDivider)
    scale.set_scale(this->_kilogramDivider); // initialize scale
#endif
}

float WeightProcessor::getWeight()
{

  Serial.printf("Kilogram Divider %f\tTare value: %f", this->_kilogramDivider, this->_weightOffset);
  RunningMedian WeightSamples = RunningMedian(3);
  for (int i = 0; i < 5; i++)
  {
    scale.power_up();
    delay(2); // wait for wakeup;
    float WeightRaw = scale.read();
    //.get_units(3);
    scale.power_down();

    float correctedWeight = (WeightRaw - this->_weightOffset) / this->_kilogramDivider;

    Serial.printf("Weight RAW Value: %f corrected to %f g", WeightRaw, correctedWeight);
    Serial.println();
    WeightSamples.add(correctedWeight);
    delay(180);
  }
  float median = WeightSamples.getMedian();
  Serial.print("Median:");
  Serial.println(median);
  return median;
}

WeightProcessor::~WeightProcessor()
{
}

bool WeightProcessor::DeviceReady()
{

  return scale.wait_ready_timeout(1000);
}

float WeightProcessor::getZeroOffset()
{

  RunningMedian WeightSamples = RunningMedian(defaultTareSamples);
  scale.power_up();
  do
  {
    scale.power_up();
    delay(2);
    float WeightRaw = scale.read();
    scale.power_down();
    Serial.print("Weight RAW Value:");
    Serial.println(WeightRaw);
    WeightSamples.add(WeightRaw);
    delay(waitTimeLoadSamples * 1000);
  } while (WeightSamples.getCount() < WeightSamples.getSize());

  float median = WeightSamples.getMedian();
  Serial.print("Median:");
  Serial.println(median);
  scale.power_down();
  return median;
}

float WeightProcessor::getRawWeight()
{

  RunningMedian WeightSamples = RunningMedian(3);
  scale.power_up();
  for (int i = 0; i < 3; i++)
  {
    scale.power_up();
    delay(2);
    float weightRaw = scale.read();
    scale.power_down();
    Serial.printf("Weight RAW Value:%f", weightRaw);
    Serial.println();
    WeightSamples.add(weightRaw);
    delay(50);
  }
  float median = WeightSamples.getMedian();
  Serial.print("Median:");
  Serial.println(median);
  scale.power_down();
  return median;
}

float WeightProcessor::getWeight(float temperatureForCompensation)
{
  if (scale.is_ready())
  {
    float weight = this->getWeight();
    // temperature compensation
    if (temperatureForCompensation < this->_calibrationTemperature)
      weight = weight + (fabs(temperatureForCompensation - this->_calibrationTemperature) * this->_calibrationFactor);
    if (temperatureForCompensation > this->_calibrationTemperature)
      weight = weight - (fabs(temperatureForCompensation - this->_calibrationTemperature) * this->_calibrationFactor);

    return weight;
  }
  return -127;
}

float WeightProcessor::toKilogram(float getWeighMeasure)
{
  return getWeighMeasure / 1000;
}

void WeightProcessor::AutotareSetup()
{
}