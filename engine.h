#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "listener.h"
#include "scene_graph.h"
#include "control.h"
#include "screenshot.h"

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

extern std::vector<std::shared_ptr<listener>> listeners;
extern std::shared_ptr<camera> cam;
extern std::shared_ptr<control> ctrl;
extern std::shared_ptr<screenshot> ss;
extern scene_graph sg;
extern int w_width, w_height;
extern void init();
extern void update();

#endif
