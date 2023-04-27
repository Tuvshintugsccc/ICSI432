
#include <GL/glut.h>
#include <iostream>
#include <cmath>

int sides = 3;
float r = 1.0f, g = 1.0f, b = 1.0f;

void drawShape() {
    float angle = 2 * M_PI / sides;
    glBegin(GL_POLYGON);
    for (int i = 0; i < sides; ++i) {
        glVertex2f(cos(i * angle), sin(i * angle));
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(r, g, b);
    drawShape();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) {
        gluOrtho2D(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w);
    } else {
        gluOrtho2D(-1.0 * (GLfloat)w / (GLfloat)h, 1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'r':
            r = 1.0f;
            g = 0.0f;
            b = 0.0f;
            break;
        case 'g':
            r = 0.0f;
            g = 1.0f;
            b = 0.0f;
            break;
        case 'b':
            r = 0.0f;
            g = 0.0f;
            b = 1.0f;
            break;
    }
    glutPostRedisplay();
}

void special(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            sides++;
            break;
        case GLUT_KEY_DOWN:
            if (sides > 3) {
                sides--;
            }
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("n olon talt, RGB ungu solino");
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMainLoop();
    return 0;
}
