#include "screenshot.h"

#include <algorithm>
#include <ctime>
#include <cstdio>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

extern int w_width, w_height;

void screenshot::savepic() const {
    time_t now = time(nullptr);
    char filename[250];
    struct tm *t = gmtime(&now);
    sprintf(filename, "Screenshot_at_%d_%d_%d_%d_%d%d.bmp", t->tm_year + 1900,
            t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec); //, ctime(&now));
    int lx = (std::min)(sx, tx), rx = (std::max)(sx, tx);
    int ly = (std::min)(sy, ty), ry = (std::max)(sy, ty);
    int w = rx - lx, h = ry - ly;
    w = (std::max)(w - 6, 0);
    h = (std::max)(h - 6, 0);
    auto *pic = new unsigned char[w * h * 4];
    glReadPixels(lx + 3, w_height - ry + 3, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pic);
    stbi_write_bmp(filename, w, h, 4, pic);
}

screenshot::screenshot() {
    sx = sy = tx = ty = 0;
    in_ss_mode = false;
}

bool screenshot::get_ss_mode() const {
    return in_ss_mode;
}

void screenshot::clear() {
    sx = sy = tx = ty = 0;
}

bool screenshot::keyboard(unsigned char key, int x, int y) {
    if (key == 'p')
        in_ss_mode = true;
    return false;
}

bool screenshot::keyboard_up(unsigned char key, int x, int y) {
    if (key == 'p')
        in_ss_mode = false;
    return false;
}

bool screenshot::mouse(int button, int state, int x, int y) {
    if (!in_ss_mode)
        return false;
    x = (std::min)(x, w_width);
    x = (std::max)(x, 0);
    y = (std::min)(y, w_height);
    y = (std::max)(y, 0);
    if (state == GLUT_DOWN) {
        sx = x;
        sy = y;
        tx = x;
        ty = y;
    } else {
        savepic();
        sx = sy = tx = ty = 0;
    }
    return true;
}

bool screenshot::motion(int x, int y) {
    if (!in_ss_mode)
        return false;
    x = (std::min)(x, w_width);
    x = (std::max)(x, 0);
    y = (std::min)(y, w_height);
    y = (std::max)(y, 0);
    tx = x;
    ty = y;
    return true;
}

void screenshot::drawRect() const {
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_POLYGON);
    int lx = (std::min)(sx, tx), rx = (std::max)(sx, tx);
    int ly = (std::min)(sy, ty), ry = (std::max)(sy, ty);
    glVertex2d(lx, w_height - ly);
    glVertex2d(lx, w_height - ry);
    glVertex2d(rx, w_height - ry);
    glVertex2d(rx, w_height - ly);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}