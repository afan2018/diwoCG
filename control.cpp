#include "control.h"

#include <cmath>
#ifndef M_PI
#define M_PI 3.1415926535
#endif

#define _x 0
#define _y 1
#define _z 2

bool orbit_control::motion(int mx, int my) {
    int dx = mx - down_x;
    int dy = my - down_y;
    float sx = 110.0f / (float)w_width;
    float sy = 110.0f / (float)w_height;
    alpha = down_alpha - (float)dx * sx;
    beta  = down_beta  - (float)dy * sy;
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
        case 'f': f_down = true; break;
        case 'g': {
            g_down = true;
            down_node = sg.selected;
            if (down_node) {
                down_ray = get_ray();
                down_mat = down_node->rotate_mat;
                down_pos[0] = down_node->translate[0];
                down_pos[1] = down_node->translate[1];
                down_pos[2] = down_node->translate[2];
                g_down_alpha = alpha;
                g_down_beta = beta;
            }
            break;
        }
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
        case 'f': f_down = false; break;
        case 'r': {
            // center selection
            if (!sg.selected) break;
            auto a = sg.selected->get_aabb();
            float dx = a.x1 - a.x0;
            float dy = a.y1 - a.y0;
            float dz = a.z1 - a.z0;
            float size = (std::max)(dx, (std::max)(dy, dz));
            float dist = size * 2.6f;
            ray r = get_ray();
            x = (a.x0 + a.x1) / 2 - r.xd * dist;
            y = (a.y0 + a.y1) / 2 - r.yd * dist;
            z = (a.z0 + a.z1) / 2 - r.zd * dist;
            break;
        }
        case 'g': g_down = false; break;
        case ' ': {
            sg.selected = sg.hovered;
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
    if (f_down && sg.selected) {
        // first, motion() updates alpha and beta, so we need to modify x, y and z accordingly
        float dx = sg.selected->translate[_x] - x;
        float dy = sg.selected->translate[_y] - y;
        float dz = sg.selected->translate[_z] - z;
        float dist = std::sqrt(dx * dx + dy * dy + dz * dz);
        ray r = get_ray();
        x = sg.selected->translate[_x] - r.xd * dist;
        y = sg.selected->translate[_y] - r.yd * dist;
        z = sg.selected->translate[_z] - r.zd * dist;
    }

    if (w_down) move( 90.0f, 0.1f);
    if (s_down) move(270.0f, 0.1f);
    if (a_down) move(180.0f, 0.1f);
    if (d_down) move(  0.0f, 0.1f);
    if (c_down) y += 0.1f;
    if (z_down) y -= 0.1f;
    if (f_down && sg.selected) {
        // second, after moving, the ray should still point at the object, where
        // ray = {
        //        x, y, z,
        //        +1.0f * sinf(alpha_rad) * cosf(beta_rad),
        //        -1.0f                   * sinf(beta_rad),
        //        -1.0f * cosf(alpha_rad) * cosf(beta_rad)
        // }
        float dx = sg.selected->translate[_x] - x;
        float dy = sg.selected->translate[_y] - y;
        float dz = sg.selected->translate[_z] - z;
        float dist = std::sqrt(dx * dx + dy * dy + dz * dz);
        float beta_rad = asinf(-dy / dist);
        float ddx = +dx / dist / cosf(beta_rad);
        float ddz = -dz / dist / cosf(beta_rad);
        float alpha_rad = atan2f(ddx, ddz);
        alpha = alpha_rad / (float)M_PI * 180.0f;
        beta = beta_rad / (float)M_PI * 180.0f;
    }

    if (!f_down && g_down && down_node) {
        ray r = get_ray();
        // world -> ray
        auto ro = mat3::rotate(g_down_beta, 1.0f, 0.0f, 0.0f) * mat3::rotate(g_down_alpha, 0.0f, 1.0f, 0.0f);
        auto rn = mat3::rotate(beta, 1.0f, 0.0f, 0.0f) * mat3::rotate(alpha, 0.0f, 1.0f, 0.0f);
        // dp in world
        vec3 dpo = {
                down_pos[_x] - down_ray.x0,
                down_pos[_y] - down_ray.y0,
                down_pos[_z] - down_ray.z0
        };
        vec3 dpn = ~rn * ro * dpo;
        // back to node
        down_node->translate[_x] = r.x0 + dpn.data[_x];
        down_node->translate[_y] = r.y0 + dpn.data[_y];
        down_node->translate[_z] = r.z0 + dpn.data[_z];
        down_node->rotate_mat = (~rn * ro) * down_mat;
    }

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