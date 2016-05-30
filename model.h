#ifndef MODEL_H
#define MODEL_H

#include <thread>
#include <array>
#include <atomic>
#include <chrono>
#include <array>

#include "utils.h"

typedef double length_t;

struct Coords2D {
    length_t x,y;
    Coords2D(length_t x, length_t y) : x(x), y(y) {}
    Coords2D(const Coords2D&) = default;
    Coords2D() : x(0), y(0) {}
    Coords2D& operator+=(const Coords2D& r) { x+=r.x; y+=r.y; return *this; }
    friend Coords2D operator*(const Coords2D& l, length_t f) { return Coords2D(l.x*f,l.y*f); }
};

class BubbleModelPrivate;

class SceneModel final
{
    std::thread* worker;
    std::atomic<bool> isStopped;
    mutable std::atomic_flag up2date;
    std::chrono::duration<double> updateInterval;

//    SharedListNode<BubbleModelPrivate*> bubbleListDelimiter;

    friend class BubbleModel;

    void doWork();

public:
    explicit SceneModel(double ui);
    ~SceneModel();
    SceneModel() = delete;
    SceneModel(const SceneModel&) = delete;

    void start();
    void stop() { isStopped = true; }

};

class BubbleModelPrivate final
{
    SceneModel& scene;

    std::chrono::time_point<std::chrono::steady_clock> timestamp;

    const length_t radius;
    SharedRW<Coords2D> pos;
    Coords2D velocity;
    SharedRW<Coords2D> acc;

//    SharedListNode<BubbleModelPrivate*>* bubbleListEntry;

    friend class BubbleModel;

public:
    BubbleModelPrivate(SceneModel& scene, const Coords2D& pos, length_t r, const Coords2D& vel);
    BubbleModelPrivate() = delete;
    BubbleModelPrivate(const BubbleModelPrivate&) = delete;
    ~BubbleModelPrivate();

};

class BubbleModel final
{
    BubbleModelPrivate * d_ptr;
public:
    BubbleModel(SceneModel& scene, const Coords2D& pos, length_t r, const Coords2D& vel);
    BubbleModel() = delete;
    BubbleModel(const BubbleModel&) = delete;
    ~BubbleModel();

    void setPos(const Coords2D& p) { d_ptr->pos.set_master(p); d_ptr->scene.up2date.clear(); }
    void setVel(const Coords2D& vel) {d_ptr->velocity = vel; d_ptr->scene.up2date.clear();}
    Coords2D getVel();

};



#endif // MODEL_H
