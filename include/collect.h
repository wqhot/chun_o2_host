#ifndef __CHUN_O2_COLLECT_H_
#define __CHUN_O2_COLLECT_H_
#ifndef NATIVE
#include <HardwareSerial.h>
#include <display.h>
#endif
#include <vector>
#include <algorithm>
#include <node.hpp>

/*
类别 |   帧头    |   ID    |    数据    |  校验和  
字节 | 0 - 3    | 4 - 15  |  16 - 19   |  20
格式 | HEAD     | u32[3]  |  float     | u8
*/
class Collection
{
public:
    Collection(NodeList &list);
#ifndef NATIVE
    bool recv();
#endif
    bool recv(uint8_t *buffer, size_t length);

private:
    const std::vector<uint8_t> HEAD{0x1, 0x2, 0x3, 0x4};
    const size_t ID_POS = 4;
    const size_t DATA_POS = 16;
    const size_t SUM_POS = 20;
    const size_t ID_LENGTH = 3;
    const size_t O2_BUFFER_LENGTH = 21;
#ifndef NATIVE
    HardwareSerial serialCollect_;
#endif
    std::vector<uint8_t> buffer_;
    NodeList &list_;
    // 帧格式解析
    void parser();
    // 计算ID
    uint8_t getID(std::vector<uint8_t>::iterator pos, uint32_t *id);
    // 计算数据
    uint8_t getData(std::vector<uint8_t>::iterator pos, float &data);
};
// extern HardwareSerial serial_collect;

#endif
