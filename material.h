#ifndef MATERIAL_H
#define MATERIAL_H

#ifdef _WIN32
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

class material {
public:
    GLfloat ambient[4]        = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuse[4]        = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat specular[4]       = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat emission[4]       = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat shininess         = 32.0f;

    virtual void colorize() {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    }
};

#endif
