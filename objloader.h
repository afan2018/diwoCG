#ifndef OBJLOADER_H
#define OBJLOADER_H

class Model_OBJ
{
public:
	Model_OBJ();
	int load(const char *filename);	// Loads the model
	void draw();					// Draws the model on the screen
	void release();				// Release the model

	float* normals;							// Stores the normals
	float* Faces_Triangles;					// Stores the triangles
	float* vertexBuffer;					// Stores the points which make the object
	long TotalConnectedPoints;				// Stores the total number of connected verteces
	long TotalConnectedTriangles;			// Stores the total number of connected triangles

	float scale;
};


#define POINTS_PER_VERTEX 3
#define TOTAL_FLOATS_IN_TRIANGLE 9;

extern Model_OBJ obj;
#endif