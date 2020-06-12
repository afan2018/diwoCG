#ifndef FPS_H
#define FPS_H

#include <iostream>
#ifdef _WIN32
#include <windows.h>
#include <gl/glut.h>
#else
#include <GLUT/glut.h>
#endif

class fps {
private:
    static long long time() {
		return glutGet(GLUT_ELAPSED_TIME);
    }

    long long last_time;
    int frames = 0;

public:
    void init() {
        last_time = time();
    }

    void update() {
        ++frames;
        auto t = time();
        if (t - last_time >= 1000) {
            std::cout << frames << std::endl;
            last_time = t;
            frames = 0;
        }
    }
};

#endif