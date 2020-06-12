#ifndef GEOMETRIES_H
#define GEOMETRIES_H

#include "scene_graph.h"
#include <windows.h>
#include <GL/GL.h>
#include <GL/glu.h>
#include <GL/glut.h>

class box : public node {
private:
    void drawBox(GLfloat size);

public:
    void render() override {
        transform();
        colorize();
        drawBox(1.0f);
    }
};

class sphere : public node {
public:
    void render() override {
        transform();
        colorize();
        glutSolidSphere(0.5f, 40, 50);
    }
};

class cylinder : public node {
public:
    bool lid = true;

    cylinder() {
        center[2] = 0.5f;
    }

    void render() override {
        transform();
        colorize();

        const GLfloat radius = 0.5f;

        GLUquadricObj *objCylinder = gluNewQuadric();
        gluCylinder(objCylinder, radius, radius, 1.0, 40, 50);
		gluDeleteQuadric(objCylinder);
        if (lid) {
            GLUquadricObj *objDiskBottom = gluNewQuadric();
            gluDisk(objDiskBottom, 0, radius, 40, 50);
            glPushMatrix();
            GLUquadricObj *objDiskTop = gluNewQuadric();
            glTranslatef(0, 0, 1.0);
            gluDisk(objDiskTop, 0, radius, 40, 50);
            glPopMatrix();
			gluDeleteQuadric(objDiskBottom);
			gluDeleteQuadric(objDiskTop);
        }
    }
};

class cone : public node {
public:
    bool lid = true;

    cone() {
        center[2] = 0.5f;
    }

    void render() override {
        transform();
        colorize();

        const GLfloat radius = 0.5f;

        GLUquadricObj *objCylinder = gluNewQuadric();
        gluCylinder(objCylinder, 0, radius, 1.0, 40, 50);
		gluDeleteQuadric(objCylinder);
        if (lid) {
            glPushMatrix();
            GLUquadricObj *objDiskTop = gluNewQuadric();
            glTranslatef(0, 0, 1.0);
            gluDisk(objDiskTop, 0, radius, 40, 50);
            glPopMatrix();
			gluDeleteQuadric(objDiskTop);
        }
    }
};

class prism : public node {
private:
    void drawPrism();

public:
    prism() {
        center[0] = center[1] = center[2] = 0.5f;
    }

    void render() override {
        transform();
        colorize();
        drawPrism();
    }
};

class frustum : public node {
private:
	void drawFrustum();

public:
	frustum() {
		center[0] = center[1] = center[2] = 0.5f;
	}

	void render() override {
		transform();
		colorize();
		drawFrustum();
	}
};
#endif
