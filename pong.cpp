//#include <windows.h> //the windows include file, required by all windows applications
#include <GL/freeglut.h> //the glut file for windows operations
                     // it also includes gl.h and glu.h for the openGL library calls
#include <math.h>
#include <iostream>
#include <random>
#include <cmath>
using namespace std;

#define PI 3.1415926535898

// x and y position for ball
double xpos, ypos, ydir, xdir;

int SPEED = 24;   // speed of timer call back in msecs
GLfloat T[16] = {1.,0.,0.,0.,\
                 0., 1., 0., 0.,\
                 0.,0.,1.,0.,\
                 0.,0.,0.,1.};
GLfloat RadiusOfBall = 4.;

// Tablero
const float BOARD_WIDTH = 160.0f;
const float BOARD_HEIGHT = 120.0f;
// Raquetas
const float RACKET_WIDTH = 10.0f;
const float RACKET_HEIGHT = 20.0f;
const float RACKET_SPEED = 5.0f;
// Posicion de las raquetas de los jugadores
double leftRacketY = BOARD_HEIGHT/2;
double rightRacketY = BOARD_HEIGHT/2;
double leftRacketX = RACKET_WIDTH/2;
double rightRacketX = BOARD_WIDTH - RACKET_WIDTH/2;
// Puntos
int rightScore = 0;
int leftScore = 0;

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
    if(rightRacketY < BOARD_HEIGHT-(RACKET_HEIGHT/2)) rightRacketY += RACKET_SPEED;
  }
  if(specialKeyStates[GLUT_KEY_DOWN]) {
    if(rightRacketY > RACKET_HEIGHT/2) rightRacketY -= RACKET_SPEED;
  }
  drawRacket(leftRacketX, leftRacketY);
  drawRacket(rightRacketX, rightRacketY);
}

double get_random_double() {
    static std::mt19937 eng(std::random_device{}());
    static std::bernoulli_distribution selector(0.5); // 50% chance for each range
    static std::uniform_real_distribution<double> negative_dist(-0.5, -0.2);
    static std::uniform_real_distribution<double> positive_dist(0.2, 0.5);
    return selector(eng) ? positive_dist(eng) : negative_dist(eng); 
}

void resetBall() {
  ypos = BOARD_HEIGHT/2;
  xpos = BOARD_WIDTH/2;
  ydir = get_random_double();
  xdir = get_random_double();
}

void score(char sideThatScored) {
  if (sideThatScored == 'L') { // Left side scored
    leftScore += 1;
  } else { // Right side scored
    rightScore += 1;
  }
  std::cout << "\rLeft score: " << leftScore << ", Right score: " << rightScore << "    " << std::flush;
  resetBall();
}

void Display(void)
{
  // swap the buffers
  glutSwapBuffers();

  //clear all pixels with the specified clear color
  glClear(GL_COLOR_BUFFER_BIT);

  // Ball collides with racket's longer side
  if ((xpos <= RadiusOfBall + RACKET_WIDTH) && (ypos <= leftRacketY + RACKET_HEIGHT/2 + RadiusOfBall && ypos >= leftRacketY - RACKET_HEIGHT/2 - RadiusOfBall) && xdir<0) {
    xdir = -xdir;
  } else if ((xpos >= BOARD_WIDTH - RadiusOfBall - RACKET_WIDTH) && (ypos <= rightRacketY + RACKET_HEIGHT/2 + RadiusOfBall && ypos >= rightRacketY - RACKET_HEIGHT/2 - RadiusOfBall) && xdir>0) {
    xdir = -xdir;
  }
  // Ball collides with racket's top or bottom side
  if ((xpos <= RadiusOfBall + RACKET_WIDTH) && (
    (ypos <= leftRacketY + RACKET_HEIGHT/2 + RadiusOfBall + 1 && ypos > leftRacketY + RACKET_HEIGHT/2 + RadiusOfBall) || 
    (ypos >= leftRacketY - RACKET_HEIGHT/2 - RadiusOfBall - 1 && ypos < leftRacketY - RACKET_HEIGHT/2 - RadiusOfBall)
    ) && xdir<0) {
    ydir = -ydir;
  } else if ((xpos >= BOARD_WIDTH - RadiusOfBall - RACKET_WIDTH) && (
    (ypos <= rightRacketY + RACKET_HEIGHT/2 + RadiusOfBall + 1 && ypos > rightRacketY + RACKET_HEIGHT/2 + RadiusOfBall) ||
    (ypos >= rightRacketY - RACKET_HEIGHT/2 - RadiusOfBall - 1 && ypos < rightRacketY - RACKET_HEIGHT/2 - RadiusOfBall)
    ) && xdir>0) {
    ydir = -ydir;
  }

	// Ball collides with walls
  // Vertical movement:
	if (ypos < RadiusOfBall && ydir < 0 ) {
		ydir = -ydir;
	} else if (ypos > BOARD_HEIGHT-RadiusOfBall && ydir > 0 ) {
    ydir = -ydir;
  }
  // Horizontal movement:
  if (xpos < RadiusOfBall) {
    score('R');
	} else if (xpos > BOARD_WIDTH-RadiusOfBall) {
    score('L');
  }

  // Compute next position
  double dx = xdir;
  double dy = ydir;
  double magnitude = std::sqrt(dx*dx + dy*dy);
  xpos = xpos + dx/magnitude;
  ypos = ypos + dy/magnitude;

  drawBall(xpos, ypos);
  computeAndDisplayRackets();
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
