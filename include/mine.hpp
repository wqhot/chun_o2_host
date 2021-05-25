#ifndef MINE
#define MINE
#ifndef NATIVE
#include <string>

#endif
namespace mutils
{
    std::string float2string(float val)
    {
        // 整数部分
        int intPart = static_cast<int>(val * 100) / 100;
        // 小数部分
        int decPart = static_cast<int>(val * 100) % 100;
        return std::to_string(intPart) + "." + std::to_string(std::abs(decPart));
    }
}
#endif