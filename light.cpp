#include "light.h"
#include <cctype>
#include <iostream>

light::light() {
    ambient = specular = diffuse = position = NULL;
    is_on = false;
}

void light::init(GLenum id, GLfloat* ambient, GLfloat* specular, GLfloat* diffuse, GLfloat* position) {
    this->id = id;
    if (ambient) {
        this->ambient = new GLfloat[3];
        memcpy(this->ambient, ambient, sizeof(GLfloat) * 3);
    }
    if (diffuse) {
        this->diffuse = new GLfloat[3];
        memcpy(this->diffuse, diffuse, sizeof(GLfloat) * 3);
    }
    if (specular) {
        this->specular = new GLfloat[3];
        memcpy(this->specular, specular, sizeof(GLfloat) * 3);
    }
    this->position = new GLfloat[3];
    memcpy(this->position, position, sizeof(GLfloat) * 3);
    base_aabb = {-0.1, 0.1, -0.1, 0.1, -0.1, 0.1};
    memcpy(center, this->position, sizeof(GLfloat) * 3);
}

void light::setbasic() {
    glLightfv(id, GL_POSITION, position);
    if (ambient) glLightfv(id, GL_AMBIENT, ambient);
    if (diffuse) glLightfv(id, GL_AMBIENT, diffuse);
    if (specular) glLightfv(id, GL_SPECULAR, specular);
    is_on = true;
}

void light::reset() {
    is_on = false;
    glDisable(id);
}

void light::flip() {
    if (is_on == true) reset();
    else set();
}

bool light::get_status() {
    return is_on;
}

spotlight::spotlight(GLenum id, GLfloat* ambient, GLfloat* specular, GLfloat* diffuse, GLfloat* position, GLfloat angle, GLfloat exponent, GLfloat* direction) {
    init(id, ambient, specular, diffuse, position);
    this->angle = angle;
    this->exponent = exponent;
    memcpy(this->direction, direction, sizeof(GLfloat) * 3);
}

void spotlight::set() {
    setbasic();
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, angle);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, exponent);
    glEnable(id);
}

normallight::normallight(GLenum id, GLfloat* ambient, GLfloat* specular, GLfloat* diffuse, GLfloat* position) {
    init(id, ambient, specular, diffuse, position);
}

bool light_env::keyboard(unsigned char key, int x, int y) {
    if (key != 'k' && key != 'l' && (!isdigit(key) || (isdigit(key) && key > '8'))) return false;
    if (key == 'l') {
        in_light_mode = true;
    } else if (key == 'k') {
        if (lights_on) {
            glDisable(GL_LIGHTING);
        } else {
            glEnable(GL_LIGHTING);
        }
        lights_on = !lights_on;
    } else {
        if (!in_light_mode) return false;
        int x = key - '1';
        if (x < lights.size()) {
            lights[x]->flip();
        }
    }
    return true;
}

bool light_env::keyboard_up(unsigned char key, int x, int y) {
    if (key != 'k' && key != 'l' && (!isdigit(key) || (isdigit(key) && key > '7'))) return false;
    if (key == 'l') {
        in_light_mode = false;
    } else if (key == 'k') {
        return true;
    } else {
        if (!in_light_mode) return false;
    }
    return true;
}

bool light_env::mouse(int button, int state, int x, int y) {
    return in_light_mode;
}

bool light_env::motion(int x, int y) {
    return in_light_mode;
}

std::shared_ptr<light> light_env::addlight(GLfloat* ambient, GLfloat* specular, GLfloat* diffuse, GLfloat* position, GLfloat angle, GLfloat exponent, GLfloat* direction) {
    if (lights.size() == 8) {
        std::cerr << "No more than 8 lights";
        return nullptr;
    }
    if (direction) {
        auto lp = std::make_shared<spotlight>(GL_LIGHT0 + lights.size(), ambient, specular, diffuse, position, angle, exponent, direction);
        lights.push_back(lp);
        return lp;
    }

    auto lp = std::make_shared<normallight>(GL_LIGHT0 + lights.size(), ambient, specular, diffuse, position);
    lights.push_back(lp);
    return lp;
}