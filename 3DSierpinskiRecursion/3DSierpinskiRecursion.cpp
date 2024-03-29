#include "Angel.h"


GLfloat v[4][3] = { { 0.0, 0.0, 1.0 }, { 0.0, 0.942809, -0.33333 },
{ -0.816497, -0.471405, -0.333333 }, { 0.816497, -0.471405, -0.333333 } };

GLfloat colors[4][3] = { { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 },
{ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 0.0 } };

int n=3;

void triangle(GLfloat *a, GLfloat *b, GLfloat *c)
{
	glVertex3fv(a);
	glVertex3fv(b);
	glVertex3fv(c);
}
void tetra(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d)
{
	glColor3fv(colors[0]);
	triangle(a, b, c);
	glColor3fv(colors[1]);
	triangle(a, c, d);
	glColor3fv(colors[2]);
	triangle(a, d, b);
	glColor3fv(colors[3]);
	triangle(b, d, c);
}


void divide_triangle(GLfloat *a, GLfloat *b, GLfloat *c, int m)
{
	GLfloat v1[3], v2[3], v3[3];
	int j;
	if (m>0)
	{
		for (j = 0; j<3; j++) v1[j] = (a[j] + b[j]) / 2;
		for (j = 0; j<3; j++) v2[j] = (a[j] + c[j]) / 2;
		for (j = 0; j<3; j++) v3[j] = (b[j] + c[j]) / 2;
		divide_triangle(a, v1, v2, m - 1);
		divide_triangle(c, v2, v3, m - 1);
		divide_triangle(b, v3, v1, m - 1);
	}
	else(triangle(a, b, c));
}

void tetrahedron(int m)
{
	glColor3f(1.0, 0.0, 0.0);
	divide_triangle( v[0], v[1], v[2], m);
	glColor3f(0.0, 1.0, 0.0);
	divide_triangle(v[3], v[2], v[1], m);
	glColor3f(0.0, 0.0, 1.0);
	divide_triangle(v[0], v[3], v[1], m);
	glColor3f(0.0, 0.0, 0.0);
	divide_triangle(v[0], v[2], v[3], m);
}

void divide_tetra(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d, int m)
{
	GLfloat mid[6][3];
	int j;
	if (m>0)
	{
		/* compute six midpoints */
		for (j = 0; j<3; j++) mid[0][j] = (a[j] + b[j]) / 2;
		for (j = 0; j<3; j++) mid[1][j] = (a[j] + c[j]) / 2;
		for (j = 0; j<3; j++) mid[2][j] = (a[j] + d[j]) / 2;
		for (j = 0; j<3; j++) mid[3][j] = (b[j] + c[j]) / 2;
		for (j = 0; j<3; j++) mid[4][j] = (c[j] + d[j]) / 2;
		for (j = 0; j<3; j++) mid[5][j] = (b[j] + d[j]) / 2;

		/* create 4 tetrahedrons  by subdivision */
		divide_tetra(a, mid[0], mid[1], mid[2], m - 1);
		divide_tetra(mid[0], b, mid[3], mid[5], m - 1);
		divide_tetra(mid[1], mid[3], c, mid[4], m - 1);
		divide_tetra(mid[2], mid[4], d, mid[5], m - 1);

	}
	else 	tetra(a, b, c, d); /* draw tetrahedron at end of recursion */
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	divide_tetra(v[0], v[1], v[2], v[3], n);
	glEnd();
	glFlush();
}
void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w,
		2.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-2.0 * (GLfloat)w / (GLfloat)h,
		2.0 * (GLfloat)w / (GLfloat)h, -2.0, 2.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void   main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);


	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowSize(500, 500);
	glutCreateWindow("3D Gasket");
	glewInit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutMainLoop();
}
