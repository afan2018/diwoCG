#pragma once
#include "scene_graph.h"
#include "engine.h"
#include "listener.h"
#include "mat.h"
#include <iostream>

class trans : public listener {
        bool front_down;
        bool back_down;
        bool left_down;
        bool right_down;
        bool up_down;
        bool down_down;
        bool add_down;
        bool sub_down;
        bool xrot_down;
        bool yrot_down;
        bool zrot_down;
    public:
        trans() {
            front_down = back_down = left_down = right_down = up_down = down_down = false;
            add_down = sub_down = xrot_down = yrot_down = zrot_down = false;
        }
        void update() {
            if (sg.selected == nullptr) return;
            if (front_down) sg.selected->translate[2] -= 0.05f;
            if (back_down) sg.selected->translate[2] += 0.05f;
            if (left_down) sg.selected->translate[0] -= 0.05f;
            if (right_down) sg.selected->translate[0] += 0.05f;
            if (up_down) sg.selected->translate[1] += 0.05;
            if (down_down) sg.selected->translate[1] -= 0.05f;
            if (add_down) {
                sg.selected->scale[0] += 0.05f;
                sg.selected->scale[1] += 0.05f;
                sg.selected->scale[2] += 0.05f;
            }
            if (sub_down) {
                sg.selected->scale[0] -= 0.05f;
                if (sg.selected->scale[0] < 1e-5) sg.selected->scale[0] = 0;
                sg.selected->scale[1] -= 0.05f;
                if (sg.selected->scale[1] < 1e-5) sg.selected->scale[1] = 0;
                sg.selected->scale[2] -= 0.05f;
                if (sg.selected->scale[2] < 1e-5) sg.selected->scale[2] = 0;
            }
            if (xrot_down) {
                mat3 rot;
                GLfloat mat[9] =
                    {
                        1, 0, 0,
                        0, cosf(0.1), -sinf(0.1),
                        0, sinf(0.1), cosf(0.1)
                    };
                memcpy(rot.data, mat, sizeof(GLfloat) * 9);
                sg.selected->rotate_mat = rot * sg.selected->rotate_mat;
            }
            if (yrot_down) {
                mat3 rot;
                GLfloat mat[9] =
                    {
                        cosf(0.1), 0, sinf(0.1),
                        0, 1, 0,
                        -sinf(0.1), 0, cosf(0.1)
                    };
                memcpy(rot.data, mat, sizeof(GLfloat) * 9);
                sg.selected->rotate_mat = rot * sg.selected->rotate_mat;
            }
            if (zrot_down) {
                mat3 rot;
                GLfloat mat[9] =
                    {
                        cosf(0.1), -sinf(0.1), 0,
                        sinf(0.1), cosf(0.1), 0,
                        0, 0, 1
                    };
                memcpy(rot.data, mat, sizeof(GLfloat) * 9);
                sg.selected->rotate_mat = rot * sg.selected->rotate_mat;
            }

        }
        bool keyboard(unsigned char key, int x, int y) override {
            switch (key) {
                case ',': up_down = true; std::cerr << "FUCK" << std::endl;break;
                case '.': down_down = true; break;
                case '=': case '+': add_down = true; break;
                case '-': case '_': sub_down = true; break;
                case 'u': xrot_down = true; break;
                case 'i': yrot_down = true; break;
                case 'o': zrot_down = true; break;
                default: break;
            }
            return false;
        }
        bool keyboard_up(unsigned char key, int x, int y) override {
            switch (key) {
                case ',': up_down = false; break;
                case '.': down_down = false; break;
                case '=': case '+': add_down = false; break;
                case '-': case '_': sub_down = false; break;
                case 'u': xrot_down = false; break;
                case 'i': yrot_down = false; break;
                case 'o': zrot_down = false; break;
                default: break;
            }
            return false;
        }
        bool specialkey(int key, int x, int y) override {
            switch (key) {
                case GLUT_KEY_UP: front_down = true; break;
                case GLUT_KEY_DOWN: back_down = true;  break;
                case GLUT_KEY_LEFT: left_down =  true; break;
                case GLUT_KEY_RIGHT: right_down = true;  break;
                default:  break;
            }
            return false;
        }
        bool specialkey_up(int key, int x, int y) override {
            switch (key) {
                case GLUT_KEY_UP: front_down = false; break;
                case GLUT_KEY_DOWN: back_down = false;  break;
                case GLUT_KEY_LEFT: left_down =  false; break;
                case GLUT_KEY_RIGHT: right_down = false;  break;
                default:  break;
            }
            return false;
        }
};