/*
Line drawing in 3D.
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "osuGraphics.h"
#include "matlib.h"
#include "lines.h"
#include <Windows.h>
#include "GL\glew.h"
#include "GL\glut.h"


void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	osuShadeModel(OSU_SMOOTH);


	osuEnable(OSU_DEPTH_TEST);

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	float lpos[4] = { 1.0, 2.0, 1.0 , 0.0 };
	float temp = .9;
	osuAmbientLight(temp);
	osuPointLight(lpos, 1.0);

	osuDiffuse(0.0, 0.0, 1.0);
	osuSpecular(1.0, 0.0, 1.0, 1.0);

	osuTranslate(-.5, 0., -2.0);
	osuBegin(OSU_POLYGON);
	osuVertex3f(0.0, -1.75, -2.5);
	osuVertex3f(0.0, 1.75, -2.5);
	osuVertex3f(0.0, 1.75, -7.5);
	osuVertex3f(0.0, -1.75, -7.5);
	osuEnd();


	osuDiffuse(1.0, 0.0, 0.0);
	osuSpecular(1.0, 1.0, 1.0, 1.0);

	osuBegin(OSU_POLYGON);
	osuVertex3f(-4.5, -1.75, -5.5);
	osuVertex3f(-4.5, 1.75, -5.5);
	osuVertex3f(4.5, 1.75, -5.5);
	osuVertex3f(4.5, -1.75, -5.5);
	osuEnd();


	glFlush();
}

void reshape(int w, int h)
{
	osuInitialize();
	
	//osuOrtho(-1.5, 1.5, -1.5,
		//	1.5, -10.0, 10.0);
	osuPerspective(90., 0.1, 1000.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv)
{
	double from[3] = { 0.0, 0.0,0.0 }, at[3] = { 0,0,-1. }, up[3] = { 0,1,0 };
	osuBeginGraphics(500, 500);
	osuLookat(from, at, up);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}