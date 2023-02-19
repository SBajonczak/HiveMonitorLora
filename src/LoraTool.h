#ifndef LoraTool_H
#define LoraTool_H
#include <Arduino.h>
class LoraTool
{

public:
    LoraTool(int CS, int RST, int IRQ);
    ~LoraTool();
    void Setup();
    void Send(String message);
    void Cycle();
    void Sleep();
    bool Receive();
    char* lastMessage;

private:
    int _cs; int _rst; int _irq;
};

#endif