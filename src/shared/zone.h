#ifndef ZONE_H
#define ZONE_H
#include <support/json.h>
#include "../engine/engine.h"
#include "cube.h"
#include <string.h>
#include <sodium.h>
vector<zone> zones;
namespace zone {
    using namespace game;
    struct worldzone {
        string name;
        string desc;
        float gravity;
    };
}
#endif //ZONE_H