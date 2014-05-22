//
//  main.cpp
//  Snake_client

#include <iostream>
#include <fstream>
#include <time.h>
#include "math.h"
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <pthread.h>

using namespace std;

int sock;
int mymsg;
int recvmssg;

int dir;
int d = 1;
int num = 7;
int num1 = 7;
int key1 = 3;
int Score = 0;
int Score1 = 0;
int Scale = 25;
int hightScore;
int N = 50,M = 30;
int w = Scale * N;
int h = Scale * M;

char sScore[15];
char sScore1[15];
char sHightScore[15];
float xm = 0.0;
float ym = 0.0;
bool down=false;

struct
{
    int x;
    int y;
} s[100];
struct
{
    int x1;
    int y1;
} s1[100];

class Fruct
{
public:
    int x,y;
    void New()
    {
        x = rand() % N;
        y = rand() % (M-3);
    }
    void DrawFruct()
    {
        glColor3f (1.0, 1.0, 0.0);
        glRectf (x*Scale, y*Scale, (x+1)*Scale, (y+1)*Scale);
    }
}m[2];

class Bomb
{
public:
    int x,y;
    
    void New()
    {
        x = rand() % N;
        y = rand() % (M-3);
    }
    void DrawBomb()
    {
        glColor3f (1.0, 0.0, 0.0);
        glBegin(GL_POLYGON);
        for(float i=0; i < 2*3.14; i += 3.14/4)
        {
            glVertex2f((x+0.5)*Scale + (0.5)*Scale*(1.1)*sin(i), (y+0.5)*Scale + (0.5)*Scale*(1.1)*cos(i));
        }
        glEnd();
    }
}u[5];

void draw_string(void *font, const char* string)
{
    while(*string)
        glutStrokeCharacter(font, *string++);
}

void DrawMenu()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBegin(GL_POLYGON); // Start
    glColor3f (0.0, 0.26, 0.0);
    glVertex3f (705.0, 650.0, 0.0);
    glColor3f (0.0, 0.19, 0.0);
    glVertex3f (500.0, 650.0, 0.0);
    glColor3f (0.0, 0.16, 0.0);
    glVertex3f (500.0, 550.0, 0.0);
    glColor3f (0.0, 0.19, 0.0);
    glVertex3f (705.0, 550.0, 0.0);
    glEnd();
    glBegin(GL_POLYGON); // Rules
    glColor3f (0.0, 0.26, 0.0);
    glVertex3f (705.0, 510.0, 0.0);
    glColor3f (0.0, 0.19, 0.0);
    glVertex3f (500.0, 510.0, 0.0);
    glColor3f (0.0, 0.16, 0.0);
    glVertex3f (500.0, 410.0, 0.0);
    glColor3f (0.0, 0.19, 0.0);
    glVertex3f (705.0, 410.0, 0.0);
    glEnd();
    glBegin(GL_POLYGON); // Exit
    glColor3f (0.0, 0.26, 0.0);
    glVertex3f (670.0, 230.0, 0.0);
    glColor3f (0.0, 0.19, 0.0);
    glVertex3f (540.0, 230.0, 0.0);
    glColor3f (0.0, 0.16, 0.0);
    glVertex3f (540.0, 140.0, 0.0);
    glColor3f (0.0, 0.19, 0.0);
    glVertex3f (670.0, 140.0, 0.0);
    glEnd();
    
    glLineWidth(4.0f);
    glColor3f (0.0,1.0,1.0);
    
    glPushMatrix();
    glTranslatef(w/(2.38), h/(1.3), 0);
    glScalef(0.4f, 0.4f, 0.4f);
    draw_string(GLUT_STROKE_ROMAN, "START");
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(w/(2.38), h/(1.7), 0);
    glScalef(0.4f, 0.4f, 0.4f);
    draw_string(GLUT_STROKE_ROMAN, "RULES");
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(w/(2.229), h/(4.4), 0);
    glScalef(0.4f, 0.4f, 0.4f);
    draw_string(GLUT_STROKE_ROMAN, "EXIT");
    glPopMatrix();
    
    glFinish();
    glutSwapBuffers();
    
}

void DrawExit()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
    glColor3f (0.0, 0.16, 0.0);
    glVertex3f (750.0, 150.0, 0.0);
    glColor3f (0.0, 0.19, 0.0);
    glVertex3f (500.0, 150.0, 0.0);
    glColor3f (0.0, 0.16, 0.0);
    glVertex3f (500.0, 90.0, 0.0);
    glColor3f (0.0, 0.19, 0.0);
    glVertex3f (750.0, 90.0, 0.0);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f (0.0, 0.16, 0.0);
    glVertex3f (670.0, 80.0, 0.0);
    glColor3f (0.0, 0.19, 0.0);
    glVertex3f (580.0, 80.0, 0.0);
    glColor3f (0.0, 0.16, 0.0);
    glVertex3f (580.0, 30.0, 0.0);
    glColor3f (0.0, 0.19, 0.0);
    glVertex3f (670.0, 30.0, 0.0);
    glEnd();
    
    glLineWidth(7.0f);
    glColor3f (1.0,0.0,0.0);
    glPushMatrix();
    glTranslatef(w/(6), h/(1.5), 0);
    glScalef(1.1f, 1.1f, 1.1f);
    draw_string(GLUT_STROKE_ROMAN, "Game over!");
    glPopMatrix();
    
    glLineWidth(2.5f);
    glColor3f (0.0,1.0,1.0);
    glPushMatrix();
    glTranslatef(w/(2.4), h/7, 0);
    glScalef(0.2f, 0.2f, 0.2f);
    draw_string(GLUT_STROKE_ROMAN, "Return to MENU");
    glPopMatrix();
    glPushMatrix();
    glTranslatef(w/(2.07), h/(15.3), 0);
    glScalef(0.2f, 0.2f, 0.2f);
    draw_string(GLUT_STROKE_ROMAN, "EXIT");
    glPopMatrix();
    
    /*glLineWidth(3.5f);
     glColor3f (0.9,0.3,0.5);
     glPushMatrix();
     glTranslatef(w/(2.8), h/(2.1), 0);
     glScalef(0.4f, 0.4f, 0.4f);
     draw_string(GLUT_STROKE_ROMAN, "Final score:");
     glPopMatrix();
     
     sprintf(sScore, "%9d", Score);
     glPushMatrix();
     glTranslatef(w/(2.8), h/(2.1), 0);
     glScalef(0.4f, 0.4f, 0.4f);
     draw_string(GLUT_STROKE_ROMAN, sScore);
     glPopMatrix();
     
     ifstream inFile("Snake.bin",ios_base::binary);
     while(inFile.peek()!=EOF) inFile >> sHightScore;
     inFile.close();
     hightScore = atoi(sHightScore);
     if ( Score > hightScore )
     {
     sprintf(sHightScore, "%9d", Score);
     ofstream outFile("Snake.bin",ios_base::binary);
     outFile << sScore;
     outFile.close();
     }
     glPushMatrix();
     glTranslatef(w/(2.8), h/(2.55), 0);
     glScalef(0.4f, 0.4f, 0.4f);
     draw_string(GLUT_STROKE_ROMAN, "Hide score:");
     glPopMatrix();
     glPopMatrix();
     glPushMatrix();
     glTranslatef(w/(1.6), h/(2.55), 0);
     glScalef(0.4f, 0.4f, 0.4f);
     draw_string(GLUT_STROKE_ROMAN, sHightScore);
     glPopMatrix();*/
    
    glFinish();
    glutSwapBuffers();
}
void DrawRules()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBegin(GL_POLYGON);
    glColor3f (0.0, 0.16, 0.0);
    glVertex3f (710.0, 150.0, 0.0);
    glColor3f (0.0, 0.19, 0.0);
    glVertex3f (490.0, 150.0, 0.0);
    glColor3f (0.0, 0.16, 0.0);
    glVertex3f (490.0, 90.0, 0.0);
    glColor3f (0.0, 0.19, 0.0);
    glVertex3f (710.0, 90.0, 0.0);
    glEnd();
    
    glLineWidth(1.8f);
    glColor3f (0.0,1.0,1.0);
    
    glPushMatrix();
    glTranslatef(w/3, h/(1.2), 0);
    glScalef(0.5f, 0.5f, 0.5f);
    draw_string(GLUT_STROKE_ROMAN, "How to play:");
    glPopMatrix();
    glPushMatrix();
    glTranslatef(w/(3), h/(1.5), 0);
    glScalef(0.2f, 0.2f, 0.2f);
    draw_string(GLUT_STROKE_ROMAN, "1.Eat fruit to score points:");
    glPopMatrix();
    glPushMatrix();
    glTranslatef(w/(3), h/(1.7), 0);
    glScalef(0.2f, 0.2f, 0.2f);
    draw_string(GLUT_STROKE_ROMAN, "2.Do not eat bombs:");
    glPopMatrix();
    glPushMatrix();
    glTranslatef(w/(3), h/(2.0), 0);
    glScalef(0.2f, 0.2f, 0.2f);
    draw_string(GLUT_STROKE_ROMAN, "3.Do not eat yourself.");
    glPopMatrix();
    glPushMatrix();
    glTranslatef(w/(3), h/(2.4), 0);
    glScalef(0.2f, 0.2f, 0.2f);
    draw_string(GLUT_STROKE_ROMAN, "4.Do not leave the window border.");
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(w/(2.5), h/7, 0);
    glScalef(0.2f, 0.2f, 0.2f);
    draw_string(GLUT_STROKE_ROMAN, "return to MENU");
    glPopMatrix();
    
    glLineWidth(3.0f);
    glColor3f (0.0,1.0,1.0);
    glPushMatrix();
    glTranslatef(w/(2.7), h/(3.3), 0);
    glScalef(0.4f, 0.4f, 0.4f);
    draw_string(GLUT_STROKE_ROMAN, "Good luck!");
    glPopMatrix();
    
    glColor3f (1.0, 1.0, 0.0);
    glRectf (780.0,500.0,805.0, 525.0);
    glColor3f (1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    for(float i=0; i < 2*3.14; i += 3.14/4)
    {
        glVertex2f(720.0 + (0.5)*Scale*(1.1)*sin(i), 450.0 + (0.5)*Scale*(1.1)*cos(i));
    }
    glEnd();
    
    glFinish();
    glutSwapBuffers();
}

void Tick()
{
    //Движение тела змейки:
    for (int i = num; i > 0; --i)
    {
        s[i].x = s[i-1].x;
        s[i].y = s[i-1].y;
    }
    for (int i = num1; i > 0; --i)
    {
        s1[i].x1 = s1[i-1].x1;
        s1[i].y1 = s1[i-1].y1;
    }
    recv(sock, &dir, sizeof(int), 0);
    //Движение головы змейки:
    switch (dir) {
        case 7:
            s[0].y+=1;
            
            break;
        case 1:
            s[0].x-=1;
            
            break;
        case 3:
            s[0].x+=1;
            
            break;
        case 5:
            s[0].y-=1;
            
            break;
        case 2:
            s1[0].x1-=1;
            break;
        case 6:
            s1[0].x1+=1;
            break;
        case 10:
            s1[0].y1-=1;
            break;
        case 14:
            s1[0].y1+=1;
            break;
    }
    int h=0;
    // Если наехали на фрукт, змейка увеличивается:
    for (int i = 0; i < 2; i++)
        if ( (s[0].x == m[i].x) && (s[0].y == m[i].y) )
        {
            num++;
            m[i].New();
            if(h!=11){
                u[h].New();
                h++;
            }
            else{
                h=0;
                u[h].New();
            }
            Score+=2;
        }
    for (int i = 0; i < 2; i++)
        if ( (s1[0].x1 == m[i].x) && (s1[0].y1 == m[i].y) )
        {
            num++;
            m[i].New();
            if(h!=11){
                u[h].New();
                h++;
            }
            else{
                h=0;
                u[h].New();
            }
            Score1+=2;
            //cout << Score1 << endl;
        }
    // Если наехали на бомбу, сокращается ее длина:
    for (int i = 0; i < 5; i++)
        if ( (s[0].x == u[i].x) && (s[0].y == u[i].y) )
        {
            if (num == 2) key1=2;
            if (num > 3)
                num = num - 2;
            else
                num = 2;
            u[i].New();
            if (Score > 0)
                Score--;
            if (Score <  0)
                Score =0;
        }
    for (int i = 0; i < 5; i++)
        if ( (s1[0].x1 == u[i].x) && (s1[0].y1 == u[i].y) )
        {
            if (num1 == 2) key1=2;
            if (num1 > 3)
                num1 = num1 - 2;
            else
                num1 = 2;
            u[i].New();
            if (Score1 > 0)
                Score1--;
            if (Score1 <  0)
                Score1 =0;
        }
    
    
    // Если вышли за границы, конец игры:
    if (s[0].x > N || s[0].x < 0 || s[0].y > (M-3) || s[0].y < 0)
    {
        key1=2;
    }
    // Если вышли за границы, конец игры:
    if (s1[0].x1 > N || s1[0].x1 < 0 || s1[0].y1 > (M-3) || s1[0].y1 < 0)
    {
        key1=2;
    }
    /*
    // если столкнулись с другой змейкой
    for (int i = 0; i < num1; i++)
        if ( (s[0].x == s1[i].x1) && (s[0].y == s1[i].y1) )
        {
            Score+=3;
            if (num1 > 3)
                num1-=3;
            else
            {
                if (num1 == 1)
                    key1 = 2;
                else
                    num1--;
            }
        }
    for (int i = 0; i < num; i++)
        if ( (s1[0].x1 == s[i].x) && (s1[0].y1 == s[i].y) )
        {
            Score1+=3;
            if (num > 3)
                num-=3;
            else
            {
                if (num == 1)
                    key1 = 2;
                else
                    num--;
            }
        }
    */
    //Если змейка наехала сама на себя, сокращается ее длина:
    for (int i = 1; i < num; i++)
        if (s[0].x == s[i].x && s[0].y == s[i].y )
        {
            num--;
            if (Score > 0)
                Score-=2;
            if (Score < 0)
                Score = 0;
        }
    for (int i = 1; i < num1; i++)
        if (s1[0].x1 == s1[i].x1 && s1[0].y1 == s1[i].y1 )
        {
            num1--;
            if (Score1 > 0)
                Score1-=2;
            if (Score1 < 0)
                Score1 = 0;
        }
}

void DrawSnake()
{
    glColor3f (0.2, 1.0, 0.2);
    for (int i = 0; i < num; i++)
    {
        glRectf (s[i].x*Scale, s[i].y*Scale, (s[i].x+1)*Scale, (s[i].y+1)*Scale);
    }
}

void DrawSnake1()
{
    glColor3f (0.3, 0.3, 1.0);
    for (int i = 0; i < num1; i++)
    {
        glRectf (s1[i].x1*Scale, s1[i].y1*Scale, (s1[i].x1+1)*Scale, (s1[i].y1+1)*Scale);
    }
}

void DrawScore()
{
    glLineWidth(1.5f);
    glColor3f (0.2, 1.0, 0.2);
    glPushMatrix();
    glTranslatef(w/(8.4), h/(1.05), 0);
    glScalef(0.3f, 0.3f, 0.3f);
    draw_string(GLUT_STROKE_ROMAN, "Player 1:");
    glPopMatrix();
    sprintf(sScore, "%9d", Score);
    glPushMatrix();
    glTranslatef(w/(8.38), h/(1.05), 0);
    glScalef(0.3f, 0.3f, 0.3f);
    draw_string(GLUT_STROKE_ROMAN, sScore);
    glPopMatrix();
    glColor3f (0.3, 0.3, 1.0);
    glPushMatrix();
    glTranslatef(w/(1.68), h/(1.05), 0);
    glScalef(0.3f, 0.3f, 0.3f);
    draw_string(GLUT_STROKE_ROMAN, "Player 2:");
    glPopMatrix();
    sprintf(sScore1, "%9d", Score1);
    glPushMatrix();
    glTranslatef(w/(1.6), h/(1.05), 0);
    glScalef(0.3f, 0.3f, 0.3f);
    draw_string(GLUT_STROKE_ROMAN, sScore1);
    glPopMatrix();
    
    glColor3f (0.3, 0.3, 1.0);
    ifstream inFile("Snake.bin",ios_base::binary);
    while(inFile.peek()!=EOF)
        inFile >> sHightScore;
    inFile.close();
    hightScore = atoi(sHightScore);
    glPushMatrix();
    glTranslatef(w/(1.0), h/(1.05), 0);
    glScalef(0.3f, 0.3f, 0.3f);
    draw_string(GLUT_STROKE_ROMAN, "Hide score:");
    glPopMatrix();
    glPushMatrix();
    glTranslatef(w/(1.0), h/(1.05), 0);
    glScalef(0.3f, 0.3f, 0.3f);
    draw_string(GLUT_STROKE_ROMAN, sHightScore);
    glPopMatrix();
    
    
    glFinish();
    glutSwapBuffers();
}

void DrawConnect()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
    glColor3f (0.0, 0.16, 0.0);
    glVertex3f (670.0, 80.0, 0.0);
    glColor3f (0.0, 0.19, 0.0);
    glVertex3f (580.0, 80.0, 0.0);
    glColor3f (0.0, 0.16, 0.0);
    glVertex3f (580.0, 30.0, 0.0);
    glColor3f (0.0, 0.19, 0.0);
    glVertex3f (670.0, 30.0, 0.0);
    glEnd();
    
    glLineWidth(5.0f);
    glColor3f (0.8,0.2,0.0);
    glPushMatrix();
    glTranslatef(w/(3.2), h/(2.2), 0);
    glScalef(0.7f, 0.7f, 0.7f);
    draw_string(GLUT_STROKE_ROMAN, "Connection..");
    glPopMatrix();
    
    glLineWidth(2.5f);
    glColor3f (0.0,1.0,1.0);
    glPushMatrix();
    glTranslatef(w/(2.07), h/(15.3), 0);
    glScalef(0.2f, 0.2f, 0.2f);
    draw_string(GLUT_STROKE_ROMAN, "Stop");
    glPopMatrix();
    glFinish();
    glutSwapBuffers();

}

void display()
{
    switch (key1)
    {
        case 1:
            glClear(GL_COLOR_BUFFER_BIT);
            glBegin(GL_POLYGON);
            glColor3f (0.0, 0.3, 0.0);
            glVertex3f (0.0, 800.0, 0.0);
            glColor3f (0.0, 0.11, 0.0);
            glVertex3f (0, 700.0, 0.0);
            glColor3f (0.0, 0.11, 0.0);
            glVertex3f (1400.0, 700.0, 0.0);
            glColor3f (0.0, 0.3, 0.0);
            glVertex3f (1400.0, 800.0, 0.0);
            glEnd();
            for( int i = 0; i < 2; i++)
                m[i].DrawFruct();
            for( int i = 0; i < 5; i++)
                u[i].DrawBomb();
            DrawSnake();
            DrawSnake1();
            DrawScore();
            break;
        case 2:
            DrawExit();
            break;
        case 3:
            DrawMenu();
            break;
        case 4:
            DrawRules();
            break;
        case 5:
            DrawConnect();
            break;
    }
    glFlush();
}

void fjfjfh()
{
    for(int i = 0; i < 2; i++)
        m[i].New();
    for(int i = 0; i < 5; i++)
        u[i].New();
    
    s[0].x = 45;
    s[0].y = 15;
    s1[0].x1 = rand() % N;
    s1[0].y1 = rand() % (M-3);
}

void MyKeyboard( int key, int a, int b)
{
    switch (key) {
        case 101:   // вверх
            dir = 7;
            send(sock, &dir, sizeof(int), 0);
            break;
        case 102:   // направо
            dir = 3;
            send(sock, &dir, sizeof(int), 0);
            break;
        case 100:   // налево
            dir = 1;
            send(sock, &dir, sizeof(int), 0);
            break;
        case 103:   // вниз
            dir = 5;
            send(sock, &dir, sizeof(int), 0);
            break;
        case 27:    // Escape
            exit(0);
            break;
    }
}

void MousePressed(int button, int state, int ax, int ay)
{
    down= button==GLUT_LEFT_BUTTON && state==GLUT_LEFT;
    if(down)
    {
        if (key1==3)
        {
            if(ax > (540.0) && ax < (670.0) && ay > (490.0) && ay < (600.0) )
            {
                exit(0);
            }
            if(ax > (500.0) && ax < (705.0) && ay > (230.0) && ay < (330.0) )
            {
                d = 1;
                glClear(GL_COLOR_BUFFER_BIT);
                fjfjfh();
                key1=4;
                display();
            }
            if(ax > (500.0) && ax < (705.0) && ay > (100.0) && ay < (190.0) )
            {
                key1=5;
                display();
            }
        }
        if (key1==2)
        {
            if(ax > (580.0) && ax < (670.0) && ay > (660.0) && ay < (690.0) )
            {
                exit(0);
            }
            if(ax > (540.0) && ax < (670.0) && ay > (580.0) && ay < (610.0) )
            {
                key1=3;
                d = 1;
                glClear(GL_COLOR_BUFFER_BIT);
                fjfjfh();
                DrawMenu();
            }
        }
        if (key1==4)
        {
            if(ax > (520.0) && ax < (680.0) && ay > (580.0) && ay < (610.0) )
            {
                key1=3;
                d = 1;
                glClear(GL_COLOR_BUFFER_BIT);
                fjfjfh();
                DrawMenu();
            }
        }
        if (key1==5)
        {
            if(ax > (580.0) && ax < (670.0) && ay > (660.0) && ay < (690.0) )
            {
                exit(0);
            }
        }
    }
    glutMouseFunc(MousePressed);
}

void CreateGlutWindow()
{
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);// Выбор режима:  одиночный буфер и RGBA цвета
    glutInitWindowSize (w, h);
    glutCreateWindow ("Snake");
    glClearColor(0.0, 0.1, 0.0, 0);
    //glClearColor(0.0, 0.18, 0.0, 0);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (0, w, 0, h);
}

void timer (int = 0)
{
    if (d==2){
        display();
    }
    Tick();
    if (recvmssg != 0)
    {
        // Обработка поворотов
        recvmssg = 0;
    }
    glutTimerFunc (80,timer,0);
}
void* listen(void *param)
{
    while (1)
    {
        recv(sock, &recvmssg, sizeof(int), 0);
    }
    return 0;
}

void waiting()
{
    bool cantStartGame = false;
    while (!cantStartGame)
    {
        if (recvmssg == 5)
        {
            cantStartGame = true;
        }
    }
    // Сцена игры
    display();
}

void start()
{
    struct sockaddr_in addr;
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8081);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
    
    printf("Connected\n");
    
    recv(sock, NULL, sizeof(int), 0);
    
    pthread_t pk;
    
    pthread_create(&pk, NULL, listen, NULL);
    
    void *ret;
    
    pthread_join(pk, &ret);
    waiting();
}



int main (int argc,char **argv)
{
    glutInit (&argc, argv);
    CreateGlutWindow();
    glutDisplayFunc (display);
    glutTimerFunc (80,timer,0);
    glutSpecialFunc (MyKeyboard);
    glutMouseFunc(MousePressed);
    start();
    glutMainLoop(); // Запуск механизма обработки событий
    
    return 0;
}


