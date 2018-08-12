import FastFiz,Rules,math,random 
shot = Rules.GameShot();
gameState = Rules.GameState.RackedState(Rules.GT_EIGHTBALL);
cue = gameState.tableState().getBall(0);
TS = gameState.tableState();
gameState.getTurnType();
shot.params = FastFiz.ShotParams(0.0,0.0,25.0,270.0,5.0);
shot.cue_x,shot.cue_y = 0.48,1.67705;
gameState.executeShot(shot);

#def dist(ball1,ball2):
#	ball1x = float(ball1.getPos().toString().split()[0])
#	ball1y = float(ball1.getPos().toString().split()[1])
#	ball2x = float(ball2.getPos().toString().split()[0])
#	ball2y = float(ball2.getPos().toString().split()[1])
#	return math.sqrt((ball1x-ball2x)**2+(ball1y-ball2y)**2)


def dist(P1,P2):
	return math.sqrt((P1.x-P2.x)**2+(P1.y-P2.y)**2)



#def angle(P1,P2):
#	twoPi = 2*math.pi
#   raw = math.atan2(P2.y-P1.y,P2.x-P1.x)
#  if(raw<0):
#        frac = raw/twoPi
#        res = frac - (int(frac))
#        raw = 1 + res*twoPi
#    elif(raw>=twoPi):
#        frac = raw/twoPi
#        res = frac - (int(frac))
#        raw = res*twoPi
#	return raw       





def angle(P1,P2,inDegrees):
    Res =  math.atan2(P2.y-P1.y,P2.x-P1.x) 
    if(inDegrees):
        Res = math.degrees(Res)
        Res = (Res+360)%360
    return Res    
    
    
    
    
    
    
#math.degrees
    
    
    
def cutAngle(alpha,beta,isDegrees):
    
    diff = abs(alpha-beta)
    if(not isDegrees):
        diff = math.degrees(diff)
    if(diff>180.0):
        diff = 360.0-diff
    return diff
    
    
    
def getGhostBall(ball,targetPos,GS):
    r = ball.getRadius()
    ballPos = GS.tableState().getBall(ball.getID()).getPos()
    beta = angle(targetPos,ballPos,0)
    ghostBall_x = ballPos.x + 2*r*math.cos(beta)
    ghostBall_y = ballPos.y + 2*r*math.sin(beta)
    return FastFiz.Point(ghostBall_x,ghostBall_y) 
    
def isCutPossible(P1,P2,P3):
    isOk = false
    alpha = angle(P1,P2)
    beta = angle(P2,P3)
    totalAngle = cutAngle(alpha,beta,1)
    if(totalAngle<90.0):
        isOK = true
    return isOk
    
    
#def straightShot(ball,targetPos):

#                        "1 4 0 0 1 16 0.028575 1 0 0.55800016617144931885 1.6769988393869978971 0.028574999999999999706 1 1 0.557 0.559 0 1"

def generate2BallState():    
    constructorString = "1 0 0 0 1 2 0.028575 1 0 0.55800007568413168002 1.6770003841400817901 0.028574999999999999706 1 1 0.223 0.322"
    tableWidth = 1.116
    tableLength = 2.236
    #Update 1 Ball Coords and State.
    old_1Ball_x_val = constructorString.split()[-2]
    old_1Ball_y_val = constructorString.split()[-1]
    new_1Ball_x_val = str(random.uniform(0.04, tableWidth-0.04))
    new_1Ball_y_val = str(random.uniform(0.04, tableLength-0.04))
    BallInPlay1 = constructorString.split()[-4]
    
    #Update Cue Ball Coords and State.
    old_CueBall_x_val = constructorString.split()[-6]
    old_CueBall_y_val = constructorString.split()[-7]
    new_CueBall_x_val = str(random.uniform(0, tableWidth))
    new_CueBall_y_val = str(random.uniform(0, tableLength))
    CueBallInPlay = constructorString.split()[-9]

    # Replace Values in 
    constructorString = constructorString.replace(old_1Ball_x_val,new_1Ball_x_val).replace(old_1Ball_y_val,new_1Ball_y_val)
    constructorString = constructorString.replace(old_CueBall_x_val,new_CueBall_x_val).replace(old_CueBall_y_val,new_CueBall_y_val)
    return Rules.GameState.Factory(constructorString)
    
    
def copyGameState(gs):
    gameString = gs.toString()
    return Rules.GameState.Factory(gameString)
    
    
    
    
#def bankShot():


#def kickShot():


#def plantShot():





