#include "control.h"

#include <cmath>
#ifndef M_PI
#define M_PI 3.1415926535
#endif

#include "engine.h"

bool orbit_control::motion(int mx, int my) {
    int dx = mx - down_x;
    int dy = my - down_y;
    alpha = down_alpha + (float)dx * -0.23f;
    beta  = down_beta  + (float)dy * -0.23f;
    if (beta < -90.0f) beta = -90.0f;
    if (beta > +90.0f) beta = +90.0f;
    return false;
}

bool orbit_control::mouse(int button, int state, int mx, int my) {
    down_x = mx;
    down_y = my;
    down_alpha = alpha;
    down_beta = beta;
    return false;
}

bool orbit_control::keyboard(unsigned char key, int mx, int my) {
    switch (key) {
        case 'w': w_down = true; break;
        case 's': s_down = true; break;
        case 'a': a_down = true; break;
        case 'd': d_down = true; break;
        case 'c': c_down = true; break;
        case 'z': z_down = true; break;
    }
    return false;
}

bool orbit_control::keyboard_up(unsigned char key, int mx, int my) {
    switch (key) {
        case 'w': w_down = false; break;
        case 's': s_down = false; break;
        case 'a': a_down = false; break;
        case 'd': d_down = false; break;
        case 'c': c_down = false; break;
        case 'z': z_down = false; break;
        case ' ': {
            // center selection
            if (!sg.selected) break;
            auto a = sg.selected->get_aabb();
            float dx = a.x1 - a.x0;
            float dy = a.y1 - a.y0;
            float dz = a.z1 - a.z0;
            float size = std::max(dx, std::max(dy, dz));
            float dist = size * 2.6f;
            ray r = get_ray();
            x = (a.x0 + a.x1) / 2 - r.xd * dist;
            y = (a.y0 + a.y1) / 2 - r.yd * dist;
            z = (a.z0 + a.z1) / 2 - r.zd * dist;
            break;
        }
    }
    return false;
}

void orbit_control::move(float angle, float dist) {
    float rad = (angle - alpha) / 180.0f * (float)M_PI;
    x += dist * cosf(rad);
    z -= dist * sinf(rad);
}

void orbit_control::update() {
    if (w_down) move( 90.0f, 0.1f);
    if (s_down) move(270.0f, 0.1f);
    if (a_down) move(180.0f, 0.1f);
    if (d_down) move(  0.0f, 0.1f);
    if (c_down) y += 0.1f;
    if (z_down) y -= 0.1f;
    glRotatef(beta, 1.0f, 0.0f, 0.0f);
    glRotatef(alpha, 0.0f, 1.0f, 0.0f);
    glTranslatef(-x, -y, -z);
}

ray orbit_control::get_ray() {
    float alpha_rad = alpha / 180.0f * (float)M_PI;
    float beta_rad = beta / 180.0f * (float)M_PI;
    return {
        x, y, z,
        +1.0f * sinf(alpha_rad) * cosf(beta_rad),
        -1.0f                   * sinf(beta_rad),
        -1.0f * cosf(alpha_rad) * cosf(beta_rad)
    };
}