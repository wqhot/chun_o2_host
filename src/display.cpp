#include <display.h>
#include <mine.hpp>
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

//0 - index
void Display::drawText(uint8_t pos, uint8_t line, std::string str)
{
    // 计算字体占用高度
    uint8_t h = u8g_.getFontAscent() - u8g_.getFontDescent();
    // 画字符
    u8g_.drawStr(pos + 1, (line + 1) * h, str.c_str());
}

void Display::send()
{
    u8g_.sendBuffer();
    u8g_.clearBuffer();
}

void Display::refresh()
{
    LOGGER << "The state of display is " + std::to_string(displayState_);
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
            str = "Node[" + std::to_string(i) + "] O2 = " + mutils::float2string(o2s[i]) + "%";
            LOGGER << str;
            drawLine(i, str);
        }
        digitalWrite(alarmPin, HIGH);
    }
    else if (displayState_ == settingScreen)
    {
        LOGGER << "Entering setting screen.";
        // 处理事件队列
        while (eventQue_.size() != 0)
        {
            Event event = eventQue_.front();
            eventQue_.pop();
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
                LOGGER << "Add event processed over. Threshold = " + mutils::float2string(threshold_);
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
                LOGGER << "Sub event processed over. Threshold = " + mutils::float2string(threshold_);
                break;
            case cancelEvent:
                setState(mainScreen);
                threshold_ = list_.getThreshold();
                refresh();
                LOGGER << "Cancel event processed over. Threshold = " + mutils::float2string(threshold_);
                return;
                break;
            case confirmEvent:
                list_.setThreshold(threshold_);
                setState(mainScreen);
                refresh();
                LOGGER << "Confirm event processed over. Threshold = " + mutils::float2string(threshold_);
                return;
                break;
            default:
                break;
            }
        }
        std::string str = "Alarm Threshold = " + mutils::float2string(threshold_) + "%";
        drawLine(0, str);
        LOGGER << "Setting screen. Threshold = " + mutils::float2string(threshold_) + "%";
        digitalWrite(alarmPin, HIGH);
    }
    else if (displayState_ == alarmScreen)
    {
        // 处理事件队列
        while (eventQue_.size() != 0)
        {
            eventQue_.pop();
        }
        //LOGGER << "ALARM screen is on.";
        std::vector<float> o2s;
        // 读取传感器值
        list_.getNodeState(o2s);
        size_t count = 0;
        std::string str = "Node less than threshold =" + mutils::float2string(list_.getThreshold()) + "% :";
        drawLine(0, str);
        for (size_t i = 0; i != o2s.size(); ++i)
        {
            std::string str0;
            if (o2s[i] < list_.getThreshold())
            {
                str0 = "Node[" + std::to_string(i) + "] O2 = " + mutils::float2string(o2s[i]) + "%";
                LOGGER << str0 + " -> Low";
                drawLine(count++, str0);
            }
        }
        digitalWrite(alarmPin, LOW);
    }
    send();
}

#endif