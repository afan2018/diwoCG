#ifndef LIGHT_H
#define LIGHT_H

#include <vector>
#ifdef _WIN32
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#include "listener.h"
#include "engine.h"

class light : public node {
public:
    GLfloat ambient [4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat diffuse [4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat specular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLenum lightId{};
    bool is_on = true;

    light() {
        scale[0] = 0.1f;
        scale[1] = 0.1f;
        scale[2] = 0.1f;
    }

    void render() override {
        transform();
        colorize();
        glutSolidSphere(0.5f, 8, 6);
    }

    void update() override {
        if (is_on) glEnable(lightId);
        else glDisable(lightId);
        GLfloat position[] = { translate[0], translate[1], translate[2], 1.0f };
        glLightfv(lightId, GL_POSITION, position);
        glLightfv(lightId, GL_AMBIENT, ambient);
        glLightfv(lightId, GL_DIFFUSE, diffuse);
        glLightfv(lightId, GL_SPECULAR, specular);
    }
};

class spotlight : public light {
public:
    GLfloat angle{};
    GLfloat exponent{};
    GLfloat direction[3] = {};

    void update() override {
        light::update();
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, angle);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, exponent);
    }
};

class point_light : public light {
public:
    point_light() {
        diffuse[0] = 0.0f;
        diffuse[1] = 0.0f;
        diffuse[2] = 0.0f;
        emission[0] = 1.0f;
        emission[1] = 1.0f;
        emission[2] = 1.0f;
    }
};

class light_env : public node, public listener {
private:
    bool lights_on = true;
    std::vector<std::shared_ptr<light>> lights;

public:
    light_env() {
        interactive = false;
    }

    bool keyboard_up(unsigned char key, int x, int y) override {
        switch (key) {
            case 'k': lights_on = !lights_on; break;
            case 'l': {
                for (auto &lp : lights) {
                    if (lp == sg.selected) {
                        lp->is_on = !lp->is_on;
                    }
                }
                break;
            }
        }
        return false;
    }

    void update() override {
        if (lights_on) {
            glEnable(GL_LIGHTING);
        } else {
            glDisable(GL_LIGHTING);
        }
    }

    void add_light(const std::shared_ptr<light>& lp) {
        if (lights.size() == 8) {
            std::cerr << "No more than 8 lights";
        }

        lp->lightId = GL_LIGHT0 + lights.size();
        lights.push_back(lp);
    }
};

#endif