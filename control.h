#ifndef CONTROL_H
#define CONTROL_H

#include <GL/glu.h>

class control {
    public:
        virtual void update() {}
};

class orbit_control : public control {
    public:
        orbit_control() {
            // TODO: register glut event listeners
        }

        virtual void update() {
            // TODO: update directions and gluLookAt()
            gluLookAt(
                0, 0, -2,
                0, 0, 0,
                0, 1, 0
            );
        }
};

#endif
