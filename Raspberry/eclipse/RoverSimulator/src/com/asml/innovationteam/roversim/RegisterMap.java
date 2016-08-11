package com.asml.innovationteam.roversim;

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
	
	static public final int REG_HEADER		=0;
	static public final int REG_MICROS		=1;
	static public final int REG_MILLIS		=2;
	static public final int REG_LEFTDIR		=3;
	static public final int REG_LEFTDC		=4;
	static public final int REG_RIGHTDIR	=5;
	static public final int REG_RIGHTDC		=6;
	static public final int REG_LEFTPOS		=7;
	static public final int REG_RIGHTPOS	=8;
	static public final int REG_AMB_COL_NE	=9;
	static public final int REG_AMB_COL_SE	=10;
	static public final int REG_AMB_COL_SW	=11;
	static public final int REG_AMB_COL_NW	=12;
	static public final int REG_AMB_LINE_N	=13;
	static public final int REG_AMB_LINE_E	=14;
	static public final int REG_AMB_LINE_S	=15;
	static public final int REG_AMB_LINE_W	=16;
	static public final int REG_IR_COL_NE	=17;
	static public final int REG_IR_COL_SE	=18;
	static public final int REG_IR_COL_SW	=19;
	static public final int REG_IR_COL_NW	=20;
	static public final int REG_IR_LINE_N	=21;
	static public final int REG_IR_LINE_E	=22;
	static public final int REG_IR_LINE_S	=23;
	static public final int REG_IR_LINE_W	=24;
	static public final int REG_TRAILER		=25;
	static public final int REG_MAX			=26;
	
	private int registers[];

	public void cloneInto(RegisterMap map) {
		map.registers = this.registers;
	}
	
}
