#ifndef __CHUN_O2_DISPLAY_H_
#define __CHUN_O2_DISPLAY_H_
#include <SPI.h>
#include <Wire.h>
#include <node.hpp>
#include <U8glib.h>

class Display
{
public:
    Display(NodeList &list);
private:
    NodeList &list_;
    U8GLIB_SSD1306_128X64 u8g_;
};

#endif