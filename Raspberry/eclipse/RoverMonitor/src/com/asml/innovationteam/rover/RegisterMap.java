package com.asml.innovationteam.rover;

import java.io.IOException;
import java.io.InputStream;

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
	
	private void setRegister8(int reg, InputStream is) throws IOException
	{
		registers[reg] = is.read();
	}
	
	private void setRegister16(int reg, InputStream is) throws IOException
	{
		int b0 = is.read();
		int b1 = is.read();
		registers[reg] = b0 + 256*b1;
	}
	
	private void setRegister32(int reg, InputStream is) throws IOException
	{
		int b0 = is.read();
		int b1 = is.read();
		int b2 = is.read();
		int b3 = is.read();
		if (b3>127)
			registers[reg] = -(b0 + 256*b1 + 256*256*b2 + 256*256*256*b3);
		else
			registers[reg] = b0 + 256*b1 + 256*256*b2 + 256*256*256*b3;
	}
	
	
	
	public final int REG_HEADER = 0;
	public final int REG_MICROS=1;
	public final int REG_MILLIS=2;
	public final int REG_LEFTDIR=3;
	public final int REG_LEFTDC=4;
	public final int REG_RIGHTDIR=5;
	public final int REG_RIGHTDC=6;
	public final int REG_COLLISION=7;
	public final int REG_LINE=8;
	public final int REG_LEFTPOS=9;
	public final int REG_RIGHTPOS=10;
	public final int REG_AMB_COL_OFFSET=11;
	public final int REG_AMB_COL_NE=12;
	public final int REG_AMB_COL_SE=13;
	public final int REG_AMB_COL_SW=14;
	public final int REG_AMB_COL_NW=15;
	public final int REG_AMB_LINE_OFFSET=16;
	public final int REG_AMB_LINE_N=17;
	public final int REG_AMB_LINE_E=18;
	public final int REG_AMB_LINE_S=19;
	public final int REG_AMB_LINE_W=20;
	public final int REG_IR_COL_NE=21;
	public final int REG_IR_COL_SE=22;
	public final int REG_IR_COL_SW=23;
	public final int REG_IR_COL_NW=24;
	public final int REG_IR_LINE_N=25;
	public final int REG_IR_LINE_E=26;
	public final int REG_IR_LINE_S=27;
	public final int REG_IR_LINE_W=28;
	public final int REG_TRAILER=29;
	public final int REG_MAX=30;
	
	private int registers[];
	
}
