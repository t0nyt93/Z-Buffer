/*

Dummy routines for matrix transformations.

These are for you to write!

*/


#include <stdio.h>
#include <math.h>
#include <fstream>
#include <Windows.h>
#include <ostream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
#include "osuGraphics.h"
#include "lines.h"
#include "vec3.h"
#include "mat4.h"
#include "osuVector.h"
#include "GL\glew.h"
#include "GL\glut.h"
//-------------------------------------------------

//------Data Structures ------------
struct myVertex {
	double myX = 0, myY = 0;
	int red, green, blue;
	int initialized = 0;
};
struct my3DVertex {
	Vec3 pos = Vec3();
	Vec3 color = Vec3();
	int intialized;
};

struct myLine {
	my3DVertex one, two;
}lDraw;

BOOL ORTHO = FALSE;
BOOL PROJECTION = FALSE;
double NP, FP;
//---------Project 2-------
std::vector<Mat4> matStack;
Vec3 u;
Mat4 Current_Transform = Mat4();
Mat4 persProj = Mat4();
Mat4 orthoProj = Mat4();
Mat4 myView = Mat4();
//------------------
//-------Project 3 ------
//Holds normal vector declared by osuNormal3f
osuVector3 globalNorm;
osuVector3 PointLight;
osuVector3 DirectionalLight;
int ambientIntesity;
osuVector3 diffuseColor;
osuVector4 specAndPhong;
std::vector<osuVector4> myPolygon;
//Set by osuEnable to show that we're z-buffering polygons
int globalDepth = 0;
double * myZBuff;
//------------------------
void osuOrtho(double left, double right, double bottom, double top, double nearp,double farp)
{
	printf("ORTHOGRAPHIC\n");
	glOrtho(left, right, bottom, top, nearp, farp);
	ORTHO = TRUE;
	PROJECTION = FALSE;

}
void osuPerspective(double fovy, double nearp, double farp) 
{
	gluPerspective(fovy, 1., nearp, farp);
	PROJECTION = TRUE;
	ORTHO = FALSE;

}

//All vertices of the polygon have been given so draw it and write them to the framebuffer. 
void osuEnd()
{
	glEnd();
	
}


void osuLoadIdentityMatrix()
{
	//Set the current Transform to the identity matrix
	glLoadIdentity();
}
void osuVertex3f(double x, double y, double z)
{
	glVertex3f(x, y, z);
}

void osuInitialize() 
{ 
	GLsizei height = 500, width = 500;
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix

	osuLoadIdentityMatrix();
}


void osuPushMatrix() 
{ 
	glPushMatrix();
}

void osuPopMatrix() 
{ 
	glPopMatrix();
}


void osuTranslate(double tx, double ty, double tz) 
{ 
	glTranslatef(tx, ty, tz);
}

void osuScale(double sx, double sy, double sz) 
{ 
	glScalef(sx, sy, sz);
}

void osuRotate(double angle, double ax, double ay, double az) { 
	

	glRotatef(angle, ax, ay, az);
		
}

void osuLookat(double from[3], double at[3], double up[3])
{
	//From = vector specifying direction from which the viewer would like to view the scene.
	//myView.SetTranslate(-from[0], -from[1], -from[2]);
	gluLookAt(from[0], from[1], from[2], at[0], at[1], at[2], up[0], up[1], up[2]);
}
//Specifies whether to use flat of smooth shading.

void osuShadeModel(OSUModel mode) {
	if (mode == OSU_SMOOTH)
	{
		glShadeModel(GL_SMOOTH);
	}
	else if (mode == OSU_FLAT)
	{
		glShadeModel(GL_FLAT);
	}
}
void osuBegin(OSUDrawable mode)
{

	if (mode == OSU_LINES)
	{
		//We're going to be drawing a polygon, push the vertices onto myPolygon
		glBegin(GL_LINES);
	}
	if (mode == OSU_TRIANGLE)
	{
		//We're going to be drawing a polygon, push the vertices onto myPolygon
		glBegin(GL_TRIANGLES);
	}
	if (mode == OSU_POLYGON)
	{
		//We're going to be drawing a polygon, push the vertices onto myPolygon
		glBegin(GL_POLYGON);
	}
}

void osuNormal3f(double x, double y,  double z) {
	globalNorm = osuVector3(x, y, z);
	glNormal3f(x, y, z);
}
void osuEnable(OSUEnable depthTestBit) {
	//Let's configure the z-buffer
	if (depthTestBit == OSU_DEPTH_TEST)
	{
		globalDepth = 1;
		int w, h;
		osuGetFramebufferSize(&w, &h);
		myZBuff = (double *)malloc(sizeof(double) * (w*h));
		glEnable(GL_DEPTH_TEST);

	}


}
//Clear out the z buffer and get it ready for use, reallocate it...
void osuClearZ() {
	int w, h;
	osuGetFramebufferSize(&w, &h);
	free(myZBuff);
	myZBuff = (double *)malloc(sizeof(double) * (w*h));

}

//Specifies a point light source at an x,y,z position with intensity i
void osuPointLight(float a[4], float) {

	GLfloat mat_shininess[] = { 50.0 };
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, a);

}
void osuDirectionalLight(float dir[3], int) {

	GLfloat mat_shininess[] = { 50.0 };
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);


}
void osuAmbientLight(float value){
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, &value);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}
void osuDiffuse(float a, float b, float c){
	GLfloat myDiffuse[] = { a,b, c , 0. };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, myDiffuse);
}
void osuSpecular(float, float, float, float)
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
}