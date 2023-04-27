#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <time.h>

using namespace std;

#define DEGREES_PER_PIXEL	0.75f
#define UNITS_PER_PIXEL		0.2f
#define ZOOM_FACTOR		0.06f

enum {
  TORSO = 0,
  LUA, LLA,   LLL, LUL,
  RUA, RLA, RLL, RUL,
  DANCE,
  QUIT
};
#define HEAD_HEIGHT 3.0
#define HEAD_RADIUS 1.0

#define TORSO_HEIGHT 5.0
#define TORSO_RADIUS 1.0

#define UPPER_ARM_HEIGHT 3.0
#define LOWER_ARM_HEIGHT 2.0
#define UPPER_ARM_RADIUS  0.5
#define LOWER_ARM_RADIUS  0.5

#define UPPER_LEG_HEIGHT 3.0
#define LOWER_LEG_HEIGHT 2.0
#define UPPER_LEG_RADIUS  0.5
#define LOWER_LEG_RADIUS  0.5

const int AXIS_SIZE = 5000;

typedef struct {
  bool leftButton;
  bool rightButton;
  int x;
  int y;
} MouseState;

MouseState mouseState = { false, false, 0, 0 };

float xRotate = 0, yRotate = 0;

GLfloat eye[] = { 0, 0, 20 };
GLfloat at[] = { 0, 0, 0 };

GLUquadricObj *t, *h, *lua, *lla, *rua, *rla, *lul, *lll, *rul, *rll;

static GLfloat theta[QUIT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0 };
short int direccion[QUIT] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
static GLfloat center[3] = { 0, 0, 0 };
static GLint angle = 0;

bool dance = false;

double randRange(double min, double max) {
  return rand() * (max - min) / RAND_MAX + min;
}

void head() {
  glPushMatrix();
  glScalef(HEAD_RADIUS, HEAD_HEIGHT / 2, HEAD_RADIUS);
  gluSphere(h, 1.0, 10, 10);
  glPopMatrix();
}

void torso() {
  glPushMatrix();
  glRotatef(90.0, 1.0, 0.0, 0.0);
  gluCylinder(t, TORSO_RADIUS, TORSO_RADIUS, TORSO_HEIGHT, 10, 10);
  glPopMatrix();
}

void left_upper_arm() {
  glPushMatrix();
  glRotatef(90, 0, 1, 0);
  gluCylinder(lua, UPPER_ARM_RADIUS, UPPER_ARM_RADIUS, UPPER_ARM_HEIGHT, 10,
              10);
  glPopMatrix();
}

void left_lower_arm() {
  glPushMatrix();
  glRotatef(90, 0, 1, 0);
  gluCylinder(lla, LOWER_ARM_RADIUS, LOWER_ARM_RADIUS, LOWER_ARM_HEIGHT, 10,
              10);
  glPopMatrix();
}

void right_upper_arm() {
  glPushMatrix();
  glRotatef(-90, 0, 1, 0);
  gluCylinder(rua, UPPER_ARM_RADIUS, UPPER_ARM_RADIUS, UPPER_ARM_HEIGHT, 10,
              10);
  glPopMatrix();
}

void right_lower_arm() {
  glPushMatrix();
  glRotatef(-90, 0, 1, 0);
  gluCylinder(lla, LOWER_ARM_RADIUS, LOWER_ARM_RADIUS, LOWER_ARM_HEIGHT, 10,
              10);
  glPopMatrix();
}

void left_upper_leg() {
  glPushMatrix();
  glRotatef(90, 1, 0, 0);
  gluCylinder(lul, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS, UPPER_LEG_HEIGHT, 10,
              10);
  glPopMatrix();
}

void left_lower_leg() {
  glPushMatrix();
  glRotatef(90, 1, 0, 0);
  gluCylinder(lll, LOWER_LEG_RADIUS, LOWER_LEG_RADIUS, LOWER_LEG_HEIGHT, 10,
              10);
  glPopMatrix();
}

void right_upper_leg() {
  glPushMatrix();
  glRotatef(90, 1, 0, 0);
  gluCylinder(rul, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS, UPPER_LEG_HEIGHT, 10,
              10);
  glPopMatrix();
}

void right_lower_leg() {
  glPushMatrix();
  glRotatef(90, 1, 0, 0);
  gluCylinder(rll, LOWER_LEG_RADIUS, LOWER_LEG_RADIUS, LOWER_LEG_HEIGHT, 10,
              10);
  glPopMatrix();
}

void DrawRobot(float x, float y, float z, float lua, float lla, float rua,
               float rla, float lul, float lll, float rul, float rll) {
  torso();
  glPushMatrix();
    glTranslatef(0, HEAD_HEIGHT / 2, 0);
    head();
  glPopMatrix();
  glPushMatrix();
    glTranslatef(TORSO_RADIUS, 0, 0);
    glRotatef(lua, 0, 0, 1);
    left_upper_arm();
    glTranslatef(UPPER_ARM_HEIGHT, 0, 0);
    glRotatef(lla, 0, 0, 1);
    left_lower_arm();
  glPopMatrix();
  glPushMatrix();
    glTranslatef(-TORSO_RADIUS, 0, 0);
    glRotatef(rua, 0, 0, 1);
    right_upper_arm();
    glTranslatef(-UPPER_ARM_HEIGHT, 0, 0);
    glRotatef(rla, 0, 0, 1);
    right_lower_arm();
  glPopMatrix();
  glPushMatrix();
  glTranslatef(TORSO_RADIUS, -TORSO_HEIGHT, 0);
    glRotatef(lul, 1, 0, 0);
    left_upper_leg();
    glTranslatef(0, -UPPER_LEG_HEIGHT, 0);
    glRotatef(lll, 1, 0, 0);
    left_lower_leg();
  glPopMatrix();
  glPushMatrix();
    glTranslatef(-TORSO_RADIUS, -TORSO_HEIGHT, 0);
    glRotatef(rul, 1, 0, 0);
    right_upper_leg();
    glTranslatef(0, -UPPER_LEG_HEIGHT, 0);
    glRotatef(rll, 1, 0, 0);
    right_lower_leg();
  glPopMatrix();
}


void InitQuadrics() {
  t = gluNewQuadric();
  gluQuadricDrawStyle(t, GLU_FILL);
  lua = gluNewQuadric();
  gluQuadricDrawStyle(lua, GLU_FILL);
  h = gluNewQuadric();
  gluQuadricDrawStyle(h, GLU_FILL);
  lla = gluNewQuadric();
  gluQuadricDrawStyle(lla, GLU_FILL);
  rua = gluNewQuadric();
  gluQuadricDrawStyle(rua, GLU_FILL);
  lul = gluNewQuadric();
  gluQuadricDrawStyle(lul, GLU_FILL);
  lll = gluNewQuadric();
  gluQuadricDrawStyle(lll, GLU_FILL);
  rul = gluNewQuadric();
  gluQuadricDrawStyle(rul, GLU_FILL);
  rll = gluNewQuadric();
  gluQuadricDrawStyle(rll, GLU_FILL);
}

void draw_axis() {
  glBegin(GL_LINES);

  glColor3f(1, 0, 0);
  glVertex3f(-AXIS_SIZE, 0, 0);
  glVertex3f(AXIS_SIZE, 0, 0);

  glColor3f(0, 1, 0);
  glVertex3f(0, -AXIS_SIZE, 0);
  glVertex3f(0, AXIS_SIZE, 0);

  glColor3f(0, 0, 1);
  glVertex3f(0, 0, -AXIS_SIZE);
  glVertex3f(0, 0, AXIS_SIZE);
  glEnd();
}


void Display(void) {

  glDrawBuffer(GL_BACK);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], 0, 1, 0);

  glRotatef(xRotate, 0, 1, 0);
  glRotatef(yRotate, 1, 0, 0);

  DrawRobot(center[0], center[1], center[2], theta[LUA], theta[LLA], theta[RUA],
            theta[RLA], theta[LUL], theta[LLL], theta[RUL], theta[RLL]);

  glFlush();
  glutSwapBuffers();
}

void genDirec() {
  for (int i = 0; i < QUIT; i++) {
    int s = randRange(-2, 2);
    while (s == 0)
      s = randRange(-2, 2);
    direccion[i] = s;
  }
}

void Idle(int d) {

  if (d) {
    theta[LUA] += 3 * direccion[LUA];
    theta[LLA] += 3 * direccion[LLA];
    theta[RUA] += 3 * direccion[RUA];
    theta[RLA] += 3 * direccion[RLA];
    theta[LUL] += 3 * direccion[LUL];
    theta[LLL] += 3 * direccion[LLL];
    theta[RUL] += 3 * direccion[RUL];
    theta[RLL] += 3 * direccion[RLL];
    if (theta[LUA] > 60 || theta[LUA] < -60)
      direccion[LUA] *= -1;
    if (theta[LLA] > 30 || theta[LLA] < -30)
      direccion[LLA] *= -1;
    if (theta[RUA] > 60 || theta[RUA] < -60)
      direccion[RUA] *= -1;
    if (theta[RLA] > 30 || theta[RLA] < -30)
      direccion[RLA] *= -1;
    if (theta[LUL] > 90 || theta[LUL] < -90)
      direccion[LUL] *= -1;
    if (theta[LLL] > 45 || theta[LLL] < -45)
      direccion[LLL] *= -1;
    if (theta[RUL] > 90 || theta[RUL] < -90)
      direccion[RUL] *= -1;
    if (theta[RLL] > 45 || theta[RLL] < -45)
      direccion[RLL] *= -1;
    glutPostRedisplay();
  }
  glutTimerFunc(10, Idle, dance);
}

void Keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 't':
      theta[LUA] += 5;
      break;
    case 'y':
      theta[LUA] -= 5;
      break;
    case 'u':
      theta[LLA] += 5;
      break;
    case 'i':
      theta[LLA] -= 5;
      break;
    case 'q':
      theta[RUA] += 5;
      break;
    case 'w':
      theta[RUA] -= 5;
      break;
    case 'e':
      theta[RLA] += 5;
      break;
    case 'r':
      theta[RLA] -= 5;
      break;
    case 'g':
      theta[LUL] += 5;
      break;
    case 'h':
      theta[LUL] -= 5;
      break;
    case 'j':
      theta[LLL] += 5;
      break;
    case 'k':
      theta[LLL] -= 5;
      break;
    case 'a':
      theta[RUL] += 5;
      break;
    case 's':
      theta[RUL] -= 5;
      break;
    case 'd':
      theta[RLL] += 5;
      break;
    case 'f':
      theta[RLL] -= 5;
      break;
    case 'm':
      dance = !dance;
      genDirec();
      break;
    case 'x':
      exit(0);
  }
  glutPostRedisplay();
}

void SpecialKey(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_LEFT:
      eye[0] = eye[0] + UNITS_PER_PIXEL;
      at[0] = at[0] + UNITS_PER_PIXEL;
      break;
    case GLUT_KEY_RIGHT:
      eye[0] = eye[0] - UNITS_PER_PIXEL;
      at[0] = at[0] - UNITS_PER_PIXEL;
      break;
    case GLUT_KEY_UP:
      eye[1] = eye[1] - UNITS_PER_PIXEL;
      at[1] = at[1] - UNITS_PER_PIXEL;
      break;
    case GLUT_KEY_DOWN:
      eye[1] = eye[1] + UNITS_PER_PIXEL;
      at[1] = at[1] + UNITS_PER_PIXEL;
      break;
    case GLUT_KEY_END:
      eye[0] = (1 + ZOOM_FACTOR) * eye[0] - at[0] * ZOOM_FACTOR;
      eye[1] = (1 + ZOOM_FACTOR) * eye[1] - at[1] * ZOOM_FACTOR;
      eye[2] = (1 + ZOOM_FACTOR) * eye[2] - at[2] * ZOOM_FACTOR;
      break;
    case GLUT_KEY_HOME:
      eye[0] = (1 - ZOOM_FACTOR) * eye[0] + at[0] * ZOOM_FACTOR;
      eye[1] = (1 - ZOOM_FACTOR) * eye[1] + at[1] * ZOOM_FACTOR;
      eye[2] = (1 - ZOOM_FACTOR) * eye[2] + at[2] * ZOOM_FACTOR;
      break;
  }
  glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN)
      mouseState.leftButton = true;
    else
      mouseState.leftButton = false;
  }
  if (button == GLUT_RIGHT_BUTTON) {
    if (state == GLUT_DOWN)
      mouseState.rightButton = true;
    else
      mouseState.rightButton = false;
  }

  mouseState.x = x;
  mouseState.y = y;
}

void MouseMove(int x, int y) {
  int xDelta = mouseState.x - x;
  int yDelta = mouseState.y - y;

  mouseState.x = x;
  mouseState.y = y;

  if (mouseState.leftButton) {
    xRotate -= xDelta * DEGREES_PER_PIXEL;
    yRotate -= yDelta * DEGREES_PER_PIXEL;
  }
  glutPostRedisplay();
}

void EnableLighting(void) {
  GLfloat mat_specular[] = { 1.0, 0.0, 0.0, 1.0 };
  GLfloat mat_diffuse[] = { 0.8, 0.0, 0.0, 1.0 };
  GLfloat mat_ambient[] = { 0.8, 0.0, 0.0, 1.0 };
  GLfloat mat_shininess = { 5.0 };

  GLfloat light_ambient[] = { 0.8, 0.0, 0.0, 1.0 };
  GLfloat light_diffuse[] = { 0.8, 0.0, 0.0, 1.0 };
  GLfloat light_specular[] = { 1.0, 0.0, 0.0, 1.0 };
  GLfloat light_position[] = { 100.0, 100.0, 100.0, 1.0 };

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

  glEnable(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

}
void myInit() {
  glClearColor(1.0, 1.0, 1.0, 1.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0f, 1.0f, 10.0f, -10.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], 0, 1, 0);

  glEnable(GL_DEPTH_TEST);
  EnableLighting();
  InitQuadrics();
}

int main(int argc, char **argv) {
  srand(time(NULL));
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(960, 640);
  glutCreateWindow("Robot lab");

  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);
  glutSpecialFunc(SpecialKey);
  glutMouseFunc(Mouse);
  glutMotionFunc(MouseMove);

  myInit();

  glutTimerFunc(10, Idle, dance);
  glutMainLoop();
}
