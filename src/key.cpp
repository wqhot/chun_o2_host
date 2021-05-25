#include <key.h>
#ifndef NATIVE
#include <logger.hpp>

#define CLICK_TIME 5

Key::Key(std::vector<uint8_t> keyList)
    : lastClickKey_(0)
{
    for (auto key : keyList)
    {
        keyStates_.insert(std::pair<uint8_t, uint32_t>(key, 0));
        pinMode(key, INPUT);     // IO口设置为输入状态
        digitalWrite(key, HIGH); // 开启上拉
    }
}

void Key::scan()
{
    for (auto &key : keyStates_)
    {
        // 按键状态读取
        int state = digitalRead(key.first);

        if (state == HIGH)
        {
            // 高电平, 计数归零
            if (key.second > CLICK_TIME)
            {
                // 之前被按下
                // 记录一次点击
                lastClickKey_ = key.first;
                // 计数归零
                key.second = 0;
            }
            else if (key.second == -1)
            {
                //之前被按下一直未抬起
                key.second = 0;
                LOGGER << std::to_string(key.first) +  " button released.";
            }
        }
        else
        {
            if (key.second >= 0)
            {
                // 低电平， 计数加一
                ++key.second;
            }
        }
    }
}

bool Key::isPress(uint8_t key, uint32_t time)
{
    // 在状态表中查找按键状态
    auto iter = keyStates_.find(key);
    // 未找到该按键的状态
    if (iter == keyStates_.end())
    {
        return false;
    }
    //LOGGER << (std::to_string(key) + " button pressed for " + std::to_string(iter->second) );
    // 满足时间要求
    if (iter->second > time)
    {
        LOGGER << (std::to_string(key) + " button pressed for " + std::to_string(iter->second) );
        iter->second = -1;
        return true;
    }
    return false;
}

bool Key::isClick(uint8_t key)
{
    if (lastClickKey_ == key)
    {
        lastClickKey_ = 0;
        LOGGER << std::to_string(key) + " button clicked";
        return true;
    }
    return false;
}

#endif