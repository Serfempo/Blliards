import FastFiz,Rules,math
shot = Rules.GameShot();
gameState = Rules.GameState.RackedState(Rules.GT_EIGHTBALL);
cue = gameState.tableState().getBall(0);
TS = gameState.tableState();
gameState.getTurnType();
shot.params = FastFiz.ShotParams(0.0,0.0,25.0,270.0,5.0);
shot.cue_x,shot.cue_y = 0.48,1.67705;
gameState.executeShot(shot);

def dist(ball1,ball2):
	ball1x = float(ball1.getPos().toString().split()[0])
	ball1y = float(ball1.getPos().toString().split()[1])
	ball2x = float(ball2.getPos().toString().split()[0])
	ball2y = float(ball2.getPos().toString().split()[1])
	return math.sqrt((ball1x-ball2x)**2+(ball1y-ball2y)**2)


def angle(x1,y1,x2,y2):
	twoPi = 2*math.pi
        raw = math.atan2(y2-y1,x2-x1)
        if(raw<0):
                frac = raw/twoPi
                res = frac - (int(frac))
                res = 1 + res*twoPi
		return math.degrees(res)
        elif(raw>=twoPi):
                frac = raw/twoPi
                res = frac - (int(frac))
                res = res*twoPi
		return math.degrees(res)       
