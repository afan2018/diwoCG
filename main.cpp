#include "engine.h"
#include "geometries.h"
#include "obj_mesh.h"

#include <memory>
#include <random>
#include <vector>

template<typename T>
std::shared_ptr<node> create() { return std::make_shared<T>(); }
std::vector<std::shared_ptr<node>(*)()> fns = {
        &create<box>,
        &create<sphere>,
        &create<cylinder>,
        &create<cone>,
        &create<prism>
};

void init() {
    cam = std::make_shared<perspective_camera>();
    ctrl = std::make_shared<orbit_control>();
    ss = std::make_shared<screenshot>();

    listeners.push_back(ss);
    listeners.push_back(ctrl);

    std::default_random_engine rng;
    std::uniform_real_distribution<float> d_color(0.0f, 1.0f);
    std::uniform_real_distribution<float> d_pos(-10.0f, 10.0f);
    std::uniform_real_distribution<float> d_size(0.5f, 2.0f);
    std::uniform_int_distribution<int> d_fn(0, fns.size() - 1);
    for (int i = 0; i < 200; ++i) {
        auto p = fns[d_fn(rng)]();
        p->color[0] = d_color(rng);
        p->color[1] = d_color(rng);
        p->color[2] = d_color(rng);
        p->translate[0] = d_pos(rng);
        p->translate[1] = d_pos(rng);
        p->translate[2] = d_pos(rng);
        p->scale[0] = p->scale[1] = p->scale[2] = d_size(rng) / 2.0f;
        sg.nodes.push_back(std::move(p));
    }

    auto p = std::make_shared<obj_mesh>("teddy.obj");
    p->translate[2] = -5.0f;
    p->scale[0] = p->scale[1] = p->scale[2] = 0.05f;
    sg.nodes.push_back(std::move(p));
}

void update() {
    // sg.nodes[0] -> rotate_angle += 0.5f;
}
