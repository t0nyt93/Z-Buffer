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

struct myTriangle {
	myVertex one = {}, two = {}, three = {};
	int currRed, currGreen, currBlue;
	double minX, maxX, minY, maxY;
}tDraw;

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
	Mat4 oT;
	oT.SetTranslate((-(left + right) / 2.), (-(bottom + top) / 2.), (-(nearp + farp) / 2.));
	orthoProj.SetOrthoGraphic(left, right, top, bottom, nearp, farp);
	orthoProj = orthoProj * oT;
	ORTHO = TRUE;
	PROJECTION = FALSE;
	NP = nearp;
	FP = farp;
}
void osuPerspective(double fovy, double nearp, double farp) 
{
	printf("PERSPECTIVE\n");

	persProj.SetPerspective(fovy, nearp, farp);
	persProj.Print();
	PROJECTION = TRUE;
	ORTHO = FALSE;
	NP = nearp;
	FP = farp;
}

//Specify the RGB of all subsequent vertices that will be drawn
void osuColor3f(double red, double green, double blue)
{
	//Convert float numbers to a r-g-b that can be written to a pixel
	tDraw.currRed = red * 255;
	tDraw.currGreen = green * 255;
	tDraw.currBlue = blue * 255;

}

//Specify x/y position in screen space of a vertex. Add it to image->values
void osuVertex2f(double x, double y)
{

	int w, h;
	osuGetFramebufferSize(&w, &h);
	//convert the 0 -> 1 coordinates into pixel coordinates.
	x *= w;
	y *= h;

	//If we haven't initialized our first vertex yet...do so
	if (tDraw.one.initialized == 0)
	{
		tDraw.one.initialized = 1;
		tDraw.one.myX = x;
		tDraw.minX = x;
		tDraw.maxX = x;
		tDraw.one.myY = y;
		tDraw.minY = y;
		tDraw.maxY = y;
		tDraw.one.red = tDraw.currRed;
		tDraw.one.green = tDraw.currGreen;
		tDraw.one.blue = tDraw.currBlue;
	}
	//If we haven't initialized our second vertex yet...do so

	else if (tDraw.two.initialized == 0)
	{

		tDraw.two.initialized = 1;
		tDraw.two.myX = x;
		tDraw.two.myY = y;
		tDraw.two.red = tDraw.currRed;
		tDraw.two.green = tDraw.currGreen;
		tDraw.two.blue = tDraw.currBlue;

		//Make sure our mins and maxes are current with this new vertex
		if (tDraw.minX > tDraw.two.myX)
			tDraw.minX = tDraw.two.myX;
		if (tDraw.maxX < tDraw.two.myX)
			tDraw.maxX = tDraw.two.myX;
		if (tDraw.minY > tDraw.two.myY)
			tDraw.minY = tDraw.two.myY;
		if (tDraw.maxY < tDraw.two.myY)
			tDraw.maxY = tDraw.two.myY;

	}
	//If we've initialized all of the other vertexes...do this one
	else if (tDraw.three.initialized == 0)
	{
		tDraw.three.initialized = 1;
		tDraw.three.myX = x;
		tDraw.three.myY = y;
		tDraw.three.red = tDraw.currRed;
		tDraw.three.green = tDraw.currGreen;
		tDraw.three.blue = tDraw.currBlue;

		//Make sure our mins and maxes are current with this new vertex
		if (tDraw.minX > tDraw.three.myX)
			tDraw.minX = tDraw.three.myX;
		if (tDraw.maxX < tDraw.three.myX)
			tDraw.maxX = tDraw.three.myX;
		if (tDraw.minY > tDraw.three.myY)
			tDraw.minY = tDraw.three.myY;
		if (tDraw.maxY < tDraw.three.myY)
			tDraw.maxY = tDraw.three.myY;


	}

}
//All vertices of the polygon have been given so draw it and write them to the framebuffer. 
void osuEnd()
{
	printf("Drawing is over?\n");
	//We're drawing lines now, not triangles.

	/*Vec3 myN, myNQ, myNR, myNS;
	int myRed, myGreen, myBlue;
	float p1, p2, p3;
	float aC, bC, cC, pC;
	float a, b, c, p;
	float area, areaOne, areaTwo, areaThree;

	//Vectors of our triangle sides
	Vec3 one = Vec3(tDraw.one.myX, tDraw.one.myY, 0.);
	Vec3 two = Vec3(tDraw.two.myX, tDraw.two.myY, 0.);
	Vec3 three = Vec3(tDraw.three.myX, tDraw.three.myY, 0.);

	//Computing the Area of the entire triangle that we're drawing. 
	aC = (two - three).Length();
	bC = (one - two).Length();
	cC = (three - one).Length();
	pC = (aC + bC + cC) / 2.;
	area = sqrt(pC*(pC - aC)*(pC - bC)*(pC - cC));

	Vec3 currPos = Vec3(0., 0., 0.);
	//Loop through this triangles minimum/maximum width
	for (int i = (tDraw.minX); i < tDraw.maxX + 1; i++)
	{
		//Loop through it's minumum/maximum heigth.b
		for (int j = (tDraw.minY); j < (tDraw.maxY + 1); j++)
		{
			//Three dimensional point inside a triangle implmentation:
			//http://web.engr.oregonstate.edu/~mjb/cs491/Handouts/vectors.1pp.pdf
			//Initialize our point within this space
			currPos = Vec3(i, j, 0.);
			//Get the Normal vector of the triangle
			myN = (three - two).Cross(one - two);
			myNQ = (three - two).Cross(currPos - two);
			myNR = (one - three).Cross(currPos - three);
			myNS = (two - one).Cross(currPos - one);
			//do my cross products conclude that the points inside my triangle?			
			if ((myN.Dot(myNQ) > 0) && (myN.Dot(myNR) >= 0) && (myN.Dot(myNS) > 0))
			{
				//Interpolation--------------------------------	
				//First subtriangle area....
				a = (currPos - three).Length();
				b = (one - currPos).Length();
				c = (three - one).Length();
				p = (a + b + c) / 2.;
				areaOne = sqrt(p*(p - a)*(p - b)*(p - c));
				//Second subtriangle area.....
				a = (currPos - two).Length();
				b = (three - currPos).Length();
				c = ((two - three).Length());
				p = (a + b + c) / 2.;
				areaTwo = sqrt((p*(p - a)*(p - b)*(p - c)));
				//Third subtriangle area.....
				a = (currPos - two).Length();
				b = (one - currPos).Length();
				c = ((two - one).Length());
				p = (a + b + c) / 2.;
				areaThree = sqrt(p*(p - a)*(p - b)*(p - c));
				//Now Convert our areas to a proportional amount of the triangle
				p1 = areaTwo / area;
				p2 = areaOne / area;
				p3 = areaThree / area;
				//Apply these proportions to the individual pixel color..
				myRed = ((p1 * tDraw.one.red) + (p2 * tDraw.two.red) + (p3 * tDraw.three.red));
				myGreen = ((p1 * tDraw.one.green) + (p2 * tDraw.two.green) + (p3 * tDraw.three.green));
				myBlue = ((p1 * tDraw.one.blue) + (p2 * tDraw.two.blue) + (p3 * tDraw.three.blue));
				// End of interpolation ---------------------------------------------------------------
				//Write to our beloved pixel. 
				osuWritePixel(i, j, myRed, myGreen, myBlue);
			}
		}
	}*/
	
}



void osuVertex3f(double x, double y, double z)
{
	if (lDraw.one.intialized == 0)
	{
		lDraw.one.intialized = 1;
		lDraw.one.pos = Vec3((float)x,(float)y,(float)z);
	}
	else if (lDraw.two.intialized == 0)
	{
		int w, h;
		osuGetFramebufferSize(&w, &h);
		lDraw.two.intialized = 1;
		lDraw.two.pos = Vec3((float)x,(float)y,(float)z);
		Vec3 p1, p2;
		double x0, y0, x1, y1, z0,z1;
		Mat4 ModelViewMat,total;
		Mat4 WindowTransform;
		WindowTransform.SetWindow(w,h);
		//If we need to perform a orthographic projection
		if ((ORTHO == TRUE) && (PROJECTION == FALSE))
		{
			ModelViewMat = myView * Current_Transform;
			total =  WindowTransform * orthoProj  * ModelViewMat;
			p1 =(total * lDraw.one.pos);
			p2 =(total * lDraw.two.pos);

			x0 = p1.x; y0 = p1.y; x1 = p2.x; y1 = p2.y;

			draw_line(x0, y0, x1, y1);
		}
		//We need to perform a perspective projection
		else if ((PROJECTION == TRUE) && (ORTHO == FALSE))
		{
			ModelViewMat = myView * Current_Transform;
			total =  persProj * ModelViewMat;
			p1 = (total * lDraw.one.pos);
			p2 = (total * lDraw.two.pos);
			//x0 = (p1.x * w);  y0 = (p1.y); x1 = (p2.x) ; y1 =(p2.y), z0 = abs(p1.z), z1 = abs(p2.z);
			printf("%d and %d\n", w, h);
			x0 = (p1.x * w)/(2.0 * p1.z); y0 = (p1.y * h) /(2.0* p1.z);
			x1 = (p2.x * w) / (2.0 * p2.z); y1 = (p2.y * h) / (2.0* p2.z);
			if (near_far_clip(NP, FP, &x0, &y0, &z0, &x1, &y1, &z1) != 0);
			{
				draw_line(x0, y0, x1, y1);
				printf("%8.2f %8.2f %8.2f %8.2f\n", x0, y0, x1, y1);

			}
		}
		//Something got messed up.
		else
			printf("Neither a project or Orthographic was given, or BOTH we're given concurrently.\n");
		lDraw = {};
	}
	
}

void osuInitialize() 
{ 
	//Create an identity matrix and push it on the stack
	Mat4 transformMat = Mat4();
	Current_Transform = transformMat;
	transformMat.SetIdentity();
	printf("Pushing identity Matrix onto the matStack.\n");
	matStack.push_back(transformMat);

}


void osuPushMatrix() 
{ 
	Mat4 newTop = Mat4(matStack[matStack.size()-1]);
	matStack.push_back(newTop);
	Current_Transform = newTop;
}

void osuPopMatrix() 
{ 
	if (matStack.size() > 1)
	{   //Ok you can do it.
		matStack.pop_back();
		Current_Transform = matStack[matStack.size()-1];
	}
	else
	{
		printf("Pop: Can't perform action on Identity Matrix.\n");
	}
}

void osuLoadIdentityMatrix()
{
	//Set the current Transform to the identity matrix
	matStack.push_back(Current_Transform);
	Current_Transform.SetIdentity();
}
void osuTranslate(double tx, double ty, double tz) 
{ 
	Mat4 t = Mat4();
	t.SetTranslate(tx, ty, tz);
	Current_Transform = Current_Transform * t;
}

void osuScale(double sx, double sy, double sz) 
{ 
	Mat4 s = Mat4();
	s.SetScale(sx, sy, sz);
	Current_Transform = Current_Transform * s;
}

void osuRotate(double angle, double ax, double ay, double az) { 
	

	Mat4 t, r;
	if (ax > 0.)
	{
		printf("Rotating around x\n");
		r.SetRotateX(angle);
		Current_Transform = Current_Transform * r;
	}
	if (ay > 0.)
	{
		printf("Rotating around y.\n");
		r.SetRotateY(angle);

		Current_Transform = Current_Transform * r;
	}
	if (az > 0.)
	{
		printf("Rotating around z\n");
		printf("CT was\n");
		Current_Transform.Print();
		r.SetRotateZ(angle);
		Current_Transform = Current_Transform * r;
		printf("Now it's\n");
		Current_Transform.Print();
	}
		
}

void osuLookat(double from[3], double at[3], double up[3])
{
	//From = vector specifying direction from which the viewer would like to view the scene.
	//myView.SetTranslate(-from[0], -from[1], -from[2]);
	Vec3 Eye, Up, Center,u,v,w;
	
	Eye = Vec3(from[0], from[1], from[2]);
	Up = Vec3(up[0], up[1], up[2]);
	Center = Vec3(at[0], at[1], at[2]);
	w = Eye - Center;
	w = w.Unit();
	u = Up.Cross(w);
	u = u.Unit();
	v = w.Cross(u);

	myView.SetCamera(u,v,w,Eye);
	Mat4 t;
	t.SetTranslate(-Eye.x, -Eye.y, -Eye.z);
	myView = myView * t;
}

void osuBegin(OSUDrawable mode)
{
	if (mode == OSU_TRIANGLE)
	{
		//We're going to draw a triangle, so intialize our data structure to hold it
		tDraw = {};
		//Also make sure to set a default color, incase the user forgets to give you one. 
		osuColor3f(1.0, 1.0, 1.0);

	}
	else if (mode == OSU_LINES)
	{
		printf("Time for vertexes\n");

		lDraw = {};
		//View for 5 WORKING
		double f[3] = {15.,50.,50.}, a[3] = {0.,0.,-45. }, myUp[3] = { 0,1,0. };
		osuLookat(f, a, myUp);

	}
	else if (mode == OSU_POLYGON)
	{
		//We're going to be drawing a polygon, push the vertices onto myPolygon
		myPolygon.clear();
	}
}

void osuNormal3f(double x, double y,  double z) {
	globalNorm = osuVector3(x, y, z);
}
void osuEnable(int depthTestBit) {
	//Let's configure the z-buffer
	if (depthTestBit == OSU_DEPTH_TEST)
	{
		globalDepth = 1;
		int w, h;
		osuGetFramebufferSize(&w, &h);
		myZBuff = (double *)malloc(sizeof(double) * (w*h));
	}

}
//Clear out the z buffer and get it ready for use, reallocate it...
void osuClearZ() {
	int w, h;
	osuGetFramebufferSize(&w, &h);
	free(myZBuff);
	myZBuff = (double *)malloc(sizeof(double) * (w*h));

}
//Specifies whether to use flat of smooth shading.
void osuShadeModel(int) {

}
//Specifies a point light source at an x,y,z position with intensity i
void osuPointLight(float a[3], float) {

}
void osuDirectionalLight(float dir[3], int) {

}
void osuAmbientLight(float){

}
void osuDiffuse(float, float, float){

}
void osuSpecular(float, float, float, float)
{
}