//#include <windows.h> //the windows include file, required by all windows applications
#include <GL/freeglut.h> //the glut file for windows operations
                     // it also includes gl.h and glu.h for the openGL library calls
#include <math.h>
#include <iostream>
using namespace std;

#define PI 3.1415926535898

double xpos, ypos, ydir, xdir;         // x and y position for house to be drawn
double rot, rdir;             // rotation
int SPEED = 50;        // speed of timer call back in msecs
GLfloat T[16] = {1.,0.,0.,0.,\
                 0., 1., 0., 0.,\
                 0.,0.,1.,0.,\
                 0.,0.,0.,1.};
GLfloat RadiusOfBall = 5.;


// Posicion de las raquetas de los jugadores
double leftRacketY = 0.0f;
double rightRacketY = 0.0f;


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
void draw_ball() {
  glColor3f(255.,255.,255.);
  MyCircle2f(0.,0.,RadiusOfBall);
}

void drawRacket(float x, float y) {
    // Draw 20x100 rectangle centered at (x,y)
    glRectf(x - 10, y - 50, x + 10, y + 50);
}

void Display(void)
{
  // swap the buffers
  glutSwapBuffers();

  //clear all pixels with the specified clear color
  glClear(GL_COLOR_BUFFER_BIT);
  // 160 is max X value in our world
	// Define X position of the ball to be at center of window
	xpos = 80.;

	// Shape has hit the ground! Stop moving and start squashing down and then back up
	if (ypos == RadiusOfBall && ydir == -1  ) {
    cout << "Touched bottom\n";
		ydir = 1;
	} else if (ypos == 120-RadiusOfBall && ydir == 1 ) {
    cout << "Touched top\n";
    ydir = -1;
  }
  ypos = ypos+ydir;

  //Translate the bouncing ball to its new position
  T[12]= xpos;
  T[13] = ypos;
  glLoadMatrixf(T);

  draw_ball();
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
  //set the clear color to be white
  //glClearColor(0.0,0.8,0.0,1.0);
  // initial position set to 0,0
  xpos = 60; ypos = RadiusOfBall; xdir = 1; ydir = 1;
  rot = 0;
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
