#pragma comment(lib, "glaux.lib")
#include <tchar.h>
#include <GL/glut.h>
#include <GL/glaux.h>

#define	N 20000

float timespan = 0.005;
float wind[3];
struct
{
	float life, fade, r, g, b, x, y, z, xv, yv, zv, xa, ya, za;
}
p[N + 2];

void initial()
{
	AUX_RGBImageRec *textureImage;
	textureImage = auxDIBImageLoad(_T("particle.bmp"));
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, textureImage->sizeX, textureImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, textureImage->data);
	glShadeModel(GL_SMOOTH);
	glClearColor(0, 0, 0, 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_TEXTURE_2D);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 0.1, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 0.8, 0, 0, 0, 0, 1, 0);
	glTranslated(0.0, -4, 0.0);
	for (int i = 1; i <= N; i++)
	{
		float x = p[i].x;
		float y = p[i].y;
		float z = p[i].z - 8;

		glColor4f(p[i].r, p[i].g, p[i].b, p[i].life);
		glBegin(GL_TRIANGLE_FAN);
		glTexCoord2d(1, 1); glVertex3f(x + 0.3f, y + 0.5f, z);
		glTexCoord2d(0, 1); glVertex3f(x - 0.3f, y + 0.5f, z);
		glTexCoord2d(0, 0); glVertex3f(x - 0.3f, y - 0.5f, z);
		glTexCoord2d(1, 0); glVertex3f(x + 0.3f, y - 0.5f, z);
		glEnd();
	}
	glFlush();
}

void step()
{
	for (int i = 1; i <= N; i++)
	{
		p[i].x += p[i].xv * timespan;
		p[i].y += p[i].yv * timespan;
		p[i].z += p[i].zv * timespan;

		p[i].xa = wind[0] + (rand() % 5 - 2) / 10.0;
		p[i].xv += p[i].xa;
		p[i].yv += p[i].ya;
		p[i].zv += p[i].za;
		p[i].life -= p[i].fade;
		if (p[i].life <= 0)
		{
			p[i].life = 1;
			p[i].fade = float(rand() % 100) / 1000 + 0.005;
			p[i].x = 0;
			p[i].y = 0;
			p[i].z = 0;
			p[i].xv = float((rand() % 6) - 3);
			p[i].yv = float((rand() % 6) - 1);
			p[i].zv = float((rand() % 6) - 3);
			p[i].ya = wind[1] + 0.08;
			p[i].za = wind[2];
			p[i].r = (rand() % 5 / 100 - 0.02) + 1;
			p[i].g = (rand() % 5 / 100 - 0.02) + 0.15;
			p[i].b = (rand() % 5 / 100 - 0.02);
			p[i].r = 1;
			p[i].g = 0.15;
			p[i].b = 0;
		}
	}
	glutPostRedisplay();
}

void keyboard(unsigned char ch, int x, int y)
{
	if (ch == 'a') wind[0] -= 0.002;
	if (ch == 'd') wind[0] += 0.002;
	if (ch == 'w') wind[1] += 0.002;
	if (ch == 's') wind[1] -= 0.002;
	if (ch == 'z') wind[1] += 0.002;
	if (ch == 'c') wind[1] -= 0.002;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(600, 600);
	glutCreateWindow("基于粒子系统的火焰模拟");
	initial();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(step);
	glutMainLoop();
	return 0;
}
