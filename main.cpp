#include <windows.h>
#include <GL/glut.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

struct Vertex {
    float x, y, z;
};

struct Face {
    int v1, v2, v3;
};

std::vector<Vertex> vertices;
std::vector<Face> faces;
std::vector<int> index_map;

float cameraX = 0.0f, cameraY = 0.0f, cameraZ = -5.0f;
float cameraAngleX = 0.0f, cameraAngleY = 0.0f;

float rotationX = 0.0f, rotationY = 0.0f;
float scale = 1.0f;


int lastX = -1, lastY = -1;
bool isMouseDown = false;
bool isAutoRotating = true;

void optimizeMesh() {
    for (size_t i = 0; i < vertices.size(); ++i) {
        for (size_t j = i + 1; j < vertices.size(); ++j) {
            if (fabs(vertices[i].x - vertices[j].x) < 0.001f &&
                fabs(vertices[i].y - vertices[j].y) < 0.001f &&
                fabs(vertices[i].z - vertices[j].z) < 0.001f) {
                index_map.push_back(j);
            }
        }
    }
}

void loadModel(const char* filename) {
    std::ifstream file(filename);
    std::string line;
    while (getline(file, line)) {
        std::istringstream sstream(line);
        std::string prefix;
        sstream >> prefix;

        if (prefix == "v") {
            // Vertex
            Vertex vertex;
            sstream >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        } else if (prefix == "f") {
            // Face
            Face face;
            sstream >> face.v1 >> face.v2 >> face.v3;
            faces.push_back(face);
        }
    }
    optimizeMesh();  
}

void setupCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 0.1f, 100.0f);
    glTranslatef(cameraX, cameraY, cameraZ);
}

void setupLighting() {
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    setupCamera();
    setupLighting();

    glPushMatrix();
    if (isAutoRotating) {
        rotationX += 0.2f;
        rotationY += 0.2f;
    }

    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glScalef(scale, scale, scale); 

    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < faces.size(); ++i) {
        const Face& face = faces[i];
        const Vertex& v1 = vertices[face.v1 - 1];
        const Vertex& v2 = vertices[face.v2 - 1];
        const Vertex& v3 = vertices[face.v3 - 1];

        glColor3f(1.0f, 0.0f, 0.0f);

        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);
    }
    glEnd();

    glPopMatrix();
    glutSwapBuffers();
}


void update(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);  // 16ms'de bir çaðýrmaya yarýyor hoca isterse deðiþtiririm.
}


void mouseMotion(int x, int y) {
    if (isMouseDown) {
        rotationX += (y - lastY) * 0.2f;
        rotationY += (x - lastX) * 0.2f;
    }

    lastX = x;
    lastY = y;

    glutPostRedisplay();
}


void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isMouseDown = true;
            isAutoRotating = false;  // Fare ile týklanýnca otomatik dönüþ durdurmaya yarýyor hoca isterse deðiþtiririm.
            lastX = x;
            lastY = y;
        } else {
            isMouseDown = false;
            isAutoRotating = true;  // Fareyi býraktýðýnda otomatik dönüþ tekrarýna yarýyor hoca isterse deðiþtiririm.
        }
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'w') cameraZ += 0.1f;
    if (key == 's') cameraZ -= 0.1f;
    if (key == 'a') cameraX += 0.1f;
    if (key == 'd') cameraX -= 0.1f;
    if (key == 'r') scale += 0.1f;  // Ölçeklemeyi artýr
    if (key == 'f') scale -= 0.1f;  // Ölçeklemeyi azalt
}


void init() {
    glEnable(GL_DEPTH_TEST);
    loadModel("model.obj");
    setupLighting();
}

// Ana fonksiyon
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Dragon OBJ Viewer ");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMotion);
    init();
    glutTimerFunc(25, update, 0);
    glutMainLoop();
    return 0;
}

