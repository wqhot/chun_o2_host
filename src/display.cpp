#include <display.h>
#ifndef NATIVE


Display::Display(NodeList &list) : 
    list_(list),
    u8g_(U8G_I2C_OPT_NONE)
{

}

#endif