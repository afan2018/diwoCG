#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <vector>
#include <memory>
#include <cmath>
#include <windows.h>
#include <GL/gl.h>
#include <gl/glut.h>

#include "mat.h"

class ray {
    public:
        GLfloat x0, y0, z0;
        GLfloat xd, yd, zd;
};

#define INTERSECT_FACE(X0, X1, XD, Y0, Y1, YD, Z0, Z1, ZD) \
    { \
        float t = (X0 - r.X1) / r.XD; \
        if (t > 0) { \
            float y = r.Y0 + t * r.YD; \
            float z = r.Z0 + t * r.ZD; \
            float yt = (y - Y0) / (Y1 - Y0); \
            float zt = (z - Z0) / (Z1 - Z0); \
            if (yt >= 0 && yt <= 1 && zt >= 0 && zt <= 1) { \
                if (!(t >= t_min)) t_min = t; \
            } \
        } \
    }

class aabb {
    public: 
        GLfloat x0, x1, y0, y1, z0, z1;
        aabb() = default;

    float intersects(ray& r) const {
            float t_min = NAN;
            INTERSECT_FACE(x0, x0, xd, y0, y1, yd, z0, z1, zd);
            INTERSECT_FACE(x1, x0, xd, y0, y1, yd, z0, z1, zd);
            INTERSECT_FACE(y0, y0, yd, x0, x1, xd, z0, z1, zd);
            INTERSECT_FACE(y1, y0, yd, x0, x1, xd, z0, z1, zd);
            INTERSECT_FACE(z0, z0, zd, y0, y1, yd, x0, x1, xd);
            INTERSECT_FACE(z1, z0, zd, y0, y1, yd, x0, x1, xd);
            return t_min;
        }

        void render(float extend) const {
            GLfloat xc = (x0 + x1) / 2;
            GLfloat yc = (y0 + y1) / 2;
            GLfloat zc = (z0 + z1) / 2;
            GLfloat xr = (x1 - x0) + extend;
            GLfloat yr = (y1 - y0) + extend;
            GLfloat zr = (z1 - z0) + extend;
            glTranslatef(xc, yc, zc);
            glScalef(xr, yr, zr);
            glColor3f(1.0f, 1.0f, 1.0f);
            glutWireCube(1.0f);
        }
};

class node {
    protected:
        aabb    base_aabb         = { -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f };

        void transform() {
            glTranslatef(translate[0], translate[1], translate[2]);
//            glRotatef(rotate_angle, rotate_axis[0], rotate_axis[1], rotate_axis[2]);
            GLfloat mat[] = {
                    rotate_mat.data[0], rotate_mat.data[3], rotate_mat.data[6], 0.0f,
                    rotate_mat.data[1], rotate_mat.data[4], rotate_mat.data[7], 0.0f,
                    rotate_mat.data[2], rotate_mat.data[5], rotate_mat.data[8], 0.0f,
                                  0.0f,               0.0f,               0.0f, 1.0f
            };
            glMultMatrixf(mat);
            glScalef(scale[0], scale[1], scale[2]);
            glTranslatef(-center[0], -center[1], -center[2]);
        }

        void colorize() {
            if (texId == 0) {
                glDisable(GL_TEXTURE_2D);
                glColor3fv(color);
            } else {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texId);
                glColor3f(1.0f, 1.0f, 1.0f);
            }
        }

    public:
        GLfloat translate[3]      = { 0.0f, 0.0f, 0.0f };
        GLfloat scale[3]          = { 1.0f, 1.0f, 1.0f };
//        GLfloat rotate_angle      = 0.0f;
//        GLfloat rotate_axis[3]    = { 0.0f, 1.0f, 0.0f };
        GLuint  texId             = 0;
        GLfloat color[3]          = { 1.0f, 1.0f, 1.0f };
        GLfloat center[3]         = { 0.0f, 0.0f, 0.0f };
        mat3    rotate_mat        = mat3::identity();
        
        aabb get_aabb() {
            return {
                base_aabb.x0 * scale[0] + translate[0],
                base_aabb.x1 * scale[0] + translate[0],
                base_aabb.y0 * scale[1] + translate[1],
                base_aabb.y1 * scale[1] + translate[1],
                base_aabb.z0 * scale[2] + translate[2],
                base_aabb.z1 * scale[2] + translate[2]
            };
        }

        virtual void render() {};
};

class scene_graph {
    public:
        std::vector<std::shared_ptr<node>> nodes;
        std::shared_ptr<node> selected;
        scene_graph() = default;

        void render(ray &r, bool need_box) {
            // update selection
            selected = nullptr;
            float t_min = NAN;
            for (auto& n : nodes) {
                aabb box = n->get_aabb();
                float t = box.intersects(r);
                if (!std::isnan(t) && !(t >= t_min)) {
                    selected = n;
                    t_min = t;
                }
            }

            // render nodes
            for (auto& n : nodes) {
                glPushMatrix();
                n->render();
                glPopMatrix();
            }

            // render selection
            if (selected && need_box) {
                glPushMatrix();
                selected->get_aabb().render(0.04f);
                glPopMatrix();
            }
        }
};

#endif
