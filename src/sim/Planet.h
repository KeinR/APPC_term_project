#ifndef SIM_PLANET_H_INCLUDED
#define SIM_PLANET_H_INCLUDED

#include <memory>

#include "Body.h"
#include "../Texture.h"

class Context;

namespace sim {
    class Planet: public Body {
        std::shared_ptr<Texture> texture;
        float scale;
    public:
        Planet(const std::shared_ptr<Texture> &texture, float scale);
        void render(Context &c) override;
    };
}

#endif
