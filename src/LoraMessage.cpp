#include "LoraMessage.h"

void LoraMessage::SetWeight(double value)
{
  this->_weight = value;
}
void LoraMessage::SetDeviceId(String id) { this->id = id; }
void LoraMessage::SetBatteryValue(double value) { this->_batteryValue = value; };
void LoraMessage::SetHumidity(double value) { this->_humidityValue = value; };
void LoraMessage::SetTemperature(double value) { this->_temperatureValue = value; };
String LoraMessage::GetMessage()
{
  return this->id +
         "|" + String(this->_batteryValue) +
         "|" + String(this->_humidityValue) +
         "|" + String(this->_temperatureValue) +
         "|" + String(this->_weight);
};