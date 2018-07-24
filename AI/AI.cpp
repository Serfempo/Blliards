/* AI.h
 * -------------------
 * Template c++ file for implementing an AI.
 */

#include "AI.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <cmath>

vector<Table::Pocket> pockets;

//double shotDifficulties
vector<double> shotDifficulties;
Table::Pocket easiestPocket;
Ball easiestBall;


void AI::breakShot()
{ 
 shot.params.a=0.0;
  shot.params.b=8.0;
  shot.params.theta = 5.0;  
  shot.params.phi = 275.0;
  shot.params.v=4.5;
  shot.cue_x=0.48;
  shot.cue_y=1.67705;
}


void AI::otherShot()
{
 // cerr << "Entering otherShot Method" << "\n" << endl;
   
  double minDifficulty = 10000;

  if(pockets.size() == 0)
  {
    pockets.push_back(Table::SW);
    pockets.push_back(Table::W);
    pockets.push_back(Table::NW);
    pockets.push_back(Table::NE);
    pockets.push_back(Table::E);
    pockets.push_back(Table::SE); 
  } 
    TurnType tt = gameState->getTurnType();    
    Point cueBallPos;

    cerr << "Turn Type: " << tt << endl;
  
    if(tt == (TT_BALL_IN_HAND || TT_BEHIND_LINE))
    {
      cueBallPos = Point(0.48,1.67705); 
    }
    else
    {
      cueBallPos = gameState->tableState().getBall(Ball::CUE).getPos();
    }


    //cerr << "Is Playing Solids: " << gameState->playingSolids() << endl;
    //cerr << "Is Open Table: " << gameState->isOpenTable() << "\n" << endl;


    vector<Ball>::const_iterator start =  gameState->tableState().getBegin();
    vector<Ball>::const_iterator end = gameState->tableState().getEnd();
      
    if((!gameState->isOpenTable()) && gameState->playingSolids())
    {
      cerr << "Playing Solids." << "\n" << endl;
      std::advance (end, -8);
    }
    else if(!gameState->isOpenTable() && !gameState->playingSolids())
    {
      cerr << "Playing Stripes." << "\n" << endl; 
      std::advance(start,9);
    }
    //else
    //{
    //  includeEightBall = 0;
    //}
    
    int numberOfBallsInPlay = 0;
    
    for(vector<Ball>::const_iterator i = start; i != end; i++)
    {
      Point objectBallPos = i->getPos();
      for(vector<Table::Pocket>::const_iterator j = pockets.begin(); j != pockets.end(); j++)
      {   

          //cerr << "Is Open Table: " << gameState->isOpenTable() <<endl;
          //cerr << "Is Playing Solids: " << gameState->playingSolids() << "\n" << endl;

          if(i->isInPlay() && i->getID() != Ball::CUE)
          {
            numberOfBallsInPlay ++;
            cerr << "Considering Ball: " << i->getIDString() << endl;

            Point pocketPos = gameState->tableState().getTable().getPocketCenter(*j);
            double d1 = dist(cueBallPos,objectBallPos);
            double d2 = dist(objectBallPos,pocketPos);
            double alpha = angle(cueBallPos,objectBallPos);
            double beta = angle(objectBallPos,pocketPos);
            double totalAngle = shotAngle(alpha,beta);
            double difficulty = shotDifficulty(d1,d2,totalAngle);
            shotDifficulties.push_back(difficulty);
         
            /*
            cerr << "White Ball at position: " << cueBallPos << endl;
	    cerr << "Ball: " << i->getIDString() << " at position: " << objectBallPos << endl; 
            cerr << "Pocket: " << gameState->tableState().getTable().pocketName(*j) << " at position: " << pocketPos << endl; 
            cerr << "White to Ball Distance: " << d1 << endl;
            cerr << "Ball to Pocket distance: " << d2 << endl;
            cerr << "Alpha: "<< alpha << endl;
            cerr << "Beta: " << beta << endl;
            cerr << "Total Angle: " << totalAngle << endl;
            cerr << "Difficulty: " << difficulty <<endl; 
            */

            if(difficulty<minDifficulty)
            {
              if(!(i->getID()==Ball::EIGHT && gameState->isOpenTable()))
              {  
                minDifficulty = difficulty;
                easiestPocket = *j;
                easiestBall = *i;
                //shot.params.phi = alpha;
                //cerr << "alpha currently: " << alpha << " and phi currently: " << shot.params.phi << " and minDifficulty: "<< minDifficulty << "\n" << endl;
              }
              else
              {
                cerr << "Ignoring EightBall" << endl;
              }
            }
          }
        
      }
    }
    
    //Each ball in play was assessed for each pocket, so was counted 6 times in the loop. Divide by this to get the real number. 
    numberOfBallsInPlay /= 6;
    cerr << "Number of Balls in Play: " << numberOfBallsInPlay << "\n" << endl;
   
    //If Only Black Left, Find Easiest Pocket To Put It In. 
    if(numberOfBallsInPlay == 0)
    {
      easiestBall = Ball::EIGHT;
    }


    //Ghost Ball Method. 
    Point easiestPocketCenter = gameState->tableState().getTable().getPocketCenter(easiestPocket);
    double r = easiestBall.getRadius();
	

    // Checking shooting angles.
    double alpha = angle(cueBallPos,easiestBall.getPos());

    if((alpha>180.0)&&(alpha<360.0)){cerr<<"Aiming Down the Table with Alpha = "<<alpha<<"\n"<<endl;}
    else{cerr<<"Aiming Up the Table with Alpha = "<<alpha<<"\n"<<endl;}
    
    
    double beta = angle(easiestPocketCenter,easiestBall.getPos());
    double ghostBall_x = easiestBall.getPos().x + 2*r*cos(beta);
    double ghostBall_y = easiestBall.getPos().y + 2*r*sin(beta);
    Point ghostBallPos = Point(ghostBall_x,ghostBall_y);


    //double distFromObjectBall = dist(easiestBall.getPos(),easiestPocketCenter);
    //double distFromGhostBall = dist(ghostBallPos,easiestPocketCenter);
    //if(distFromObjectBall>distFromGhostBall){cerr<<"Ghost Ball on Wrong Side."<<"\n"<<endl;}
    //else{cerr<<"Ghost Ball on Correct Side."<<"\n"<<endl;}
   

    //shot.params.phi = angle(cueBallPos,easiestBall.getPos());
    shot.params.phi = angle(cueBallPos,ghostBallPos)*180.0/M_PI;
    //cerr << "Pocket Centre: " << easiestPocketCenter << " , " << "Ball Centre: " << easiestBall.getPos() << " , " << "Ghost Ball Centre: " << ghostBallPos << " , " << "Beta(Object): " << beta << " , "<< "Beta(Ghost): " <<angle(easiestPocketCenter,ghostBallPos) <<"\n"<<endl;
    shot.params.a=0.0;
    shot.params.b=0.0;
    shot.params.theta = 5.0;
    //shot.params.phi = 275.0;
    shot.params.v=4.5;
    shot.cue_x=0.48;
    shot.cue_y=1.67705;
    shot.ball = easiestBall.getID();
    shot.pocket = easiestPocket;
    while(gameState->tableState().isPhysicallyPossible(shot.params) == 128){shot.params.theta += 5.0;} 
  

  //shot.decision=Pool::DEC_CONCEDE;
}



Pool::Decision AI::decide()
{
  return AIBase::decide();
}
