#pragma once
#include <cinttypes>
#include "containers.h"

using AssetID = uint32_t;

class Drawable {
public:
    AssetID texture_id;
    Transform* transform;
    int layer_index = 0;
};

struct less_than_compare_key_Drawable {
    inline bool operator() (const Drawable& d1, const Drawable& d2)
    {
        return (d1.layer_index < d2.layer_index);
    }
};

