#ifndef GEOMETRIES_H
#define GEOMETRIES_H

#include "scene_graph.h"

#ifdef _WIN32
#include <windows.h>
#include <GL/GL.h>
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#endif

// use this to enable drawing with lists
// #define USE_LISTS

class box : public node {
private:
	static void draw_box(GLfloat size);

public:
    box() : node("geometry/box") {}

	void render() override {
		transform();
		colorize();
        draw_box(1.0f);
	}
};

class sphere : public node {
private:
	GLUquadricObj *objSphere;

#ifdef USE_LISTS
	GLint listId;
#endif
	void draw_sphere() {
        gluSphere(objSphere, 0.5, 40, 50);
	}

public:
	sphere() : node("geometry/sphere") {
		objSphere = gluNewQuadric();

		gluQuadricDrawStyle(objSphere, GLU_FILL);
		gluQuadricTexture(objSphere, GL_TRUE);
		gluQuadricNormals(objSphere, GLU_SMOOTH);

#ifdef USE_LISTS
		listId = glGenLists(1);
		glNewList(listId, GL_COMPILE);
		draw_sphere();
		glEndList();
#endif
	}

	~sphere() {
#ifdef USE_LISTS
        glDeleteLists(listId, 1);
#endif

		gluDeleteQuadric(objSphere);
	}

	void render() override {
		transform();
		colorize();

		glPushMatrix();
#ifdef USE_LISTS
		glCallList(listId);
#else
		draw_sphere();
#endif
		glPopMatrix();
	}
};

class cylinder : public node {
private:
	GLUquadricObj *objCylinder;
	GLUquadricObj *objDiskBottom;
	GLUquadricObj *objDiskTop;

#ifdef USE_LISTS
	GLuint listId;
#endif

	void draw_cylinder() {
		const GLfloat radius = 0.5f;

        gluCylinder(objCylinder, radius, radius, 1.0, 40, 5);
        gluDisk(objDiskBottom, 0, radius, 40, 50);
        glTranslatef(0, 0, 1.0);
        gluDisk(objDiskTop, 0, radius, 40, 50);
	}

public:
	cylinder() : node("geometry/cylinder") {
		center[2] = 0.5f;
		objCylinder = gluNewQuadric();
		objDiskBottom = gluNewQuadric();
		objDiskTop = gluNewQuadric();

		gluQuadricOrientation(objDiskTop, GLU_OUTSIDE);
		gluQuadricOrientation(objDiskBottom, GLU_INSIDE);

		gluQuadricDrawStyle(objCylinder, GLU_FILL);
		gluQuadricTexture(objCylinder, GL_TRUE);
		gluQuadricNormals(objCylinder, GLU_SMOOTH);
		gluQuadricDrawStyle(objDiskBottom, GLU_FILL);
		gluQuadricTexture(objDiskBottom, GL_TRUE);
		gluQuadricNormals(objDiskBottom, GLU_SMOOTH);
		gluQuadricDrawStyle(objDiskTop, GLU_FILL);
		gluQuadricTexture(objDiskTop, GL_TRUE);
		gluQuadricNormals(objDiskTop, GLU_SMOOTH);

#ifdef USE_LISTS
		listId = glGenLists(1);
		glNewList(listId, GL_COMPILE);
		draw_cylinder();
		glEndList();
#endif
	}

	~cylinder() {
#ifdef USE_LISTS
        glDeleteLists(listId, 1);
#endif

		gluDeleteQuadric(objCylinder);
		gluDeleteQuadric(objDiskBottom);
		gluDeleteQuadric(objDiskTop);
	}

	void render() override {
		transform();
		colorize();

		glPushMatrix();
#ifdef USE_LISTS
		glCallList(listId);
#else
		draw_cylinder();
#endif
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

	cone() : node("geometry/cone") {
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
	static void draw_prism();

public:
	prism() : node("geometry/prism") {
		center[0] = center[1] = center[2] = 0.5f;
	}

	void render() override {
		transform();
		colorize();
        draw_prism();
	}
};

class frustum : public node {
private:
	static void draw_frustum();

public:
	frustum() : node("geometry/frustum") {
		center[0] = center[1] = center[2] = 0.5f;
	}

	void render() override {
		transform();
		colorize();
        draw_frustum();
	}
};
#endif
