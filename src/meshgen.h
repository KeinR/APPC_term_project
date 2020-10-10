#ifndef MESHGEN_H_INCLUDED
#define MESHGEN_H_INCLUDED

#include "Mesh.h"

namespace mg {
    // Should re-use meshes - therefore, should only be called once!
    Mesh sphere(unsigned int quality = 40);
    Mesh cube();
}

#endif
