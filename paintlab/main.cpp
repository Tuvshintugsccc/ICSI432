#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <stack>
using namespace std;
class line
{
 public:
 vector<pair<float, float>> points;
 int r, g, b;
 float width;
 line(int r, int g, int b, float width) : r(r), g(g), b(b), width(width){};
};
vector<line> lines;
float line_width = 1;
int window_width = 800;
int window_height = 800;
int workarea_width = 800;
int workarea_height = 800;
int dx = window_width - workarea_width, dy = window_height - workarea_height;
float r = 0;
float g = 0;
float b = 0;
static void resize(int width, int height)
{
 glViewport(0, 0, window_width, window_height);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluOrtho2D(0, window_width, 0, window_height);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
}
void draw_lines()
{
 for (int i = lines.size() - 1; i >= 0; i--)
 {
 glColor3d(lines[i].r, lines[i].g, lines[i].b);
 glLineWidth(lines[i].width);
 glBegin(GL_LINE_STRIP);
 for (auto j : lines[i].points)
 {
 glVertex2f(j.first, j.second);
 }
 glEnd();
 }
}
static void display(void)
{
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glViewport(0, 0, window_width, window_height);
 glShadeModel(GL_SMOOTH);
 draw_lines();
 glutSwapBuffers();
}

static void myMouse(int button, int state, int x, int y)
{
 if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
 {
 if (dx < x && y < workarea_height)
 lines.push_back(line(r, g, b, line_width));
 }
}
void motion(int x, int y)
{
 if (dx < x && y < workarea_height)
 lines.back().points.push_back({x, (window_height - y)});
}
static void idle(void)
{
 glutPostRedisplay();
}
int main(int argc, char *argv[])
{
 glutInit(&argc, argv);
 glutInitWindowSize(window_width, window_height);
 glutInitWindowPosition(0, 0);
 glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
 glutCreateWindow("Paint");
 glutReshapeFunc(resize);
 glutDisplayFunc(display);
 glutIdleFunc(idle);
 glutMouseFunc(myMouse);
 glutMotionFunc(motion);
 glClearColor(1, 1, 1, 1);
 glEnable(GL_CULL_FACE);
 glCullFace(GL_BACK);
 glEnable(GL_DEPTH_TEST);
 glDepthFunc(GL_LESS);
 glEnable(GL_LIGHT0);
 glEnable(GL_NORMALIZE);
 glEnable(GL_COLOR_MATERIAL);
 glEnable(GL_LIGHTING);
 glutMainLoop();
 return EXIT_SUCCESS;
}
