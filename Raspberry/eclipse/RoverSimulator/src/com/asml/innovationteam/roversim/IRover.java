package com.asml.innovationteam.roversim;

public interface IRover {
	enum LineOrientation {
		N,E,S,W
	}
	enum CollisionOrientation {
		NE, SE, SW, NW
	}
	int getLeftPos();
	int getRightPos();
	int getActiveLine(LineOrientation sensor);
	int getActiveCollision(CollisionOrientation sensor);
	int getAmbientLine(LineOrientation sensor);
	int getAmbientCollision(CollisionOrientation sensor);
	void setLeftDC(int dc);
	void setRightDC(int dc);
	void fillRegisterMap(RegisterMap map);
}
