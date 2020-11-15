#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cfloat>

#include "gluvi.h"
#include "fluidsim.h"
#include "openglutils.h"
#include "array2_utils.h"

using namespace std;

//Try changing the grid resolution
int grid_resolution = 100;
scalar timestep = 0.02;
scalar grid_width = 100.0;

//[modified]
//int grid_resolution = 50;
//scalar timestep = 0.005;
//scalar grid_width = 50.0;

FluidSim sim;

//Gluvi stuff
//-------------
Gluvi::PanZoom2D cam(-0.1, -0.35, 1.2);
double oldmousetime;
Vector2s oldmouse;
void display();
void mouse(int button, int state, int x, int y);
void drag(int x, int y);
void timer(int junk);


//Boundary definition - several circles in a circular domain.

Vector2s c0(50,50), c1(70,50), c2(30,35), c3(50,70), c4(40, 40);
Vector2s s0(10,5);
scalar rad0 = 40,  rad1 = 30,  rad2 = 10,   rad3 = 10;
Vector2s o0(0.0, 0.0), o1(50, 50);

//Main testing code
//-------------
int main(int argc, char **argv)
{
  //[modified]: outout file
    ofstream myfile;
    myfile.open("output.txt");
  //Setup viewer stuff
  Gluvi::init("Basic Fluid Solver with Static Variational Boundaries", &argc, argv);
  Gluvi::camera=&cam;
  Gluvi::userDisplayFunc=display;
  Gluvi::userMouseFunc=mouse;
  Gluvi::userDragFunc=drag;
  glClearColor(1,1,1,1);
  
  glutTimerFunc(1000, timer, 0);
    
    //Set up the simulation
    sim.initialize(o0, grid_width, grid_resolution, grid_resolution, 1.0);

    // 修改了boundary type
//  sim.root_boundary = new FluidSim::Boundary(c0, Vector2s(rad1, 0.0), FluidSim::BT_CIRCLE, true);
    sim.root_boundary = new FluidSim::Boundary(c0, Vector2s(30, 30), FluidSim::BT_BOX, true);
    
//    sim.root_boundary = new FluidSim::Boundary(c0, Vector2s(30, 30), FluidSim::BT_BOX, true);
    
  
  sim.root_sources = NULL;

  sim.update_boundary();
  sim.init_random_particles();

    
  Gluvi::run();
    
    myfile.close();
  
  delete sim.root_boundary;
  
  return 0;
}


void display(void)
{
  sim.render();
}

void mouse(int button, int state, int x, int y)
{
  Vector2s newmouse;
  cam.transform_mouse(x, y, newmouse.data());
  //double newmousetime=get_time_in_seconds();
  
  oldmouse=newmouse;
  //oldmousetime=newmousetime;
}

void drag(int x, int y)
{
  Vector2s newmouse;
  cam.transform_mouse(x, y, newmouse.data());
  //double newmousetime=get_time_in_seconds();
  
  oldmouse=newmouse;
  //oldmousetime=newmousetime;
}

void timer(int junk)
{
  sim.advance(timestep);
    sim.writein();
  
  glutPostRedisplay();
  glutTimerFunc(30, timer, 0);
  
}





