#ifndef SCENE_GREAPH_H
#define SCENE_GREAPH_H

#include <vector>
#include <memory>
#include <GL/gl.h>

class aabb {
    public: 
        GLfloat x0, x1, y0, y1, z0, z1;
        aabb(
            GLfloat x0, GLfloat x1, GLfloat y0,
            GLfloat y1, GLfloat z0, GLfloat z1
        ) : x0(x0), x1(x1), y0(y0), y1(y1), z0(z0), z1(z1) {}
};

class node {
    protected:
        aabb    base_aabb         = { -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f };

        void transform() {
            glTranslatef(translate[0], translate[1], translate[2]);
            glRotatef(rotate_angle, rotate_axis[0], rotate_axis[1], rotate_axis[2]);
            glScalef(scale[0], scale[1], scale[2]);
        }

    public:
        GLfloat translate[3]      = { 0.0f, 0.0f, 0.0f };
        GLfloat scale[3]          = { 1.0f, 1.0f, 1.0f };
        GLfloat rotate_angle      = 0.0f;
        GLfloat rotate_axis[3]    = { 0.0f, 1.0f, 0.0f };
        GLint   texId             = 0;
        
        aabb get_aabb() {
            return aabb(
                base_aabb.x0 * scale[0] + translate[0],
                base_aabb.x1 * scale[0] + translate[0],
                base_aabb.y0 * scale[1] + translate[1],
                base_aabb.y1 * scale[1] + translate[1],
                base_aabb.z0 * scale[2] + translate[2],
                base_aabb.z1 * scale[2] + translate[2]
            );
        }

        virtual void render() {};
};

class scene_graph {
    public:
        std::vector<std::unique_ptr<node>> nodes;
        scene_graph() = default;

        render() {
            for (auto& n : nodes) {
                glPushMatrix();
                n -> render();
                glPopMatrix();
            }
        }
};

#endif
