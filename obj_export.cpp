#include "obj_export.h"
#include "mat.h"

void obj_export::export_objs() {
    std::ofstream fout;
    fout.open("export.txt");
    int offset = 0;
    for (auto obj : objs) {
        for (int tCounter = 0; tCounter < obj->nTriangles; tCounter += TOTAL_FLOATS_IN_TRIANGLE) {
            for (int i = 0; i < POINTS_PER_VERTEX; i++) {
                vec3 v = {
                    (obj->vertices[tCounter + i * 3] * obj->scale[0] + obj->translate[0] + 10),
                    (obj->vertices[tCounter + i * 3 + 1] * obj->scale[1] + obj->translate[1] + 10),
                    (obj->vertices[tCounter + i * 3 + 2] * obj->scale[2] + obj->translate[2] + 10)
                };
                v = obj->rotate_mat * v;
                fout << "v " << v.data[0] << ' ' << v.data[1] << ' ' << v.data[2] << endl;
            }
            fout << "f " << offset + tCounter / POINTS_PER_VERTEX + 1 << " " << offset + tCounter / POINTS_PER_VERTEX + 2 << " " << offset + tCounter / POINTS_PER_VERTEX + 3 << endl;
        }
        offset += obj->nVertices;
    }
    fout.close();
    cout << "export finished." << endl;
}

bool obj_export::keyboard_up(unsigned char key, int x, int y) {
    if (key == 'e') {
        export_objs();
    }
    return false;
}