#ifndef LIGHT_H
#define LIGHT_H

#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#ifdef _WIN32
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#include "listener.h"
#include "engine.h"

#define MAX_LIGHTS 8

class light_env : public actor, public listener {
private:
    bool lights_on = true;
    int lights_used = 0;

public:
    bool keyboard_up(unsigned char key, int x, int y) override;
    void update() override;
    GLenum get_light_id();
};

class light : public node {
public:
    GLfloat ambient [4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat diffuse [4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat specular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLenum  light_id{};
    bool is_on = true;

    explicit light(std::string type = "light");
    void render() override;
    void update() override;
};

class spotlight : public light {
public:
    GLfloat angle{};
    GLfloat exponent{};
    GLfloat direction[3] = {};

    spotlight();
    void update() override;
};

class point_light : public light {
public:
    point_light();
};

#endif