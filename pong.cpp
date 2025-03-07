//#include <windows.h> //the windows include file, required by all windows applications
#include <GL/freeglut.h> //the glut file for windows operations
                     // it also includes gl.h and glu.h for the openGL library calls
#include <math.h>
#include <iostream>
using namespace std;

#define PI 3.1415926535898

// x and y position for ball
double xpos, ypos, ydir, xdir;

int SPEED = 50;   // speed of timer call back in msecs
GLfloat T[16] = {1.,0.,0.,0.,\
                 0., 1., 0., 0.,\
                 0.,0.,1.,0.,\
                 0.,0.,0.,1.};
GLfloat RadiusOfBall = 4.;

// Tablero
float boardWidth = 160.0f;
float boardHeight = 120.0f;
// Raquetas
float racketWidth = 4.0f;
float racketHeight = 20.0f;
// Posicion de las raquetas de los jugadores
double leftRacketY = boardHeight/2;
double rightRacketY = boardHeight/2;
double leftRacketX = racketWidth/2;
double rightRacketX = boardWidth - racketWidth/2;


GLint circle_points = 100;
void MyCircle2f(GLfloat centerx, GLfloat centery, GLfloat radius){
  GLint i;
  GLdouble angle;
  glBegin(GL_POLYGON);
  for (i = 0; i < circle_points; i++) {
    angle = 2*PI*i/circle_points;
    glVertex2f(centerx+radius*cos(angle), centery+radius*sin(angle));
  }
  glEnd();
}

// Draw the ball, centered at the origin
void drawBall(float x, float y) {
  glColor3f(255.,255.,255.);
  MyCircle2f(x,y,RadiusOfBall);
}

void drawRacket(float x, float y) {
    // Draw 20x100 rectangle centered at (x,y)
    glRectf(x-(racketWidth/2), y-(racketHeight/2), x+(racketWidth/2), y+(racketHeight/2));
}

void Display(void)
{
  // swap the buffers
  glutSwapBuffers();

  //clear all pixels with the specified clear color
  glClear(GL_COLOR_BUFFER_BIT);

	// Ball collides with walls
  // Vertical movement:
  // 120 is max Y value in our world
	if (ypos < RadiusOfBall && ydir < 0 ) {
    cout << "Touched bottom wall\n";
		ydir = -ydir;
	} else if (ypos > boardHeight-RadiusOfBall && ydir > 0 ) {
    cout << "Touched top wall\n";
    ydir = -ydir;
  }
  ypos = ypos+ydir;
  // Horizontal movement:
  // 160 is max X value in our world
  if (xpos < RadiusOfBall && xdir < 0 ) {
    cout << "Touched left wall\n";
		xdir = -xdir;
	} else if (xpos > boardWidth-RadiusOfBall && xdir > 0 ) {
    cout << "Touched right wall\n";
    xdir = -xdir;
  }
  xpos = xpos+xdir;

  drawRacket(leftRacketX, leftRacketY);
  drawRacket(rightRacketX, rightRacketY);

  //Translate the bouncing ball to its new position
  //T[12]= xpos;
  //T[13] = ypos;
  //glLoadMatrixf(T);

  drawBall(xpos, ypos);
  glutPostRedisplay();
}


void reshape (int w, int h)
{
   // on reshape and on startup, keep the viewport to be the entire size of the window
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();

   // keep our logical coordinate system constant
   gluOrtho2D(0.0, 160.0, 0.0, 120.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity ();

}


void init(void){
  xpos = 60; ypos = RadiusOfBall; xdir = 0.7; ydir = 1;
}


int main(int argc, char* argv[])
{
  glutInit( & argc, argv );
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (320, 240);
  glutCreateWindow("Bouncing Ball");
  init();
  glutDisplayFunc(Display);
  glutReshapeFunc(reshape);
  glutMainLoop();

  return 1;
}
