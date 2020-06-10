#ifndef CONTROL_H
#define CONTROL_H

#include <windows.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "scene_graph.h"
#include "listener.h"

class control : public listener {
    public:
        virtual void update() {}
        virtual ray get_ray() = 0;
};

class orbit_control : public control {
    private:
        int down_x, down_y;
        float down_alpha, down_beta;
        bool w_down = false;
        bool s_down = false;
        bool a_down = false;
        bool d_down = false;
        bool other_down = false;

        float alpha = 0.0f;
        float beta = 0.0f;
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        void move(float angle, float dist);
    public:
        orbit_control() = default;
        void update() override;
        ray get_ray() override;

        bool motion(int x, int y) override;
        bool mouse(int button, int state, int x, int y) override;
        bool keyboard(unsigned char key, int x, int y) override;
        bool keyboard_up(unsigned char key, int x, int y) override;
};
#endif
