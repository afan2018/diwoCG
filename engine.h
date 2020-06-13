#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#include "scene_graph.h"

class camera {
    public:
        virtual void transform(int width, int height) = 0;
};

class perspective_camera : public camera {
    public:
        GLfloat fovy    = 60.0f;
        GLfloat zNear   = 0.01f;
        GLfloat zFar    = 1000.0f;

        void transform(int width, int height) override {
            GLfloat aspect = (GLfloat)width / (GLfloat)height;
            gluPerspective(fovy, aspect, zNear, zFar);
        }
};

class actor {
public:
    virtual void update() {}
};

extern scene_graph sg;
extern int w_width, w_height;
extern void init();

#endif
