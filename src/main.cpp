#ifndef NATIVE
#include <Arduino.h>
#include <Wire.h>
#include <node.hpp>
#include <collect.h>
#include <key.h>
#include <logger.hpp>

NodeList list;
Collection collection(list);
Display display(list);
Logger LOGGER;
#define KEY_ADD PB5      // PB5 加
#define KEY_SUB PB10     // PB10 减
#define KEY_CONFIRM PB11 // PB11 确认
#define KEY_CANCEL PB8   // PB8 取消
Key key({KEY_ADD, KEY_SUB, KEY_CONFIRM, KEY_CANCEL});
bool enableAlarm = true;

void setup()
{
    display.begin();
    delay(100);
    display.drawText(1, 1, "Oxygen concentration monitor");
    list.setThreshold(40.0);
    LOGGER << "Init Finish";
}

void loop()
{
    // 是否需要刷新屏幕
    bool needRefresh = false;
    // 接收Node发送的数据
    bool isRecv = collection.recv();
    // 获取报警状态
    bool isAlarm = list.isAlarm();
    // 检测长按按钮事件
    if (key.isPress(KEY_CANCEL, 100) && !(isAlarm && enableAlarm))
    {
        // 长按取消键1s进入设置界面
        display.setState(settingScreen);
        needRefresh = true;
        LOGGER << "CANCAL button pressed.";
    }
    else if (key.isPress(KEY_CONFIRM, 100))
    {
        // 长按确认键1s关闭报警功能
        if (isAlarm)
        {
            LOGGER << "Alarm State.CONFIRM button pressed.";
            enableAlarm = false;
            needRefresh = true;
            display.setState(mainScreen);
        }else{
            LOGGER << "CONFIRM button pressed.";
        }
    }
    else
    {
        if (key.isClick(KEY_ADD))
        {
            // 事件队列中增加事件
            display.addEventQueue(addEvent);
            LOGGER << "ADD button clicked.";
            needRefresh = true;
        }
        else if (key.isClick(KEY_SUB))
        {
            display.addEventQueue(subEvent);
            LOGGER << "SUB button clicked.";
            needRefresh = true;
        }
        else if (key.isClick(KEY_CONFIRM))
        {
            display.addEventQueue(confirmEvent);
            LOGGER << "CONFIRM button clicked.";
            needRefresh = true;
        }
        else if (key.isClick(KEY_CANCEL))
        {
            display.addEventQueue(cancelEvent);
            LOGGER << "CANCEL button clicked.";
            needRefresh = true;
        }
    }
    // 报警状态
    if (isAlarm && enableAlarm)
    {
        display.setState(alarmScreen);
        needRefresh = true;
    }
    // 扫描一次按键状态
    key.scan();
    needRefresh = isRecv || needRefresh;
    // 刷新屏幕
    if (needRefresh)
    {
        display.refresh();
    }
    delay(10);
}


#endif