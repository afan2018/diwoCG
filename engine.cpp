#include "engine.h"

camera *cam;
control *ctrl;
scene_graph sg;

int w_width, w_height;

void reshape(int width, int height) {
    if (height == 0) height = 1;
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
    cam -> transform(w_width, w_height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    ctrl -> update();
    ray r = ctrl -> get_ray();
    sg.render(r);

    // cross
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
    glDisable(GL_BLEND);

    glutSwapBuffers();
}

void idle() {
    glutPostRedisplay();
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
    // TODO: more initialization operations

    init();

    glutMainLoop();

    return 0;
}