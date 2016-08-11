package com.asml.innovationteam.roversim;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

public class RoverFromFile implements IRover {

	private FileReader freader;
	private BufferedReader breader;
	private Map<Integer, Integer> currentLine = new HashMap<Integer, Integer>();
	private String filename;
	private int myLeftDC;
	private int myRightDC;

	public RoverFromFile(String filename) {
		this.filename = filename;
		reopenFile();
	}

	private int getRegister(int r) {
		int result;
		Integer rAsInteger = new Integer(r);
		Integer vAsInteger = getCurrentLine().get(rAsInteger);
		if (vAsInteger == null) {
			vAsInteger = getNextLine().get(rAsInteger);
		}
		getCurrentLine().remove(rAsInteger);
		result = vAsInteger.intValue();

		return result;
	}

	private Map<Integer, Integer> getNextLine() {
		for (int tr = 0; tr < 20; tr++) {
			try {
				String line = readLineFromReader();
				String[] fields = line.split("\t");
				currentLine.clear();
				for (int i = 0; i < fields.length; i++) {
					currentLine.put(new Integer(i), Integer.parseInt(fields[i]));
				}
				return currentLine;
			} catch (NumberFormatException e) {
				// Skip, go to next line.
			}
		}
		System.err.println("No decent line found");
		return null;
	}

	private Map<Integer, Integer> getCurrentLine() {
		if (currentLine == null) {
			String line = readLineFromReader();
			String[] fields = line.split("\t");
			currentLine.clear();
			for (int i = 0; i < fields.length; i++) {
				currentLine.put(new Integer(i), Integer.parseInt(fields[i]));
			}
		}
		return currentLine;
	}

	private String readLineFromReader() {
		String result = null;
		try {
			if (breader == null) {
				reopenFile();
				result = breader.readLine();
			} else {
				result = breader.readLine();
				if (result == null) {
					reopenFile();
					result = breader.readLine();
				}
			}
		} catch (IOException e) {
			result = null;
		}
		return result;
	}

	private void reopenFile() {
		try {
			if (freader != null)
				freader.close();
			if (breader != null)
				breader.close();
			freader = new FileReader(filename);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			System.err.println("Unable to open file: " + filename);
		} catch (IOException e) {
			System.err.println("Exception when opening file: " + filename);
			e.printStackTrace();
		}
		breader = new BufferedReader(freader);
	}

	@Override
	public int getLeftPos() {
		return getRegister(7);
	}

	@Override
	public int getRightPos() {
		return getRegister(8);
	}

	@Override
	public int getActiveLine(LineOrientation sensor) {
		switch (sensor) {
		case N:
			return getRegister(21);
		case E:
			return getRegister(22);
		case S:
			return getRegister(23);
		case W:
			return getRegister(24);

		default:
			return -1;
		}
	}

	@Override
	public int getActiveCollision(CollisionOrientation sensor) {
		switch (sensor) {
		case NE:
			return getRegister(17);
		case SE:
			return getRegister(18);
		case SW:
			return getRegister(19);
		case NW:
			return getRegister(20);

		default:
			return -1;
		}
	}

	@Override
	public int getAmbientLine(LineOrientation sensor) {
		switch (sensor) {
		case N:
			return getRegister(13);
		case E:
			return getRegister(14);
		case S:
			return getRegister(15);
		case W:
			return getRegister(16);

		default:
			return -1;
		}
	}

	@Override
	public int getAmbientCollision(CollisionOrientation sensor) {
		switch (sensor) {
		case NE:
			return getRegister(9);
		case SE:
			return getRegister(10);
		case SW:
			return getRegister(11);
		case NW:
			return getRegister(12);

		default:
			return -1;
		}
	}

	@Override
	public void setLeftDC(int dc) {
		// TODO Auto-generated method stub
		myLeftDC = dc;
	}

	@Override
	public void setRightDC(int dc) {
		// TODO Auto-generated method stub
		myRightDC = dc;
	}

	@Override
	public void fillRegisterMap(RegisterMap map) {
		map.setRegister(RegisterMap.REG_HEADER, 1526703360);
		map.setRegister(RegisterMap.REG_MICROS, getMicros());
		map.setRegister(RegisterMap.REG_MILLIS, getMillis());
		map.setRegister(RegisterMap.REG_LEFTDIR, getLeftDC() > 0 ? 1 : 0);
		map.setRegister(RegisterMap.REG_LEFTDC, getLeftDC());
		map.setRegister(RegisterMap.REG_RIGHTDIR, getRightDC() > 0 ? 1 : 0);
		map.setRegister(RegisterMap.REG_RIGHTDC, getRightDC());
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

	private int getRightDC() {
		return myRightDC;
	}

	private int getLeftDC() {
		return myLeftDC;
	}

	private int getMillis() {
		return getRegister(2);
	}

	private int getMicros() {
		return getRegister(1);
	}

}
