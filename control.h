#ifndef CONTROL_H
#define CONTROL_H

#include <GL/glu.h>
#include <GL/glut.h>

class control {
    public:
        virtual void update() {}
};

class orbit_control : public control {
    private:
        static void motion_static(int x, int y);
        static void mouse_static(int button, int state, int x, int y);
        static void keyboard_static(unsigned char key, int x, int y);\
        static void keyboard_up_static(unsigned char key, int x, int y);
        void motion(int x, int y);
        void mouse(int button, int state, int x, int y);
        void keyboard(unsigned char key, int x, int y);
        void keyboard_up(unsigned char key, int x, int y);

        int down_x, down_y;
        float down_alpha, down_beta;
        bool w_down = false;
        bool s_down = false;
        bool a_down = false;
        bool d_down = false;

        float alpha = 0.0f;
        float beta = 0.0f;
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        void move(float angle, float dist);
    public:
        orbit_control();
        virtual void update();
};

#endif
