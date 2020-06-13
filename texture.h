#ifndef TEXTURE_H
#define TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef _WIN32
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

GLuint tex_load(const std::string& filename) {
    int x, y, n;
    unsigned char *data = stbi_load(filename.c_str(), &x, &y, &n, 3);

    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 x,
                 y,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 data);

    stbi_image_free(data);

    return texId;
}

#endif
