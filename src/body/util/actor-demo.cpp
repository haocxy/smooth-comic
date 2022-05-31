#include "actor-demo.h"

namespace myapp::demo::actor {

void actor_demo_01()
{
    CalcActor calcActor;
    AskActor askActor(calcActor);

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

}

