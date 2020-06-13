#ifndef CONTROL_H
#define CONTROL_H

#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#include <gl/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif
#include <memory>

#include "scene_graph.h"
#include "listener.h"
#include "engine.h"

class control : public listener, public actor {
    public:
        virtual ray get_ray() = 0;
};

class orbit_control : public control {
    private:
        int down_x          = {};
        int down_y          = {};
        float down_alpha    = {};
        float down_beta     = {};
        ray down_ray        = {};
        mat3 down_mat       = {};
        float down_pos[3]   = {};
        float g_down_alpha  = {};
        float g_down_beta   = {};
        std::shared_ptr<node> down_node;

        bool w_down = false;
        bool s_down = false;
        bool a_down = false;
        bool d_down = false;
        bool z_down = false;
        bool c_down = false;
        bool g_down = false;

        float alpha = 0.0f;
        float beta  = 0.0f;
        float x     = 0.0f;
        float y     = 0.0f;
        float z     = 0.0f;

        void move(float angle, float dist);
    public:
        orbit_control() = default;
        void update() override;
        ray get_ray() override;

        bool motion(int mx, int my) override;
        bool mouse(int button, int state, int mx, int my) override;
        bool keyboard(unsigned char key, int x, int y) override;
        bool keyboard_up(unsigned char key, int x, int y) override;
};
#endif
