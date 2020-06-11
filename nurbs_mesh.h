#include "geometries.h"

class nurbs : public node {
    private:
        GLfloat color[3] = { 1.0f, 1.0f, 1.0f };
        GLfloat control_pts[50][50][4]; // The 4th dim is weight
        GLfloat knots[2][100];
        GLfloat mesh[500][500][4];
        GLfloat meshlen[2]; // should be in (0, 1)
        int deg[2], ctrlsize[2], meshsize[2];

        void calc_basis(int dim, std::vector<GLfloat>& basis, GLfloat u);
        void calc_mesh();
    public:
        nurbs(GLfloat* cpts, GLfloat* knotsx, GLfloat* knotsy, int r, int c, int p, int q, GLfloat dx, GLfloat dy);
        // (r + 1) * (c + 1) control points, [0, r + p + 1] x-knots, [0, c + q + 1] y-knots.
        void render();
};