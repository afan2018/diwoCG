#pragma once
#include "engine.h"

class listener {
public:
    listener();
    virtual void motion(int x, int y) {}
    virtual void mouse(int button, int state, int x, int y) {}
    virtual void keyboard(unsigned char key, int x, int y) {}
    virtual void keyboard_up(unsigned char key, int x, int y) {}
};

extern std::vector<listener*> vlistener;