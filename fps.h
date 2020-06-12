#ifndef FPS_H
#define FPS_H

#include <iostream>
#include <sys/time.h>

class fps {
private:
    static long long time() {
        timeval tv{};
        gettimeofday(&tv, nullptr);
        return (((long long) tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
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
        if (t > last_time + 1000) {
            std::cout << frames << std::endl;
            last_time += 1000LL;
            frames = 0;
        }
    }
};

#endif