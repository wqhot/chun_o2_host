#ifndef __CHUN_O2_NODE_HPP_
#define __CHUN_O2_NODE_HPP_
#include <vector>
#include <stdint-gcc.h>
#include <algorithm>

class Node
{
public:
    Node() : id_{0, 0, 0}
    {
    }
    Node(uint32_t *id) : id_{id[0], id[1], id[2]}
    {
    }
    Node(const Node &node) : id_{node.getId()[0], node.getId()[1], node.getId()[2]} {

                             };
    Node(Node &&) = delete;

    bool compareId(const uint32_t *id) const
    {
        return (id_[0] == id[0] && id_[1] == id[1] && id_[2] == id[2]);
    }

    bool operator==(const Node &node) const
    {
        return compareId(node.getId());
    }

    void setO2Num(float o2Num)
    {
        Node::o2Num_ = o2Num;
    }

    float getO2Num()
    {
        return o2Num_;
    }

    const uint32_t *getId() const
    {
        return id_;
    }

private:
    const uint32_t id_[3];
    float o2Num_;
};

class NodeList
{
public:
    NodeList()
    {
    }
    Node &getNode(uint32_t *id)
    {
        auto check = [id](Node &node) {
            return node.compareId(id);
        };
        std::vector<Node>::iterator it = std::find_if(list_.begin(), list_.end(), check);
        if (it != list_.end())
        {
            return *it;
        }
        else
        {
            Node node(id);
            list_.push_back(node);
            return *(list_.end() - 1);
        }
    }

private:
    std::vector<Node> list_;
};

#endif