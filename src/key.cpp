#include <key.h>
#ifndef NATIVE
#include <logger.hpp>

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
            if (key.second > 0)
            {
                // 之前被按下
                // 记录一次点击
                lastClickKey_ = key.first;
            }
            key.second = 0;
        }
        else
        {
            // 低电平， 计数加一
            ++key.second;
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
    LOGGER << (std::to_string(key) + " button pressed for " + std::to_string(iter->second) );
    // 满足时间要求
    if (iter->second > time)
    {
        iter->second = 0;
        return true;
    }
    return false;
}

bool Key::isClick(uint8_t key)
{
    if (lastClickKey_ == key)
    {
        lastClickKey_ = 0;
        return true;
    }
    return false;
}

#endif