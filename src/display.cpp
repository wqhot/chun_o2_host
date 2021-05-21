#include <display.h>
#ifndef NATIVE
#include <logger.hpp>

Display::Display(NodeList &list) : list_(list),
                                   u8g_(PB1, PB2, PB3, PB4),
                                   alarmPin(PB9),
                                   displayState_(mainScreen)
{
    // 设置字体大小 对齐方式
    u8g_.setFont(u8g_font_6x13);
    u8g_.setFontRefHeightText();
    u8g_.setFontPosTop();

    // 设置前景颜色
    u8g_.setDefaultForegroundColor();
    pinMode(alarmPin, OUTPUT);
    digitalWrite(alarmPin, LOW);
}

void Display::drawText(uint8_t pos, uint8_t line, std::string str)
{
    // 计算字体占用高度
    uint8_t h = u8g_.getFontAscent() - u8g_.getFontDescent();
    // 画字符
    u8g_.drawStr(pos, line * h, str.c_str());
}

void Display::refresh()
{
    if (displayState_ == mainScreen)
    {
        // 处理事件队列, 直接清空
        while (eventQue_.size() != 0)
        {
            eventQue_.pop();
        }
        LOGGER << "Refreshing main screen.";
        std::vector<float> o2s;
        // 读取传感器值
        list_.getNodeState(o2s);
        for (size_t i; i != o2s.size(); ++i)
        {
            std::string str;
            // 整数部分
            int intPart = static_cast<int>(o2s[i] * 100) / 100;
            // 小数部分
            int decPart = static_cast<int>(o2s[i] * 100) % 100;
            str = "Node[" + std::to_string(i) + "] O2 = " + std::to_string(intPart) + "." + std::to_string(decPart) + "%";
            LOGGER << str;
            drawText(0, i, str);
        }
        digitalWrite(alarmPin, LOW);
    }
    else if (displayState_ == settingScreen)
    {
        // 处理事件队列
        while (eventQue_.size() != 0)
        {
            Event event = eventQue_.front();
            switch (event)
            {
            case addEvent:
                threshold_ += 0.1;
                if (threshold_ >= 100)
                {
                    threshold_ = 100.0;
                }
                LOGGER << "Add event processed over. Threshold = " << std::to_string(threshold_);
                break;
            case subEvent:
                threshold_ -= 0.1;
                if (threshold_ <= 0)
                {
                    threshold_ = 0.0;
                }
                LOGGER << "Sub event processed over. Threshold = " << std::to_string(threshold_);
                break;
            case cancelEvent:
                setState(mainScreen);
                refresh();
                LOGGER << "Cancel event processed over. Threshold = " << std::to_string(threshold_);
                return;
                break;
            case confirmEvent:
                list_.setThreshold(threshold_);
                setState(mainScreen);
                refresh();
                LOGGER << "Confirm event processed over. Threshold = " << std::to_string(threshold_);
                return;
                break;
            default:
                break;
            }
            eventQue_.pop();
        }
        // 整数部分
        int intPart = static_cast<int>(threshold_ * 100) / 100;
        // 小数部分
        int decPart = static_cast<int>(threshold_ * 100) % 100;
        std::string str = "Alarm Threshold = " + std::to_string(intPart) + "." + std::to_string(decPart) + "%";
        drawText(0, 0, str);
        LOGGER << "Setting screen. Threshold = " << std::to_string(threshold_);
        digitalWrite(alarmPin, LOW);
    }
    else if (displayState_ == alarmScreen)
    {
        // 处理事件队列
        while (eventQue_.size() != 0)
        {
            eventQue_.pop();
        }
        LOGGER << "ALARM screen is on.";
        std::vector<float> o2s;
        // 读取传感器值
        list_.getNodeState(o2s);
        for (size_t i; i != o2s.size(); ++i)
        {
            std::string str;
            if (o2s[i] > list_.getThreshold())
            {
                // 整数部分
                int intPart = static_cast<int>(o2s[i] * 100) / 100;
                // 小数部分
                int decPart = static_cast<int>(o2s[i] * 100) % 100;
                str = "Node[" + std::to_string(i) + "] O2 = " + std::to_string(intPart) + "." + std::to_string(decPart) + "% less than threshold!";
                LOGGER << str;
                drawText(0, i, str);
            }
            else
            {
                // 整数部分
                int intPart = static_cast<int>(o2s[i] * 100) / 100;
                // 小数部分
                int decPart = static_cast<int>(o2s[i] * 100) % 100;
                str = "Node[" + std::to_string(i) + "] O2 = " + std::to_string(intPart) + "." + std::to_string(decPart) + "%";
                LOGGER << str;
                drawText(0, i, str);
            }
        }
        digitalWrite(alarmPin, HIGH);
    }
}

#endif