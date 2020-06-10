#pragma once
#include <GL/gl.h>
#include <GL/glut.h>
#include <algorithm>
#include <ctime>
#include <cstdio>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "control.h"

class screenshot : public listener {
private:
    int sx, sy;
    int tx, ty;
    int height;
    bool in_ss_mode;
    void savepic();
public:
    screenshot();
    bool get_ss_mode();
    void clear();
    virtual void keyboard(unsigned char key, int x, int y);
    virtual void keyboard_up(unsigned char key, int x, int y);
    void mouse(int button, int state, int x, int y);
    void motion(int x, int y);
    void drawRect(int height);
};