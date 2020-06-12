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
private:
	GLUquadricObj *objSphere;
	GLint listId;
public:
	sphere() {
		objSphere = gluNewQuadric();

		gluQuadricDrawStyle(objSphere, GLU_FILL);
		gluQuadricTexture(objSphere, GL_TRUE);
		gluQuadricNormals(objSphere, GLU_SMOOTH);

		listId = glGenLists(1);
		glNewList(listId, GL_COMPILE);
		gluSphere(objSphere, 0.5, 40, 50);
		glEndList();
	}

	~sphere() {
        glDeleteLists(listId, 1);

		gluDeleteQuadric(objSphere);
	}

	void render() override {
		transform();
		colorize();

		glPushMatrix();
		glCallList(listId);
		glPopMatrix();
	}
};

class cylinder : public node {
private:
	GLUquadricObj *objCylinder;
	GLUquadricObj *objDiskBottom;
	GLUquadricObj *objDiskTop;

	GLuint listId;

public:
	cylinder() {
		center[2] = 0.5f;
		objCylinder = gluNewQuadric();
		objDiskBottom = gluNewQuadric();
		objDiskTop = gluNewQuadric();

		const GLfloat radius = 0.5f;

		gluQuadricDrawStyle(objCylinder, GLU_FILL);
		gluQuadricTexture(objCylinder, GL_TRUE);
		gluQuadricNormals(objCylinder, GLU_SMOOTH);
		gluQuadricDrawStyle(objDiskBottom, GLU_FILL);
		gluQuadricTexture(objDiskBottom, GL_TRUE);
		gluQuadricNormals(objDiskBottom, GLU_SMOOTH);
		gluQuadricDrawStyle(objDiskTop, GLU_FILL);
		gluQuadricTexture(objDiskTop, GL_TRUE);
		gluQuadricNormals(objDiskTop, GLU_SMOOTH);

		listId = glGenLists(1);
		glNewList(listId, GL_COMPILE);
		gluCylinder(objCylinder, radius, radius, 1.0, 40, 5);
		gluDisk(objDiskBottom, 0, radius, 40, 50);

		glTranslatef(0, 0, 1.0);
		gluDisk(objDiskTop, 0, radius, 40, 50);

		glEndList();
	}

	~cylinder() {
        glDeleteLists(listId, 1);

		gluDeleteQuadric(objCylinder);
		gluDeleteQuadric(objDiskBottom);
		gluDeleteQuadric(objDiskTop);
	}

	void render() override {
		transform();
		colorize();

		glPushMatrix();
		glCallList(listId);
		glPopMatrix();
	}
};

class cone : public node {
private:
	GLUquadricObj *objCylinder;
	GLUquadricObj *objDiskTop;
	GLint listId;

public:
	bool lid = true;

	cone() {
		center[2] = 0.5f;
		objCylinder = gluNewQuadric();
		objDiskTop = gluNewQuadric();

		gluQuadricDrawStyle(objCylinder, GLU_FILL);
		gluQuadricTexture(objCylinder, GL_TRUE);
		gluQuadricNormals(objCylinder, GLU_SMOOTH);
		gluQuadricDrawStyle(objDiskTop, GLU_FILL);
		gluQuadricTexture(objDiskTop, GL_TRUE);
		gluQuadricNormals(objDiskTop, GLU_SMOOTH);

		const GLfloat radius = 0.5f;

		listId = glGenLists(1);
		glNewList(listId, GL_COMPILE);

		gluCylinder(objCylinder, 0, radius, 1.0, 40, 50);
		glPushMatrix();
		glTranslatef(0, 0, 1.0);
		gluDisk(objDiskTop, 0, radius, 40, 50);
		glPopMatrix();

		glEndList();
	}

	~cone() {
		gluDeleteQuadric(objCylinder);
		gluDeleteQuadric(objDiskTop);
	}

	void render() override {
		transform();
		colorize();

		glPushMatrix();
		glCallList(listId);
		glPopMatrix();
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
