package com.asml.innovationteam.roversim;

import java.util.HashMap;
import java.util.Map;

public class Rover implements IRover {
	private int leftPos;
	private int rightPos;
	private int leftDC;
	private int rightDC;
	private Map<LineOrientation, Integer> activeLineSensors = new HashMap<LineOrientation, Integer>();
	private Map<CollisionOrientation, Integer> activeCollisionSensors = new HashMap<CollisionOrientation, Integer>();
	private Map<LineOrientation, Integer> ambientLineSensors = new HashMap<LineOrientation, Integer>();
	private Map<CollisionOrientation, Integer> ambientCollisionSensors = new HashMap<CollisionOrientation, Integer>();
	private int millis;
	private int micros;
	

	public int getMillis() {
		millis = millis+1;
		return millis;
	}

	public int getMicros() {
		micros = (int) (Math.random() * (1<<31));
		return micros;
	}

	@Override
	public int getLeftPos() {
		leftPos += leftDC; 
		return leftPos;
	}

	@Override
	public int getRightPos() {
		rightPos += rightDC;
		return rightPos;
	}

	@Override
	public int getActiveLine(LineOrientation sensor) {
		activeLineSensors.put(sensor, (int) (Math.random() * 1024));
		return activeLineSensors.get(sensor);
	}

	@Override
	public int getActiveCollision(CollisionOrientation sensor) {
		activeCollisionSensors.put(sensor, (int) (Math.random() * 1024));
		return activeCollisionSensors.get(sensor);
	}

	@Override
	public int getAmbientLine(LineOrientation sensor) {
		ambientLineSensors.put(sensor, (int) (Math.random() * 10));
		return ambientLineSensors.get(sensor);
	}

	@Override
	public int getAmbientCollision(CollisionOrientation sensor) {
		ambientCollisionSensors.put(sensor, (int) (Math.random() * 10));
		return ambientCollisionSensors.get(sensor);
	}

	@Override
	public void setLeftDC(int dc) {
		leftDC = dc;
	}

	@Override
	public void  setRightDC(int dc) {
		rightDC = dc;
	}

	@Override
	public void fillRegisterMap(RegisterMap map) {
		map.setRegister(RegisterMap.REG_HEADER,1526703360 );
		map.setRegister(RegisterMap.REG_MICROS, getMicros() );
		map.setRegister(RegisterMap.REG_MILLIS, getMillis() );
		map.setRegister(RegisterMap.REG_LEFTDIR, leftDC > 0 ? 1 : 0 );
		map.setRegister(RegisterMap.REG_LEFTDC, leftDC);
		map.setRegister(RegisterMap.REG_RIGHTDIR, rightDC > 0 ? 1: 0);
		map.setRegister(RegisterMap.REG_RIGHTDC, rightDC);
		map.setRegister(RegisterMap.REG_LEFTPOS, getLeftPos());
		map.setRegister(RegisterMap.REG_RIGHTPOS, getRightPos());
		map.setRegister(RegisterMap.REG_AMB_COL_NE, getAmbientCollision(CollisionOrientation.NE));
		map.setRegister(RegisterMap.REG_AMB_COL_SE, getAmbientCollision(CollisionOrientation.SE));
		map.setRegister(RegisterMap.REG_AMB_COL_SW, getAmbientCollision(CollisionOrientation.SW));
		map.setRegister(RegisterMap.REG_AMB_COL_NW, getAmbientCollision(CollisionOrientation.NW));
		map.setRegister(RegisterMap.REG_AMB_LINE_N, getAmbientLine(LineOrientation.N));
		map.setRegister(RegisterMap.REG_AMB_LINE_E, getAmbientLine(LineOrientation.E));
		map.setRegister(RegisterMap.REG_AMB_LINE_S, getAmbientLine(LineOrientation.S));
		map.setRegister(RegisterMap.REG_AMB_LINE_W, getAmbientLine(LineOrientation.W));
		map.setRegister(RegisterMap.REG_IR_COL_NE, getActiveCollision(CollisionOrientation.NE));
		map.setRegister(RegisterMap.REG_IR_COL_SE, getActiveCollision(CollisionOrientation.SE));
		map.setRegister(RegisterMap.REG_IR_COL_SW, getActiveCollision(CollisionOrientation.SW));
		map.setRegister(RegisterMap.REG_IR_COL_NW, getActiveCollision(CollisionOrientation.NW));
		map.setRegister(RegisterMap.REG_IR_LINE_N, getActiveLine(LineOrientation.N));
		map.setRegister(RegisterMap.REG_IR_LINE_E, getActiveLine(LineOrientation.E));
		map.setRegister(RegisterMap.REG_IR_LINE_S, getActiveLine(LineOrientation.S));
		map.setRegister(RegisterMap.REG_IR_LINE_W, getActiveLine(LineOrientation.W));
		map.setRegister(RegisterMap.REG_TRAILER, 1509991935);		
	}


}
