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
    double dist(Point point1, Point point2){return sqrt(pow((point1.x-point2.x),2.0)+pow((point1.y-point2.y) ,2.0));}

    double angle(Point p1, Point p2)
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

   double shotDifficulty(double d1, double d2, double angle)
   {
     return (d1+d2)*angle + 1;
   }


};

#endif
