#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif

#include "listener.h"

class screenshot : public listener {
private:
    int sx, sy;
    int tx, ty;
    bool in_ss_mode;
    void savepic() const;

public:
    screenshot();
    bool get_ss_mode() const;
    void clear();

    bool keyboard(unsigned char key, int x, int y) override;
    bool keyboard_up(unsigned char key, int x, int y) override;
    bool mouse(int button, int state, int x, int y) override;
    bool motion(int x, int y) override;

    void drawRect() const;
};

#endif
