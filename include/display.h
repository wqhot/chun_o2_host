#ifndef __CHUN_O2_DISPLAY_H_
#define __CHUN_O2_DISPLAY_H_
#ifndef NATIVE

#include <Wire.h>
#include <node.hpp>
#include <U8g2lib.h>
#include <SPI.h>
#include <queue>

enum DisplayState
{
    mainScreen,     // 显示节点状态列表
    settingScreen,  // 显示阈值设置界面
    alarmScreen     // 显示报警界面
};

enum Event
{
    addEvent,     // 加事件
    subEvent,     // 减事件
    confirmEvent, // 确认事件
    cancelEvent  // 取消事件
};

class Display
{
public:
    Display(NodeList &list);
    void drawText(uint8_t pos, uint8_t line, std::string str);
    void refresh();
    inline void setState(DisplayState state)
    {
        displayState_ = state;
        if (state == settingScreen)
        {
            threshold_ = list_.getThreshold();
        }
    }
    inline void addEventQueue(Event event)
    {
        eventQue_.push(event);
    }

private:
    NodeList &list_;
    // 临时保存的阈值
    float threshold_;
    // 事件队列
    std::queue<Event> eventQue_;
    U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI u8g_;
    uint8_t alarmPin;
    // 显示状态
    DisplayState displayState_;
};
#endif
#endif