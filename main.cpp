#include "engine.h"
#include "geometries.h"
#include "obj_mesh.h"
#include "nurbs_mesh.h"
#include "texture_ctrl.h"
#include "light.h"
#include "material.h"
#include "obj_export.h"

#include <memory>
#include <random>
#include <vector>

extern std::shared_ptr<light_env> lights;
extern std::shared_ptr<obj_export> obj_ex;

template<typename T>
std::shared_ptr<node> create() { return std::make_shared<T>(); }

std::vector<std::shared_ptr<node>(*)()> fns = {
        &create<box>,
        &create<sphere>,
        &create<cylinder>,
        &create<cone>,
        &create<prism>,
        &create<frustum>
};

GLfloat make_distant(GLfloat f) {
    if (f > 0) f += 3;
    else f -= 3;
    return f;
}

void init() {
    GLuint texId = texture_ctrl::tex_load("lego.png");

    std::default_random_engine rng;

    {
        std::uniform_real_distribution<float> d_mat(0.2f, 1.0f);
        auto mtrl = std::make_shared<material>();
        float tmp = d_mat(rng);
        mtrl->ambient[0] = mtrl->ambient[1] = mtrl->ambient[2] = tmp;
        tmp = d_mat(rng);
        mtrl->diffuse[0] = mtrl->diffuse[1] = mtrl->diffuse[2] = tmp;
        tmp = d_mat(rng);
        mtrl->specular[0] = mtrl->specular[1] = mtrl->specular[2] = tmp;
        auto p = fns[0]();
        p->mtrl = mtrl;
        p->color[0] = 1.0;
        p->color[1] = 0.5;
        p->color[2] = 0.2;
        p->translate[0] = 2;
        p->translate[1] = -3;
        p->translate[2] = -3;
        p->scale[1] = 1;
        p->texId = texId;
        p->rotate_mat = mat3::rotate(20, 0.5, 0.5, 0.5);
        sg.nodes.push_back(std::move(p));
    }

    {
        std::uniform_real_distribution<float> d_mat(0.2f, 1.0f);
        auto mtrl = std::make_shared<material>();
        float tmp = d_mat(rng);
        mtrl->ambient[0] = mtrl->ambient[1] = mtrl->ambient[2] = tmp;
        tmp = d_mat(rng);
        mtrl->diffuse[0] = mtrl->diffuse[1] = mtrl->diffuse[2] = tmp;
        tmp = d_mat(rng);
        mtrl->specular[0] = mtrl->specular[1] = mtrl->specular[2] = tmp;
        auto p = fns[1]();
        p->mtrl = mtrl;
        p->color[0] = 1.0;
        p->color[1] = 0.5;
        p->color[2] = 0.2;
        p->translate[0] = 2;
        p->translate[1] = -3;
        p->translate[2] = 0;
        p->texId = texId;
        sg.nodes.push_back(std::move(p));
    }

    {
        std::uniform_real_distribution<float> d_mat(0.2f, 1.0f);
        auto mtrl = std::make_shared<material>();
        float tmp = d_mat(rng);
        mtrl->ambient[0] = mtrl->ambient[1] = mtrl->ambient[2] = tmp;
        tmp = d_mat(rng);
        mtrl->diffuse[0] = mtrl->diffuse[1] = mtrl->diffuse[2] = tmp;
        tmp = d_mat(rng);
        mtrl->specular[0] = mtrl->specular[1] = mtrl->specular[2] = tmp;
        auto p = fns[2]();

        p->mtrl = mtrl;
        p->color[0] = 1.0;
        p->color[1] = 0.5;
        p->color[2] = 0.2;
        p->translate[0] = 2;
        p->translate[1] = 0;
        p->translate[2] = -3;
        sg.nodes.push_back(std::move(p));
    }

    {
        std::uniform_real_distribution<float> d_mat(0.2f, 1.0f);
        auto mtrl = std::make_shared<material>();
        float tmp = d_mat(rng);
        mtrl->ambient[0] = mtrl->ambient[1] = mtrl->ambient[2] = tmp;
        tmp = d_mat(rng);
        mtrl->diffuse[0] = mtrl->diffuse[1] = mtrl->diffuse[2] = tmp;
        tmp = d_mat(rng);
        mtrl->specular[0] = mtrl->specular[1] = mtrl->specular[2] = tmp;
        auto p = fns[3]();

        p->mtrl = mtrl;
        p->color[0] = 0.6;
        p->color[1] = 0.4;
        p->color[2] = 0.1;
        p->translate[0] = 2;
        p->translate[1] = 0;
        p->translate[2] = 0;
        sg.nodes.push_back(std::move(p));
    }

    {
        std::uniform_real_distribution<float> d_mat(0.2f, 1.0f);
        auto mtrl = std::make_shared<material>();
        float tmp = d_mat(rng);
        mtrl->ambient[0] = mtrl->ambient[1] = mtrl->ambient[2] = tmp;
        tmp = d_mat(rng);
        mtrl->diffuse[0] = mtrl->diffuse[1] = mtrl->diffuse[2] = tmp;
        tmp = d_mat(rng);
        mtrl->specular[0] = mtrl->specular[1] = mtrl->specular[2] = tmp;
        auto p = fns[4]();

        p->mtrl = mtrl;
        p->color[0] = 0.1;
        p->color[1] = 0.4;
        p->color[2] = 0.6;
        p->translate[0] = 2;
        p->translate[1] = -6;
        p->translate[2] = -6;
        sg.nodes.push_back(std::move(p));
    }

    {
        std::uniform_real_distribution<float> d_mat(0.2f, 1.0f);
        auto mtrl = std::make_shared<material>();
        float tmp = d_mat(rng);
        mtrl->ambient[0] = mtrl->ambient[1] = mtrl->ambient[2] = tmp;
        tmp = d_mat(rng);
        mtrl->diffuse[0] = mtrl->diffuse[1] = mtrl->diffuse[2] = tmp;
        tmp = d_mat(rng);
        mtrl->specular[0] = mtrl->specular[1] = mtrl->specular[2] = tmp;
        auto p = fns[5]();

        p->mtrl = mtrl;
        p->color[0] = 0.3;
        p->color[1] = 0.6;
        p->color[2] = 0.2;
        p->translate[0] = 3;
        p->translate[1] = -3;
        p->translate[2] = -6;
        sg.nodes.push_back(std::move(p));
    }

    {    
        std::uniform_real_distribution<float> d_color(0.0f, 1.0f);
        std::uniform_real_distribution<float> d_pos(-10.0f, 10.0f);
        std::uniform_real_distribution<float> d_size(0.5f, 2.0f);
        std::uniform_int_distribution<int> d_fn(0, fns.size() - 1);
        std::uniform_real_distribution<float> d_rot(-1.0f, 1.0f);
        std::uniform_real_distribution<float> d_has_tex(0.0f, 1.0f);
        for (int i = 0; i < 100; ++i)
        {
            std::uniform_real_distribution<float> d_mat(0.2f, 1.0f);
            auto mtrl = std::make_shared<material>();
            float tmp = d_mat(rng);
            mtrl->ambient[0] = mtrl->ambient[1] = mtrl->ambient[2] = tmp;
            tmp = d_mat(rng);
            mtrl->diffuse[0] = mtrl->diffuse[1] = mtrl->diffuse[2] = tmp;
            tmp = d_mat(rng);
            mtrl->specular[0] = mtrl->specular[1] = mtrl->specular[2] = tmp;
            auto p = fns[d_fn(rng)]();
            p->mtrl = mtrl;
            p->color[0] = d_color(rng);
            p->color[1] = d_color(rng);
            p->color[2] = d_color(rng);
            p->translate[0] = make_distant(d_pos(rng));
            p->translate[1] = make_distant(d_pos(rng));
            p->translate[2] = make_distant(d_pos(rng));
            p->scale[0] = p->scale[1] = p->scale[2] = d_size(rng) / 2.0f;
            p->texId = d_has_tex(rng) > 0.9 ? texId : 0;
            float rotate_axis[] = {d_rot(rng), d_rot(rng), d_rot(rng)};
            p->ctrl = [rotate_axis](auto &t) {
                if (sg.selected.get() != &t) {
                    t.rotate_mat = t.rotate_mat * mat3::rotate(0.5f, rotate_axis[0], rotate_axis[1], rotate_axis[2]);
                }
            };
            sg.nodes.push_back(std::move(p));
        }
    }
#define READ
#ifndef READ
    {
        std::uniform_real_distribution<float> d_mat(0.2f, 1.0f);
        auto mtrl = std::make_shared<material>();
        float tmp = d_mat(rng);
        mtrl->ambient[0] = mtrl->ambient[1] = mtrl->ambient[2] = tmp;
        tmp = d_mat(rng);
        mtrl->diffuse[0] = mtrl->diffuse[1] = mtrl->diffuse[2] = tmp;
        tmp = d_mat(rng);
        mtrl->specular[0] = mtrl->specular[1] = mtrl->specular[2] = tmp;
        auto p = std::make_shared<obj_mesh>("teddy.txt");
        p->mtrl = mtrl;
        p->translate[2] = -5.0f;
        p->scale[0] = p->scale[1] = p->scale[2] = 0.05f;
        p->prescale = 0.05f;
        obj_ex->objs.push_back(p);
        sg.nodes.push_back(std::move(p));
    }

    {
        std::uniform_real_distribution<float> d_mat(0.2f, 1.0f);
        auto mtrl = std::make_shared<material>();
        float tmp = d_mat(rng);
        mtrl->ambient[0] = mtrl->ambient[1] = mtrl->ambient[2] = tmp;
        tmp = d_mat(rng);
        mtrl->diffuse[0] = mtrl->diffuse[1] = mtrl->diffuse[2] = tmp;
        tmp = d_mat(rng);
        mtrl->specular[0] = mtrl->specular[1] = mtrl->specular[2] = tmp;
        auto p = std::make_shared<obj_mesh>("teddy.txt");
        p->mtrl = mtrl;
        p->translate[2] = -2.0f;
        p->scale[0] = p->scale[1] = p->scale[2] = 0.05f;
        p->prescale = 0.05f;
        obj_ex->objs.push_back(p);
        sg.nodes.push_back(std::move(p));
    }
#endif

#ifdef READ
    {
        std::uniform_real_distribution<float> d_mat(0.2f, 1.0f);
        auto mtrl = std::make_shared<material>();
        float tmp = d_mat(rng);
        mtrl->ambient[0] = mtrl->ambient[1] = mtrl->ambient[2] = tmp;
        tmp = d_mat(rng);
        mtrl->diffuse[0] = mtrl->diffuse[1] = mtrl->diffuse[2] = tmp;
        tmp = d_mat(rng);
        mtrl->specular[0] = mtrl->specular[1] = mtrl->specular[2] = tmp;
        auto p = std::make_shared<obj_mesh>("export.txt");
        p->mtrl = mtrl;
        // p->translate[2] = -2.0f;
        // p->scale[0] = p->scale[1] = p->scale[2] = 0.05f;
        p->prescale = 1.0f;
        obj_ex->objs.push_back(p);
        sg.nodes.push_back(std::move(p));
    }
#endif

    {
        std::uniform_real_distribution<GLfloat> d_alt(-0.5f, 0.5f);
        std::uniform_real_distribution<float> d_pos(-3.0f, 3.0f);

        GLfloat cpts[6 * 6 * 4]; // n = m = 5
        GLfloat knotsx[6 + 4], knotsy[6 + 4]; // p = q = 3
        for (int i = 0; i <= 3; ++i) {
            knotsx[i] = knotsy[i] = 0;
        }
        knotsx[4] = knotsy[4] = 0.33;
        knotsx[5] = knotsy[5] = 0.66;
        for (int i = 6; i <= 9; ++i) {
            knotsx[i] = knotsy[i] = 1;
        }

        /*GLfloat alts[6][6] = {
            -0.0499978,-0.0414968,0.0101353,0.0391611,0.0467956,-0.031031,
            0.00149759,-0.0101992,-0.0237094,0.0243512,-0.0410452,0.00603899,
            0.00822297,0.0309567,0.00919188,0.00117125,0.0376634,0.0495085,
            0.0226212,0.0466611,-0.0202898,-0.00739492,0.0399498,0.0152999,
            0.0401534,0.0461533,-0.0335287,0.0357987,0.0406845,-0.0205974,
            0.0436244,-0.00853554,-0.0191543,0.00148929,-0.010457,0.0289785,
        };*/

        GLfloat alts[6][6] = {
            -0.0899978,-0.0814968,-0.08101353,-0.0891611,-0.0867956,-0.089031,
            -0.080149759,-0.0101992,-0.0937094,0.0943512,-0.0410452,-0.080603899,
            -0.080822297,0.0309567,0.00919188,0.00117125,0.0376634,-0.0895085,
            -0.08226212,0.0466611,-0.0202898,-0.00739492,0.0399498,-0.08152999,
            -0.0801534,-0.0761533,-0.0735287,-0.0757987,-0.0706845, -0.08205974,
            -0.0896244,-0.080853554,-0.08191543,-0.080148929,-0.0810457,-0.08989785,
        };

        for (int i = 0, k = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++, k += 4) {
                cpts[k] = -1 + i * 0.2;
                cpts[k + 1] = alts[i][j] * 10;
                // cpts[k + 1] = ((i - 2) * (i - 2) + (j - 2) * (j - 2)) / 100.0f;
                cpts[k + 2] = -1 + j * 0.2;
                cpts[k + 3] = 1.0f / 36;
            }
        }

        std::uniform_real_distribution<float> d_mat(0.2f, 1.0f);
        auto mtrl = std::make_shared<material>();
        float tmp = d_mat(rng);
        mtrl->ambient[0] = mtrl->ambient[1] = mtrl->ambient[2] = tmp;
        tmp = d_mat(rng);
        mtrl->diffuse[0] = mtrl->diffuse[1] = mtrl->diffuse[2] = tmp;
        tmp = d_mat(rng);
        mtrl->specular[0] = mtrl->specular[1] = mtrl->specular[2] = tmp;
        auto p = std::make_unique<nurbs>(cpts, knotsx, knotsy, 5, 5, 3, 3, 0.005f, 0.005f);
        p->mtrl = mtrl;
        p->translate[0] = d_pos(rng);
        p->translate[1] = d_pos(rng);
        p->translate[2] = d_pos(rng);
        sg.nodes.push_back(std::move(p));
    }

    {
        auto p = std::make_shared<point_light>();
        p->ambient[0] = 0.0f;
        p->ambient[1] = 0.0f;
        p->ambient[2] = 0.0f;
        p->visible = false;
        p->interactive = false;
        sg.nodes.push_back(std::move(p));
    }

    {
        auto p = std::make_shared<point_light>();
        p->diffuse[0] = 1.0f;
        p->diffuse[1] = 1.0f;
        p->diffuse[2] = 1.0f;
        p->specular[0] = 0.0f;
        p->specular[1] = 3.0f;
        p->specular[2] = 3.0f;
        p->attenuation = 0.14f;
        sg.nodes.push_back(std::move(p));
    }

    {
        auto p = std::make_shared<point_light>();
        p->diffuse[0] = 0.7f;
        p->diffuse[1] = 0.3f;
        p->diffuse[2] = 0.3f;
        p->translate[0] = 3.0f;
        p->translate[1] = 3.0f;
        p->translate[2] = -3.0f;
        p->specular[0] = 0.5f;
        p->specular[1] = 1.0f;
        p->specular[2] = 0.2f;
        p->attenuation = 0.14f;
        p->translate[0] = -3;
        p->translate[1] = -3;
        p->translate[2] = -3;
        p->is_on = false;
        sg.nodes.push_back(std::move(p));
    }

    for (auto &p : sg.nodes) {
        p->translate[0] -= 10;
        p->translate[1] -= 10;
        p->translate[2] -= 10;
    }
}
