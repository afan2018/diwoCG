#ifndef GEOMETRIES_H
#define GEOMETRIES_H

#include "scene_graph.h"
#include <GL/glut.h>

class box : public node {
    public:
        void render() {
            transform();
            glutSolidCube(0.5);
        }
};

#endif
