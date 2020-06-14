#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "engine.h"
#include "listener.h"
#include "mat.h"

extern std::shared_ptr<control> ctrl;

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
        ray r = ctrl->get_ray();
        float sinb = -r.yd;
        float cosb = sqrtf(1 - sinb * sinb);
        float sina = r.xd / cosb; // WHAT IF cosb = 0 ???
        float cosa = -r.zd / cosb;
        float nx[3] = {cosa, 0, sina};
        float ny[3] = {sina * sinb, cosb, -cosa * sinb};
        std::cerr << ny[0] << ' ' << ny[1] << ' ' << ny[2] << std::endl;
        float nz[3] = {
            ny[1] * nx[2] - ny[2] * nx[1],
            ny[2] * nx[0] - ny[0] * nx[2],
            ny[0] * nx[1] - ny[1] * nx[0]
        };
        auto selected = sg.selected;
        float* vec = NULL;
        float coef;
        if (front_down) {
            vec = nz;
            coef = 0.05;
        }
        if (back_down) {
            vec = nz;
            coef = -0.05;
        }
        if (left_down) {
            vec = nx;
            coef = -0.05;
        }
        if (right_down) {
            vec = nx;
            coef = 0.05;
        }
        if (up_down) {
            vec = ny;
            coef = 0.05;
        }
        if (down_down) {
            vec = ny;
            coef = -0.05;
        }
        if (vec) {
            for (int i = 0; i < 3; ++i) {
                selected->translate[i] += vec[i] *coef;
            }
        }
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
            mat3 rot = mat3::rotate(5, nz[0], nz[1], nz[2]);
            selected->rotate_mat = rot * selected->rotate_mat;
        }
        if (yrot_down) {
            mat3 rot = mat3::rotate(5, ny[0], ny[1], ny[2]);
            selected->rotate_mat = rot * selected->rotate_mat;
        }
        if (zrot_down) {
            mat3 rot = mat3::rotate(5, nx[0], nx[1], nx[2]);
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
