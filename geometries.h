#ifndef GEOMETRIES_H
#define GEOMETRIES_H

#include "scene_graph.h"
#include <GL/glut.h>

class box : public node {
    public:
        void render() {
            transform();
            glColor3f(1.0f, 0.0f, 0.0f);
            glutSolidCube(0.5f);
        }
};

#endif
