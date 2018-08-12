/** Simple example of using the FastFiz library without writing an AI
 */
#include "FastFiz.h"
#include "Rules.h"
#include "LogFile.h"
#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace Pool;

/*
int numLineSphereIntersections(Vector &p1, Vector &p2, Vector &p3, double rad, double& root1, double& root2) {
     root1 = -1;
     root2 = -1;
     Vector l = (p2 - p1).norm();
     Vector c(p3 - p1);
     double det = pow(l.dot(c), 2.0) - c.dot(c) + pow(rad, 2.0);
     if ( Utils::fless(det, 0.0) ) return 0;
     else if ( Utils::fgreater(det, 0.0) ) {
       root1 = l.dot(c)-det;
       root2 = l.dot(c)+det;
       return 2;
     }
     else {
       root1 = l.dot(c);
       return 1;
     }
   }
*/
int main(int argc, char * const argv[]) {

    

  // Output version info
  cerr << getFastFizVersion() << endl;
  cerr << getRulesVersion() << endl;
  
  // Generate Gaussian noise using tournament values.
  GaussianNoise gn;
  
  // Create a new eight ball state with no time limits.
  EightBallState myState(0,0); 
  GameState& st = myState;
  cerr << "Initial racked Game State is: " << st << endl;
 
 
  Vector p1(0.0,0.0,1.0);
  Vector p2(0.0,2.0,1.0);
  Vector p3(0.0,1.0,1.0);


  double root1 = -1;
  double root2 = -1;
  double rad = 1.0;
  Vector t = p2-p1;
  double m = t.mag();
  Vector l(t.x/m,t.y/m,t.z/m);  
  Vector c(p3 - p1);
  double det = pow(l.dot(c), 2.0) - c.dot(c) + pow(rad, 2.0);
  if (det < 0.0) {cerr << "No Path Intersections" << endl;}
  else if (det > 0.0) {
    root1 = l.dot(c)-det;
    root2 = l.dot(c)+det;
    cerr << "Two Path Intersections at " << root1 << " and "<< root2 << endl;
  } 
  else {
    root1 = l.dot(c);
    cerr << "One Path Intersection at " << root1 << endl;
  } 


  //double rad = 1.0;
  //double root1,root2; 
  //int ans = numLineSphereIntersections(p1,p2,p3,rad,root1,root2);

  //cerr << "numLineSphereIntersections = " << ans << endl; 

  // Start a log file
  LogWriter lw("example.log",GT_EIGHTBALL,&gn,"Example Agent");
  
  // A typical break shot
  GameShot myShot = {ShotParams(0.0,0.0,25.0,270.0,5.0), // Shot parameters
    0.48,1.67705, // Cue position
    Ball::UNKNOWN_ID,Table::UNKNOWN_POCKET, // No need to call ball/pocket
    DEC_NO_DECISION, // No decision to make.
    0.0}; // No time spent.
  
  // Noise is added, shot executed, and log written.
  ShotResult res = lw.LogAndExecute(st,myShot);
  
  cerr << "Shot result is: " << res << endl;
  
  if ((res == SR_OK) /*||(res == SR_OK_LOST_TURN)*/ ) { // If you kept your turn
    lw.write(st);
    srand ( time(NULL) );
    GameShot myNextShot = {
      ShotParams(0.0,0.0,25.0,(360.0*rand())/RAND_MAX,5.0), // random direction
      0.0, 0.0, // cue position not needed
      Ball::THREE,Table::SW, // call a ball and a pocket
      DEC_NO_DECISION, // No decision to make.
      0.0}; // No time spent.
    lw.write(myNextShot); // write shot to log file, even if not successful.
    res = st.executeShot(myNextShot); // Execute the shot on the table (no noise added).
    cerr << "Second shot result is " << res << endl;
  }
  
  cerr << "New Game State is: " << st << endl;
  
  lw.write(st);
  return 0;
}
