#ifndef MODEL_H
#define MODEL_H

#include <thread>
#include <array>
#if 0
typedef std::array<double,2> Coords;

class SharedData
{
    friend class Model;
public:
    SharedData() : first(nullptr) {}
    SharedData(const SharedData&) = delete;
    ~SharedData() {destroy();}

};

class Model final
{
    struct Node {
        volatile Coords * pos;
        volatile Coords * acc;
        volatile Node * prev;
        volatile Node * next;
        Node(const Coords& xy) : pos(new Coords(xy)), acc(new Coords{0,0}), prev(nullptr), next(nullptr) {}
    };
    volatile Node* firstNode;

    std::thread worker;
    volatile bool stop;
public:
    Model();
    ~Model();
    Model(const Model&) = delete;
    Model() = delete;

    void * addBubble(const Coords& xy);
    void delBubble(void* node);
    void moveBubble(void* node, const Coords& xy) const;
    inline Coords getAcc(void* node) const { return *(static_cast<Node*>(node)->acc); }
};
#endif
#endif // MODEL_H
