#ifndef SIM_PLANET_H_INCLUDED
#define SIM_PLANET_H_INCLUDED

#include "Body.h"

class Context;

namespace sim {
    class Planet: public Body {
    public:
        Planet();
        void render(Context &c) override;
    };
}

#endif
