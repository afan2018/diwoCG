#include "listener.h"
#include <vector>
#include <GL/gl.h>

class light {
protected:
    GLfloat* ambient;
    GLfloat* specular;
    GLfloat* diffuse;
    GLfloat* position;
    GLenum id;
    bool is_on;

public:
    light();
    bool get_status();
    void init(GLenum id, GLfloat* ambient, GLfloat* specular, GLfloat* diffuse, GLfloat* position);
    void setbasic();
    void reset();
    void flip();
    virtual void set() {setbasic(); glEnable(id);}
};

class spotlight : public light {
    GLfloat angle;
    GLfloat exponent;
    GLfloat direction[3];
public:
    spotlight(GLenum id, GLfloat* ambient, GLfloat* specular, GLfloat* diffuse, GLfloat* position, GLfloat angle, GLfloat exponent, GLfloat* direction);
    void set();
};

class normallight : public light {
public:
    normallight(GLenum id, GLfloat* ambient, GLfloat* specular, GLfloat* diffuse, GLfloat* position);
};


class light_env : public listener
{
private:
    bool in_light_mode;
    bool lights_on = false;
    std::vector<light> lights;

public:
    light_env() = default;

    bool keyboard(unsigned char key, int x, int y) override;
    bool keyboard_up(unsigned char key, int x, int y) override;
    bool mouse(int button, int state, int x, int y) override;
    bool motion(int x, int y) override;
    
    void addlight(GLfloat* ambient, GLfloat* specular, GLfloat* diffuse, GLfloat* position, GLfloat angle=0.0f, GLfloat exponent=0.0f, GLfloat* direction=NULL);
};