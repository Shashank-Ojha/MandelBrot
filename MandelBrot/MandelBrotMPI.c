// MandelBrot.c
// Shashank Ojha

//MPIcc
//
//
//make clean
//make
//mpirun
//mpirun -np 4 -machinefile hosts.txt a.out
//mpirun -np 4 a.out
//mpirun -np 4 a.out

#include <stdio.h>
#include <GL/glut.h>
#include "mpi.h"
#include <math.h>
#include <unistd.h>
#include <time.h>
// 
#define M 1200 
#define N 900 
// 
int count =  0 ; 
double pxorigin = 0.0;
double pyorigin = 0.0;
double xwidth = 4.0;
double ywidth = 3.0;
int trials = 100;


int        rank   ;
int        size   ;
MPI_Status status ;
int        tag = 0   ;
int w ;

int xp = 0;
double yp  = 0;


void genBot(int trials)
{
    int data_i[N];
    int data_magHigh[N];
    int i = 0;
    for( i = 0 ; i < N ; i++ )
      { 

            double xr = (1.0*xp * xwidth/1200.0) - (xwidth/2.0) + pxorigin;
            double yr = -1*((i * ywidth/900.0) - (ywidth/2.0)) + pyorigin;
            double a, b;
            a= 0.0;
            b= 0.0;

            int magHigh = 0;
            int j;
            j = 0;
            while( j <= trials)
            {
               j++;
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
            data_i[i] = j;
            data_magHigh[i] = magHigh;

      }

      MPI_Send( data_i, N , MPI_INT , 0 , tag , MPI_COMM_WORLD ) ;
      MPI_Send( data_magHigh, N , MPI_INT , 0 , tag , MPI_COMM_WORLD ) ;
       
}

void displayfunc(void)
{
   w = 0;
   int workers[size]; //index is worker and value is column
   workers[0] = 0;
   int col = 0;
   glClear(GL_COLOR_BUFFER_BIT);
   for( w = 1 ; w < size ; w++ ) // get workers started
   {
      MPI_Send(&col, 1, MPI_INT, w , tag , MPI_COMM_WORLD) ;
      MPI_Send(&pxorigin, 1, MPI_DOUBLE, w , tag , MPI_COMM_WORLD) ;
      MPI_Send(&pyorigin, 1, MPI_DOUBLE, w , tag , MPI_COMM_WORLD) ;
      MPI_Send(&xwidth, 1, MPI_DOUBLE, w , tag , MPI_COMM_WORLD) ;
      MPI_Send(&ywidth, 1, MPI_DOUBLE, w , tag , MPI_COMM_WORLD) ;
      MPI_Send(&trials, 1, MPI_INT, w , tag , MPI_COMM_WORLD) ;
      
      //associate which worker is doing which column
      workers[w] = col;
      col++;
   }

   while(col < M) // go through all the cols
   {
      int data_i[N];
      int data_magHigh[N];
      MPI_Recv(data_i, N, MPI_INT, MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status) ;
      w = status.MPI_SOURCE ; // to get which worker is giving you the information
      MPI_Recv(data_magHigh, N, MPI_INT , w , tag , MPI_COMM_WORLD , &status) ;
      int currCol = workers[w];
      int currRow;
      for(currRow = 0; currRow < N ; currRow++)
      {
         int i = data_i[currRow];
         int magHigh = data_magHigh[currRow];
         if(magHigh)
            {
               glColor3f( 0.1*log(i) , 0.2*log(i) , 0.3 * log(i) ); // brown
            }
         else
            {
                glColor3f( 0, 0, 0 ) ; // black
               //glColor3f( 0.3*log(i) , 0.4*log(i) , 0.8 * log(i) );
            }
         glBegin(GL_POINTS);
         glVertex2f(currCol,currRow);
         glEnd();
        // printf("%d-%d", currRow, currCol);
      
      }

      MPI_Send(&col, 1, MPI_INT, w , tag , MPI_COMM_WORLD) ;
      MPI_Send(&pxorigin, 1, MPI_DOUBLE, w , tag , MPI_COMM_WORLD) ;
      MPI_Send(&pyorigin, 1, MPI_DOUBLE, w , tag , MPI_COMM_WORLD) ;
      MPI_Send(&xwidth, 1, MPI_DOUBLE, w , tag , MPI_COMM_WORLD) ;
      MPI_Send(&ywidth, 1, MPI_DOUBLE, w , tag , MPI_COMM_WORLD) ;
      MPI_Send(&trials, 1, MPI_INT, w , tag , MPI_COMM_WORLD) ;
      workers[w] = col;
      col++;
   }
               //printf("%d", col);
int j;
   for( j = 1 ; j < size ; j++ ) // go through all the cols
   {
      int data_i[N];
      int data_magHigh[N];
      MPI_Recv(data_i, N, MPI_INT, MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status) ;
      w = status.MPI_SOURCE ; // to get which worker is giving you the information
      MPI_Recv(data_magHigh, N, MPI_INT , w , tag , MPI_COMM_WORLD , &status) ;
      int currCol = workers[w];
      int currRow;
      for(currRow = 0; currRow < N ; currRow++)
      {
         int i = data_i[currRow];
         int magHigh = data_magHigh[currRow];
         if(magHigh)
            {
               glColor3f( 0.1*log(i) , 0.2*log(i) , 0.3 * log(i) ); // brown
            }
         else
            {
               glColor3f( 0, 0, 0 ) ; // black
            }
         glBegin(GL_POINTS);
         glVertex2f(currCol,currRow);
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
      glutPostRedisplay();

   }

}
void motionfunc(int xscr,int yscr)
{
   printf("Motion ( %d , %d ).\n" , xscr , yscr ) ;
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
      glutPostRedisplay();
   }
   if( key == GLUT_KEY_DOWN )
   {
      printf("Up arrow pressed.\n");
      trials = trials-10;
      printf("trials: %d\n", trials);
      glutPostRedisplay();
   }
}
void closefunc(void)
{
   printf("Window closed.\n");
}


/////////////////////////////////////////////////            MPI Code
int main( int argc , char* argv[] )
{
   MPI_Init(      &argc          , &argv ) ;
   MPI_Comm_size( MPI_COMM_WORLD , &size ) ; // same
   MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ; // different

   if(rank == 0) // Manager
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
   
   }
   else
   {
    while(1)
      {
      
      // int xp = 0;
      MPI_Recv(&xp, 1, MPI_INT, 0 , tag , MPI_COMM_WORLD , &status) ;
      MPI_Recv(&pxorigin, 1, MPI_DOUBLE, 0 , tag , MPI_COMM_WORLD , &status) ;
      MPI_Recv(&pyorigin, 1, MPI_DOUBLE, 0 , tag , MPI_COMM_WORLD , &status) ;
      MPI_Recv(&xwidth, 1, MPI_DOUBLE, 0 , tag , MPI_COMM_WORLD , &status) ;
      MPI_Recv(&ywidth, 1, MPI_DOUBLE, 0 , tag , MPI_COMM_WORLD , &status) ;
      MPI_Recv(&trials, 1, MPI_INT, 0 , tag , MPI_COMM_WORLD , &status) ;
      genBot(trials);
      //glEnd();

      }
   }
   MPI_Finalize();
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