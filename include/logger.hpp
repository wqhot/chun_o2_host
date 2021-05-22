#ifndef __CHUN_O2_LOGGER_H_
#define __CHUN_O2_LOGGER_H_
#ifndef NATIVE
#include <HardwareSerial.h>
#include <string>

class Logger
{
public:
    Logger()
    : logSerial(PA10, PA9)
    {
        logSerial.begin(115200);
    }
    
    Logger& operator << (std::string str)
    {
        logSerial.write(str.c_str(), str.size());
        logSerial.write("\n");
        return *this;
    }

    Logger& operator << (const char* str)
    {
        logSerial.write(str);
        logSerial.write("\n");
        return *this;
    }
private:
    HardwareSerial logSerial;
};

extern Logger LOGGER;

#endif
#endif