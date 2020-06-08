#include "engine.h"

camera *c;
scene_graph sg;

void reshape(int width, int height) {
    if (height = 0) height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    c -> transform(width, height);
}

void redraw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    sg.render();

    glutSwapBuffers();
}

void idle() {
    glutPostRedisplay();
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

    init();

    glutMainLoop();

    return 0;
}