#include <display.h>



Display::Display(NodeList &list) : 
    list_(list),
    u8g_(U8G_I2C_OPT_NONE)
{

}