#include "engine.h"
#include "scene_graph.h"
#include "geometries.h"

void init() {
	c = new perspective_camera();
	box b;
	sg.nodes.push_back(b);
}
