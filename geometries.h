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
        GLfloat color[3] = { 1.0f, 1.0f, 1.0f };

        void render() override {
            transform();
            glColor3fv(color);
            drawBox(1.0f);
        }
};

class sphere : public node {
	public:
		GLfloat color[3] = { 1.0f, 1.0f, 1.0f };

		void render() override {
			transform();
			glColor3fv(color);
			glutSolidSphere(1.0f, 40, 50);
		}
};

class cylinder : public node {
	public:
		GLfloat color[3] = { 1.0f, 1.0f, 1.0f };
		GLfloat radius = 1.0;
		bool lid = false;
		
		void render() override {
			transform();
			glColor3fv(color);
			GLUquadricObj *objCylinder = gluNewQuadric();
			gluCylinder(objCylinder, radius, radius, 1.0, 40, 50);
			if (lid) {
				GLUquadricObj *objDiskBottom = gluNewQuadric();
				gluDisk(objDiskBottom, 0, radius, 40, 50);
				glPushMatrix();
				GLUquadricObj *objDiskTop = gluNewQuadric();
				glTranslatef(0, 0, 1.0);
				gluDisk(objDiskTop, 0, radius, 40, 50);
				glPopMatrix();
			}
		}
};

class cone : public node {
	public:
		GLfloat color[3] = { 1.0f, 1.0f, 1.0f };
		GLfloat radius = 1.0;
		bool lid = false;

		void render() override {
			transform();
			glColor3fv(color);
			GLUquadricObj *objCylinder = gluNewQuadric();
			gluCylinder(objCylinder, 0, radius, 1.0, 40, 50);
			if (lid) {
				glPushMatrix();
				GLUquadricObj *objDiskTop = gluNewQuadric();
				glTranslatef(0, 0, 1.0);
				gluDisk(objDiskTop, 0, radius, 40, 50);
				glPopMatrix();
			}
		}
};

class prism : public node {
	private:
		void drawPrism();

	public:
		GLfloat color[3] = { 1.0f, 1.0f, 1.0f };

		void render() override {
			transform();
			glColor3fv(color);
			drawPrism();
		}
};

#endif
