#ifndef NATIVE
#include <Arduino.h>
#include <Wire.h>
#include <node.hpp>
#include <collect.h>
#include <key.h>

NodeList list;
Collection collection(list);
Display display(list);
// PB5 上
// PB6 下
// PB7 取消
// PB8 确认
Key key({PB5, PB6, PB7, PB8});

void setup()
{
    display.drawText(0, 0, "hello world!");
}

void loop()
{
    bool isRecv = collection.recv();
    
    // 收到数据
    if (isRecv)
    {
        
    }
    delay(10);
}

#endif