#include <chrono>
#include <cassert>
#include "model.h"

#include <QDebug>

void SceneModel::doWork() {
    while(!isStopped) {
        auto scheduledUpdate = std::chrono::steady_clock::now() + updateInterval;
        if (!up2date.test_and_set()) {

        }
        std::this_thread::sleep_until(scheduledUpdate);
    }
}

SceneModel::SceneModel(double ui)
    : worker(nullptr), isStopped(true), up2date(ATOMIC_FLAG_INIT), updateInterval(ui)
//      ,bubbleListDelimiter()
{
    assert(isStopped.is_lock_free());
}

SceneModel::~SceneModel()
{
    isStopped = true;
    if(worker && worker->joinable()) worker->join();
//    bubbleListDelimiter.destroy();
}

void SceneModel::start() {
    if (!isStopped) return;
    isStopped = false;
    worker = new std::thread(&SceneModel::doWork,this);
}


BubbleModelPrivate::BubbleModelPrivate(SceneModel &scene, const Coords2D& pos, length_t r, const Coords2D& vel)
    : scene(scene), timestamp(std::chrono::steady_clock::now()),
      radius(r), pos(pos), velocity(vel), acc(Coords2D())
//      ,bubbleListEntry(nullptr)
{
}
BubbleModel::BubbleModel(SceneModel &scene, const Coords2D& pos, length_t r, const Coords2D& vel)
    : d_ptr(new BubbleModelPrivate(scene,pos,r,vel))
{
//    d_ptr->bubbleListEntry = d_ptr->scene.bubbleListDelimiter.insert(d_ptr);
    d_ptr->scene.up2date.clear();

}

BubbleModel::~BubbleModel() {
//    d_ptr->bubbleListEntry->erase();
    d_ptr->scene.up2date.clear();
}

Coords2D BubbleModel::getVel() {
    auto now =  std::chrono::steady_clock::now();
    std::chrono::duration<length_t> diff = now - d_ptr->timestamp;
    length_t step = diff.count();
    d_ptr->timestamp = now;
    Coords2D accelerate = d_ptr->acc.get_slave();
    return d_ptr->velocity+=accelerate*step;
}
