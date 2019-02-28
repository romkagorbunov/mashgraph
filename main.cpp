#include "geom.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <ctime>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <glut.h>
#else
#include <GL/glut.h>
#endif
#define mp make_pair
#define pb push_back

using namespace std;

const int WinWid = 1600;
const int WinHei = 1600;

ld wid = 1;
ld hei = 1;

const int refDepth = 4;
vector<TPoint> tasks;

TRay camera = TRay(TPoint(0, 5, 20), TVector(0, 0, -1));
TColor color;
vector<TObject*> objects;
vector<TLight> lights;

TColor answer[WinWid + 2][WinHei + 2];

void render() {
	for (int x = -WinWid / 2; x <= WinWid / 2; x += 1) {
		for (int y = -WinHei / 2; y <= WinHei / 2; y += 1) {
			tasks.pb(TPoint(x, y));
		}
	}

	for (int x = -WinWid / 2; x <= WinWid / 2; x += 1) {
		for (int y = -WinHei / 2; y <= WinHei / 2; y += 1) {
			ld xx = (ld) (x + 1) / WinWid * wid, yy = (ld) (y + 1) / WinHei * hei;
			TVector curx = (camera.direction * (camera.direction + TPoint(0, 1, 0)));
			TVector cury = (curx * camera.direction);
			curx = curx / curx.len();
			cury = cury / cury.len();
			TVector cur = curx * xx + cury * yy;
			TRay go = TRay(camera.position, camera.direction + cur);
			answer[x + WinWid / 2][y + WinHei / 2] = TraceRay(go, 1e-3, inf / 2, refDepth);
		}
	}
	for (int x = -WinWid / 2; x <= WinWid / 2; x += 1) {
		for (int y = -WinHei / 2; y <= WinHei / 2; y += 1) {
			TColor color = TColor(0, 0, 0);
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					if (x + WinWid / 2 + dx >= 0 && x + WinWid / 2 + dx <= WinWid && y + WinHei / 2 + dy >= 0 && y + WinHei / 2 + dy <= WinHei) {
						color = color + answer[x + WinWid / 2 + dx][y + WinHei / 2 + dy];
					}
				}
			}
			answer[x + WinWid / 2][y + WinHei / 2] = color / 9;
		}
	}
}

void Draw() {
	ld tim1 = clock();
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(1.0);
	
	render();
	{
		for (int x = -WinWid / 2; x <= WinWid / 2; x++) {
			for (int y = -WinHei / 2; y <= WinHei / 2; y++) {
				TColor color = answer[x + WinWid / 2][y + WinHei / 2];
				glColor3f(color.r, color.g, color.b);
				glBegin(GL_POINTS);
				glVertex2i(x, y);
				glEnd();

			}
		}
	}
	glutSwapBuffers();
	cerr << "Time: " << (clock() - tim1) / CLOCKS_PER_SEC << endl;
}

void Initialize() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-WinWid / 2, WinWid / 2, -WinHei / 2, WinHei / 2, -200, 200);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WinWid, WinHei);
	glutInitWindowPosition(100, 200);
	glutCreateWindow("RayTracing");

	TSphere* sphere;

	sphere = new TSphere(TPoint(0, 0, 0), 1, TObject(TColor(1, 0, 0), 10, 0.2));
	objects.pb(sphere);
	sphere = new TSphere(TPoint(2, 2, 0), 1, TObject(TColor(0, 1, 0), 10, 0.2));
	objects.pb(sphere);
	sphere = new TSphere(TPoint(0, -5000, 0), 4999, TObject(TColor(1, 0, 1), 20, 0.1));
	objects.pb(sphere);

	lights.pb(TLight(Ambient, 0.2, TPoint()));
	lights.pb(TLight(Point, 0.8, TPoint(50, 50, 10)));
	
	glutDisplayFunc(Draw);
	Initialize();
	glutMainLoop();
	for (auto u : objects) {
		delete u;
	}
	return 0;
}
