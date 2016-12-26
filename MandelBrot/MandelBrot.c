// MandelBrot.c
// Shashank Ojha

//MPIcc
//
//

#include <stdio.h>
//#include <GL/glut.h>
//#include "mpi.h"
#include <math.h>
#include <unistd.h>
#include <time.h>
// 
#define M 1200 
#define N 900 
// 
int count =  0 ; 
int ascii = 48 ; 
int topLeftx = -2;
int topLefty = 1.5;
int bottomRightx = 2;
int bottomRighty = -1.5;
double pxorigin = 0.0;
double pyorigin = 0.0;
double xwidth = 4.0;
double ywidth = 3.0;
int trials = 100;
int        rank   ;
int        size   ;
MPI_Status status ;
int        tag    ;


void genBot(int xp, int yp, int trials)
{
   double xr = (xp * xwidth/1200.0) - (xwidth/2.0) + pxorigin;
   double yr = -1*((yp * ywidth/900.0) - (ywidth/2.0)) + pyorigin;
   double a, b;
   a= 0.0;
   b= 0.0;

   int magHigh = 0;
   int i;
   i = 0;
   while( i <= trials)
   {
      i++;
      double anew = a*a - b*b + xr;
      double bnew = 2*a*b + yr;
      a = anew;
      b = bnew;
      if(sqrt(a*a + b*b) > 2)
      {
         magHigh = 1;
         break;
      }
   }
         if(magHigh)
            {
               glColor3f( 0.1*log(i) , 0.2*log(i) , 0.3 * log(i) ); // brown
            }
         else
            {
               glColor3f( 0, 0, 0 ) ; // black
            }
         glBegin(GL_POINTS);
         glVertex2f(xp,yp);
         glEnd();
      
   
}

void displayfunc(void)
{
   double xp, yp;
   xp = 0;
   yp = 0;
   glClear(GL_COLOR_BUFFER_BIT);
   for( xp = 0 ; xp < M ; xp++ )
   {
      for( yp = 0 ; yp < N ; yp++ )
      { 
         genBot(xp,yp,trials);
         glEnd();
      }
   }
   glutSwapBuffers() ;
}

void mousefunc(int button,int state,int xscr,int yscr)
{
   if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
   {
      printf("Left mouse clicked.\n");
      pxorigin = pxorigin + (xscr * xwidth/1200.0) - (xwidth/2.0);
      pyorigin = pyorigin + -1*((yscr*ywidth/900.0) - (ywidth/2.0));
      xwidth /= 2.0;
      ywidth /= 2.0;
     // displayfunc();
      // int topLeftx = xscr;
      // int topLefty = yscr;
      printf("TopLeft: %d, %d\n", topLeftx, topLefty);
     // glutDisplayFunc(displayfunc);
      glutPostRedisplay();

   }

}
void motionfunc(int xscr,int yscr)
{
   printf("Motion ( %d , %d ).\n" , xscr , yscr ) ;
   // drawBox();
   // displayfunc();
   // glutDisplayFunc(displayfunc);
}
void keyfunc(unsigned char key,int xscr,int yscr)
{
   printf("Key %c pressed.\n" , key);
}
void specialfunc(int key,int xscr,int yscr)
{
   if( key == GLUT_KEY_UP )
   {
      printf("Up arrow pressed.\n");
      trials = trials+10;
      printf("trials: %d\n", trials);
      displayfunc();
      glutDisplayFunc(displayfunc);
   }
   if( key == GLUT_KEY_DOWN )
   {
      printf("Up arrow pressed.\n");
      trials = trials-10;
      printf("trials: %d\n", trials);
      displayfunc();
      glutDisplayFunc(displayfunc);
   }
}
void closefunc(void)
{
   printf("Window closed.\n");
}


/////////////////////////////////////////////////            MPI Code
int main( int argc , char* argv[] )
{


      glutInit(&argc,argv);
      glutInitDisplayMode(GLUT_SINGLE| GLUT_RGB);
      glutInitWindowSize(M,N);
      glutInitWindowPosition(100,50);
      glutCreateWindow("");
      glClearColor(1.0,1.0,1.0,0.0);
      glShadeModel(GL_SMOOTH);
      //
      glViewport(0,0,(GLsizei)M,(GLsizei)N); // reshape
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(0.0,1.0*M,1.0*N,0.0); // invert y-coords
      glMatrixMode(GL_MODELVIEW);
      //
      // glutIdleFunc(idlefunc);
      glutDisplayFunc(displayfunc);
      glutReshapeFunc(NULL);
      glutMouseFunc(mousefunc);
      glutMotionFunc(motionfunc);
      glutKeyboardFunc(keyfunc);
      glutSpecialFunc(specialfunc);
      glutWMCloseFunc(closefunc);
      //
      glutMainLoop();
   

   return 0;

}



// void genBot(int xp, int yp)
// {
//    for( xp = 0 ; xp < M ; xp++ )
//    {
//       for( yp = 0 ; yp < N ; yp++ )
//       { 
//          double xm = (xp/200.0)- 2; //cartesian
//          double ym = (yp/200.0)- 1.5;
//          int magOB = 0;

//          int trials;
//          double a = 0.0;
//          double b = 0.0;
//          for(trials = 0; trials < tmax; trials++ )
//          {
//             double anew = a*a - b*b +xm;
//             double bnew = 2*a*b +ym;
//             a = anew;
//             b = bnew;
//             if(sqrt(a*a + b*b) > 2)
//                 {
//                   magOB = 1;
//                   break;
//                 }  
//          }
//          if(magOB==1)
//             {
//                glColor3f( 0.1*log(trials) , 0.2*log(trials) , 0.3 * log(trials) ); // brown
//             }
//          else
//             {
//                glColor3f( 0, 0, 0 ) ; // black
//             }
//          glBegin(GL_POINTS);
//          glVertex2f(xp,yp);
//          glEnd();
//       }
//    }
// }

// void drawBox(void)
// {
//    int x ;
//    int y ;
//    glColor3f( 0, 0, 0 ) ; // black
//    for( x = topLeftx; x < bottomRightx; x++)
//    {
//       for ( y = topLefty; y < bottomRighty; y++)
//       {
//          double x = (x/200.0)- 2; //cartesian
//          double y = (y/200.0)- 1.5;
//          glBegin(GL_POINTS);
//          glVertex2f(x,y);
//          glEnd();
//       }
//    }
//    glutSwapBuffers() ;
// }

// void displayfunc(void)
// {
//    int xp , yp ; //pixels
//    glClear(GL_COLOR_BUFFER_BIT);
//    genBot(xp, yp);
//    glutSwapBuffers() ;
// }


   // if(button==GLUT_LEFT_BUTTON && state==GLUT_UP)
   // {      
   //    printf("unclicked.\n");
   //    int bottomRightx = xscr;
   //    int bottonRighty = yscr;
   //    printf("bottomRight: %d, %d\n", bottomRightx, bottonRighty);
   //    displayfunc();
   //    glutDisplayFunc(displayfunc);
   // }

   // else if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
   // {
   //    printf("Right mouse clicked.\n");
   // }