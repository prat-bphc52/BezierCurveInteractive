#include<stdio.h>
#include<iostream>
#include"line.h"
#include"circle.h"
#include"helper.h"
#include <GL/glut.h>
#include"bezier1.h"

int pts=0;
double coordX[100];
double coordY[100];

int calcPts[2];//to store the intermediate result of beizerPt function (a single calculated point - a pair of values x and y)

int plotPts=0;
int plotPtsX[1500];
int plotPtsY[1500];

bool isComputing = false;

bool isDragging;
int dragPoint=-1;

void Bezier::bezierPt(int count, double *arrX, double *arrY, double param){
    if(count==1){
        calcPts[0]=(int)arrX[0];
        calcPts[1]=(int)arrY[0];
    }
    else{
        double aX[count-1];
        double aY[count-1];
        for(int i=0;i<=count-2;i++){
            aX[i] = (1-param)*arrX[i]+param*arrX[i+1];
            aY[i] = (1-param)*arrY[i]+param*arrY[i+1];
        }
        bezierPt(count-1, aX, aY, param);
    }
}

void Bezier::computeBezierCurve(){
    // std::cout<<"Started computing Beizer Curve"<<std::endl;
    plotPts = 0;
    if(pts<2)
        return;
    isComputing =  true;
    for(double i=0; i<=1;i+=0.001){
        bezierPt(pts, coordX, coordY, i);
        if(plotPts==0 || plotPtsX[plotPts]!=*calcPts || plotPtsY[plotPts]!=*(calcPts+1)){
            plotPtsX[plotPts] = *calcPts;
            plotPtsY[plotPts++] = *(calcPts+1);
        }
    }
    // std::cout<<"Finished computing Beizer Curve"<<std::endl;
    isComputing =  false; 
}

void Bezier::addNewPoint(int x, int y){
    for(int i=0;i<pts;i++){
        if(Helper::distanceBetweenPts(coordX[i],coordY[i],x,y)<15){
            printf("A point is already present in the vicinity\n");
            return;
        }
    }
    coordX[pts]= x;
    coordY[pts++]= y;
    computeBezierCurve();
    glutPostRedisplay();
}

void Bezier::removePoint(int x, int y){
    for(int i=0;i<pts;i++){
        if(Helper::distanceBetweenPts(coordX[i],coordY[i],x,y)<10){
            for(int j=i;j<pts-1;j++){
                coordX[j]=coordX[j+1];
                coordY[j]=coordY[j+1];
            }
            pts--;
            computeBezierCurve();
            glutPostRedisplay();
            return;
        }
    }
    printf("No point selected\n");
}

void mouseClickCallback(int button, int state, int x, int y){
    y = Helper::WINDOW_HEIGHT -y;
    if(button == GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        for(int i=0;i<pts;i++){
            if(Helper::distanceBetweenPts(coordX[i],coordY[i],x,y)<10){
                dragPoint = i;
                return;
            }
        }
    } else if(button == GLUT_LEFT_BUTTON && state==GLUT_UP) {
        if(!isDragging){
            Bezier bezier;
            bezier.addNewPoint(x, y);
        }
        isDragging = false;
        dragPoint = -1;
    } else if(button == GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
        Bezier bezier;
        bezier.removePoint(x,y);
    }
}

void mouseMoveCallback(int x, int y){
    y = Helper::WINDOW_HEIGHT -y;
    isDragging = true;
    if(dragPoint==-1)
        return;
    coordX[dragPoint]=x;
    coordY[dragPoint]=y;
    Bezier bezier;
    bezier.computeBezierCurve();
    glutPostRedisplay();
}

void draw(){
    Helper::clearScreen();
    Circle circle;
    Line line;
    for(int i=0;i<pts;i++){
        glColor3f(1.0,1.0,1.0); 
        circle.drawCircle(coordX[i],coordY[i], 10);
        glColor3f(1.0, 0.0, 0.0);
        line.drawLine(coordX[i]-15, coordY[i], coordX[i]+15, coordY[i]);
        line.drawLine(coordX[i], coordY[i]-15, coordX[i], coordY[i]+15);
    }

    if(!isComputing){
        glBegin(GL_POINTS);
        glColor3f(0.0, 1.0, 0.0);
        for(int i=0;i<plotPts;i++)
            Helper::plotPoint(plotPtsX[i], plotPtsY[i]);
        glEnd();
    }
    glFlush();
}

void Bezier::initBezierCurve(int *argc, char **argv){
    Helper::createWindow(argc, argv);
    glutDisplayFunc(draw);
    glutMouseFunc(mouseClickCallback);
    glutMotionFunc(mouseMoveCallback);
    glutMainLoop();
}
