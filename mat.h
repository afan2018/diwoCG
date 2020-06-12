#ifndef MAT_H
#define MAT_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>

using namespace std;
#define IDX(a, b) ((a * 3) + b)
#ifndef M_PI
#define M_PI 3.1415926535
#endif

class vec3 {
public:
    float data[3];

    vec3() = default;

    vec3 operator~() const {
        float length = std::sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]);
        return {
                data[0] / length,
                data[1] / length,
                data[2] / length,
        };
    }
};

class mat3 {
public:
    float data[9];

    mat3() = default;

    mat3 &operator=(const mat3 &m) {
        if (this != &m) std::memcpy(data, m.data, 9 * sizeof(float));
        return *this;
    }

    mat3 operator*(const mat3 &m) const {
        return mat3::multiply(*this, m);
    }

    mat3 operator~() const {
        return mat3::invert(*this);
    }

    mat3 operator/(const mat3 &m) const {
        return *this * ~m;
    }

    vec3 operator*(const vec3 &v) const {
        return {
                data[0] * v.data[0] + data[1] * v.data[1] + data[2] * v.data[2],
                data[3] * v.data[0] + data[4] * v.data[1] + data[5] * v.data[2],
                data[6] * v.data[0] + data[7] * v.data[1] + data[8] * v.data[2]
        };
    }

    mat3 operator+(const mat3 &m) const {
        return {
            data[0] + m.data[0], data[1] + m.data[1], data[2] + m.data[2],
            data[3] + m.data[3], data[4] + m.data[4], data[5] + m.data[5],
            data[6] + m.data[6], data[7] + m.data[7], data[8] + m.data[8]
        };
    }

    mat3 operator-(const mat3 &m) const {
        return {
                data[0] - m.data[0], data[1] - m.data[1], data[2] - m.data[2],
                data[3] - m.data[3], data[4] - m.data[4], data[5] - m.data[5],
                data[6] - m.data[6], data[7] - m.data[7], data[8] - m.data[8]
        };
    }

    mat3 operator*(float n) const {
        return {
            data[0] * n, data[1] * n, data[2] * n,
            data[3] * n, data[4] * n, data[5] * n,
            data[6] * n, data[7] * n, data[8] * n
        };
    }

    static mat3 rotate(float angle, float x, float y, float z) {
        float rad = angle / 180.0f * M_PI;
        vec3 v = {x, y, z};
        vec3 u = ~v;
        mat3 s = {
                0, -u.data[2], u.data[1],
                u.data[2], 0, -u.data[0],
                -u.data[1], u.data[0], 0
        };
        mat3 p = {
                u.data[0] * u.data[0], u.data[0] * u.data[1], u.data[0] * u.data[2],
                u.data[1] * u.data[0], u.data[1] * u.data[1], u.data[1] * u.data[2],
                u.data[2] * u.data[0], u.data[2] * u.data[1], u.data[2] * u.data[2]
        };
        return p + (mat3::identity() - p) * cos(rad) + s * sin(rad);
    }

    float det() const {
        return      data[IDX(0, 0)] * data[IDX(1, 1)] * data[IDX(2, 2)] +
                    data[IDX(0, 1)] * data[IDX(1, 2)] * data[IDX(2, 0)] +
                    data[IDX(0, 2)] * data[IDX(1, 0)] * data[IDX(2, 1)] -
                    data[IDX(2, 0)] * data[IDX(1, 1)] * data[IDX(0, 2)] -
                    data[IDX(1, 0)] * data[IDX(0, 1)] * data[IDX(2, 2)] -
                    data[IDX(0, 0)] * data[IDX(2, 1)] * data[IDX(1, 2)];
    }

    static mat3 invert(const mat3 &m) {
        float det = m.det();
        mat3 ans{};
        ans.data[IDX(0, 0)] = (m.data[IDX(1, 1)] * m.data[IDX(2, 2)] - m.data[IDX(1, 2)] * m.data[IDX(2, 1)]) / det;
        ans.data[IDX(0, 1)] = -(m.data[IDX(0, 1)] * m.data[IDX(2, 2)] - m.data[IDX(2, 1)] * m.data[IDX(0, 2)]) / det;
        ans.data[IDX(0, 2)] = (m.data[IDX(0, 1)] * m.data[IDX(1, 2)] - m.data[IDX(1, 1)] * m.data[IDX(0, 2)]) / det;
        ans.data[IDX(1, 0)] = -(m.data[IDX(1, 0)] * m.data[IDX(2, 2)] - m.data[IDX(1, 2)] * m.data[IDX(2, 0)]) / det;
        ans.data[IDX(1, 1)] = (m.data[IDX(0, 0)] * m.data[IDX(2, 2)] - m.data[IDX(0, 2)] * m.data[IDX(2, 0)]) / det;
        ans.data[IDX(1, 2)] = -(m.data[IDX(0, 0)] * m.data[IDX(1, 2)] - m.data[IDX(1, 0)] * m.data[IDX(0, 2)]) / det;
        ans.data[IDX(2, 0)] = (m.data[IDX(1, 0)] * m.data[IDX(2, 1)] - m.data[IDX(1, 1)] * m.data[IDX(2, 0)]) / det;
        ans.data[IDX(2, 1)] = -(m.data[IDX(0, 0)] * m.data[IDX(2, 1)] - m.data[IDX(2, 0)] * m.data[IDX(0, 1)]) / det;
        ans.data[IDX(2, 2)] = (m.data[IDX(0, 0)] * m.data[IDX(1, 1)] - m.data[IDX(0, 1)] * m.data[IDX(1, 0)]) / det;
        return ans;
    }

    static mat3 multiply(const mat3 &a, const mat3 &b) {
        mat3 ans{};
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    ans.data[IDX(i, j)] += a.data[IDX(i, k)] * b.data[IDX(k, j)];
                }
            }
        }
        return ans;
    }

    static mat3 identity() {
        return {
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f
        };
    }
};

#endif
