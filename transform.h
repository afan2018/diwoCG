#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "engine.h"
#include "listener.h"
#include "mat.h"

class transformation : public listener, public actor {
    bool front_down = false;
    bool back_down  = false;
    bool left_down  = false;
    bool right_down = false;
    bool up_down    = false;
    bool down_down  = false;
    bool add_down   = false;
    bool sub_down   = false;
    bool xrot_down  = false;
    bool yrot_down  = false;
    bool zrot_down  = false;
    
public:
    void update() override {
        if (!sg.selected) return;
        auto selected = sg.selected;
        if (front_down) selected->translate[2] -= 0.05f;
        if (back_down)  selected->translate[2] += 0.05f;
        if (left_down)  selected->translate[0] -= 0.05f;
        if (right_down) selected->translate[0] += 0.05f;
        if (up_down)    selected->translate[1] += 0.05;
        if (down_down)  selected->translate[1] -= 0.05f;
        if (add_down) {
            selected->scale[0] += 0.05f;
            selected->scale[1] += 0.05f;
            selected->scale[2] += 0.05f;
        }
        if (sub_down) {
            selected->scale[0] -= 0.05f;
            if (selected->scale[0] < 1e-5) selected->scale[0] = 0;
            selected->scale[1] -= 0.05f;
            if (selected->scale[1] < 1e-5) selected->scale[1] = 0;
            selected->scale[2] -= 0.05f;
            if (selected->scale[2] < 1e-5) selected->scale[2] = 0;
        }
        if (xrot_down) {
            mat3 rot{
                    1, 0, 0,
                    0, cosf(0.1), -sinf(0.1),
                    0, sinf(0.1), cosf(0.1)
            };
            selected->rotate_mat = rot * selected->rotate_mat;
        }
        if (yrot_down) {
            mat3 rot{
                    cosf(0.1), 0, sinf(0.1),
                    0, 1, 0,
                    -sinf(0.1), 0, cosf(0.1)
            };
            selected->rotate_mat = rot * selected->rotate_mat;
        }
        if (zrot_down) {
            mat3 rot{
                    cosf(0.1), -sinf(0.1), 0,
                    sinf(0.1), cosf(0.1), 0,
                    0, 0, 1
            };
            selected->rotate_mat = rot * selected->rotate_mat;
        }
    }

    bool keyboard(unsigned char key, int x, int y) override {
        switch (key) {
            case ',':
                up_down = true;
                break;
            case '.':
                down_down = true;
                break;
            case '=':
            case '+':
                add_down = true;
                break;
            case '-':
            case '_':
                sub_down = true;
                break;
            case 'u':
                xrot_down = true;
                break;
            case 'i':
                yrot_down = true;
                break;
            case 'o':
                zrot_down = true;
                break;
            default:
                break;
        }
        return false;
    }

    bool keyboard_up(unsigned char key, int x, int y) override {
        switch (key) {
            case ',':
                up_down = false;
                break;
            case '.':
                down_down = false;
                break;
            case '=':
            case '+':
                add_down = false;
                break;
            case '-':
            case '_':
                sub_down = false;
                break;
            case 'u':
                xrot_down = false;
                break;
            case 'i':
                yrot_down = false;
                break;
            case 'o':
                zrot_down = false;
                break;
            default:
                break;
        }
        return false;
    }

    bool special_key(int key, int x, int y) override {
        switch (key) {
            case GLUT_KEY_UP:
                front_down = true;
                break;
            case GLUT_KEY_DOWN:
                back_down = true;
                break;
            case GLUT_KEY_LEFT:
                left_down = true;
                break;
            case GLUT_KEY_RIGHT:
                right_down = true;
                break;
            default:
                break;
        }
        return false;
    }

    bool special_key_up(int key, int x, int y) override {
        switch (key) {
            case GLUT_KEY_UP:
                front_down = false;
                break;
            case GLUT_KEY_DOWN:
                back_down = false;
                break;
            case GLUT_KEY_LEFT:
                left_down = false;
                break;
            case GLUT_KEY_RIGHT:
                right_down = false;
                break;
            default:
                break;
        }
        return false;
    }
};

#endif
