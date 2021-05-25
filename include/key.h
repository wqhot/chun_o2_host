#ifndef __CHUN_O2_KEY_H_
#define __CHUN_O2_KEY_H_
#ifndef NATIVE
#include <Wire.h>
#include <map>
#include <vector>

class Key
{
public:
    Key(std::vector<uint8_t> keyList);
    // 扫描一次按键状态
    void scan();
    // 检测按键是否被长按
    bool isPress(uint8_t key, uint32_t time);
    // 检测按键是否被点击
    bool isClick(uint8_t key);

private:
    // 用于保存按键状态，0表示未被按下，>0表示按下的时间, -1表示按下后超时未抬起
    std::map<uint8_t, int32_t> keyStates_;
    // 用于保存最近一次的点击状态, 0表示最近没有点击发生
    uint8_t lastClickKey_;
};

#endif
#endif