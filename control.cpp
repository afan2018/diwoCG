#include "control.h"

orbit_control* instance;

void orbit_control::motion_static(int x, int y) {
    if (instance) instance -> motion(x, y);
}

void orbit_control::mouse_static(int button, int state, int x, int y) {
    if (instance) instance -> mouse(button, state, x, y);
}

void orbit_control::motion(int x, int y) {
    int dx = x - down_x;
    int dy = y - down_y;
    alpha = down_alpha + dx * -0.23f;
    beta = down_beta + dy * -0.23f;
    if (beta < -90.0f) beta = -90.0f;
    if (beta > +90.0f) beta = +90.0f;
}

void orbit_control::mouse(int button, int state, int x, int y) {
    down_x = x;
    down_y = y;
    down_alpha = alpha;
    down_beta = beta;
}

orbit_control::orbit_control() {
    // TODO: register glut event listeners
    instance = this;
    glutMotionFunc(&motion_static);
    glutMouseFunc(&mouse_static);
}

void orbit_control::update() {
    glRotatef(beta, 1.0f, 0.0f, 0.0f);
    glRotatef(alpha, 0.0f, 1.0f, 0.0f);
}
