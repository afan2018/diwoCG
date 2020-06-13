#include "nurbs_mesh.h"
#include <cstring>
#include <algorithm>
#include <iostream>

const double eps = 1e-8;

void nurbs::calc_basis(int dim, std::vector<GLfloat>& basis, GLfloat u) {
    int k = std::upper_bound(knots[dim], knots[dim] + ctrlsize[dim] + deg[dim] + 1, u) - knots[dim] - 1;
    int p = deg[dim];
    if (k < p || k > ctrlsize[dim] + 1) return;
    
    GLfloat* knot = knots[dim];
    int n = 2 * deg[dim] + 1;
    std::vector<GLfloat> temp(n, 0);
    temp[p] = 1;

    for (int j = 1; j <= p; ++j) {
        for (int i = k - p, h = 0; h < n - j; h++, i++) {
            GLfloat a = u - knot[i];
            GLfloat div = knot[i + j] - knot[i];
            a = fabs(div) < eps ? 0 : a / div;

            GLfloat b = knot[i + j + 1] - u;
            div = knot[i + j + 1] - knot[i + 1];
            b = fabs(div) < eps ? 0 : b / div;

            temp[h] = a * temp[h] + b * temp[h + 1];
        }
    }
    std::copy(temp.begin(), temp.begin() + p + 1, basis.begin() + k - p);
}

void nurbs::calc_mesh() {
    for (int i = 0; i < meshsize[0]; i++) {
        for (int j = 0; j < meshsize[1]; j++) {
            GLfloat u = i * meshlen[0], v = j * meshlen[1];
            std::vector<GLfloat> bx(100, 0);
            std::vector<GLfloat> by(100, 0);
            calc_basis(0, bx, u);
            calc_basis(1, by, v);
            for (int k = 0; k < deg[0]; ++k) {
                for (int l = 0; l < deg[1]; ++l) {
                    for (int m = 0; m < 4; ++m) {
                        mesh[i][j][m] += control_pts[k][l][m] * bx[k] * by[l];
                    }
                }
            }
        }
    }
}

nurbs::nurbs(GLfloat* cpts, GLfloat* knotsx, GLfloat* knotsy, int r, int c, int p, int q, GLfloat dx, GLfloat dy)
    : node("geometry/nurbs") {
    memset(mesh, 0, sizeof(mesh));
    meshlen[0] = dx;
    meshlen[1] = dy;
    ctrlsize[0] = r;
    ctrlsize[1] = c;
    deg[0] = p;
    deg[1] = q;
    meshsize[0] = static_cast<int>(ceil(1 / dx));
    meshsize[1] = static_cast<int>(ceil(1 / dy));
    for (int i = 0, k = 0; i <= r; i++) {
        for (int j = 0; j <= c; j++, k += 4) {
            memcpy(control_pts[i][j], cpts + k, sizeof(GLfloat) * 4);
        }
    }
    memcpy(knots[0], knotsx, sizeof(GLfloat) * (r + p + 2));
    memcpy(knots[1], knotsy, sizeof(GLfloat) * (c + q + 2));
    
    calc_mesh();
    GLfloat border[6];
    border[0] = border[2] = border[4] = (std::numeric_limits<GLfloat>::max)();
    border[1] = border[3] = border[5] = -(std::numeric_limits<GLfloat>::max)();
    for (int i = 0; i < meshsize[0]; i++) {
        for (int j = 0; j < meshsize[1]; j++) {
            for (int k = 0; k < 3; ++k) {
                mesh[i][j][k] /= mesh[i][j][3];
                border[k * 2] = (std::min)(border[k * 2], mesh[i][j][k]);
                border[k * 2 + 1] = (std::max)(border[k * 2 + 1], mesh[i][j][k]);
            }
        }
    }
    for (int k = 0; k < 3; ++k) {
        center[k] = (border[k * 2] + border[k * 2 + 1]) / 2;
    }
    base_aabb = {
            border[0] - center[0],
            border[1] - center[0],
            border[2] - center[1],
            border[3] - center[1],
            border[4] - center[2],
            border[5] - center[2]
    };
}

void Normal(float norm[], const float *coord1, const float *coord2, const float *coord3)
{
    float va[3], vb[3], vr[3], val;
    va[0] = coord1[0] - coord2[0];
    va[1] = coord1[1] - coord2[1];
    va[2] = coord1[2] - coord2[2];

    vb[0] = coord1[0] - coord3[0];
    vb[1] = coord1[1] - coord3[1];
    vb[2] = coord1[2] - coord3[2];

    vr[0] = va[1] * vb[2] - vb[1] * va[2];
    vr[1] = vb[0] * va[2] - va[0] * vb[2];
    vr[2] = va[0] * vb[1] - vb[0] * va[1];

    val = sqrt(vr[0] * vr[0] + vr[1] * vr[1] + vr[2] * vr[2]);

    norm[0] = vr[0] / val;
    norm[1] = vr[1] / val;
    norm[2] = vr[2] / val;
}

void DrawTriangle(const GLfloat *a, const GLfloat *b, const GLfloat *c)
{
    GLfloat norm[3];
    Normal(norm, a, b, c);
    glNormal3fv(norm);
    glVertex3fv(a);
    glVertex3fv(b);
    glVertex3fv(c);
    norm[0] = -norm[0];
    norm[1] = -norm[1];
    norm[2] = -norm[2];
    GLfloat pts[3][3];
    memcpy(pts[0], a, sizeof(GLfloat) * 3);
    memcpy(pts[1], b, sizeof(GLfloat) * 3);
    memcpy(pts[2], c, sizeof(GLfloat) * 3);
    glNormal3fv(norm);
    for (int i = 0; i < 3; ++i)
    {
        pts[i][1] += 1e-2;
    }
    for (int i = 0; i < 3; ++i)
    {
        glVertex3fv(pts[i]);
    }
}

void nurbs::render()
{
    transform();
    glColor3fv(color);
    GLfloat norm[3];
    for (int i = 0; i + 1 < meshsize[0]; i++)
    {
        //glBegin(GL_QUAD_STRIP);
        glBegin(GL_TRIANGLES);
        for (int j = 0; j + 1 < meshsize[1]; j++)
        {
            DrawTriangle(mesh[i][j], mesh[i + 1][j], mesh[i][j + 1]);
            DrawTriangle(mesh[i + 1][j + 1], mesh[i][j + 1], mesh[i + 1][j]);
        }
        glEnd();
        //glEnd();
    }
}