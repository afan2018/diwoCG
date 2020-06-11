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

nurbs::nurbs(GLfloat* cpts, GLfloat* knotsx, GLfloat* knotsy, int r, int c, int p, int q, GLfloat dx, GLfloat dy) {
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
    border[0] = border[2] = border[4] = std::numeric_limits<GLfloat>::max();
    border[1] = border[3] = border[5] = -std::numeric_limits<GLfloat>::max();
    for (int i = 0; i < meshsize[0]; i++) {
        for (int j = 0; j < meshsize[1]; j++) {
            for (int k = 0; k < 3; ++k) {
                mesh[i][j][k] /= mesh[i][j][3];
                border[k * 2] = std::min(border[k * 2], mesh[i][j][k]);
                border[k * 2 + 1] = std::max(border[k * 2 + 1], mesh[i][j][k]);
            }
        }
    }
    for (int k = 0; k < 3; ++k) {
        center[k] = (border[k * 2] + border[k * 2 + 1]) / 2;
    }
    base_aabb = {border[0], border[1], border[2], border[3], border[4], border[5]};
}

void nurbs::render() {
    transform();
    glColor3fv(color);
    for (int i = 0; i + 1 < meshsize[0]; i++) {
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j < meshsize[1]; j++) {
            glVertex3fv(mesh[i][j]);
            glVertex3fv(mesh[i + 1][j]);
        }
        glEnd();
    }
}