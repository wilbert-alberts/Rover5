package com.asml.innovationteam.rover;

public class RegisterMap {
	public RegisterMap ()
	{
		registers = new int[REG_MAX];
	}
	
	public int getRegister(int reg)
	{
		return registers[reg];
	}
	
	public void setRegister(int reg, int value)
	{
		registers[reg] = value;
	}
	
	public void setRegister(int reg, byte[] bytes)
	{
		int value = 0;
		for (int i=0; i<bytes.length; i++) {
			value = value*256 + bytes[i];
		}
		registers[reg] = value;
	}
	
	static public final int REG_HEADER = 0;
	static public final int REG_MICROS=1;
	static public final int REG_MILLIS=2;
	static public final int REG_LEFTDIR=3;
	static public final int REG_LEFTDC=4;
	static public final int REG_RIGHTDIR=5;
	static public final int REG_RIGHTDC=6;
	static public final int REG_COLLISION=7;
	static public final int REG_LINE=8;
	static public final int REG_LEFTPOS=9;
	static public final int REG_RIGHTPOS=10;
	static public final int REG_AMB_COL_OFFSET=11;
	static public final int REG_AMB_COL_NE=12;
	static public final int REG_AMB_COL_SE=13;
	static public final int REG_AMB_COL_SW=14;
	static public final int REG_AMB_COL_NW=15;
	static public final int REG_AMB_LINE_OFFSET=16;
	static public final int REG_AMB_LINE_N=17;
	static public final int REG_AMB_LINE_E=18;
	static public final int REG_AMB_LINE_S=19;
	static public final int REG_AMB_LINE_W=20;
	static public final int REG_IR_COL_NE=21;
	static public final int REG_IR_COL_SE=22;
	static public final int REG_IR_COL_SW=23;
	static public final int REG_IR_COL_NW=24;
	static public final int REG_IR_LINE_N=25;
	static public final int REG_IR_LINE_E=26;
	static public final int REG_IR_LINE_S=27;
	static public final int REG_IR_LINE_W=28;
	static public final int REG_TRAILER=29;
	static public final int REG_MAX=30;
	
	private int registers[];
	
}
