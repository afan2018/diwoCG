#include "engine.h"

#ifdef _WIN32
#include <windows.h>
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include "fps.h"
#include "listener.h"
#include "control.h"
#include "screenshot.h"
#include "light.h"
#include "transform.h"

std::vector<std::shared_ptr<listener>> listeners;
std::vector<std::shared_ptr<actor>> actors;
std::shared_ptr<camera> cam;
std::shared_ptr<control> ctrl;
std::shared_ptr<screenshot> ss;
std::shared_ptr<light_env> lights;

scene_graph sg;
int w_width, w_height;
fps f;

void reshape(int width, int height) {
    if (height == 0)
        height = 1;
    glViewport(0, 0, width, height);
    w_width = width;
    w_height = height;
}

void redraw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    cam->transform(w_width, w_height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (auto& ap : actors) {
        ap->update();
    }

    ray r = ctrl->get_ray();
    sg.render(r, !ss->get_ss_mode());

    // cross
    if (!ss->get_ss_mode()) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, w_width, 0, w_height, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);

        glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(w_width / 2 - 10.0f, w_height / 2);
        glVertex2f(w_width / 2 + 10.0f, w_height / 2);
        glVertex2f(w_width / 2, w_height / 2 - 10.0f);
        glVertex2f(w_width / 2, w_height / 2 + 10.0f);
        glEnd();
    } else {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, w_width, 0, w_height, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        ss->drawRect(w_height, w_width);
    }

    f.update();

    glutSwapBuffers();
}

void idle() {
    glutPostRedisplay();
}

void motion(int x, int y) {
    for (const auto &l : listeners) {
        if (l->motion(x, y)) break;
    }
}

void mouse(int button, int state, int x, int y) {
    for (const auto &l : listeners) {
        if (l->mouse(button, state, x, y)) break;
    }
}

void keyboard(unsigned char key, int x, int y) {
    for (const auto &l : listeners) {
        if (l->keyboard(key, x, y)) break;
    }
}

void keyboard_up(unsigned char key, int x, int y) {
    for (const auto &l : listeners) {
        if (l->keyboard_up(key, x, y)) break;
    }
}

void special_key(int key, int x, int y)  {
    for (const auto &l : listeners) {
        if (l->special_key(key, x, y)) break;
    }
}

void special_key_up(int key, int x, int y)  {
    for (const auto &l : listeners) {
        if (l->special_key_up(key, x, y)) break;
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(480, 480);
    glutCreateWindow("Simple GLUT App");

    glutDisplayFunc(&redraw);
    glutReshapeFunc(&reshape);
    glutIdleFunc(&idle);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // actors & listeners
    auto tf = std::make_shared<transformation>();
    ctrl = std::make_shared<orbit_control>();
    ss = std::make_shared<screenshot>();
    lights = std::make_shared<light_env>();
    cam = std::make_shared<perspective_camera>();

    actors.push_back(tf);
    actors.push_back(ctrl);
    actors.push_back(lights);

    listeners.push_back(ss);
    listeners.push_back(lights);
    listeners.push_back(ctrl);

    init();

    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    glutKeyboardFunc(&keyboard);
    glutKeyboardUpFunc(&keyboard_up);
    glutSpecialFunc(special_key);
    glutSpecialUpFunc(special_key_up);
    glutIgnoreKeyRepeat(true);

    f.init();

    glutMainLoop();

    return 0;
}