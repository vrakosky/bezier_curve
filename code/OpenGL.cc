/*********************************************************/
/*                    openGL.cc                          */
/*********************************************************/
/*                                                       */
/*********************************************************/ 

/* inclusion des fichiers Glut-OpenGL, de Mathématiques et de C++ */
#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <jerror.h>
#define Pi 3.141592654

/*edit candappane_vincent 1012017*/
using namespace std;

double Scal=36;

double trX=0.0,trY=0.0,dist=0.;//,trZ=0.0
char presse;
int anglex,angley,x,y,xold,yold;

/* Prototype des fonctions */
void affichage();//  procedure a modifier en fonction de la scene
void clavier(unsigned char touche,int x,int y); 
void reshape(int x,int y);
void idle();
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);
void openGL(double x, double y, double r0,double g0, double b0, double size);
void trace_segment(double x0, double y0,double x1, double y1, double red, double green, double blue, double size);
void trace_courbe(double red, double green, double blue, double size);
//-************************************************************
//
//  Procedure avec mise en file des sommets des primitives
//
//-***********************************************************
void init();

int main(int argc,char **argv)
{
  /* initialisation de glut et creation de la fenetre */
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(1000,1000);
  glutCreateWindow("RQBC et conique.");
  /* Initialisation d'OpenGL */
  glClearColor(1.0,1.0,1.0,0.0);
  glColor3f(0.0,0.0,0.0);
  glPointSize(2.0);
  glEnable(GL_DEPTH_TEST);


   glColor3f(0.0,0.0,0.0);
   //glEdgeFlag(GL_FALSE);
   glEdgeFlag(GL_TRUE);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
 // glEnable(GL_LIGHTING);
  glDisable(GL_LIGHTING);


  /* enregistrement des fonctions de rappel */
  init();
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mousemotion);


  /* Entree dans la boucle principale glut */
  glutMainLoop();
  return 0;
}

void clavier(unsigned char touche,int x,int y)
{
  switch (touche)
    {
    case 'q' : /*la touche 'q' permet de quitter le programme */
        exit(0);
    case '+' :
      dist+=0.5;
      Scal=Scal+0.5;
      glutPostRedisplay();
      break;
    case '-' :
      dist-=0.5;
      Scal=Scal-0.5;
      glutPostRedisplay();
      break;
    case '6' : trX-=0.25;  glutPostRedisplay();  break;
    case '4' : trX+=0.25;  glutPostRedisplay();  break;
    case '8' : trY+=0.25;  glutPostRedisplay();  break;
    case '2' : trY-=0.25;  glutPostRedisplay();  break;
    }
}



void reshape(int x,int y)
{
  glViewport(0, 0, (GLsizei) x, (GLsizei) y);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //taille de la scene
  double  Ortho=-150;
  glOrtho(-Ortho,Ortho,-Ortho,Ortho,-Ortho,Ortho);// fenetre
  glMatrixMode(GL_MODELVIEW);
  glViewport(0,0,x,y);
}

void mouse(int button, int state,int x,int y)
{
  /* si on appuie sur le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
  {
    presse = 1; /* le booleen presse passe a 1 (vrai) */
    xold = x; /* on sauvegarde la position de la souris */
    yold=y;
  }
  /* si on relache le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
    presse=0; /* le booleen presse passe a 0 (faux) */
}

void mousemotion(int x,int y)
  {
    if (presse) /* si le bouton gauche est presse */
    {
      /* on modifie les angles de rotation de l'objet
   en fonction de la position actuelle de la souris et de la derniere
   position sauvegardee */
      anglex=anglex+(x-xold); 
      angley=angley+(y-yold);
      glutPostRedisplay(); /* on demande un rafraichissement de l'affichage */
    }
    
    xold=x; /* sauvegarde des valeurs courante de le position de la souris */
    yold=y;
  }



/****************************************************************
 **                                                            **
 **                    Affichage de  la scene                  **
 **                                                            **
 ****************************************************************/

void openGL(double x, double y, double r0,double g0, double b0, double size)
{
  glColor3f(r0,g0,b0);  //initialisation de la couleur
  glPointSize(size);  // initialisation de la taille
  glBegin(GL_POINTS); // on trace un point
  glVertex2f(x,y);  // coordonnees du point
  glEnd();  // fin de glBegin
}

void trace_segment(double x0, double y0,double x1, double y1, double red, double green, double blue, double size)
{
  glColor3f(red,green,blue);//initialisation de la couleur
  glLineWidth(size); // initialisation de la taille
  glBegin(GL_LINES); // on trace un segment
  glVertex2f(x0,y0); // coordonnees du premier point
  glVertex2f(x1,y1); // coordonnees du dernier point 
  glEnd(); // fin de glBegin
}

//fonction ou les objets sont a definir
void init()
{ 
  int i;
	//CAS PARTICULIER :
  //double xA=1, yA=1, xB=1, yB=2, xC=2, yC=2, xD=2, yD=1;  --> PARALLELLE INVERSER
  //double xA=1, yA=1, xB=1, yB=2, xC=2, yC=1, xD=2, yD=2;  --> PARALLELLE INVERSER
  //double xA=1, yA=1, xB=2, yB=2, xC=1, yC=2, xD=2, yD=1; --> PERPENDICULAIRE 1
  //double xA=1, yA=1, xB=2, yB=2, xC=2, yC=1, xD=1, yD=2; --> PERPENDICULAIRE 2
  //double xA=1, yA=1, xB=1, yB=2, xC=3, yC=1, xD=1, yD=3; --> 1 SEGMENT AB vertical 

  //double xA=1, yA=1, xB=2, yB=1, xC=3, yC=1, xD=1, yD=3; --> 1 SEGMENT AB horizontal
  //double xA=1, yA=1, xB=2, yB=1, xC=3, yC=1, xD=4, yD=1; --> AB & CD horizontaux

	double xA, yA, xB, yB, xC, yC, xD, yD;
  cout<<"Entrer les coordonnees des points 'A,B,C,D' des deux segments AB et CD"<<endl;
  cout<<"Entrer A(x,y):"<<endl;
  cin>>xA;
  cin>>yA;
  cout<<"Entrer B(x,y):"<<endl;
  cin>>xB;
  cin>>yB;
  cout<<"Entrer C(x,y):"<<endl;
  cin>>xC;
  cin>>yC;
  cout<<"Entrer D(x,y):"<<endl;
  cin>>xD;
  cin>>yD;


  double xM1=xB;
  double yM1=yB;
  double xM2=(6*xB-xA)/5;
  double yM2=(6*yB-yA)/5;
  double xM3=(7*xB-2*xA)/5;
  double yM3=(7*yB-2*yA)/5;
  double xM4=(7*xC-2*xD)/5;
  double yM4=(7*yC-2*yD)/5;
  double xM5=(6*xC-xD)/5;
  double yM5=(6*yC-yD)/5;
  double xM6=xC;
  double yM6=yC;

  
  double t,xt,yt;
  for (t=0;t<=1;t+=0.01){
    xt=pow(1-t,5)*xM1+5*t*pow(1-t,4)*xM2+10*t*t*pow(1-t,3)*xM3+10*pow(t,3)*pow(1-t,2)*xM4+5*pow(t,4)*(1-t)*xM5+pow(t,5)*xM6;
    yt=pow(1-t,5)*yM1+5*t*pow(1-t,4)*yM2+10*t*t*pow(1-t,3)*yM3+10*pow(t,3)*pow(1-t,2)*yM4+5*pow(t,4)*(1-t)*yM5+pow(t,5)*yM6;
  }

  GLdouble tab_point_control[6][2] = {
  {xM1, yM1}, {xM2, yM2}, 
  {xM3, yM3}, {xM4, yM4},
  {xM5, yM5}, {xM6, yM6}};

  double xO=0.,yO=0.,xI=1.,yI=0.,xJ=0.,yJ=1.;
  //création des points O,I et J
  glNewList(1,GL_COMPILE_AND_EXECUTE); //liste numero 1
    openGL(xI,yI,1.,0.,0.,10.); //I
    openGL(xJ,yJ,0.,0.5,0.,10.); //J
    openGL(xO,yO,0.,0.,1.,15.);//O
  glEndList();
  //création des segments [OI] et [OJ]
  glNewList(2,GL_COMPILE_AND_EXECUTE);  //liste numero 2
    trace_segment(xO,yO,xI,yI,1.0,0.0,1.0,2.0); // on trace [OI]
    trace_segment(xO,yO,xJ,yJ,1.0,0.50,0.0,2.0);// on trace [OJ]
  glEndList();
  glNewList(3,GL_COMPILE_AND_EXECUTE); //liste numero 3
    openGL(xA,yA,0.,0.,1.,5.); //A
    openGL(xB,yB,1.,0.,0.,5.); //B en rouge
    openGL(xC,yC,1.,0.,0.,5.);//C en rouge
    openGL(xD,yD,0.,0.5,0.,5.);//D
  glEndList();
  //création des deux segments [AB] et [CD]
  glNewList(4,GL_COMPILE_AND_EXECUTE);  //liste numero 4
    trace_segment(xA,yA,xB,yB,0.0,0.0,1.0,2.0); //on trace [AB] en bleu
    trace_segment(xC,yC,xD,yD,0.0,0.5,0.0,2.0); //on trace [CD] en vert
  glEndList();
  //création des points de controle
  glNewList(5,GL_COMPILE_AND_EXECUTE);  //liste numero 5
    glPointSize(5.0);
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_POINTS);
      for (i = 0; i < 6; i++) 
         glVertex2f(tab_point_control[i][0], tab_point_control[i][1]);
    glEnd();
  glEndList();
  //création de la courbe de façon G2
  glNewList(6,GL_COMPILE_AND_EXECUTE); //liste numero 6
      //trace_courbe(1.0,0.0,0.0,2.0);
      for (t=0;t<=1;t+=0.0001){
        xt=pow(1-t,5)*xM1+5*t*pow(1-t,4)*xM2+10*t*t*pow(1-t,3)*xM3+10*pow(t,3)*pow(1-t,2)*xM4+5*pow(t,4)*(1-t)*xM5+pow(t,5)*xM6;
        yt=pow(1-t,5)*yM1+5*t*pow(1-t,4)*yM2+10*t*t*pow(1-t,3)*yM3+10*pow(t,3)*pow(1-t,2)*yM4+5*pow(t,4)*(1-t)*yM5+pow(t,5)*yM6;
        openGL(xt,yt,1.,0.,0.,2.); 
      }
  glEndList();


  cout<<"\n Voila, c'est fini, la courbe a été tracée"<<endl;

}   
   
// fonction permettant d'afficher les objets en utilisant des listes   
void affichage() 
{
  /* effacement de l'image avec la couleur de fond */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glTranslatef(0.0,0.0,dist);
 // En 2D    
  glRotatef(-anglex+angley,0.0,0.0,1.0);
  glScalef(Scal,Scal,Scal); // diminution de la vue de la scene
  glRotatef(180,0.0,1.0,0.0);
  glRotatef(180,1.0,0.0,0.0);
  glTranslatef(-trX,trY,0.);
      glCallList(1); // appel de la liste numero 1
      glCallList(2); // appel de la liste numero 2
      glCallList(3); // appel de la liste numero 3
      glCallList(4); // appel de la liste numero 4
      glCallList(5); // appel de la liste numero 3
      glCallList(6); // appel de la liste numero 4
 glFlush(); 
  // On echange les buffers
  glutSwapBuffers();
}
