#include <display.h>
#ifndef NATIVE


Display::Display(NodeList &list) : 
    list_(list),
    u8g_(PB1, PB2, PB3, PB4)
{
    // 设置字体大小 对齐方式
    u8g_.setFont(u8g_font_6x13);
    u8g_.setFontRefHeightText();
    u8g_.setFontPosTop();

    // 设置前景颜色
    u8g_.setDefaultForegroundColor();
}

void Display::drawText(uint8_t pos, uint8_t line, std::string str)
{
    // 计算字体占用高度
    uint8_t h = u8g_.getFontAscent() - u8g_.getFontDescent();
    // 画字符
    u8g_.drawStr(pos, line * h, str.c_str());
}

#endif