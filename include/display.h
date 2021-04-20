#ifndef __CHUN_O2_DISPLAY_H_
#define __CHUN_O2_DISPLAY_H_
#ifndef NATIVE
#include <SPI.h>
#include <Wire.h>
#include <node.hpp>
#include <U8glib.h>
#include <U8glibPrint.h>
#include <U8glibPrintable.h>

class Display
{
public:
    Display(NodeList &list);
private:
    NodeList &list_;
    U8GLIB_SSD1306_128X64 u8g_;
};
#endif
#endif