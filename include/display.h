#ifndef __CHUN_O2_DISPLAY_H_
#define __CHUN_O2_DISPLAY_H_
#ifndef NATIVE
#include <SPI.h>
#include <Wire.h>
#include <node.hpp>
#include <U8glib.h>
#include <U8glibPrint.h>
#include <U8glibPrintable.h>

enum DisplayState
{
    mainScreen,     // 显示节点状态列表
    settingScreen,  // 显示阈值设置界面
    alarmScreen     // 显示报警界面
};

class Display
{
public:
    Display(NodeList &list);
    void drawText(uint8_t pos, uint8_t line, std::string str);
    void refresh();

private:
    NodeList &list_;
    U8GLIB_SSD1306_128X64 u8g_;

    // 显示状态
    DisplayState displayState_;
};
#endif
#endif