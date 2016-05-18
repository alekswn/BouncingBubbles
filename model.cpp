#include "model.h"
#if 0
static void doWork(SharedList list) {
    while (list.isLive()) {
        list.doDelete();
        //TODO :  do update
    }
}

Model::Model(const SharedList& list)
    : worker(doWork(list))
{}

Model::~Model() { worker.join(); }

#endif
