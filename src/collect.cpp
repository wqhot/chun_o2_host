#include <collect.h>

Collection::Collection(NodeList &list) : 
#ifndef NATIVE
    serialCollect_(1),
#endif
    list_(list)
{
#ifndef NATIVE
    serialCollect_.begin(115200, SERIAL_8N1);
#endif
}

void Collection::recv(uint8_t *buffer, size_t length)
{
    buffer_.insert(buffer_.end(), buffer, buffer + length);
    // 从头解析buffer_
    // 找到帧头
    std::vector<uint8_t>::iterator head_pos = std::find_first_of(buffer_.begin(), buffer_.end(), HEAD.begin(), HEAD.end());
    if (head_pos == buffer_.end())
    {
        return;
    }
    // 删除head_pos以前内容
    buffer_.erase(buffer_.begin(), head_pos);
    if (buffer_.size() >= O2_BUFFER_LENGTH)
    {
        parser();
    }
}

#ifndef NATIVE
void Collection::recv()
{
    uint8_t buffer[O2_BUFFER_LENGTH];
    size_t length = serialCollect_.readBytes(buffer, O2_BUFFER_LENGTH);
    buffer_.insert(buffer_.end(), buffer, buffer + length);
    // 从头解析buffer_
    // 找到帧头
    std::vector<uint8_t>::iterator head_pos = std::find_first_of(buffer_.begin(), buffer_.end(), HEAD.begin(), HEAD.end());
    if (head_pos == buffer_.end())
    {
        return;
    }
    // 删除head_pos以前内容
    buffer_.erase(buffer_.begin(), head_pos);
    if (buffer_.size() >= O2_BUFFER_LENGTH)
    {
        parser();
    }
}
#endif

uint8_t Collection::getID(std::vector<uint8_t>::iterator pos, uint32_t *id)
{
    uint8_t sum = 0;
    union transfer
    {
        uint32_t id;
        uint8_t data[4];
    };
    for (size_t i = 0; i != ID_LENGTH; i++)
    {
        transfer t;
        t.data[0] = *(pos + i * 4 + 0);
        t.data[1] = *(pos + i * 4 + 1);
        t.data[2] = *(pos + i * 4 + 2);
        t.data[3] = *(pos + i * 4 + 3);
        sum += t.data[0] + t.data[1] + t.data[2] + t.data[3];
        *(id + i) = t.id;
    }
    return sum;
}

uint8_t Collection::getData(std::vector<uint8_t>::iterator pos, float &data)
{
    uint8_t sum = 0;
    union transfer
    {
        float f;
        uint8_t data[4];
    };

    transfer t;
    t.data[0] = *(pos + 0);
    t.data[1] = *(pos + 1);
    t.data[2] = *(pos + 2);
    t.data[3] = *(pos + 3);
    sum = t.data[0] + t.data[1] + t.data[2] + t.data[3];
    data = t.f;
    return sum;
}

void Collection::parser()
{
    if (buffer_.size() < O2_BUFFER_LENGTH)
    {
        return;
    }
    // 确认帧头
    std::vector<uint8_t>::iterator head_pos = std::find_first_of(buffer_.begin(), buffer_.end(), HEAD.begin(), HEAD.end());
    if (head_pos != buffer_.begin())
    {
        return;
    }
    uint8_t sum = 0;
    // id
    uint32_t id[ID_LENGTH] = {0};
    sum += getID(buffer_.begin() + ID_POS, id);
    // data
    float o2 = 0.f;
    sum += getData(buffer_.begin() + DATA_POS, o2);
    if (sum != buffer_[SUM_POS])
    {
        // 删除head以前内容
        buffer_.erase(buffer_.begin(), buffer_.begin() + HEAD.size());
        return;
    }
    // 删除这一帧
    buffer_.erase(buffer_.begin(), buffer_.begin() + O2_BUFFER_LENGTH);
    list_.getNode(id).setO2Num(o2);
}