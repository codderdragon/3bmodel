
// projeodevi.cpp
#include <GL/glut.h>
#include <vector>
#include <iostream>

float angle = 0.0f;
float scale = 1.0f;
bool growing = true;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(angle, 1.0f, 1.0f, 0.0f);
    glScalef(scale, scale, scale);

    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0);
    glVertex3f(0, 1, 0);
    glColor3f(0, 1, 0);
    glVertex3f(-1, -1, 0);
    glColor3f(0, 0, 1);
    glVertex3f(1, -1, 0);
    glEnd();

    glutSwapBuffers();
}

void timer(int v) {
    angle += 1.0f;
    if (growing) {
        scale += 0.01f;
        if (scale >= 1.5f) growing = false;
    } else {
        scale -= 0.01f;
        if (scale <= 1.0f) growing = true;
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3B Model Goruntuleyici");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(16, timer, 0);
    glutMainLoop();
    return 0;
}
