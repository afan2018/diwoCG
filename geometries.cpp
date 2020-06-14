#include "geometries.h"

#define TESSELLATE
#ifdef TESSELLATE

#ifndef GRANULARITY
#define GRANULARITY 5
#endif

#include <cstring>

GLenum mode;
GLfloat normal[3]   = {};

class vertex {
public:
    GLfloat position[3] = {};
    GLdouble tex[2]     = {};
};

std::array<vertex, 4> vertices;
int pointer = {};

void tes_glBegin(GLenum _mode) {
    glBegin(GL_TRIANGLES);
    mode = _mode;
    pointer = 0;
}

void tes_glEnd() {
    glEnd();
}

void tes_glNormal3fv(const GLfloat arr[]) {
    std::memcpy(normal, arr, sizeof(GLfloat) * 3);
}

void tes_glNormal3f(GLfloat a, GLfloat b, GLfloat c) {
    GLfloat n[] = { a, b, c };
    tes_glNormal3fv(n);
}

void tes_glTexCoord2d(GLdouble u, GLdouble v) {
    vertices[pointer].tex[0] = u;
    vertices[pointer].tex[1] = v;
}

void tessellate(int, int, int);

void tes_glVertex3fv(const GLfloat arr[]) {
    std::memcpy(vertices[pointer].position, arr, sizeof(GLfloat) * 3);
    ++pointer;
    switch (mode) {
        case GL_TRIANGLES:
            if (pointer == 3) {
                tessellate(0, 1, 2);
                pointer = 0;
            }
            break;
        case GL_QUADS:
            if (pointer == 3) {
                tessellate(0, 1, 2);
            } else if (pointer == 4) {
                tessellate(2, 3, 0);
                pointer = 0;
            }
            break;
        default: break;
    }
}

void tes_glVertex3f(GLfloat x, GLfloat y, GLfloat z) {
    GLfloat n[] = { x, y, z };
    tes_glVertex3fv(n);
}

template <typename T, size_t n>
std::array<T, n> interpolate(std::array<T, n> a, std::array<T, n> b, float t) {
    std::array<T, n> ans = {};
    for (int i = 0; i < n; ++i) {
        ans[i] = a[i] * (1 - t) + b[i] * t;
    }
    return ans;
}

typedef std::array<float, 3> point;

template <typename T, size_t n>
std::array<T, n> sum(const T a[], const T b[], const T c[], point p) {
    std::array<T, n> ans = {};
    for (int i = 0; i < n; ++i) {
        ans[i] = a[i] * p[0] + b[i] * p[1] + c[i] * p[2];
    }
    return ans;
}

void nail(int a, int b, int c, point p) {
    auto tex = sum<GLdouble, 2>(vertices[a].tex, vertices[b].tex, vertices[c].tex, p);
    glTexCoord2dv(tex.data());
    auto position = sum<GLfloat, 3>(vertices[a].position, vertices[b].position, vertices[c].position, p);
    glVertex3fv(position.data());
}

void fractal(int level, int a, int b, int c, point q, point w, point e) {
    if (level != GRANULARITY) {
        point qw = interpolate(q, w, 0.5f);
        point we = interpolate(w, e, 0.5f);
        point eq = interpolate(e, q, 0.5f);
        fractal(level + 1, a, b, c, eq, q, qw);
        fractal(level + 1, a, b, c, qw, w, we);
        fractal(level + 1, a, b, c, we, e, eq);
        fractal(level + 1, a, b, c, qw, we, eq);
    } else {
        nail(a, b, c, q);
        nail(a, b, c, w);
        nail(a, b, c, e);
    }
}

void tessellate(int a, int b, int c) {
    glNormal3fv(normal);
    fractal(0, a, b, c,
            { 1, 0, 0 },
            { 0, 1, 0 },
            { 0, 0, 1 });
}

#define glBegin tes_glBegin
#define glEnd tes_glEnd
#define glNormal3fv tes_glNormal3fv
#define glNormal3f tes_glNormal3f
#define glTexCoord2d tes_glTexCoord2d
#define glVertex3fv tes_glVertex3fv
#define glVertex3f tes_glVertex3f
#endif

void box::draw_box(GLfloat size) {
    static GLfloat n[6][3] =
            {
                    {-1.0, 0.0,  0.0},
                    {0.0,  1.0,  0.0},
                    {1.0,  0.0,  0.0},
                    {0.0,  -1.0, 0.0},
                    {0.0,  0.0,  1.0},
                    {0.0,  0.0,  -1.0}
            };
    static GLint faces[6][4] =
            {
                    {0, 1, 2, 3},
                    {3, 2, 6, 7},
                    {7, 6, 5, 4},
                    {4, 5, 1, 0},
                    {5, 6, 2, 1},
                    {7, 4, 0, 3}
            };
    GLfloat v[8][3];
    GLint i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    glBegin(GL_QUADS);
    for (i = 5; i >= 0; i--) {
        glNormal3fv(&n[i][0]);
        glTexCoord2d(1, 0);
        glVertex3fv(&v[faces[i][0]][0]);
        glTexCoord2d(0, 0);
        glVertex3fv(&v[faces[i][1]][0]);
        glTexCoord2d(0, 1);
        glVertex3fv(&v[faces[i][2]][0]);
        glTexCoord2d(1, 1);
        glVertex3fv(&v[faces[i][3]][0]);
    }
    glEnd();
}

void prism::draw_prism() {
    // back endcap
    glBegin(GL_TRIANGLES);
    glNormal3f(0, 0, -1);
	glTexCoord2d(1, 0);
    glVertex3f(1.0f, 0.0f, 0.0f);
	glTexCoord2d(0, 0);
    glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2d(0, 1);
    glVertex3f(0.0f, 1.0f, 0.0f);

    // front endcap
    glNormal3f(0, 0, 1);
	glTexCoord2d(1, 0);
    glVertex3f(1.0f, 0.0f, 1.0f);
	glTexCoord2d(0, 0);
    glVertex3f(0.0f, 0.0f, 1.0f);
	glTexCoord2d(0, 1);
    glVertex3f(0.0f, 1.0f, 1.0f);
    glEnd();

    // bottom
    glBegin(GL_QUADS);
    glNormal3f(0, -1, 0);
    glTexCoord2d(0, 0);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2d(1, 0);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glTexCoord2d(1, 1);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glTexCoord2d(0, 1);
    glVertex3f(0.0f, 0.0f, 1.0f);

    // back
    glNormal3f(-1, 0, 0);
    glTexCoord2d(0, 0);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2d(0, 1);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glTexCoord2d(1, 1);
    glVertex3f(0.0f, 1.0f, 1.0f);
    glTexCoord2d(1, 0);
    glVertex3f(0.0f, 0.0f, 1.0f);

    // top
    glNormal3f(1, 1, 0);
    glTexCoord2d(0, 1);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glTexCoord2d(1, 1);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glTexCoord2d(1, 0);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glTexCoord2d(0, 0);
    glVertex3f(0.0f, 1.0f, 1.0f);
    glEnd();
}

void frustum::draw_frustum() {
	// back endcap
	glBegin(GL_TRIANGLES);
    glNormal3f(0, 0, -1);
	glTexCoord2d(1, 0);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glTexCoord2d(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2d(0, 1);
	glVertex3f(0.0f, 1.0f, 0.0f);

	// front endcap
    glNormal3f(0, 0, 1);
	glTexCoord2d(1, 0);
	glVertex3f(0.5f, 0.0f, 1.0f);
	glTexCoord2d(0, 0);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glTexCoord2d(0, 1);
	glVertex3f(0.0f, 0.5f, 1.0f);
	glEnd();

	// bottom
	glBegin(GL_QUADS);
    glNormal3f(0, -1, 0);
	glTexCoord2d(0, 1);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2d(1, 1);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glTexCoord2d(0.5, 0);
	glVertex3f(0.5f, 0.0f, 1.0f);
	glTexCoord2d(0, 0);
	glVertex3f(0.0f, 0.0f, 1.0f);

	// back
    glNormal3f(-1, 0, 0);
	glTexCoord2d(1, 1);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2d(0, 1);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2d(0.5, 0);
	glVertex3f(0.0f, 0.5f, 1.0f);
	glTexCoord2d(1, 0);
	glVertex3f(0.0f, 0.0f, 1.0f);

	// top
    glNormal3f(1, 1, 0.5);
	glTexCoord2d(0, 0);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2d(1, 0);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glTexCoord2d(0.75, 0.75);
	glVertex3f(0.5f, 0.0f, 1.0f);
	glTexCoord2d(0.25, 0.75);
	glVertex3f(0.0f, 0.5f, 1.0f);
	glEnd();
}
