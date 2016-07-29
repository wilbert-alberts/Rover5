package com.asml.innovationteam.rover;

import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.util.Random;

public class RoverReaderSim implements IRoverReader {
	private boolean connected= false;
	private Random rnd = new Random();

	public RoverReaderSim(String address, int port) throws UnknownHostException, IOException {
		connected = true;
	}
	
	public void disconnect() {
		connected= false;
	}
	
	public void getInSync() throws IOException {
		return;
	}
	
	public void readRover(RegisterMap map) throws IOException
	{
		setRegisterU32(map, RegisterMap.REG_HEADER );
		setRegister32(map, RegisterMap.REG_MICROS);
		setRegister32(map, RegisterMap.REG_MILLIS);
		setRegister8(map, RegisterMap.REG_LEFTDIR);
		setRegister8(map, RegisterMap.REG_LEFTDC);
		setRegister8(map, RegisterMap.REG_RIGHTDIR);
		setRegister8(map, RegisterMap.REG_RIGHTDC);
		setRegister8(map, RegisterMap.REG_COLLISION);
		setRegister8(map, RegisterMap.REG_LINE);
		setRegister32(map, RegisterMap.REG_LEFTPOS);
		setRegister32(map, RegisterMap.REG_RIGHTPOS);
		setRegister16(map, RegisterMap.REG_AMB_COL_OFFSET);
		setRegister16(map, RegisterMap.REG_AMB_COL_NE);
		setRegister16(map, RegisterMap.REG_AMB_COL_SE);
		setRegister16(map, RegisterMap.REG_AMB_COL_SW);
		setRegister16(map, RegisterMap.REG_AMB_COL_NW);
		setRegister16(map, RegisterMap.REG_AMB_LINE_OFFSET);
		setRegister16(map, RegisterMap.REG_AMB_LINE_N);
		setRegister16(map, RegisterMap.REG_AMB_LINE_E);
		setRegister16(map, RegisterMap.REG_AMB_LINE_S);
		setRegister16(map, RegisterMap.REG_AMB_LINE_W);
		setRegister16(map, RegisterMap.REG_IR_COL_NE);
		setRegister16(map, RegisterMap.REG_IR_COL_SE);
		setRegister16(map, RegisterMap.REG_IR_COL_SW);
		setRegister16(map, RegisterMap.REG_IR_COL_NW);
		setRegister16(map, RegisterMap.REG_IR_LINE_N);
		setRegister16(map, RegisterMap.REG_IR_LINE_E);
		setRegister16(map, RegisterMap.REG_IR_LINE_S);
		setRegister16(map, RegisterMap.REG_IR_LINE_W);
		setRegisterU32(map, RegisterMap.REG_TRAILER);

		try {
			Thread.sleep(333);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	private void setRegister8(RegisterMap map, int reg) throws IOException
	{
		
		map.setRegister(reg, (int)(rnd.nextDouble()*256));
	}
	
	private void setRegister16(RegisterMap map, int reg) throws IOException
	{
		map.setRegister(reg, (int)(rnd.nextDouble()*65536));
	}
	
	private void setRegisterU32(RegisterMap map, int reg) throws IOException
	{
		map.setRegister(reg, rnd.nextInt());
	}
	
	private void setRegister32(RegisterMap map, int reg) throws IOException
	{
		setRegisterU32(map,reg);
	}

}
