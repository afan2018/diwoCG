#include "texture_ctrl.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

std::string texture_ctrl::get_filepath() {
    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_OpenDialog( "png,jpg", NULL, &outPath );
    if ( result == NFD_OKAY ) {
        return std::string(outPath);
    }
    else if (result == NFD_CANCEL) {
        puts("User pressed cancel.");
    } else {
        printf("Error: %s\n", NFD_GetError());
    }
    return "";
}

GLuint texture_ctrl::tex_load(const std::string& filename) {
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

bool texture_ctrl::keyboard_up(unsigned char key, int x, int y) {
    if (key == 't' && sg.selected) {
        auto selectedNode = sg.selected;
        if (selectedNode->texId == 0) {
            std::string fileName = get_filepath();
            if (!fileName.empty()) {
                selectedNode->texId = tex_load(fileName);
            }
        } else {
            selectedNode->texId = 0;
        }
    }
    return false;
}