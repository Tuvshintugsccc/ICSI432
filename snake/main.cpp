/*Include Files*/
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <GL/glut.h>

#define UP 1
#define Down 2
#define LEFT 3
#define RIGHT 4

GLint   lvl = 1;
GLint   points = 0;
GLint   size = 0;
GLbyte  gameOver = true;
GLbyte  EnableLight = true;


GLint   bodyPos[2][100] = { {} };
GLint   x = 5;
GLint   z = 10;
GLint   oldX[2] = {};
GLint   oldZ[2] = {};
GLbyte  direction = 0;

GLint   bx = 0;
GLint   bz = 0;

GLint   w = 1280;
GLint   h = 960;
GLint   Giw = 0;
GLint   Gih = 0;
GLint   Gfw = 150;
GLint   Gfh = 150;

static GLfloat view_rotx = 45.0F;
static GLfloat view_roty = 0.0F;
static GLfloat view_rotz = 0.0F;

static GLfloat headRotation = 90.0F;
static GLfloat zoom = -300.0F;

DWORD   g_dwLastFPS = 0;
int		g_nFPS = 0, g_nFrames = 0;

void initLight()
{
	GLfloat ambientColor[] = { 0.3f, 0.4f, 0.8f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	GLfloat lightColor0[] = { 0.95f, 0.5f, 0.5f, 1.0f };
	GLfloat lightPos0[] = { 4.0f, 0.0f, 8.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f };
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
}


void myInit(void)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3, 0.80, 0.9, 0.0f); 
	if (EnableLight) {
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0); 
		glEnable(GL_LIGHT1);
		glEnable(GL_NORMALIZE);
	}
}

//Configure window resize
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1, 800.0);
}

void Write(char *string) {
	while (*string)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *string++);
}
void ManipulateViewAngle() {
	glRotatef(view_rotx, 1.0, 0.0, 0.0);
	glRotatef(view_roty, 0.0, 1.0, 0.0);
	glRotatef(view_rotz, 0.0, 0.0, 1.0);
}

void Reset() {
	x = 5;
	z = 10;
	direction = 0;
	lvl = 1;
	points = 0;
	size = 0;
	gameOver = false;
	view_rotx = 45.0F;
	view_roty = 0.0F;
	view_rotz = 0.0F;
	headRotation = 90.0F;
	zoom = -300.0F;
}

void WelcomeScreen() {
	char tmp_str[40];

	glColor3f(0.0, 0.0, 0);
	glRasterPos2f(0, 20);
	Write("Snake Game - Final Project by Tuvshintugs");

	glColor3f(0.18, 0.28, 0.29);
	glRasterPos2f(0, 10);
	Write("Computer Graphics - ICSI432");

	glColor3f(0, 0, 1);
	glRasterPos2f(0, 0);
	Write("'N' deer darj ehluulne uu.");
}

void DrawSnake() {
	int  i;


	glPushMatrix();
	ManipulateViewAngle();

	glPushMatrix();
	glColor3f(0.2, 0.2, 0.2);//black place
	glTranslatef(75.0, -16.0, 75.0);
	glScalef(155, 5.0, 155);
	glutSolidCube(1);
	glPopMatrix();

	glColor3f(1.0, 0.86, 0);
	glTranslatef(x, -10.0, z);
	glScalef(0.5, 0.5, 0.5);
	glutSolidSphere(10, 20, 20);
	glRotatef(headRotation, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(0, 0.0, 6.0);
	glScalef(0.8, 1.0, 1.0);
	glutSolidCone(10, 10, 20, 20);
	glColor3f(0, 0, 0);
	glTranslatef(-4.0, 10.0, 0.0);
	glScalef(0.3, 0.3, 0.3);
	glutSolidSphere(10, 20, 20);
	glTranslatef(26.0, 0.0, 0.0);
	glutSolidSphere(10, 20, 20);
	glPopMatrix();


	for (i = 0; i<size; i++) {
		glPushMatrix();
		ManipulateViewAngle();
		glTranslatef(bodyPos[0][i], -10.0, bodyPos[1][i]);
		glColor3f(0.91, 0.06, 0.17);
		glScalef(0.5, 0.5, 0.5);
		glutSolidSphere(7, 20, 20);
		glPopMatrix();
	}
}

void DrawFood()
{
	glPushMatrix();
	ManipulateViewAngle();
	glTranslatef(bx, -10.0, bz);
	glColor3f(0.4, 0.7, 0.4);
	glScalef(0.5, 0.5, 0.5);
	glutSolidSphere(7, 20, 20);
	glPopMatrix();
}

void GameStatus() {

	char tmp_str[40];

	glColor3f(0.18, 0.28, 0.29);
	glRasterPos2f(5, 50);

	sprintf(tmp_str, "Level: %d Onoo: %d", lvl, points);
	Write(tmp_str);
	glRasterPos2f(5, 40);
	Write("Up, Down, Left, Right arrow-uudaar udirdana.");
}

int RandomNumber(int high, int low)
{
	return (rand() % (high-low))+low;
}

void newFood() {
	time_t seconds;
	time(&seconds);
	srand((unsigned int)seconds);
	bx = RandomNumber(Gfw-Giw, Giw+10);
	bz = RandomNumber(Gfh-Gih, Gih+10);
}

void getFPS() {
	char tmp_str[40];

	if (GetTickCount() - g_dwLastFPS >= 1000)		
	{
		g_dwLastFPS = GetTickCount();	
		g_nFPS = g_nFrames;					
		g_nFrames = 0;				
	}
	g_nFrames++;

	glRasterPos2f(75, 50);


	glRasterPos2f(50, 60);

}

bool collision() {
	int i;

	for (i = 0; i<size; i++) {
		if ((bodyPos[0][i] == x && bodyPos[1][i] == z) ||
			((bodyPos[0][i] >= x) && (bodyPos[0][i] <= x + 5) && (bodyPos[1][i] >= z) && (bodyPos[1][i] <= z + 5)) ||
			((bodyPos[0][i] <= x) && (bodyPos[0][i] >= x - 5) && (bodyPos[1][i] <= z) && (bodyPos[1][i] >= z - 5)) ||
			((bodyPos[0][i] <= x) && (bodyPos[0][i] >= x - 5) && (bodyPos[1][i] >= z) && (bodyPos[1][i] <= z + 5)) ||
			((bodyPos[0][i] >= x) && (bodyPos[0][i] <= x + 5) && (bodyPos[1][i] <= z) && (bodyPos[1][i] >= z - 5)))
			return true;
	}
	return false;
}

void Run(int value) {
	int i;

	oldX[1] = x;
	oldZ[1] = z;
	switch (direction) {
	case RIGHT:
		headRotation = 90;
		x += 6;
		if (x > Gfw-2) x = Giw-1;
		break;
	case LEFT:
		headRotation = -90;
		x -= 6;
		if (x < 0) x = Gfw-2;
		break;
	case UP:
		headRotation = 0;
		z += 6;
		if (z > Gfh-2) z = Gih-1;
		break;
	case Down:
		headRotation = 180;
		z -= 6;
		if (z < 2) z = Gfh-2;
		break;
	}

	if (collision()) gameOver = true;

	if ((x == bx && z == bz) ||
		((x >= bx) && (x <= bx + 4) && (z >= bz) && (z <= bz + 4)) ||
		((x <= bx) && (x >= bx - 4) && (z <= bz) && (z >= bz - 4)) ||
		((x <= bx) && (x >= bx - 4) && (z >= bz) && (z <= bz + 4)) ||
		((x >= bx) && (x <= bx + 4) && (z <= bz) && (z >= bz - 4))) {
		points++;
		if (points < 100) size++;
		if (points%5 == 0 && lvl < 15) lvl++;
		newFood();
	}

	for (i = 0; i<size; i++) {
		oldX[0] = oldX[1];
		oldZ[0] = oldZ[1];
		oldX[1] = bodyPos[0][i];
		oldZ[1] = bodyPos[1][i];
		bodyPos[0][i] = oldX[0];
		bodyPos[1][i] = oldZ[0];
	}

	glutTimerFunc(130-lvl*4, Run, 0);
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (EnableLight) initLight();

	glTranslatef(-60.0, 40.0, zoom);
	if (!gameOver) {
		GameStatus();
		DrawFood();
		DrawSnake();
	}
	else
		WelcomeScreen();

	getFPS();
	glutPostRedisplay();
	glutSwapBuffers();
}

void Special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_RIGHT:
		if (direction != LEFT)
			direction = RIGHT;
		break;
	case GLUT_KEY_LEFT:
		if (direction != RIGHT)
			direction = LEFT;
		break;
	case GLUT_KEY_UP:
		if (direction != UP)
			direction = Down;
		break;
	case GLUT_KEY_DOWN:
		if (direction != Down)
			direction = UP;
		break;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'x': view_rotx += 2;
		glutPostRedisplay();
		break;
	case 'X': view_rotx -= 2;
		glutPostRedisplay();
		break;
	case 'y': view_roty += 2;
		glutPostRedisplay();
		break;
	case 'Y': view_roty -= 2;
		glutPostRedisplay();
		break;
	case 'z': view_rotz += 2;
		glutPostRedisplay();
		break;
	case 'Z': view_rotz -= 2;
		glutPostRedisplay();
		break;
	case 'a': size++;
		glutPostRedisplay();
		break;
	case 'A': size--;
		glutPostRedisplay();
		break;
	case '+': zoom++;
		glutPostRedisplay();
		break;
	case '-': zoom--;
		glutPostRedisplay();
		break;
	case 'n': Reset();
		glutPostRedisplay();
		break;

		//ESC to Exit
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

int main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(80, 80);
	glutCreateWindow("Snake Game - Final Project");
	glutSpecialFunc(Special);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(Display);
	glutReshapeFunc(resize);

	newFood();
	Run(0);

	myInit();
	glutMainLoop();
	return 0;
}
