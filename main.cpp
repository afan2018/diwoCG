#include "engine.h"
#include "scene_graph.h"
#include "geometries.h"
#include "control.h"

#include <memory>

void init() {
	cam = new perspective_camera();
	ctrl = new orbit_control();
	sg.nodes.push_back(std::make_unique<box>());
}

void update() {
	sg.nodes[0] -> rotate_angle += 0.5f;
}
