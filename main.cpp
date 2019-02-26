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

using namespace std;

const int WinWid = 1600;
const int WinHei = 1600;

ld wid = 1;
ld hei = 1;

const int refDepth = 4;

ray camera = ray(point(0, 2, -23), vect(0, 0, 1));
Color color;
vector<obj_sphere> object_spheres;
vector<obj_plane> object_planes;
vector<obj_cube> object_cubes;
vector<light> lights;

Color answer[WinWid + 2][WinHei + 2];

void Draw1() {
	for (int x = -WinWid / 2; x <= WinWid / 2; x += 1) {
		for (int y = -WinHei / 2; y <= WinHei / 2; y += 1) {
			ld xx = (ld)(x + 1) / WinWid * wid, yy = (ld)(y + 1) / WinHei * hei;
			vect curx = (camera.direction * (camera.direction + point(0, 1, 0)));
			vect cury = (curx * camera.direction);
			curx = curx / curx.len();
			cury = cury / cury.len();
			vect cur = curx * xx + cury * yy;
			ray go = ray(camera.center, camera.direction + cur);


			answer[x + WinWid / 2][y + WinHei / 2] = TraceRay(go, 1e-3, inf / 2, refDepth);
		}
	}
}

void Draw() {
	ld tim1 = clock();
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(1.0);
	
	count_Colors();
	#pragma omp parallel private(x, y)
    #pragma omp for collapse(8) schedule(dynamic, CHUNK)
	{
		for (int x = -WinWid / 2; x <= WinWid / 2; x++) {
			for (int y = -WinHei / 2; y <= WinHei / 2; y++) {
				Color color = Color(0, 0, 0);
				for (int dx = -1; dx <= 1; dx++) {
					for (int dy = -1; dy <= 1; dy++) {
						if (x + WinWid / 2 + dx >= 0 && x + WinWid / 2 + dx <= WinWid && y + WinHei / 2 + dy >= 0 && y + WinHei / 2 + dy <= WinHei) {
							color = color + answer[x + WinWid / 2 + dx][y + WinHei / 2 + dy];
						}
					}
				}
				color = color / 9;
				glColor3f(color.R, color.G, color.B);
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

	object_spheres.pb(obj_sphere(point(0, -1, 0), 1, Color(1, 0, 0), 500, 0.2));
	object_spheres.pb(obj_sphere(point(2, 0, 1), 1, Color(0, 0, 1), 500, 0.3));
	object_spheres.pb(obj_sphere(point(-2, 0, 1), 1, Color(0, 1, 0), 10, 0.4));
	object_spheres.pb(obj_sphere(point(0, -5001, 0), 5000, Color(1, 1, 0), 1000, 0.5));
	object_spheres.pb(obj_sphere(point(0, 3, 2), 2, Color(0, 1, 1), -1, 0.9));
	
	object_planes.pb(obj_plane(point(0, 0, 11), point(0, 1, 11), point(1, 0, 11), Color(1, 1, 1), -1, 0));
	//object_planes.pb(obj_plane(point(5, 0, 0), point(5, 1, 0), point(5, 1, 1), Color(1, 1, 1), 10, 0));
	//object_planes.pb(obj_plane(point(-5, 0, 0), point(-5, 1, 0), point(-5, 1, 1), Color(1, 1, 1), 10, 0));
	//object_planes.pb(obj_plane(point(0, 10, 0), point(0, 10, 1), point(1, 10, 0), Color(1, 0.5, 1), 10, 1));

	//object_cubes.pb(obj_cube(point(5, 0, 0), point(1, 1, 0), point(1, -1, 1), point(-0.5, 0.5, 1), Color(1, 0, 0), 500, 0.1));
	//object_cubes.pb(obj_cube(point(-3, 1, 0), point(0, 0, -1), point(0, -1, 0), point(-1, 0, 0), Color(1, 0, 0), 300, 0.1));
	//object_cubes.pb(obj_cube(point(25, 10, 25), point(0, 0, -50), point(0, -12, 0), point(-50, 0, 0), Color(0, 1, 1), 300, 0.2));

	//object_planes.pb(obj_plane(point(8, 8, 21 - 10), point(0, 8, 21 - 10), point(0, 8 - 15, 21), Color(1, 0, 0), 10, 0.3));

	lights.pb(light(Ambient, 0.2, point(0, 0, 0)));
	int cnt = 20;
	for (int i = 0; i < cnt; i++) {
		lights.pb(light(Point, 0.6 / cnt, point(8, 2, -10) + (point(-8, 2, -2) - point(8, 2, -2)) / cnt * i));
	}
	//lights.pb(light(Point, 0.3, point(8, 2, -10)));
	//lights.pb(light(Point, 0.3, point(-8, 2, -10)));
	lights.pb(light(Directional, 0.2, point(1, 4, 4)));


	glutDisplayFunc(Draw);
	Initialize();
	glutMainLoop();
	return 0;
}
