#include "light.h"

extern std::shared_ptr<light_env> lights;

bool light_env::keyboard_up(unsigned char key, int x, int y) {
    switch (key) {
        case 'k':
            lights_on = !lights_on;
            break;
        case 'l': {
            auto prefix = "light"s;
            if (std::equal(prefix.begin(), prefix.end(), sg.selected->type.begin())) {
                auto &lp = reinterpret_cast<std::shared_ptr<light> &>(sg.selected);
                lp->is_on = !lp->is_on;
            }
            break;
        }
    }
    return false;
}

void light_env::update() {
    if (lights_on) {
        glEnable(GL_LIGHTING);
    } else {
        glDisable(GL_LIGHTING);
    }
    lights_used = 0;
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        glDisable(GL_LIGHT0 + i);
    }
}

GLenum light_env::get_light_id() {
    if (lights_used == MAX_LIGHTS) {
        std::cerr << "No more than 8 lights";
        return 0;
    }
    return GL_LIGHT0 + (lights_used++);
}

light::light(std::string type) : node(std::move(type)) {
    scale[0] = 0.1f;
    scale[1] = 0.1f;
    scale[2] = 0.1f;
}

void light::render() {
    transform();
    colorize();
    glutSolidSphere(0.5f, 8, 6);
}

void light::update() {
    light_id = lights->get_light_id();
    if (is_on) glEnable(light_id);
    else glDisable(light_id);
    GLfloat position[] = {translate[0], translate[1], translate[2], 1.0f};
    glLightfv(light_id, GL_POSITION, position);
    glLightfv(light_id, GL_AMBIENT, ambient);
    glLightfv(light_id, GL_DIFFUSE, diffuse);
    glLightfv(light_id, GL_SPECULAR, specular);
    glLightf(light_id, GL_LINEAR_ATTENUATION, attenuation);
}

spotlight::spotlight() : light("light/spotlight") {}

void spotlight::update() {
    light::update();
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, angle);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, exponent);
}

point_light::point_light() : light("light/point_light") {
    diffuse[0] = 0.0f;
    diffuse[1] = 0.0f;
    diffuse[2] = 0.0f;
    emission[0] = 1.0f;
    emission[1] = 1.0f;
    emission[2] = 1.0f;
}