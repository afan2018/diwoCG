#include "engine.h"
#include "scene_graph.h"
#include "geometries.h"
#include "control.h"

#include <iostream>
#include <memory>
#include <random>

void init() {
	cam = std::make_shared<perspective_camera>();
	ctrl = std::make_shared<orbit_control>();
	ss = std::make_shared<screenshot>();

	listeners.push_back(std::weak_ptr<listener>(ss));
	listeners.push_back(std::weak_ptr<listener>(ctrl));

	std::default_random_engine rng;
	std::uniform_real_distribution<float> d_color(0.0f, 1.0f);
	std::uniform_real_distribution<float> d_pos(-10.0f, 10.0f);
	std::uniform_real_distribution<float> d_size(0.5f, 2.0f);
	for (int i = 0; i < 200; ++i) {
		auto p = std::make_unique<box>();
		p -> color[0] = d_color(rng);
		p -> color[1] = d_color(rng);
		p -> color[2] = d_color(rng);
		p -> translate[0] = d_pos(rng);
		p -> translate[1] = d_pos(rng);
		p -> translate[2] = d_pos(rng);
		p -> scale[0] = p -> scale[1] = p -> scale[2] = d_size(rng) / 2.0f;
		sg.nodes.push_back(std::move(p));

	}
}

void update() {
	// sg.nodes[0] -> rotate_angle += 0.5f;
}
