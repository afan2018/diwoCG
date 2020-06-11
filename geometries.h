#ifndef GEOMETRIES_H
#define GEOMETRIES_H

#include "scene_graph.h"
#include <windows.h>
#include <GL/glut.h>

class box : public node {
    public:
        // TODO: add parameters
        GLfloat color[3] = { 1.0f, 1.0f, 1.0f };

        void render() {
            transform();
            // TODO: read parameters and render, you might want
            // to rewrite glutSolidCube()
            glColor3fv(color);
            glutSolidCube(1.0f);
        }
};

// TODO: add more geometries

#endif
