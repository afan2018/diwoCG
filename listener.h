#ifndef LISTENER_H
#define LISTENER_H

#include <memory>

class listener : public std::enable_shared_from_this<listener>
{
public:
    listener() = default;
    virtual bool motion(int x, int y) { return false; }
    virtual bool mouse(int button, int state, int x, int y) { return false; }
    virtual bool keyboard(unsigned char key, int x, int y) { return false; }
    virtual bool keyboard_up(unsigned char key, int x, int y) { return false; }
    virtual bool special_key(int key, int x, int y) {return false;}
    virtual bool special_key_up(int key, int x, int y) {return false;}
};

#endif