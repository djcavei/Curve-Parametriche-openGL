#include <iostream>
#include <gl/glut.h>
#include <cmath>
#include <vector>
#include <string>

using std::vector;
using std::pair;
using std::string;

void draw_line(float x, float y, float x1, float y1);
float pi = 3.14/2;
float t = 0;
float precision = 0.025;
float lx = -0.2f, lz = -1.0f, ly = 0.0f;
// XZ position of the camera
float x = 0.f, z = 6.0f, y = 0.0f;
float v = 0.5;

float shock_pink[3] = {255.0f/255.0f, 32.0f/255.0f, 143.0f/255.0f};


//va a caso la velocità...
int when = 5;
int default_when = 5;
int w = 1;
float color_ratio = 0.01f;


class Curve {
private:
	vector<pair<float,float>> coor{};
	string func; //TODO fare un array di operazioni senza parsare ogni volta
	float colour[3];
public:
	Curve() { coor; func = ""; colour[0] = 1.0, colour[1] = 0.0, colour[2] = 0.0; };
	pair<float, float> function(string s) {
		pair<float, float> res{};
		//parsing della funzione
		res.first = t * cos(t);
		res.second = t * sin(t);
		t += precision;
		return res;
	};
	void add_points() {
		pair<float, float> temp{};
		temp = function(func);
		coor.push_back(temp);
	};
	void change_colour() {
		if (colour[w] < 1.00f)
			colour[w] += color_ratio;
		else {
			colour[(w + 2) % 3] -= color_ratio;
			if (colour[(w + 2) % 3] < 0.00f)
				w = (w + 1) % 3;
		}
	}
	void reset_colour() {
		w = 1;
		colour[0] = 1.0; colour[1] = colour[2] = 0.0;
	}
	void draw_curve() {
		reset_colour();
		pi = 3.14 / 2;
		for (int i = 0; i < (int)coor.size() - 1; ++i) {
			change_colour();
			glColor3f(colour[0], colour[1], colour[2]);
			draw_line(coor[i].first, coor[i].second, coor[i + 1].first, coor[i + 1].second);
		}
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

//void draw_line(float x, float y, float x1, float y1) {
//	glBegin(GL_LINES);
//	glVertex2d(x, y);
//	glVertex2d(x1, y1);
//	glEnd();
//}

void draw_line(float x, float y, float x1, float y1) {
	glBegin(GL_QUADS);
	glVertex3f(x + cos(pi), y, sin(pi));
	glVertex3f(x - cos(pi), y, -sin(pi));
	pi += 0.1;
	glVertex3f(x1 - cos(pi), y1, -sin(pi));
	glVertex3f(x1 + cos(pi) ,y1, sin(pi));
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
	//serve a disegnare gli assi
	/*glColor3f(0.9f, 0.009f, 0.009f);
	draw_line(0, 0, 30, 0);
	glColor3f(0.0f, 1.0f, 0.0f);
	draw_line(0, 0, 0, 30);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 50);
	glEnd();*/
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
	glutInitWindowPosition(0, 0);
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
