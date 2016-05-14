/**
*	CS551 Graphics Support Code 
*	Implementation File 
*	Author:		Turk, Metoyer
*	Date:		1/10/07
*/

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library		
#include <gl\glut.h>		//glut library
#include <stdio.h>
#include <malloc.h>
#include "osuGraphics.h"

typedef unsigned char Byte;


// osuImage Data Type Implementation                                
long osuArray3DTo1D ( int chan, int w, int sizeW, int h, int sizeH )
{
   // convert 3D array coordinates to a single lookup value 
   return ( chan * sizeW * sizeH + w * sizeH + h );
}

/*-----------------------------------*/

long osuImageValueCount ( osuImage *I )
{
   return ( I->w * I->h * 3 );
}

/*-----------------------------------*/

void osuImageInit ( osuImage *I )
{
   I->w = I->h = 0;
   I->values = (int *) 0;
}

/*-----------------------------------*/

void osuImageDestroy ( osuImage *I )
{
   free( I->values );
}

/*-----------------------------------*/

void osuImageGetSize ( osuImage *I, int *w, int *h )
{
   *w = I->w;
   *h = I->h;
}

/*-----------------------------------*/

void osuImageSetSize ( osuImage *I, int w, int h )
{
   long count;

   free( I->values );

   I->w = w;
   I->h = h;
   count = osuImageValueCount( I );
   I->values = (int *) calloc( (unsigned int) count, sizeof( int ) );
   if ( ! I->values ) {
      fprintf( stderr, "osuImage not allocated\n" );
      exit( 2 );
   }
}

/*-----------------------------------*/



void osuImageReadPixel ( osuImage *I, int w, int h, int *r, int *g, int *b )
{
   long index = osuArray3DTo1D( OSU_RED, w, I->w, h, I->h );
   *r = I->values[ index ];

   index = osuArray3DTo1D( OSU_GREEN, w, I->w, h, I->h );
   *g = I->values[ index ];

   index = osuArray3DTo1D( OSU_BLUE, w, I->w, h, I->h );
   *b = I->values[ index ];
}

/*-----------------------------------*/

void osuImageWritePixel ( osuImage *I, int w, int h, int r, int g, int b )
{
   long index;

   index = osuArray3DTo1D( OSU_RED, w, I->w, h, I->h );
   I->values[ index ] = r;

   index = osuArray3DTo1D( OSU_GREEN, w, I->w, h, I->h );
   I->values[ index ] = g;

   index = osuArray3DTo1D( OSU_BLUE, w, I->w, h, I->h );
   I->values[ index ] = b;
}


// Single Window Interface 

static int /* OSUWriteMode */ OSUCurrentMode;
static osuImage OSUCurrentImage;

void osuBeginGraphics ( int w, int h )
{
	int argc = 1;
	char *argv = "Foo" ;
	glutInit(&argc,&argv);

	// set up a window with RGB color and one buffer 
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(w,h);
	glutInitWindowPosition(0,0);
	glutCreateWindow("CS551 OSUGL");
    //gluOrtho2D( -0.5, w - 0.5, -0.5, h - 0.5 );



   //	set the single window state 
   OSUCurrentMode = OSU_OVERWRITE;

   //	build a frame buffer for reading and writing 
   osuImageInit( &OSUCurrentImage );
   osuImageSetSize( &OSUCurrentImage, w, h );
   osuClear( 0, 0, 0 );
}

void osuEndGraphics(int w, int h)
{
	osuImageDestroy(&OSUCurrentImage);
}

/*-----------------------------------*/

void osuGetFramebufferSize ( int *w, int *h )
{
   *w = OSUCurrentImage.w;
   *h = OSUCurrentImage.h;
}

/*-----------------------------------*/


/*-----------------------------------*/

void osuClear ( int r, int g, int b )
{
   int i, j;


   glClearColor( (float) r, (float) g, (float) b, 0.0 );
   glClear( GL_COLOR_BUFFER_BIT );

   // clear image 
   for ( i = 0; i < OSUCurrentImage.w; i++ )
      for ( j = 0; j < OSUCurrentImage.h; j++ )
         osuImageWritePixel( &OSUCurrentImage, i, j, r, g, b );
}

/*-----------------------------------*/

void osuFlush ()
{
   glFlush ();
}

/*-----------------------------------*/

void osuSetWriteMode ( int /* OSUWriteMode */ mode )
{
   OSUCurrentMode = mode;
}

/*-----------------------------------*/

int osuXOR ( int a, int b )
{
   return ( a ^ b);
}


/*-----------------------------------*/

void osuWritePixel ( int x, int y, int r, int g, int b )
{
   int or, og, ob;

   if ( x < 0 || x >= OSUCurrentImage.w || y < 0 || y >= OSUCurrentImage.h )
   {
     fprintf( stderr, "Attempted to write a pixel outside the image " );
     fprintf( stderr, "bounds: x = %d, y = %d\n", x, y );
     return;
   }

   if ( OSUCurrentMode == OSU_XOR )
   {
      osuImageReadPixel( &OSUCurrentImage, x, y, &or, &og, &ob );
      r = osuXOR ( r, or );
      g = osuXOR ( g, og );
      b = osuXOR ( b, ob );
   }


   glColor3f( r / 255.0, g / 255.0, b / 255.0 );
   glBegin( GL_POINTS );
     glVertex2i( x, y );
   glEnd();


   osuImageWritePixel( &OSUCurrentImage, x, y, r, g, b );
}

/*-----------------------------------*/

void osuRedraw ( void )
{
   int x, y;
   int r, g, b;

   // Reapply fake 2D screen - problems occur otherwise 
   // Nothing is redrawn if this is used

   glClear(GL_COLOR_BUFFER_BIT);
   glBegin( GL_POINTS );

      for ( x = 0; x < OSUCurrentImage.w; x++ ) {
         for ( y = 0; y < OSUCurrentImage.h; y++ ) {
            osuImageReadPixel( &OSUCurrentImage, x, y, &r, &g, &b );
            glColor3f( r / 255.0, g / 255.0, b / 255.0 );
            glVertex2i( x, y );
		}
     }

   glEnd();
   glFlush();
}


void keyPress(char key, int x, int y)
{
	switch (key)
	{
		case 'q':
			exit(0);
		default:
			printf("Press 'q' key to quit \n");

	}
}

/*-----------------------------------*/

void osuWaitOnEscape ()
{   
	glutKeyboardFunc(keyPress);
	glutDisplayFunc(osuRedraw);
	glutMainLoop();
}

/*-----------------------------------*/

