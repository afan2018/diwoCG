#pragma once
#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <algorithm>
#include <ctime>
#include <cstdio>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "control.h"

class screenshot : public listener
{
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

    bool keyboard(unsigned char key, int x, int y) override;
    bool keyboard_up(unsigned char key, int x, int y) override;
    bool mouse(int button, int state, int x, int y) override;
    bool motion(int x, int y) override;
    
    void drawRect(int height);
};