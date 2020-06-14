#include "engine.h"
#include "geometries.h"
#include "obj_mesh.h"
#include "nurbs_mesh.h"
#include "texture.h"
#include "light.h"

#include <memory>
#include <random>
#include <vector>

extern std::shared_ptr<light_env> lights;

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

void init() {
    GLuint texId = tex_load("lego.png");

    std::default_random_engine rng;
    std::uniform_real_distribution<float> d_color(0.0f, 1.0f);
    std::uniform_real_distribution<float> d_pos(-10.0f, 10.0f);
    std::uniform_real_distribution<float> d_size(0.5f, 2.0f);
    std::uniform_int_distribution<int> d_fn(0, fns.size() - 1);
    std::uniform_real_distribution<float> d_rot(-1.0f, 1.0f);
    std::uniform_real_distribution<float> d_has_tex(0.0f, 1.0f);
    for (int i = 0; i < 200; ++i) {
        auto p = fns[d_fn(rng)]();
        p->color[0] = d_color(rng);
        p->color[1] = d_color(rng);
        p->color[2] = d_color(rng);
        p->translate[0] = d_pos(rng);
        p->translate[1] = d_pos(rng);
        p->translate[2] = d_pos(rng);
        p->scale[0] = p->scale[1] = p->scale[2] = d_size(rng) / 2.0f;
        p->texId = d_has_tex(rng) > 0.9 ? texId : 0;
        float rotate_axis[] = { d_rot(rng), d_rot(rng), d_rot(rng) };
        p->ctrl = [rotate_axis](auto& t) {
            if (sg.selected.get() != &t) {
                t.rotate_mat = t.rotate_mat * mat3::rotate(0.5f, rotate_axis[0], rotate_axis[1], rotate_axis[2]);
            }
        };
        sg.nodes.push_back(std::move(p));
    }

    auto p = std::make_shared<obj_mesh>("teddy.obj");
    p->translate[2] = -5.0f;
    p->scale[0] = p->scale[1] = p->scale[2] = 0.05f;
    sg.nodes.push_back(std::move(p));
	
	std::uniform_real_distribution<GLfloat> altitude(-0.05f, 0.05f);
	GLfloat cpts[5 * 5 * 4]; // n = m = 4
	GLfloat knotsx[5 + 4], knotsy[5 + 4]; // p = q = 3
	for (int i = 0; i <= 3; ++i) {
		knotsx[i] = knotsy[i] = 0;
	}
	knotsx[4] = knotsy[4] = 0.5;
	for (int i = 5; i <= 8; ++i) {
		knotsx[i] = knotsy[i] = 1;
	}
	for (int i = 0, k = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++, k += 4) {
			cpts[k] = -1 + i * 0.02;
			cpts[k + 1] = altitude(rng);
			// cpts[k + 1] = ((i - 2) * (i - 2) + (j - 2) * (j - 2)) / 30.0f;
			cpts[k + 2] = -1 + j * 0.02;
			cpts[k + 3] = 1.0f / 25;
		}
	}
	auto p_test_nurbs = std::make_unique<nurbs>(cpts, knotsx, knotsy, 4, 4, 3, 3, 0.005f, 0.005f);
    p_test_nurbs->translate[0] = d_pos(rng);
    p_test_nurbs->translate[1] = d_pos(rng);
    p_test_nurbs->translate[2] = d_pos(rng);
	sg.nodes.push_back(std::move(p_test_nurbs));

    {
        auto lp = std::make_shared<point_light>();
        lp->ambient[0] = 0.0f;
        lp->ambient[1] = 0.0f;
        lp->ambient[2] = 0.0f;
        lp->visible = false;
        lp->interactive = false;
        sg.nodes.push_back(std::move(lp));
    }

    {
        auto lp = std::make_shared<point_light>();
        lp->diffuse[0] = 1.0f;
        lp->diffuse[1] = 1.0f;
        lp->diffuse[2] = 1.0f;
        lp->translate[0] = 6.0f;
        lp->translate[1] = 4.0f;
        lp->translate[2] = -8.0f;
        lp->specular[0] = 0.0f;
        lp->specular[1] = 3.0f;
        lp->specular[2] = 3.0f;
        lp->attenuation = 0.14f;
        sg.nodes.push_back(std::move(lp));
    }
}
