#ifndef OBJ_MESH_H
#define OBJ_MESH_H

#include "scene_graph.h"
#include <string>

#define POINTS_PER_VERTEX 3
#define TOTAL_FLOATS_IN_TRIANGLE 9

class obj_mesh : public node {
public:
    float *normals;
    float *vertices;
    long nVertices = 0;
    long nTriangles = 0;

    float prescale = 1;

    explicit obj_mesh(const std::string& filename);
    ~obj_mesh();

    void render() override;
};

#endif