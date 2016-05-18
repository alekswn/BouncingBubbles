#ifndef SHAREDLIST_H
#define SHAREDLIST_H
#if 0
class SharedList
{
    struct Node {
        dataT* data;
        Node * next;
    };

    Node * first;
public:
    SharedList() : first(nullptr) {}
    ~SharedList() {destroy();}

    ssize_t add(const dataT& data);
    void del(ssize_t index);
};
#endif
#endif // SHAREDLIST_H
