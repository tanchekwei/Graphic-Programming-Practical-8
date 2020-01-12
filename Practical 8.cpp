#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
#include <conio.h>
#include <string>
#include <math.h>

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")

#define WINDOW_TITLE "Bridge"
#define PI 3.141592654f

// Mode
int currentMode = 1;
bool toggleLighting = true;

// Screen
const float screenWidth = 800, screenHeight = 600;

// Camera
float orthoLeft = -5.0f, orthoRight = 5.0f, orthoBottom = -5.0f, orthoTop = 5.0f, orthoZNear = 0, orthoZFar = 10.0f;
float prespFovy = 60;
float prespLeft = -1.0f, prespRight = 1.0f, prespBottom = -1.0f, prespTop = 1.0f, prespZNear = 0, prespZFar = 10.0f;
float lastCursorPosX, lastCursorPosY;
float cameraPosX = 0, cameraPosY = 0, cameraPosZ = 0, cameraRotX = 0, cameraRotY = 0;
POINT cursorPos;

// Lighting
float lightPos[] = {0, 1, 0};
float lightPos2[] = {1, 1.2f, 0};
float lightMovementSpd = 0.1f;
float lightDif[] = {1, 1, 1};

// Model
int currentModel = 1;
float modelColor[] = {0.52f, 0.52f, 0.52f};
float rotDeg = 0.0, rotSpd = 1;

float shadow[] = {0, 1, 0};

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_CHAR:
        switch (currentMode)
        {
        case 0: // Ortho Camera
            switch (wParam)
            {
            case 0x41: // A L-
                orthoLeft--;
                break;
            case 0x61: // a L+
                orthoLeft++;
                break;

            case 0x44: // D R+
                orthoRight++;
                break;
            case 0x64: // d R-
                orthoRight--;
                break;

            case 0x57: // W T+
                orthoTop++;
                break;
            case 0x77: // w T-
                orthoTop--;
                break;

            case 0x53: // S B-
                orthoBottom--;
                break;
            case 0x73: // s B+
                orthoBottom++;
                break;

            case 0x51: // Q N+
                orthoZNear++;
                break;
            case 0x71: // q N-
                if (orthoZNear > 0)
                {
                    orthoZNear--;
                }
                break;

            case 0x45: // E F+
                orthoZFar++;
                break;
            case 0x65: // e F-
                if (orthoZFar > 0)
                {
                    orthoZFar--;
                }
                break;
            }
            break;
        case 1: // Presp Camera
            switch (wParam)
            {
            case 0x5A: // FOVY-
                prespFovy--;
                break;

            case 0x43: // FOVY+
                prespFovy++;
                break;
            }
            break;
        }

        break;

    case WM_KEYDOWN:
        float xrotrad, yrotrad;

        // Switch current mode
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        case 0x31: // Camera Mode
            currentMode = 0;
            break;
        case 0x32:
            currentMode = 1;
            break;
        case 'O':
            currentModel = 0;
            break;
        case 'P':
            currentModel = 1;
            break;

            // Light Movement
        case VK_NUMPAD8:
            lightPos[1] += lightMovementSpd;
            break;
        case VK_NUMPAD2:
            lightPos[1] -= lightMovementSpd;
            break;
        case VK_NUMPAD4:
            lightPos[0] -= lightMovementSpd;
            break;
        case VK_NUMPAD6:
            lightPos[0] += lightMovementSpd;
            break;
        case VK_NUMPAD7:
            lightPos[2] += lightMovementSpd;
            break;
        case VK_NUMPAD9:
            lightPos[2] -= lightMovementSpd;
            break;
        case VK_DOWN:
            rotDeg -= rotSpd;
            break;
        case VK_UP:
            rotDeg += rotSpd;
            break;
        case VK_LEFT:
            lightPos2[0] -= lightMovementSpd;
            break;
        case VK_RIGHT:
            lightPos2[0] += lightMovementSpd;
            break;

            // Eye movement
        case 0x57: // Ww
            yrotrad = (cameraRotY / 180 * PI);
            xrotrad = (cameraRotX / 180 * PI);
            cameraPosX += float(sin(yrotrad)) * 0.05;
            cameraPosZ -= float(cos(yrotrad)) * 0.05;
            cameraPosY -= float(sin(xrotrad)) * 0.05;
            break;
        case 0x53: // Ss
            yrotrad = (cameraRotY / 180 * PI);
            xrotrad = (cameraRotX / 180 * PI);
            cameraPosX -= float(sin(yrotrad)) * 0.05;
            cameraPosZ += float(cos(yrotrad)) * 0.05;
            cameraPosY += float(sin(xrotrad)) * 0.05;
            break;
        case 0x41: // Aa
            yrotrad = (cameraRotY / 180 * PI);
            cameraPosX -= float(cos(yrotrad)) * 0.05;
            cameraPosZ -= float(sin(yrotrad)) * 0.05;
            break;
        case 0x44: // Dd
            yrotrad = (cameraRotY / 180 * PI);
            cameraPosX += float(cos(yrotrad)) * 0.05;
            cameraPosZ += float(sin(yrotrad)) * 0.05;
            break;
        case VK_SPACE:
            if (toggleLighting)
            {
                toggleLighting = false;
            }
            else
            {
                toggleLighting = true;
            }
            break;
        case VK_SHIFT:
            cameraPosY += lightMovementSpd;
            break;
        case VK_CONTROL:
            cameraPosY -= lightMovementSpd;
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
/*
Basic Shape
*/

void drawSphere()
{
    glColor3ub(199, 120, 28);
    GLUquadricObj *var = gluNewQuadric();
    gluQuadricDrawStyle(var, GLU_FILL);
    gluSphere(var, 0.5f, 64, 64);
    gluDeleteQuadric(var);
}

void drawCylinder()
{
    GLUquadricObj *var = gluNewQuadric();
    gluQuadricDrawStyle(var, GLU_LINE);
    gluCylinder(var, 0, 0.5f, 0.5f, 16, 16);
    gluDeleteQuadric(var);
}

void drawPyramid()
{
    float points[5][3] = {
        {0.5f, 0, 0.5f},   // A [0][]
        {-0.5f, 0, 0.5f},  // B [1][]
        {-0.5f, 0, -0.5f}, // C [2][]
        {0.5f, 0, -0.5f},  // D [3][]
        {0, 0.5f, 0},      // E [4][]
    };

    glBegin(GL_QUADS);
    {
        // Bottom (ABCD)
        glVertex3f(points[0][0], points[0][1], points[0][2]);
        glVertex3f(points[1][0], points[1][1], points[1][2]);
        glVertex3f(points[2][0], points[2][1], points[2][2]);
        glVertex3f(points[3][0], points[3][1], points[3][2]);
    }
    glEnd();

    glBegin(GL_TRIANGLES);
    {
        // Left (BCE)
        glVertex3f(points[1][0], points[1][1], points[1][2]);
        glVertex3f(points[2][0], points[2][1], points[2][2]);
        glVertex3f(points[4][0], points[4][1], points[4][2]);
    }
    glEnd();

    glBegin(GL_TRIANGLES);
    {
        // Back (CDE)
        glVertex3f(points[2][0], points[2][1], points[2][2]);
        glVertex3f(points[3][0], points[3][1], points[3][2]);
        glVertex3f(points[4][0], points[4][1], points[4][2]);
    }
    glEnd();

    glBegin(GL_TRIANGLES);
    {
        // Right (DAE)
        glVertex3f(points[3][0], points[3][1], points[3][2]);
        glVertex3f(points[0][0], points[0][1], points[0][2]);
        glVertex3f(points[4][0], points[4][1], points[4][2]);
    }
    glEnd();

    glBegin(GL_TRIANGLES);
    {
        // Front (ABE)
        glVertex3f(points[0][0], points[0][1], points[0][2]);
        glVertex3f(points[1][0], points[1][1], points[1][2]);
        glVertex3f(points[4][0], points[4][1], points[4][2]);
    }
    glEnd();
}

void drawCube()
{
    float points[8][3] = {
        {0.5f, 0.5f, 0.5f},    // A [0][]
        {0.5f, -0.5f, 0.5f},   // B [1][]
        {-0.5f, -0.5f, 0.5f},  // C [2][]
        {-0.5f, 0.5f, 0.5f},   // D [3][]
        {0.5f, 0.5f, -0.5f},   // E [4][]
        {0.5f, -0.5f, -0.5f},  // F [5][]
        {-0.5f, -0.5f, -0.5f}, // G [6][]
        {-0.5f, 0.5f, -0.5f},  // H [7][]
    };

    glBegin(GL_QUADS);
    {
        // Front (ABCD)
        glVertex3f(points[0][0], points[0][1], points[0][2]);
        glVertex3f(points[1][0], points[1][1], points[1][2]);
        glVertex3f(points[2][0], points[2][1], points[2][2]);
        glVertex3f(points[3][0], points[3][1], points[3][2]);

        // Back (EFGH)
        glVertex3f(points[4][0], points[4][1], points[4][2]);
        glVertex3f(points[5][0], points[5][1], points[5][2]);
        glVertex3f(points[6][0], points[6][1], points[6][2]);
        glVertex3f(points[7][0], points[7][1], points[7][2]);

        // Left (CDHg)
        glVertex3f(points[2][0], points[2][1], points[2][2]);
        glVertex3f(points[3][0], points[3][1], points[3][2]);
        glVertex3f(points[7][0], points[7][1], points[7][2]);
        glVertex3f(points[6][0], points[6][1], points[6][2]);

        // Right (ABFE)
        glVertex3f(points[0][0], points[0][1], points[0][2]);
        glVertex3f(points[1][0], points[1][1], points[1][2]);
        glVertex3f(points[5][0], points[5][1], points[5][2]);
        glVertex3f(points[4][0], points[4][1], points[4][2]);

        // Top (ADHE)
        glVertex3f(points[0][0], points[0][1], points[0][2]);
        glVertex3f(points[3][0], points[3][1], points[3][2]);
        glVertex3f(points[7][0], points[7][1], points[7][2]);
        glVertex3f(points[4][0], points[4][1], points[4][2]);

        // Bottom (BCGF)
        glVertex3f(points[1][0], points[1][1], points[1][2]);
        glVertex3f(points[2][0], points[2][1], points[2][2]);
        glVertex3f(points[6][0], points[6][1], points[6][2]);
        glVertex3f(points[5][0], points[5][1], points[5][2]);
    }
    glEnd();
}

//--------------------------------------------------------------------
/*
Construction
*/

//--------------------------------------------------------------------
/*
Settings
*/

void setLighting()
{
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, modelColor);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT1, GL_AMBIENT, modelColor);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDif);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);
    glEnable(GL_LIGHT1);
}

void setShadow()
{
    GLfloat matrix[16] =
        {
            shadow[1], -shadow[0], 0, 0,
            0, 0, 0, 0,
            0, -shadow[2], shadow[1], 0,
            0, -1, 0, shadow[1]};

    glDisable(GL_LIGHTING);

    glPushMatrix();
    {
        glTranslatef(-lightPos[0] - 2.0f, 0.01f, -lightPos[2]); // TAG
        glMultMatrixf(matrix);
        glColor3f(0, 0, 0);
        drawSphere();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-lightPos[0] + 2.0f, 0.01f, -lightPos[2]); // TAG
        glMultMatrixf(matrix);
        glColor3f(0, 0, 0);
        drawSphere();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-lightPos2[0] + 2.0f, 0.01f, -lightPos2[2]); // TAG
        glMultMatrixf(matrix);
        glColor3f(0, 0, 0);
        drawSphere();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-lightPos2[0] - 2.0f, 0.01f, -lightPos2[2]); // TAG
        glMultMatrixf(matrix);
        glColor3f(0, 0, 0);
        drawSphere();
    }
    glPopMatrix();
}

//--------------------------------------------------------------------

void display()
{
    glClearColor(0.18f, 0.54f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glPushMatrix();
    {
        //glScalef(6.0f, 0.01f, 6.0f);
        //drawCube();
        glBegin(GL_QUADS);
        {
            glVertex3f(5.0f, -1.0f, 5.0f);
            glVertex3f(-5.0f, -1.0f, 5.0f);
            glVertex3f(-5.0f, -1.0f, -5.0f);
            glVertex3f(5.0f, -1.0f, -5.0f);
        }
        glEnd();
    }
    glPopMatrix();

    glPushMatrix();
    glColor3f(modelColor[0], modelColor[1], modelColor[2]);
    //glTranslatef(0, 1.0f, 0); // TAG
    switch (currentModel)
    {
    case 0:
        glPushMatrix();
        {
            glTranslatef(-1.0f, 0, 0);
            drawSphere();
        }
        glPopMatrix();
        glPushMatrix();
        {
            glTranslatef(1.0f, 0, 0);
            drawSphere();
        }
        glPopMatrix();
        break;
    case 1:
        drawPyramid();
        break;
    }
    glPopMatrix();

    if (toggleLighting)
    {
        setShadow();

        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHTING);

        glPushMatrix();
        {
            glColor3f(1, 1, 1);
            glTranslatef(lightPos[0], lightPos[1], lightPos[2]); // TAG
            glScalef(0.1f, 0.1f, 0.1f);
            drawSphere();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glColor3f(1, 1, 1);
            glTranslatef(lightPos2[0], lightPos2[1], lightPos2[2]); // TAG
            glScalef(0.1f, 0.1f, 0.1f);
            drawSphere();
        }
        glPopMatrix();

        setLighting();
    }
    else
    {
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHTING);
    }
}

//--------------------------------------------------------------------

void setCamera()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    switch (currentMode)
    {
    case 0:
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //glOrtho(orthoLeft, orthoRight, orthoBottom, orthoTop, -orthoZNear, -orthoZFar);
        gluPerspective(prespFovy, screenWidth / screenHeight, 0.001, 1000);
        gluLookAt(3, 5, 4, 0, 0, 0, 0, 1, 0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        break;
    case 1:
        gluPerspective(prespFovy, screenWidth / screenHeight, 0.1, 1000);
        glFrustum(prespLeft, prespRight, prespBottom, prespTop, prespZNear, prespZFar);

        if (GetCursorPos(&cursorPos))
        {
            int diffCursorPosX = cursorPos.x - lastCursorPosX; //check the difference between the current x and the last x position
            int diffCursorPosY = cursorPos.y - lastCursorPosY; //check the difference between the current y and the last y position
            lastCursorPosX = cursorPos.x;                      //set lastCursorPosX to the current x position
            lastCursorPosY = cursorPos.y;                      //set lastCursorPosY to the current y position
            cameraRotX += (float)diffCursorPosY;               //set the cameraRotX to cameraRotX with the addition of the difference in the y position
            cameraRotY += (float)diffCursorPosX;               //set the cameraRotX to cameraRotY with the addition of the difference in the x position
            if (cameraRotX > 180)
                cameraRotX -= 180;
            if (cameraRotY > 180)
                cameraRotY -= 180;
        }
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(cameraRotX, 1.0, 0.0, 0.0);                //rotate our camera on the x-axis (left and right)
        glRotatef(cameraRotY, 0.0, 1.0, 0.0);                //rotate our camera on the y-axis (up and down)
        glTranslated(-cameraPosX, -cameraPosY, -cameraPosZ); //translate the screen to the position of our camera

        break;
    }
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpfnWndProc = WindowProcedure;
    wc.lpszClassName = WINDOW_TITLE;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassEx(&wc))
        return false;

    HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, screenWidth, screenHeight,
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
    if (!wglMakeCurrent(hdc, hglrc))
        return false;

    //--------------------------------
    //	End initialization
    //--------------------------------

    ShowWindow(hWnd, nCmdShow);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    cursorPos.x = screenWidth / 2;
    cursorPos.y = screenHeight / 2;
    lastCursorPosX = cursorPos.x;
    lastCursorPosY = cursorPos.y;
    ShowCursor(false);
    SetCursorPos(cursorPos.x, cursorPos.y);

    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        setCamera();
        display();

        SwapBuffers(hdc);
    }

    UnregisterClass(WINDOW_TITLE, wc.hInstance);

    return true;
}
//--------------------------------------------------------------------
