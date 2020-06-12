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
    GLfloat ambient [3] = { 1.0f, 1.0f, 0.0f };
    GLfloat diffuse [3] = { 1.0f, 1.0f, 1.0f };
    GLfloat specular[3] = { 0.0f, 0.0f, 0.0f };
    GLenum lightId{};
    bool is_on = false;

    void render() override {
        transform();
        colorize();
        glutSolidTeapot(0.2);
    }

    void update() override {
        if (is_on) glEnable(lightId);
        else glDisable(lightId);
        glLightfv(lightId, GL_POSITION, translate);
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
};

class light_env : public node, public listener {
private:
    bool lights_on = false;
    std::vector<std::shared_ptr<light>> lights;

public:
    light_env() = default;

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