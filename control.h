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
        void motion(int x, int y);
        void mouse(int button, int state, int x, int y);

        int down_x, down_y;
        float down_alpha, down_beta;
        
        float alpha = 0.0f;
        float beta = 0.0f;
    public:
        orbit_control();
        virtual void update();
};

#endif
