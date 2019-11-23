#include <iostream>
#include "Model.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h> // standard definitions
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <vector>
#include <time.h>

using namespace std;


#define ESC 27

//----------------------------------------------------------------------
// Global variables
//
// The coordinate system is set up so that the (x,y)-coordinate plane
// is the ground, and the z-axis is directed upwards. The y-axis points
// to the north and the x-axis points to the east.
//
// The values (x,y) are the current camera position. The values (lx, ly)
// point in the direction the camera is looking. The variables angle and
// deltaAngle control the camera's angle. The variable deltaMove
// indicates the amount of incremental motion for the camera with each
// redraw cycle. The variables isDragging and xDragStart are used to
// monitor the mouse when it drags (with the left button down).
//----------------------------------------------------------------------



// Camera position
float x = 0.0, y = 0.0; // initially 5 units south of origin
float deltaMove = 0.0; // initially camera doesn't move
float rAngle = 0;
bool isEnhance = false;


float y_pawn[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
int move_pawn_index = -1;
vector<int> moveForwardPawn;
vector<int> moveBackwardPawn;

//float y_knight[2] = {0.0,0.0};
vector<int> moveKnight1Direction;
vector<int> moveKnight2Direction;

// Camera direction
float lx = 0.0, ly = 0.0, lz = 0.0; // camera points initially along y-axis
float angle = 0.0; // angle of rotation for the camera direction
float deltaAngle = 0.0; // additional angle change when dragging
bool light0 = false;
bool light1 = false;
// Mouse drag control
int isDragging = 0; // true when dragging
int xDragStart = 0; // records the x-coordinate when dragging starts

// overall map
int map[8][8] = {
        {1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1}
};

// pawn map
int pawnMap[8][2] = {
        {0,1},
        {1,1},
        {2,1},
        {3,1},
        {4,1},
        {5,1},
        {6,1},
        {7,1}
};
// knight map
int knightMap[2][2] = {
        {1,0},
        {6,0}
};

//----------------------------------------------------------------------
// Reshape callback
//
// Window size has been set/changed to w by h pixels. Set the camera
// perspective to 45 degree vertical field of view, a window aspect
// ratio of w/h, a near clipping plane at depth 1, and a far clipping
// plane at depth 100. The viewport is the entire window.
//
//----------------------------------------------------------------------



// ---------- enhanced model ----------------

Model   Pawn("../model/pawn.obj");
Model   Rook("../model/rook.obj");
Model   Knight("../model/knight.obj");
Model   Bishop("../model/bishop.obj");
Model   King("../model/king.obj");
Model   Queen("../model/queen.obj");
// ---------- enhanced model ----------------


void init(void)
{
    glClearColor(1.0, 0.0, 0.0, 0.0);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SMOOTH);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    //glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHTING);




    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
}


void changeSize(int w, int h)
{
    float ratio = ((float)w) / ((float)h); // window aspect ratio
    glMatrixMode(GL_PROJECTION); // projection matrix is active
    glLoadIdentity(); // reset the projection
    gluPerspective(45.0, ratio, 0.1, 100.0); // perspective transformation
    glMatrixMode(GL_MODELVIEW); // return to modelview mode
    glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
}

//----------------------------------------------------------------------
// Draw one snowmen (at the origin)
//
// A snowman consists of a large body sphere and a smaller head sphere.
// The head sphere has two black eyes and an orange conical nose. To
// better create the impression they are sitting on the ground, we draw
// a fake shadow, consisting of a dark circle under each.
//
// We make extensive use of nested transformations. Everything is drawn
// relative to the origin. The snowman's eyes and nose are positioned
// relative to a head sphere centered at the origin. Then the head is
// translated into its final position. The body is drawn and translated
// into its final position.
//----------------------------------------------------------------------

// =========================draw chessboard========================================

void drawChessBoard()
{
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            glPushMatrix();
            glTranslatef(i*1,j*1,0);
            if((i%2==0 && j%2==0)||(i%2==1 && j%2==1))
            {
                glColor3f(0.0,0.0,0.0);
            }
            else
            {
                glColor3f(1,1,1);
            }
            glBegin(GL_POLYGON);
            glVertex3f(0.0,0.0,0.0);
            glVertex3f(1.0,0.0,0.0);
            glVertex3f(1.0,1.0,0.0);
            glVertex3f(0.0,1.0,0.0);
            glEnd();
            glPopMatrix();
        }
    }
}

// =========================draw chessboard========================================



// =========================draw chess pieces======================================
// light pawn, glutSolidSphere
void drawPawn(bool isLight)
{
    //draw big sphere

    //draw edge

    if(isLight) glColor3f(1.0, 1.0, 0.9569);
    else glColor3f(0.5882, 0.2941, 0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0);
    GLdouble eqn [4]={0.0,0.0,1.0,0.0};
    glClipPlane(GL_CLIP_PLANE0,eqn);
    glEnable(GL_CLIP_PLANE0);
    glutSolidSphere(0.375, 20, 20);
    glDisable(GL_CLIP_PLANE0);



    // draw second big
    //glColor3f(255, 255, 240); // set drawing color to white
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.4); // position head
    glutSolidSphere(0.25, 20, 20); // head sphere

    // draw small
   // glColor3f(150, 150, 150); // set drawing color to white
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.35); // position head
    glutSolidSphere(0.15, 20, 20); // head sphere


    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

}

void drawRook(bool isLight)
{


    if(isLight) glColor3f(1.0, 1.0, 0.9569);
    else glColor3f(0.5882, 0.2941, 0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0);
    glutSolidCube(0.75);
    glPopMatrix();

}

// draw knight glutSolidTeapot
void drawKnight(bool isLight)
{
    if(isLight) glColor3f(1.0, 1.0, 0.9569);
    else glColor3f(0.5882, 0.2941, 0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0);
    glutSolidTeapot(0.4);
    glPopMatrix();
}

// draw bishop
void drawBishop(bool isLight)
{
    if(isLight) glColor3f(1.0, 1.0, 0.9569);
    else glColor3f(0.5882, 0.2941, 0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0);
    glutSolidCone(0.375, 1, 20,20);
    glPopMatrix();
}
// draw queen
void drawQueen(bool isLight)
{

    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    if(isLight) glColor3f(1.0, 1.0, 0.9569);
    else glColor3f(0.5882, 0.2941, 0);


    glScalef(0.43,0.43,0.577);
    glutSolidTetrahedron();

    glPopMatrix();
}

// draw king
void drawKing(bool isLight)
{
    if(isLight) glColor3f(1.0, 1.0, 0.9569);
    else glColor3f(0.5882, 0.2941, 0);
    glScalef(0.5,0.5,0.5);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0);
    glutSolidOctahedron();

    glPopMatrix();
}
// =========================draw chess pieces======================================






//----------------------------------------------------------------------
// Update with each idle event
//
// This incrementally moves the camera and requests that the scene be
// redrawn.
//----------------------------------------------------------------------
void update(void)
{
    if (deltaMove) { // update camera position
        x += deltaMove * lx * 0.1;
        y += deltaMove * ly * 0.1;
    }
    glutPostRedisplay(); // redisplay everything
}

//----------------------------------------------------------------------
// Draw the entire scene
//
// We first update the camera location based on its distance from the
// origin and its direction.
//----------------------------------------------------------------------
void renderScene(void)
{
    int i, j;

    // Clear color and depth buffers
    glClearColor(0.0, 0.0, 0.0, 1.0); // sky color is light blue
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();


    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);


    //glEnable(GL_LIGHT1);
    GLfloat light0_ambient[] = { 0.2, 0.2, 0.2, 1.0};
    GLfloat light0_specular[] = { 0.8, 0.8, 0.8, 1.0};


    GLfloat light1_diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
    GLfloat light1_position[] = { -5.0, -5.0, 8.0};
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };


    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light1_position);


    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glEnable(GL_DEPTH_TEST);





    // Set the camera centered at (x,y,1) and looking along directional
    // vector (lx, ly, 0), with the z-axis pointing up
    gluLookAt(
            4, -5, 10+lz,
            4, 4, 0.0,
            0.0, 0.0, 1.0);
    glTranslatef(4.0,4.0,0.0);
    glRotatef(rAngle,0, 0, 1);
    glTranslatef(-4,-4,0.0);
    drawChessBoard();

    // ============== draw light part ===========================
    // place 8 light pawn
    for (i = 0; i < 8; i++)
    {

        glPushMatrix();
        glTranslatef(i*1+0.5, 1.5+y_pawn[i], 0);
        if(!isEnhance) drawPawn(true);
        else {

            glScalef(0.015f, 0.015f, 0.015f);
            glRotatef(90, 1.0f, 0.0f, 0.0f);
            glColor3f(1.0, 1.0, 0.9569);

            Pawn.Draw();
        }
        glPopMatrix();



    }

    // place 2 light rook
    for(i = 0; i < 2; i++)
    {
        glPushMatrix();
        glTranslatef(i * 7 + 0.5,0.5,0.0);
        if(!isEnhance)
        {
            glTranslatef(0,0,0.375);
            drawRook(true);
        }
        else {
            glScalef(0.01f, 0.01f, 0.01f);
            glRotatef(90, 1.0f, 0.0f, 0.0f);
            glColor3f(1.0, 1.0, 0.9569);

            Rook.Draw();
        }
        glPopMatrix();
    }


    // place 2 knights_light
    for(i = 0;i<2;i++)
    {
        glPushMatrix();
        //glTranslatef(i * 5 + 1.5 + ,0.5+y_knight[i],0);
        glTranslatef(knightMap[i][0]+0.5,knightMap[i][1]+0.5,0);

        if(!isEnhance)
        {
            glTranslatef(0,0,0.5);
            glRotatef(90,1,0,0);
            drawKnight(true);
        }
        else {
            glScalef(0.01f, 0.01f, 0.01f);

            glRotatef(90, 1.0f, 0.0f, 0.0f);
            glColor3f(1.0, 1.0, 0.9569);
            Knight.Draw();

        }
        glPopMatrix();
    }

    // place 2 bishop
    for(i = 0;i<2;i++)
    {
        glPushMatrix();
        glTranslatef(i * 3 + 2.5,0.5,0.0);
        if(!isEnhance) drawBishop(true);
        else {
            glScalef(0.01f, 0.01f, 0.01f);
            glRotatef(90, 1.0f, 0.0f, 0.0f);
            glColor3f(1.0, 1.0, 0.9569);
            Bishop.Draw();
        }
        glPopMatrix();
    }

    // place light queen
    glPushMatrix();
    glTranslatef(3.5,0.5,0.0);
    if(!isEnhance)
    {
        glTranslatef(0,0,0.5);
        drawQueen(true);
    }
    else {
        glScalef(0.01f, 0.01f, 0.01);
        glRotatef(90, 1.0f, 0.0f, 0.0f);
        glColor3f(1.0, 1.0, 0.9569);
        Queen.Draw();
    }

    glPopMatrix();

    // place light king
    glPushMatrix();
    glTranslatef(4.5,0.5,0.0);
    if(!isEnhance)
    {
        glTranslatef(0,0,0.5);
        drawKing(true);
    }
    else {
        glScalef(0.01f, 0.01f, 0.01f);
        glRotatef(90, 1.0f, 0.0f, 0.0f);
        glColor3f(1.0, 1.0, 0.9569);
        King.Draw();
    }

    glPopMatrix();
    // ============== draw light part ===========================


    // ============== draw dark part ===========================

    for (i = 0; i < 8; i++)
    {
        glPushMatrix();
        glTranslatef(i*1+0.5, 6.5, 0);
        if(!isEnhance) {
            drawPawn(false);
        } else{
            glScalef(0.015f, 0.015f, 0.015f);
            glRotatef(90, 1.0f, 0.0f, 0.0f);
            glColor3f(0.5882, 0.2941, 0);

            Pawn.Draw();
        }
        glPopMatrix();
    }

    // place 2 dark rook
    for(i = 0; i < 2; i++)
    {
        glPushMatrix();
        glTranslatef(i * 7 + 0.5,7.5,0.0);
        if(!isEnhance)
        {
            glTranslatef(0,0,0.375);
            drawRook(false);
        }
        else {
            glScalef(0.01f, 0.01f, 0.01f);
            glRotatef(90, 1.0f, 0.0f, 0.0f);
            glColor3f(0.5882, 0.2941, 0);

            Rook.Draw();
        }
        glPopMatrix();
    }

// place dark knight
    for(i = 0;i<2;i++)
    {
        glPushMatrix();
        glTranslatef(i * 5 + 1.5,7.5,0.0);
        if(!isEnhance)
        {
            glTranslatef(0,0,0.5);
            glRotatef(90,1,0,0);
            glRotatef(90, 0.0f, 1.0f, 0.0f);
            drawKnight(false);
        }
        else {
            glScalef(0.01f, 0.01f, 0.01f);

            glRotatef(90, 1.0f, 0.0f, 0.0f);
            glRotatef(180, 0.0f, 1.0f, 0.0f);
            glColor3f(0.5882, 0.2941, 0);



            Knight.Draw();
        }
        glPopMatrix();
    }

    // place 2 dark bishop
    for(i = 0;i<2;i++)
    {
        glPushMatrix();
        glTranslatef(i * 3 + 2.5,7.5,0.0);
        if(!isEnhance)
        {

            drawBishop(false);
        }
        else {
            glScalef(0.01f, 0.01f, 0.01f);
            glRotatef(90, 1.0f, 0.0f, 0.0f);
            glColor3f(0.5882, 0.2941, 0);

            Bishop.Draw();
        }
        glPopMatrix();
    }

    // place dark queen
    glPushMatrix();
    glTranslatef(3.5,7.5,0);
    if(!isEnhance)
    {
        glTranslatef(0,0,0.5);
        drawQueen(false);
    }
    else {
        glScalef(0.01f, 0.01f, 0.01f);
        glRotatef(90, 1.0f, 0.0f, 0.0f);
        glColor3f(0.5882, 0.2941, 0);

        Queen.Draw();
    }

    glPopMatrix();

    // place dark king
    glPushMatrix();
    glTranslatef(4.5,7.5,0.0);
    if(!isEnhance)
    {
        glTranslatef(0,0,0.5);
        drawKing(false);
    }
    else {
        glScalef(0.01f, 0.01f, 0.01f);
        glRotatef(90, 1.0f, 0.0f, 0.0f);
        glColor3f(0.5882, 0.2941, 0);

        King.Draw();
    }

    glPopMatrix();
    // ============== draw dark part ===========================

    glutSwapBuffers(); // Make it all visible



}

//----------------------------------------------------------------------
// User-input callbacks
//
// processNormalKeys: ESC, q, and Q cause program to exit
// pressSpecialKey: Up arrow = forward motion, down arrow = backwards
// releaseSpecialKey: Set incremental motion to zero
//----------------------------------------------------------------------
void processNormalKeys(unsigned char key, int xx, int yy)
{

    if (key == ESC || key == 'q' || key == 'Q')
    {
        exit(0);
    }
    else if(key == 'e' || key == 'E')
    {
        if(isEnhance == true) isEnhance=false;
        else isEnhance = true;
    }
    else if(key == 'd' || key == 'D')
    {
        lz -= 0.25;
    }
    else if(key == 'u' || key == 'U')
    {
        lz += 0.25;
    }
    else if(key == '0')
    {
        if(light0) light0=false;
        else light0=true;

        if(light0) glEnable(GL_LIGHT0);
        else glDisable(GL_LIGHT0);
    }
    else if(key == '1')
    {

        if(light1) light1=false;
        else light1=true;

        if(light1) glEnable(GL_LIGHT1);
        else glDisable(GL_LIGHT1);
    }
    else if(key == 'r' || key == 'R')
    {
        rAngle += 10;
    }
    else if(key == 'K' || key == 'k')
    {
        bool fineKnight=false;
        int knight_x_tmp = 0;
        int knight_y_tmp = 0;
        int direction = -1;
        int move_knight_index = -1;


        moveKnight1Direction.clear();
        moveKnight2Direction.clear();
        for(int i=0; i<2; i++)
        {
            knight_x_tmp = knightMap[i][0];
            knight_y_tmp = knightMap[i][1];
            // 上1右2，case0
            if(knight_x_tmp < 6 && knight_y_tmp < 7 && map[knight_y_tmp+1][knight_x_tmp+2] == 0)
            {
                fineKnight = true;
                if(i==0) moveKnight1Direction.push_back(0);
                else moveKnight2Direction.push_back(0);
            }
            // 上2右1，case1
            if(knight_x_tmp<7 && knight_y_tmp < 6 && map[knight_y_tmp+2][knight_x_tmp+1] == 0)
            {
                fineKnight = true;
                if(i==0) moveKnight1Direction.push_back(1);
                else moveKnight2Direction.push_back(1);
            }
            // 上2左1, case2
            if(knight_x_tmp > 0 && knight_y_tmp < 6 && map[knight_y_tmp+2][knight_x_tmp-1] == 0)
            {
                fineKnight = true;
                if(i==0) moveKnight1Direction.push_back(2);
                else moveKnight2Direction.push_back(2);
            }
            // 上1左2，case3
            if(knight_x_tmp >1 && knight_y_tmp <7 && map[knight_y_tmp + 1][knight_x_tmp-2] == 0)
            {
                fineKnight = true;
                if(i==0) moveKnight1Direction.push_back(3);
                else moveKnight2Direction.push_back(3);
            }
            //下1左2，case4
            if(knight_x_tmp > 2 && knight_y_tmp > 1 && map[knight_y_tmp-1][knight_x_tmp-2] == 0)
            {
                fineKnight = true;
                if(i==0) moveKnight1Direction.push_back(4);
                else moveKnight2Direction.push_back(4);
            }
            // 下2左1，case5
            if(knight_x_tmp > 0 && knight_y_tmp > 1 && map[knight_y_tmp-2][knight_x_tmp-1]==0)
            {
                fineKnight = true;
                if(i==0) moveKnight1Direction.push_back(5);
                else moveKnight2Direction.push_back(5);
            }
            // 下2右1，case6
            if(knight_x_tmp < 7 && knight_y_tmp > 1 && map[knight_y_tmp-2][knight_x_tmp+1]==0)
            {
                fineKnight = true;
                if(i==0) moveKnight1Direction.push_back(6);
                else moveKnight2Direction.push_back(6);
            }
            // 下1右2，case7
            if(knight_x_tmp < 6 && knight_y_tmp > 0 && map[knight_y_tmp-1][knight_x_tmp+2]==0)
            {
                fineKnight = true;
                if(i==0) moveKnight1Direction.push_back(7);
                else moveKnight2Direction.push_back(7);
            }
        }
        srand((unsigned)time(NULL));

        if(moveKnight1Direction.size()!=0 && moveKnight2Direction.size()==0)
        {
            move_knight_index = 0;
        }
        else if(moveKnight1Direction.size()==0 && moveKnight2Direction.size()!=0)
        {
            move_knight_index = 1;

        }
        else if(moveKnight1Direction.size()!=0 && moveKnight2Direction.size()!=0)
        {
            move_knight_index = rand()%2;
        }
        if(move_knight_index == 1)
        {
            direction = rand() % (moveKnight2Direction.size());
            direction = moveKnight2Direction[direction];
        }
        else if(move_knight_index == 0)
        {
            direction = rand() % (moveKnight1Direction.size());
            direction = moveKnight1Direction[direction];
        }
        if(direction != -1)
        {
            switch (direction)
            {
                case 0:
                    knight_x_tmp = knightMap[move_knight_index][0];
                    knight_y_tmp = knightMap[move_knight_index][1];
                    map[knight_y_tmp][knight_x_tmp] = 0;
                    map[knight_y_tmp+1][knight_x_tmp+2] = 1;
                    knightMap[move_knight_index][0] += 2;
                    knightMap[move_knight_index][1] += 1;
                    break;
                case 1:
                    knight_x_tmp = knightMap[move_knight_index][0];
                    knight_y_tmp = knightMap[move_knight_index][1];
                    map[knight_y_tmp][knight_x_tmp] = 0;
                    map[knight_y_tmp+2][knight_x_tmp+1] = 1;
                    knightMap[move_knight_index][0] += 1;
                    knightMap[move_knight_index][1] += 2;
                    break;
                case 2:
                    knight_x_tmp = knightMap[move_knight_index][0];
                    knight_y_tmp = knightMap[move_knight_index][1];
                    map[knight_y_tmp][knight_x_tmp] = 0;
                    map[knight_y_tmp+2][knight_x_tmp-1] = 1;
                    knightMap[move_knight_index][0] -= 1;
                    knightMap[move_knight_index][1] += 2;
                    break;
                case 3:
                    knight_x_tmp = knightMap[move_knight_index][0];
                    knight_y_tmp = knightMap[move_knight_index][1];
                    map[knight_y_tmp][knight_x_tmp] = 0;
                    map[knight_y_tmp+1][knight_x_tmp-2] = 1;
                    knightMap[move_knight_index][0] -= 2;
                    knightMap[move_knight_index][1] += 1;
                    break;
                case 4:
                    knight_x_tmp = knightMap[move_knight_index][0];
                    knight_y_tmp = knightMap[move_knight_index][1];
                    map[knight_y_tmp][knight_x_tmp] = 0;
                    map[knight_y_tmp-1][knight_x_tmp-2] = 1;
                    knightMap[move_knight_index][0] -= 2;
                    knightMap[move_knight_index][1] -= 1;
                    break;
                case 5:
                    knight_x_tmp = knightMap[move_knight_index][0];
                    knight_y_tmp = knightMap[move_knight_index][1];
                    map[knight_y_tmp][knight_x_tmp] = 0;
                    map[knight_y_tmp-2][knight_x_tmp-1] = 1;
                    knightMap[move_knight_index][0] -= 1;
                    knightMap[move_knight_index][1] -= 2;
                    break;
                case 6:
                    knight_x_tmp = knightMap[move_knight_index][0];
                    knight_y_tmp = knightMap[move_knight_index][1];
                    map[knight_y_tmp][knight_x_tmp] = 0;
                    map[knight_y_tmp-2][knight_x_tmp+1] = 1;
                    knightMap[move_knight_index][0] += 1;
                    knightMap[move_knight_index][1] -= 2;
                    break;
                case 7:
                    knight_x_tmp = knightMap[move_knight_index][0];
                    knight_y_tmp = knightMap[move_knight_index][1];
                    map[knight_y_tmp][knight_x_tmp] = 0;
                    map[knight_y_tmp-1][knight_x_tmp+2] = 1;
                    knightMap[move_knight_index][0] += 2;
                    knightMap[move_knight_index][1] -= 1;
                    break;
            }
        }

    }

    else if(key == 'p' || key == 'P')
    {
        bool foundForwardPawn = false;
        bool foundBackwardPawn = false;
        int pawn_x_tmp = 0;
        int pawn_y_tmp = 0;
        move_pawn_index = -1;
        moveForwardPawn.clear();
        moveBackwardPawn.clear();
        for(int i=0; i<8;i++)
        {
            pawn_x_tmp = pawnMap[i][0];
            pawn_y_tmp = pawnMap[i][1];
            if(pawn_y_tmp < 7 && map[pawn_y_tmp+1][pawn_x_tmp] == 0){
                foundForwardPawn = true;
                //move_pawn_index = i;
                moveForwardPawn.push_back(i);
            }
            if(pawn_y_tmp > 0 && map[pawn_y_tmp-1][pawn_x_tmp] == 0)
            {
                foundBackwardPawn = true;
                //move_pawn_index = i;
                moveBackwardPawn.push_back(i);
            }
        }
        if(foundForwardPawn == true && foundBackwardPawn != true) //只能向前哦
        {
            srand((unsigned)time(NULL));
            int forward_num = moveForwardPawn.size();
            move_pawn_index = rand()%forward_num;
            move_pawn_index = moveForwardPawn[move_pawn_index];
            y_pawn[move_pawn_index] += 1;
            //更新map和knightMap
            pawn_x_tmp = pawnMap[move_pawn_index][0];
            pawn_y_tmp = pawnMap[move_pawn_index][1];
            map[pawn_y_tmp][pawn_x_tmp] = 0;
            map[pawn_y_tmp+1][pawn_x_tmp] = 1;
            pawnMap[move_pawn_index][1] += 1;
        }
        else if(foundBackwardPawn == true && foundForwardPawn == false) //只能向后
        {
            // pick one and move backward
            int backward_num = moveBackwardPawn.size();
            move_pawn_index = rand()%backward_num;
            move_pawn_index = moveBackwardPawn[move_pawn_index];
            y_pawn[move_pawn_index] -= 1;
            //更新map和knightMap
            pawn_x_tmp = pawnMap[move_pawn_index][0];
            pawn_y_tmp = pawnMap[move_pawn_index][1];
            map[pawn_y_tmp][pawn_x_tmp] = 0;
            map[pawn_y_tmp-1][pawn_x_tmp] = 1;
            pawnMap[move_pawn_index][1] -= 1;
        }
        else if(foundBackwardPawn == true && foundForwardPawn == true) //任意向前向后
        {
            srand((unsigned)time(NULL));
            int case_direction = rand()%2;
            if(case_direction == 0)
            {
                // pick one and move forward
                int forward_num = moveForwardPawn.size();
                move_pawn_index = rand()%forward_num;
                move_pawn_index = moveForwardPawn[move_pawn_index];
                y_pawn[move_pawn_index] += 1;
                //更新map和knightMap
                pawn_x_tmp = pawnMap[move_pawn_index][0];
                pawn_y_tmp = pawnMap[move_pawn_index][1];
                map[pawn_y_tmp][pawn_x_tmp] = 0;
                map[pawn_y_tmp+1][pawn_x_tmp] = 1;
                pawnMap[move_pawn_index][1] += 1;

            }
            else if(case_direction == 1)
            {
                // pick one and move backward
                int backward_num = moveBackwardPawn.size();
                move_pawn_index = rand()%backward_num;
                move_pawn_index = moveBackwardPawn[move_pawn_index];
                y_pawn[move_pawn_index] -= 1;
                //更新map和knightMap
                pawn_x_tmp = pawnMap[move_pawn_index][0];
                pawn_y_tmp = pawnMap[move_pawn_index][1];
                map[pawn_y_tmp][pawn_x_tmp] = 0;
                map[pawn_y_tmp-1][pawn_x_tmp] = 1;
                pawnMap[move_pawn_index][1] -= 1;

            }
        }
    }
}



//----------------------------------------------------------------------
// Main program  - standard GLUT initializations and callbacks
//----------------------------------------------------------------------
int main(int argc, char **argv)
{
    printf("\n\
-----------------------------------------------------------------------\n\
  OpenGL Sample Program:\n\
  - Drag mouse left-right to rotate camera\n\
  - Hold up-arrow/down-arrow to move camera forward/backward\n\
  - q or ESC to quit\n\
-----------------------------------------------------------------------\n");

    // general initializations
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("3D Chess");

    init();





    // register callbacks
    glutReshapeFunc(changeSize); // window reshape callback
    glutDisplayFunc(renderScene); // (re)display callback
    glutIdleFunc(update); // incremental update
    glutIgnoreKeyRepeat(1); // ignore key repeat when holding key down
    glutKeyboardFunc(processNormalKeys); // process standard key clicks


    // OpenGL init

    glEnable(GL_DEPTH_TEST);




    // enter GLUT event processing cycle
    glutMainLoop();

    return 0; // this is just to keep the compiler happy
}