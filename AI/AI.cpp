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

  //Temp Variable to Enter Difficulty Loop.
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

    Point cueBallPos;

    if(gameState->getTurnType() == (TT_BALL_IN_HAND || TT_BEHIND_LINE)){cueBallPos = Point(0.48,1.67705);}
    else{cueBallPos = gameState->tableState().getBall(Ball::CUE).getPos();}

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
    
  
    for(vector<Ball>::const_iterator i = start; i != end; i++)
    {
      Point objectBallPos = i->getPos();
      for(vector<Table::Pocket>::const_iterator j = pockets.begin(); j != pockets.end(); j++)
      {   

        if(i->isInPlay() && i->getID() != Ball::CUE)
        {
          Point pocketPos = gameState->tableState().getTable().getPocketCenter(*j);
          double difficulty = shotDifficulty(cueBallPos,objectBallPos,pocketPos);
          
          if(difficulty<minDifficulty)
          {
            if(!(i->getID()==Ball::EIGHT && gameState->isOpenTable()))
            {  
              minDifficulty = difficulty;
              easiestPocket = *j;
              easiestBall = *i;
            }else{
              cerr << "Ignoring EightBall" << endl;
            }
          }
        }     
      }
    }
    
    cerr << "Number of Balls in Play: " << numberOfBallsAvailable() << "\n" << endl;

   
    //If Only Black Left, Find Easiest Pocket To Put It In. 
    if(numberOfBallsAvailable() == 0)
     {
      cerr << "8 Ball Case." << "\n" <<endl;
      easiestBall = gameState->tableState().getBall(Ball::EIGHT);
    
    
     //Running Pocket Loop for 8 Ball Case.
     for(vector<Table::Pocket>::const_iterator j = pockets.begin(); j != pockets.end(); j++)
     { 
       Point objectBallPos = easiestBall.getPos();
       Point pocketPos = gameState->tableState().getTable().getPocketCenter(*j);
       double difficulty = shotDifficulty(cueBallPos,objectBallPos,pocketPos);
       
       if(difficulty<minDifficulty)
       {
         minDifficulty = difficulty;
         easiestPocket = *j;
       }
         
     }
    }

    // Set Shot Parameters.    
    shot.params.phi = shootingAngle(cueBallPos,easiestBall,easiestPocket);
    shot.params.a=0.0;
    shot.params.b=0.0;
    shot.params.theta = 5.0;
    shot.params.v=4.5;
    shot.cue_x=0.48;
    shot.cue_y=1.67705;
    shot.ball = easiestBall.getID();
    shot.pocket = easiestPocket;
    while(gameState->tableState().isPhysicallyPossible(shot.params) == 128){shot.params.theta += 5.0;} 
 
}



Pool::Decision AI::decide()
{
  return AIBase::decide();
}
