#ifndef LoraMessage_H
#define LoraMessage_H
#include <Arduino.h>
class LoraMessage
{

public:
    LoraMessage(char *Topic, String messageData);
    ~LoraMessage();
    void SetWeight(double value);
    void SetDeviceId(String id);
    void SetBatteryValue(double value);
    void SetHumidity(double value);
    void SetTemperature(double value);
    String GetMessage();    
private:
    double _weight;
    String id;
    double _batteryValue;
    double _humidityValue;
    double _temperatureValue;
};

#endif