#ifndef TEXTURE_CTRL_H
#define TEXTURE_CTRL_H

#ifdef _WIN32
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#include <string>
#include "listener.h"
#include "engine.h"
#include "nfd.h"

class texture_ctrl : public listener {
private:
    std::string get_filepath();
public:
    static GLuint tex_load(const std::string& filename);
    bool keyboard_up(unsigned char key, int x, int y) override;
};


#endif
