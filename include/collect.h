#ifndef __CHUN_O2_COLLECT_H_
#define __CHUN_O2_COLLECT_H_
#include <HardwareSerial.h>
#include <vector>
#include <algorithm>
#include <node.hpp>
#include <display.h>

/*
类别 |   帧头    |   ID    |    数据    |  校验和  
字节 | 0 - 3    | 4 - 15  |  16 - 19   |  20
格式 | HEAD     | u32[3]  |  float     | u8
*/
class Collection
{
public:
    Collection(NodeList &list);
    void recv();

private:
    const std::vector<uint8_t> HEAD{0x1, 0x2, 0x3, 0x4};
    const size_t ID_POS = 4;
    const size_t DATA_POS = 16;
    const size_t SUM_POS = 20;
    const size_t ID_LENGTH = 3;
    const size_t O2_BUFFER_LENGTH = 21;
    HardwareSerial serialCollect_;
    std::vector<uint8_t> buffer_;
    NodeList &list_;

    void parser();
    uint8_t getID(std::vector<uint8_t>::iterator pos, uint32_t *id);
    uint8_t getData(std::vector<uint8_t>::iterator pos, float &data);
};
// extern HardwareSerial serial_collect;

#endif
