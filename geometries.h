#ifndef GEOMETRIES_H
#define GEOMETRIES_H

#include "scene_graph.h"
#include <GL/glut.h>

class box : public node {
    public:
        // TODO: add parameters

        void render() {
            transform();
            // TODO: read parameters and render, you might want
            // to rewrite glutSolidCube()
            glColor3f(1.0f, 0.0f, 0.0f);
            glutSolidCube(0.5f);
        }
};

// TODO: add more geometries

#endif
