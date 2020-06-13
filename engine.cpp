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

std::vector<std::shared_ptr<listener>> listeners;
std::shared_ptr<camera> cam;
std::shared_ptr<control> ctrl;
std::shared_ptr<screenshot> ss;
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
    update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    cam->transform(w_width, w_height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    ctrl->update();
    ray r = ctrl->get_ray();
    sg.render(r, !ss->get_ss_mode());
    glPopMatrix();

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

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    // TODO: enable configuration
    glutInitWindowSize(480, 480);
    glutCreateWindow("Simple GLUT App");

    glutDisplayFunc(&redraw);
    glutReshapeFunc(&reshape);
    glutIdleFunc(&idle);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    // TODO: more initialization operations

    init();

    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    glutKeyboardFunc(&keyboard);
    glutKeyboardUpFunc(&keyboard_up);
    glutIgnoreKeyRepeat(true);

    f.init();

    glutMainLoop();

    return 0;
}