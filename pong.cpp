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
const float BOARD_WIDTH = 160.0f;
const float BOARD_HEIGHT = 120.0f;
// Raquetas
const float RACKET_WIDTH = 4.0f;
const float RACKET_HEIGHT = 20.0f;
const float RACKET_SPEED = 5.0f;
// Posicion de las raquetas de los jugadores
double leftRacketY = BOARD_HEIGHT/2;
double rightRacketY = BOARD_HEIGHT/2;
double leftRacketX = RACKET_WIDTH/2;
double rightRacketX = BOARD_WIDTH - RACKET_WIDTH/2;


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
    glRectf(x-(RACKET_WIDTH/2), y-(RACKET_HEIGHT/2), x+(RACKET_WIDTH/2), y+(RACKET_HEIGHT/2));
}

// Add key state tracking
#define KEY_COUNT 256
bool keyStates[KEY_COUNT] = {false};
bool specialKeyStates[KEY_COUNT] = {false};
// Modified keyboard handlers
void keyboard(int key, int x, int y) {
    specialKeyStates[key] = true;  // Key pressed
}
void keyboardUp(int key, int x, int y) {
    specialKeyStates[key] = false; // Key released
}
void keyboardNormal(unsigned char key, int x, int y) {
    keyStates[key] = true;        // Key pressed
}
void keyboardNormalUp(unsigned char key, int x, int y) {
    keyStates[key] = false;       // Key released
}

void computeAndDisplayRackets() {
  // Handle left paddle (W/S)
  if(keyStates['w'] || keyStates['W']) {
      if(leftRacketY < BOARD_HEIGHT-(RACKET_HEIGHT/2)) leftRacketY += RACKET_SPEED;
  }
  if(keyStates['s'] || keyStates['S']) {
      if(leftRacketY > RACKET_HEIGHT/2) leftRacketY -= RACKET_SPEED;
  }
  // Handle right paddle (Up/Down)
  if(specialKeyStates[GLUT_KEY_UP]) {
    printf("Up key is pressed\n");
    if(rightRacketY < BOARD_HEIGHT-(RACKET_HEIGHT/2)) rightRacketY += RACKET_SPEED;
  }
  if(specialKeyStates[GLUT_KEY_DOWN]) {
    printf("Down key is pressed\n");
    if(rightRacketY > RACKET_HEIGHT/2) rightRacketY -= RACKET_SPEED;
  }

  drawRacket(leftRacketX, leftRacketY);
  drawRacket(rightRacketX, rightRacketY);
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
	} else if (ypos > BOARD_HEIGHT-RadiusOfBall && ydir > 0 ) {
    cout << "Touched top wall\n";
    ydir = -ydir;
  }
  ypos = ypos+ydir;
  // Horizontal movement:
  // 160 is max X value in our world
  if (xpos < RadiusOfBall && xdir < 0 ) {
    cout << "Touched left wall\n";
		xdir = -xdir;
	} else if (xpos > BOARD_WIDTH-RadiusOfBall && xdir > 0 ) {
    cout << "Touched right wall\n";
    xdir = -xdir;
  }
  xpos = xpos+xdir;

  drawBall(xpos, ypos);

  computeAndDisplayRackets();

  //Translate the bouncing ball to its new position
  //T[12]= xpos;
  //T[13] = ypos;
  //glLoadMatrixf(T);

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

  glutKeyboardFunc(keyboardNormal);
  glutKeyboardUpFunc(keyboardNormalUp);
  glutSpecialFunc(keyboard);
  glutSpecialUpFunc(keyboardUp);

  init();
  glutSetKeyRepeat(0);
  glutDisplayFunc(Display);
  glutReshapeFunc(reshape);
  glutMainLoop();

  return 1;
}
