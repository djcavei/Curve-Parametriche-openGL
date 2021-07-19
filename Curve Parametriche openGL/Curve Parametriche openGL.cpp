#include <iostream>
#include <gl/glut.h>
#include <cmath>
#include <vector>

using std::vector;
using std::pair;

void draw_line(float x, float y, float x1, float y1);

float t = 0;
float precision = 0.025;
float lx = 0.0f, lz = -1.0f, ly = 0.0f;
// XZ position of the camera
float x = 2.0f, z = 4.0f, y = 1.0f;
float v = 0.5;

float shock_pink[3] = {255.0f/255.0f, 32.0f/255.0f, 143.0f/255.0f};

int when = 10;
int default_when = 10;


class Curve {
private:
	vector<pair<float,float>> coor{};
public:
	Curve() { coor; };
	pair<float, float> function() {
		pair<float, float> res{};
		//parsing della funzione
		res.first = t; //qui metterai la funzione parametrica
		res.second = pow(t,2);
		t += precision;
		return res;
	};
	void add_points() {
		pair<float, float> temp{};
		temp = function();
		coor.push_back(temp);
	};
	void draw_curve() {
		for (int i = 0; i < (int)coor.size() - 1 ; ++i)
			draw_line(coor[i].first, coor[i].second, coor[i + 1].first, coor[i + 1].second);
	};
};

Curve curve{};

void processNormalKeys(unsigned char key, int xx, int yy) {
	switch (key) {
		case 27: exit(0); break;
		case 'w': y += v; break;
		case 'a': x -= v; break;
		case 's': y -= v; break;
		case 'd': x += v; break;
		case 'i': z += v; break;
		case 'k': z -= v; break;
	}
}

void draw_line(float x, float y, float x1, float y1) {
	glBegin(GL_LINES);
	glVertex2d(x, y);
	glVertex2d(x1, y1);
	glEnd();
}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(x, y, z,
		x + lx, y + ly, z + lz,
		0.0f, 1.0f, 0.0f);

	glColor3f(0.9f, 0.009f, 0.009f);
	draw_line(0, 0, 30, 0);
	glColor3f(0.0f, 1.0f, 0.0f);
	draw_line(0, 0, 0, 30);
	glColor3f(shock_pink[0], shock_pink[1], shock_pink[2]);
	curve.draw_curve();
	//aggiunta punto ogni 40 cicli
	if (--when == 0) {
		curve.add_points();
		when = default_when;
	}
	glutSwapBuffers();
}

void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio = 1.0 * w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45, ratio, 1, 1000);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(2560, 1440);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Curve Parametriche");

	// register callbacks
	glutDisplayFunc(renderScene);
	// Here is our new entry in the main function
	glutReshapeFunc(changeSize);
	// here is the idle func registration
	glutIdleFunc(renderScene);

	glutKeyboardFunc(processNormalKeys);
	//glutSpecialFunc(processSpecialKeys);

	////glutIgnoreKeyRepeat(1);
	//glutSpecialUpFunc(releaseKey);

	//glutMouseFunc(mouseButton);
	//glutMotionFunc(mouseMove);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}
