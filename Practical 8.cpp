#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#define WINDOW_TITLE "OpenGL Window"

boolean sphereDisplay = true;
float zPos = 0.0, xPos = -1.2, yPos = 1.0, angle = 0.0;
float zPos2 = 0.0, xPos2 = -1.2, yPos2 = 1.0;
float zPos3 = 0.0, xPos3 = 1.2, yPos3 = 1.0;
float zPos4 = 0.0, xPos4 = 1.2, yPos4 = 1.0;
float xShadow = 0.0, yShadow = 1.0, zShadow = 0.0;
float onOff = 0;
float amb[] = { 1.0,0,1 };
float amb1[] = { 0.0,1.0,1 };
float pos[] = { 0.0,1.0,0.0 };
float posLight1[] = { 0.0, 1.0,0.0 };
LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;

		case VK_RIGHT:
			pos[0] -= 0.1;
			xPos3 += 0.1;
			xPos += 0.1;
			break;

		case VK_LEFT:
			pos[0] += 0.1;
			xPos -= 0.1;
			xPos3 -= 0.1;
			break;

		case VK_NUMPAD6:
			posLight1[0] -= 0.1;
			xPos2 += 0.1;
			xPos4 += 0.1;
			break;

		case VK_NUMPAD4:
			posLight1[0] += 0.1;
			xPos2 -= 0.1;
			xPos4 -= 0.1;
			break;

		case VK_SPACE:
			if (onOff == 0) {
				onOff = 1;
			}
			else if (onOff == 1) {
				glDisable(GL_LIGHTING);
				onOff = 0;
			}
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------
void light() {
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb1);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, amb);
	glLightfv(GL_LIGHT1, GL_POSITION, posLight1);
	glEnable(GL_LIGHT1);

}
void shadow() {
	GLfloat matrix[16] =
	{ yShadow, -xShadow, 0, 0,
		0, 0, 0, 0,
		0, -zShadow,yShadow, 0,
		0, -1, 0, yShadow };
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glRotatef(angle, 1, 0, 0);
	glTranslatef(xPos, 0.01, zPos);
	glMultMatrixf(matrix);
	glColor3f(0.0f, 0.0f, 0.0f);
	GLUquadricObj* sphere1 = NULL;
	sphere1 = gluNewQuadric();
	gluSphere(sphere1, 0.5, 50, 50);
	gluDeleteQuadric(sphere1);
	glPopMatrix();

	glPushMatrix();
	glRotatef(angle, 1, 0, 0);
	glTranslatef(xPos2, 0.01, zPos2);
	glMultMatrixf(matrix);
	glColor3f(0.0f, 0.0f, 0.0f);
	GLUquadricObj* sphere2 = NULL;
	sphere2 = gluNewQuadric();
	gluSphere(sphere2, 0.5, 50, 50);
	gluDeleteQuadric(sphere2);
	glPopMatrix();

	glPushMatrix();
	glRotatef(angle, 1, 0, 0);
	glTranslatef(xPos3, 0.01, zPos3);
	glMultMatrixf(matrix);
	glColor3f(0.0f, 0.0f, 0.0f);
	GLUquadricObj* sphere3 = NULL;
	sphere3 = gluNewQuadric();
	gluSphere(sphere3, 0.5, 50, 50);
	gluDeleteQuadric(sphere3);
	glPopMatrix();

	glPushMatrix();
	glRotatef(angle, 1, 0, 0);
	glTranslatef(xPos4, 0.01, zPos4);
	glMultMatrixf(matrix);
	glColor3f(0.0f, 0.0f, 0.0f);
	GLUquadricObj* sphere4 = NULL;
	sphere4 = gluNewQuadric();
	gluSphere(sphere4, 0.5, 50, 50);
	gluDeleteQuadric(sphere4);
	glPopMatrix();
}
void display()
{
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	light();
	glColor3f(.9, .9, .9);
	glPushMatrix();
	glRotatef(angle, 1, 0, 0);
	
		glPushMatrix();
		glTranslatef(-1.2, 0, 0);
		GLUquadricObj* sphere1 = NULL;
		sphere1 = gluNewQuadric();
		gluSphere(sphere1, 0.5, 50, 50);
		gluDeleteQuadric(sphere1);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(1.2, 0, 0);
		GLUquadricObj* sphere2 = NULL;
		sphere2 = gluNewQuadric();
		gluSphere(sphere2, 0.5, 50, 50);
		gluDeleteQuadric(sphere2);
		glPopMatrix();
	
	glBegin(GL_QUADS);
	glVertex3f(-3, -1.0, -3);
	glVertex3f(-3, -1.0, 3);
	glVertex3f(3, -1.0, 3);
	glVertex3f(3, -1.0, -3);
	glEnd();
	glPopMatrix();
	if (onOff == 1)
	{
		shadow();
		glEnable(GL_LIGHTING);
	}
}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 800,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glTranslatef(0, 0, -1);
	gluPerspective(45.0, 1.0, 0.1, 5.0);
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 50.0);
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------
