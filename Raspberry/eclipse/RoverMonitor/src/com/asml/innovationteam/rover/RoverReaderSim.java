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
		setRegister(map, RegisterMap.REG_HEADER, 1<<32 );
		setRegister(map, RegisterMap.REG_MICROS, 1<<32 );
		setRegister(map, RegisterMap.REG_MILLIS, 1<<32 );
		setRegister(map, RegisterMap.REG_LEFTDIR, 1 );
		setRegister(map, RegisterMap.REG_LEFTDC, 1<<8);
		setRegister(map, RegisterMap.REG_RIGHTDIR, 1);
		setRegister(map, RegisterMap.REG_RIGHTDC, 1<<8);
		setRegister(map, RegisterMap.REG_COLLISION, 1<<8);
		setRegister(map, RegisterMap.REG_LINE, 1<<8 );
		setRegister(map, RegisterMap.REG_LEFTPOS, 1<<10);
		setRegister(map, RegisterMap.REG_RIGHTPOS, 1<<10);
		setRegister(map, RegisterMap.REG_AMB_COL_OFFSET, 1<<10);
		setRegister(map, RegisterMap.REG_AMB_COL_NE, 1<<10);
		setRegister(map, RegisterMap.REG_AMB_COL_SE, 1<<10);
		setRegister(map, RegisterMap.REG_AMB_COL_SW, 1<<10);
		setRegister(map, RegisterMap.REG_AMB_COL_NW, 1<<10);
		setRegister(map, RegisterMap.REG_AMB_LINE_OFFSET, 1<<10);
		setRegister(map, RegisterMap.REG_AMB_LINE_N, 1<<10);
		setRegister(map, RegisterMap.REG_AMB_LINE_E, 1<<10);
		setRegister(map, RegisterMap.REG_AMB_LINE_S, 1<<10);
		setRegister(map, RegisterMap.REG_AMB_LINE_W, 1<<10);
		setRegister(map, RegisterMap.REG_IR_COL_NE, 1<<10);
		setRegister(map, RegisterMap.REG_IR_COL_SE, 1<<10);
		setRegister(map, RegisterMap.REG_IR_COL_SW, 1<<10);
		setRegister(map, RegisterMap.REG_IR_COL_NW, 1<<10);
		setRegister(map, RegisterMap.REG_IR_LINE_N, 1<<10);
		setRegister(map, RegisterMap.REG_IR_LINE_E, 1<<10);
		setRegister(map, RegisterMap.REG_IR_LINE_S, 1<<10);
		setRegister(map, RegisterMap.REG_IR_LINE_W, 1<<10);
		setRegister(map, RegisterMap.REG_TRAILER, 1<<32);

		try {
			Thread.sleep(100);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	private void setRegister(RegisterMap map, int reg, int max) throws IOException
	{
		
		map.setRegister(reg, (int)Math.round(rnd.nextDouble()*max));
		//map.setRegister(reg, 33);
	}

}
