//
//  main.cpp
//  CohenSutherlandLineClipp
//
//  Created by 최준호 on 11. 8. 25..
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#if !defined(__APPLE__)
#include <OpenGL/OpenGL.h>
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include <iostream>

#define T_HEIGHT 400    
#define T_WIDTH  400

static int gwidth;
static int gheight;

typedef int OutCode;

static double xmin = -0.5;
static double xmax = 0.5;
static double ymin = -0.5;
static double ymax = 0.5;

const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

static double p0x, p0y, p1x, p1y;


void convertDeviceXY2OpenglXY(int x, int y, float * ox, float  * oy)
{
    int w = gwidth;
    int h = gheight;
    *ox = (float)((x - (float)w/2.0)*(float)(1.0/ (float)(w/2.0)));
    *oy = -(float)((y - (float)h/2.0)*(float)(1.0/ (float)(h/2.0)));
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINE_LOOP); // colored line-loop square
    glColor3f(1.0, 1.0, 1.0);
    
    //float ox, oy;
    //convertDeviceXY2OpenglXY(xmin, ymin, &ox, &oy);
    glVertex2f(xmin, ymin);
    
    //convertDeviceXY2OpenglXY(xmax, ymin, &ox, &oy);
    glVertex2f(xmax, ymin);
    
    //convertDeviceXY2OpenglXY(xmax, ymax, &ox, &oy);
    glVertex2f(xmax, ymax);
    
    //convertDeviceXY2OpenglXY(xmin, ymax, &ox, &oy);
    glVertex2f(xmin, ymax);
    
    glEnd();
    glFlush();
}




void handleResize(int w, int h)
{
    gwidth = w;
    gheight = h;
    glViewport(0, 0, w, h);
    
}




//cohen sutherland algorithm
OutCode ComputeOutCode(double x, double y)
{
    OutCode code;
    
    code = INSIDE;          // initialised as being inside of clip window
    
    if (x < xmin)           // to the left of clip window
        code |= LEFT;
    else if (x > xmax)      // to the right of clip window
        code |= RIGHT;
    if (y < ymin)           // below the clip window
        code |= BOTTOM;
    else if (y > ymax)      // above the clip window
        code |= TOP;
    
    return code;
}


void CohenSutherlandLineClipAndDraw(double x0, double y0, double x1, double y1)
{
    // compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
    OutCode outcode0 = ComputeOutCode(x0, y0);
    OutCode outcode1 = ComputeOutCode(x1, y1);
    bool accept = false;
    
    while (true) {
        if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
            accept = true;
            break;
        } else if (outcode0 & outcode1) { // Bitwise AND is not 0. Trivially reject and get out of loop
            break;
        } else {
            // failed both tests, so calculate the line segment to clip
            // from an outside point to an intersection with clip edge
            double x, y;
            
            // At least one endpoint is outside the clip rectangle; pick it.
            OutCode outcodeOut = outcode0? outcode0 : outcode1;
            
            // Now find the intersection point;
            // use formulas y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0)
            if (outcodeOut & TOP) {           // point is above the clip rectangle
                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                y = ymax;
            } else if (outcodeOut & BOTTOM) { // point is below the clip rectangle
                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                y = ymin;
            } else if (outcodeOut & RIGHT) {  // point is to the right of clip rectangle
                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                x = xmax;
            } else if (outcodeOut & LEFT) {   // point is to the left of clip rectangle
                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                x = xmin;
            }
            // Now we move outside point to intersection point to clip
            // and get ready for next pass.
            if (outcodeOut == outcode0) {
                x0 = x;
                y0 = y;
                outcode0 = ComputeOutCode(x0, y0);
            } else {
                x1 = x;
                y1 = y;
                outcode1 = ComputeOutCode(x1, y1);
            }
        }
    }
    if (accept) {
        // Following functions are left for implementation by user based on his platform(OpenGL/graphics.h etc.)
        //DrawRectangle(xmin, ymin, xmax, ymax);
        //LineSegment(x0, y0, x1, y1);
        
        //float ox,oy;
        
        glBegin(GL_LINE_LOOP);
        
        glColor3f(1.0, 0, 0);
        //convertDeviceXY2OpenglXY(x0, y0, &ox, &oy);
        glVertex2f(x0, y0);
        
        //convertDeviceXY2OpenglXY(x1, y1, &ox, &oy);
        glVertex2f(x1, y1);
        
        glEnd();
        glFlush();
    }
}


void processMouse(int button, int state, int x, int y)
{
    float ox;
    float oy;
    if(state == GLUT_DOWN)
    {
        if(button == GLUT_LEFT_BUTTON)
        {
            //Left Button code.
            glBegin(GL_LINE_LOOP);
            glColor3f(1.0, 1.0, 1.0);
            convertDeviceXY2OpenglXY(x, y, &ox, &oy);
            glVertex2f(ox, oy);
            
            p0x = (double)ox;
            p0y = (double)oy;
            
            
            
        }
        else if(button == GLUT_MIDDLE_BUTTON)
        {
            //middle button code
        }
        else if(button == GLUT_RIGHT_BUTTON)
        {
            //right button code
            convertDeviceXY2OpenglXY(x, y, &ox, &oy);
            glVertex2f(ox, oy);
            
            glEnd();
            glFlush();
            
            p1x = (double)ox;
            p1y = (double)oy;
            
            CohenSutherlandLineClipAndDraw(p0x, p0y, p1x, p1y);
            
        }
    }
    else
    {
        //alternate code
    }
}



int main (int argc, char * argv[]) //argv const 제거 
{
    gwidth = T_WIDTH;
    gheight = T_HEIGHT;
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(gwidth, gheight);
    glutInitWindowPosition(0,0);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(handleResize);
    glutMouseFunc(processMouse);
    glutMainLoop();
    return 0;
}


