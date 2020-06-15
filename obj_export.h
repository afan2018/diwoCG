#pragma once
#include "obj_mesh.h"
#include "listener.h"
#include <fstream>
#include <vector>

class obj_export : public listener {
public:
    obj_export() = default;
    ~obj_export() = default;
    std::vector<std::shared_ptr<obj_mesh>> objs;
    void export_objs();
    bool keyboard_up(unsigned char key, int x, int y) override;
};