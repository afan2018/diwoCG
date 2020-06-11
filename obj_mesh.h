#ifndef OBJ_MESH_H
#define OBJ_MESH_H

#include "scene_graph.h"
#include <string>

class obj_mesh : public node {
private:
    float *normals;
    float *vertices;
    long nVertices = 0;
    long nTriangles = 0;

public:
    explicit obj_mesh(const std::string& filename);
    ~obj_mesh();

    void render() override;
};

#endif