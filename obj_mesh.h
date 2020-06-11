#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "scene_graph.h"

class obj_mesh : public node {
private:
    const char *filename;
    float *normals;                             // Stores the normals
    float *Faces_Triangles;                     // Stores the triangles
    float *vertexBuffer;                        // Stores the points which make the object
    long TotalConnectedPoints       = 0;        // Stores the total number of connected vertices
    long TotalConnectedTriangles    = 0;        // Stores the total number of connected triangles

    void load();

public:
    obj_mesh(const char *filename);
    ~obj_mesh();

    void render() override;
};

#endif