/* AI.h
 * -------------------
 * Template header file for implementing an AI.
 */
#ifndef _AI_H_
#define _AI_H_


#include "Rules.h"
#include "AIBase.h"
#include <cmath>


using namespace Pool;

class AI : public Pool::AIBase
{
  public:
    AI(libconfig::Config &config, Stopwatch *stopwatch): AIBase(config,stopwatch) {};
    virtual bool forGame(Pool::GameType gt) {return (gt==Pool::GT_EIGHTBALL);}
  protected:
    virtual void breakShot();
    virtual void otherShot();
    virtual Pool::Decision decide(); // optional
    

    double dist(Point point1, Point point2) const
    {
      return sqrt(pow((point1.x-point2.x),2.0)+pow((point1.y-point2.y) ,2.0));
    }

    
    double angle(Point p1, Point p2) const
    {
      double raw = atan2(p2.y-p1.y,p2.x-p1.x);
      double twoPi = 2*M_PI;
      if(raw<0)
      {
        double frac = raw / twoPi;
        double residual = frac - ((int)frac);
        raw = (1+residual)*twoPi;
      }
      if(raw >= twoPi)
      {
        double frac = raw / twoPi;
        double residual = frac - ((int)frac);
        raw = residual*twoPi;
      }
      
      return raw;  // Return Radians.
      //return raw*180 / M_PI; Old usage, converted to degrees. 
   }   


   double shotAngle(double angle1, double angle2)
   {
     double diff = abs(angle1-angle2);
     if(diff>180.0)
     {
       return 360.0 - diff;
     }
     return diff;
   }


   /*
   double shotDifficulty(double d1, double d2, double angle)
   {
     return (d1+d2)*angle + 1;
   }
   */

   double shotDifficulty(Point P1, Point P2, Point P3)
   {
     double d1 = dist(P1,P2);
     double d2 = dist(P2,P3);
     double alpha = angle(P1,P2);
     double beta = angle(P2,P3);
     double totalAngle = shotAngle(alpha,beta);
     return (d1+d2)*totalAngle+1;
   }

   double shootingAngle(Point cueBall, Ball ball, Table::Pocket pocket)
   {
     Point pocketPos = gameState->tableState().getTable().getPocketCenter(pocket);
     Point ballPos = ball.getPos();
     double r = ball.getRadius();
     double beta = angle(pocketPos,ballPos);
     double ghostBall_x = ball.getPos().x + 2*r*cos(beta);
     double ghostBall_y = ball.getPos().y + 2*r*sin(beta);
     Point ghostBallPos = Point(ghostBall_x,ghostBall_y);
     return angle(cueBall,ghostBallPos)*180.0/M_PI;
   }//*/


   int numberOfBallsAvailable()
   {
    int temp=0;
    vector<Ball>::const_iterator start = gameState->tableState().getBegin();
    vector<Ball>::const_iterator end = gameState->tableState().getEnd();
    if((!gameState->isOpenTable()) && gameState->playingSolids()){
      //cerr << "Playing Solids." << "\n" << endl;
      std::advance (end, -8);
    }else if(!gameState->isOpenTable() && !gameState->playingSolids()){
      //cerr << "Playing Stripes." << "\n" << endl;
      std::advance(start,9);
    }

    for(vector<Ball>::const_iterator i = start; i != end; i++)
    {
      if(i->isInPlay() && i->getID() != Ball::CUE && i->getID() != Ball::EIGHT)
      { 
        cerr << i->getIDString() << endl;
        temp++;
      }
    }
    return temp;
   }

   void checkShotParams(){}


   /*  Need to change this to use numLineSphereIntersections. 
   bool isShotPossible(Point cueBallPos, Ball targetBall, Table::Pocket targetPocket)
   {
    ostringstream oss;
    ostream &os = oss;
    gameState->toStream(os);
    string gs_string = oss.str();
    GameState * simGameState = GameState::Factory(gs_string);
    Shot *shotObj;
    GameShot simShot;
    simShot.params.phi=shootingAngle(cueBallPos,targetBall,targetPocket);
    simShot.params.a=0.0;
    simShot.params.b=0.0;
    simShot.params.theta=5.0;
    simShot.params.v=4.5;

    ShotResult res = simGameState->executeShot(simShot,&shotObj);

    return (res==SR_OK);
   } */
   
   double dot(Vector v1, Vector v2) {
     return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
   }

   Vector norm(Vector v) const {
   double m = v.mag();
   if(m==0.0){cerr<<"Vector Magnitude must Be non-zero."<<endl;}
   return Vector(v.x/m,v.y/m,v.z/m);
   }
 
   int numLineSphereIntersections(Vector &p1, Vector &p2, Vector &p3, double rad, double& root1, double& root2) const   {
     root1 = -1;
     root2 = -1;
     Vector l = norm(p2-p1);
     Point cueBallPos(p1.x,p1.y);
     Point objectBallPos(p2.x,p2.y);
     //double phi = angle(cueBallPos,objectBallPos);
     Vector c;
     //if(phi>=0.0 && phi < 180.0) {
       c = (p1-p3);
     //}
     //else{
     //  c = (p3-p1);
     //}
     double det = pow(l.dot(c), 2.0) - c.dot(c) + pow(rad, 2.0);
     if ( det < 0.0 ) return 0;
     else if ( det > 0.0 ) {
       root1 = l.dot(c)-det;
       root2 = l.dot(c)+det;
       
       l.operator*=(root1);
       Vector t(p1-l);
       Point pi(t.x,t.y);
       cerr << "First point of intersection is: " << pi << endl;
       cerr << "Radius ~ : " << t.z << endl;
       return 2;
     }
     else {
       root1 = l.dot(c);
       cerr << "Root 1 is : " << root1 << endl;
       return 1;
     }
   }
   

   bool isPathBlocked(Ball startBall, Ball endBall, const GameState *gs) const {
      double root1;
      double root2; 
      double rad = gs->tableState().getBall(Ball::CUE).getRadius();
      Point startPoint = startBall.getPos();
      Point endPoint = endBall.getPos();
      double phi = angle(startPoint,endPoint);
      double a = phi + 90.0;
      double b = phi - 90.0;
      Vector p1a(startPoint.x+rad*cos(a),startPoint.y+rad*sin(a),rad);  // Need the points of each side of the ball. 
      Vector p1b(startPoint.x+rad*cos(b),startPoint.y+rad*sin(b),rad);
      Vector p2a(endPoint.x+rad*cos(a),endPoint.y+rad*sin(a),rad);
      Vector p2b(endPoint.x+rad*cos(b),endPoint.y+rad*sin(b),rad);
      vector<Ball>::const_iterator start = gs->tableState().getBegin();
      vector<Ball>::const_iterator end = gs->tableState().getEnd();
      int temp =0;
      for(vector<Ball>::const_iterator i = start; i != end; i++) 
      {
        Point otherBallPos = i->getPos();
        double d1 = dist(startBall.getPos(),endBall.getPos());
        double d2 = dist(otherBallPos,endBall.getPos());
        double d3 = dist(startBall.getPos(),otherBallPos);
      
        if( i->isInPlay() && i->getID() != startBall.getID() && i->getID() != endBall.getID() && (d1>d2) && (d1>d3)) {
          Vector p3(otherBallPos.x,otherBallPos.y,rad);
          //Check Line from First side of the ball to the target.
          if(numLineSphereIntersections(p1a,p2a,p3,rad,root1,root2) > 0){
             temp++;
             //cerr << "Cue Ball at: " << startBall.getPos() << endl;
             //cerr << "Object Ball at: " << endBall.getPos() << endl;
             //cerr << "Other Ball at: " << i->getPos() << endl;
           }
          else if(numLineSphereIntersections(p1b,p2b,p3,rad,root1,root2) > 0){
             temp++;
          }
        }             
      }
      if (temp==0) {return false;}
      else return true;
     }


   //bool isBallBlocked(){}
   //bool isPocketBlocked(){}




};

#endif
