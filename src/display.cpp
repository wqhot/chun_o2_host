#include <display.h>
#ifndef NATIVE
#include <logger.hpp>

Display::Display(NodeList &list) : list_(list),
                                   u8g_(U8G2_R0, /* clock=*/PA5, /* data=*/PA7, /* cs=*/PB3, /* dc=*/PB4, /* reset=*/PB12),
                                   alarmPin(PB9),
                                   displayState_(mainScreen)
{
    pinMode(alarmPin, OUTPUT);
    digitalWrite(alarmPin, HIGH);
}

void Display::begin()
{
    u8g_.begin();

    u8g_.clearBuffer();                 // clear the internal memory
    u8g_.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
}

std::string float2string(float val)
{
    // 整数部分
    int intPart = static_cast<int>(val * 100) / 100;
    // 小数部分
    int decPart = static_cast<int>(val * 100) % 100;
    return std::to_string(intPart) + "." + std::to_string(std::abs(decPart));
}

void Display::drawText(uint8_t pos, uint8_t line, std::string str)
{
    // 计算字体占用高度
    uint8_t h = u8g_.getFontAscent() - u8g_.getFontDescent();
    // 画字符
    u8g_.drawStr(pos, line * h, str.c_str());
    u8g_.sendBuffer();
}

void Display::refresh()
{
    u8g_.clearBuffer();
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
        for (size_t i = 0; i != o2s.size(); ++i)
        {
            std::string str;
            // 整数部分
            int intPart = static_cast<int>(o2s[i] * 100) / 100;
            // 小数部分
            int decPart = static_cast<int>(o2s[i] * 100) % 100;
            str = "Node[" + std::to_string(i) + "] O2 = " + std::to_string(intPart) + "." + std::to_string(decPart) + "%";
            LOGGER << str;
            drawText(1, i + 1, str);
        }
        digitalWrite(alarmPin, HIGH);
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
                if (threshold_ <= 0)
                {
                    threshold_ = 0.0;
                }
                LOGGER << "Add event processed over. Threshold = " + float2string(threshold_);
                break;
            case subEvent:
                threshold_ -= 0.1;
                if (threshold_ >= 100)
                {
                    threshold_ = 100.0;
                }
                if (threshold_ <= 0)
                {
                    threshold_ = 0.0;
                }
                LOGGER << "Sub event processed over. Threshold = " + float2string(threshold_);
                break;
            case cancelEvent:
                setState(mainScreen);
                refresh();
                LOGGER << "Cancel event processed over. Threshold = " + float2string(threshold_);
                return;
                break;
            case confirmEvent:
                list_.setThreshold(threshold_);
                setState(mainScreen);
                refresh();
                LOGGER << "Confirm event processed over. Threshold = " + float2string(threshold_);
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
        drawText(1, 1, str);
        LOGGER << "Setting screen. Threshold = " + std::to_string(intPart) + "." + std::to_string(decPart) + "%";
        digitalWrite(alarmPin, HIGH);
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
        for (size_t i = 0; i != o2s.size(); ++i)
        {
            std::string str;
            if (o2s[i] < list_.getThreshold())
            {
                // 整数部分
                int intPart = static_cast<int>(o2s[i] * 100) / 100;
                // 小数部分
                int decPart = static_cast<int>(o2s[i] * 100) % 100;
                // 整数部分
                int thresholdIntPart = static_cast<int>(list_.getThreshold() * 100) / 100;
                // 小数部分
                int thresholdDecPart = static_cast<int>(list_.getThreshold() * 100) % 100;
                str = "Node[" + std::to_string(i) + "] O2 = " + std::to_string(intPart) + "." + std::to_string(decPart) + "% less than threshold that = " + std::to_string(thresholdIntPart) + "." + std::to_string(thresholdDecPart);
                LOGGER << str;
                drawText(1, i + 1, str);
            }
            else
            {
                // 整数部分
                int intPart = static_cast<int>(o2s[i] * 100) / 100;
                // 小数部分
                int decPart = static_cast<int>(o2s[i] * 100) % 100;
                str = "Node[" + std::to_string(i) + "] O2 = " + std::to_string(intPart) + "." + std::to_string(decPart) + "%";
                LOGGER << str;
                drawText(1, i + 1, str);
            }
        }
        digitalWrite(alarmPin, LOW);
    }
}

#endif