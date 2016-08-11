package com.asml.innovationteam.rover;

import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class RoverReader implements IRoverReader {
	private Socket socket;
	private InputStream is;
	private boolean connected= false;

	public RoverReader(String address, int port) throws UnknownHostException, IOException {
		socket = new Socket(address, port);
		is = socket.getInputStream();
		connected = true;
	}
	
	@Override
	public void disconnect() {
		connected= false;
		try {
			is.close();
			socket.close();
			
		} catch (Exception e) {
			// TODO: handle exception
		}
	}
	
	@Override
	public void getInSync() throws IOException {
		int cursor = 0;
		char trailer[];
		int c;

		trailer = new char[4];
		trailer[0] = 0xFF;
		trailer[1] = 0xA5;
		trailer[2] = 0x00;
		trailer[3] = 0x5A;

		while (connected) {
			c = is.read();
			if (c == trailer[cursor]) {
				cursor++;
				if (cursor == 4) {
					return;
				}
			} else {
				cursor = 0;
			}
		}
	}
	
	@Override
	public void readRover(RegisterMap map) throws IOException
	{
		setRegisterU32(is, map, RegisterMap.REG_HEADER );
		setRegister32(is, map, RegisterMap.REG_MICROS);
		setRegister32(is, map, RegisterMap.REG_MILLIS);
		setRegister8(is, map, RegisterMap.REG_LEFTDIR);
		setRegister8(is, map, RegisterMap.REG_LEFTDC);
		setRegister8(is, map, RegisterMap.REG_RIGHTDIR);
		setRegister8(is, map, RegisterMap.REG_RIGHTDC);
		setRegister32(is, map, RegisterMap.REG_LEFTPOS);
		setRegister32(is, map, RegisterMap.REG_RIGHTPOS);
		setRegister16(is, map, RegisterMap.REG_AMB_LINE_N);
		setRegister16(is, map, RegisterMap.REG_AMB_LINE_E);
		setRegister16(is, map, RegisterMap.REG_AMB_LINE_S);
		setRegister16(is, map, RegisterMap.REG_AMB_LINE_W);
		setRegister16(is, map, RegisterMap.REG_AMB_COL_NE);
		setRegister16(is, map, RegisterMap.REG_AMB_COL_SE);
		setRegister16(is, map, RegisterMap.REG_AMB_COL_SW);
		setRegister16(is, map, RegisterMap.REG_AMB_COL_NW);
		setRegister16(is, map, RegisterMap.REG_IR_LINE_N);
		setRegister16(is, map, RegisterMap.REG_IR_LINE_E);
		setRegister16(is, map, RegisterMap.REG_IR_LINE_S);
		setRegister16(is, map, RegisterMap.REG_IR_LINE_W);
		setRegister16(is, map, RegisterMap.REG_IR_COL_NE);
		setRegister16(is, map, RegisterMap.REG_IR_COL_SE);
		setRegister16(is, map, RegisterMap.REG_IR_COL_SW);
		setRegister16(is, map, RegisterMap.REG_IR_COL_NW);
		setRegisterU32(is, map, RegisterMap.REG_TRAILER);
	}
	
	private void setRegister8(InputStream is, RegisterMap map, int reg) throws IOException
	{
		byte[] arr = new byte[1];
		is.read(arr, 0, arr.length);
		map.setRegister(reg, arr);
	}
	
	private void setRegister16(InputStream is, RegisterMap map, int reg) throws IOException
	{
		byte[] arr = new byte[2];
		is.read(arr, 0, arr.length);
		ByteBuffer bb = ByteBuffer.wrap(arr);
		bb.order(ByteOrder.LITTLE_ENDIAN);
		map.setRegister(reg, bb.getShort());
	}
	
	private void setRegisterU32(InputStream is, RegisterMap map, int reg) throws IOException
	{
		byte[] arr = new byte[4];
		is.read(arr, 0, arr.length);
		map.setRegister(reg, arr);
	}
	
	private void setRegister32(InputStream is, RegisterMap map, int reg) throws IOException
	{
		byte[] arr = new byte[4];
		is.read(arr, 0, 4);
		ByteBuffer bb = ByteBuffer.wrap(arr);
		bb.order(ByteOrder.LITTLE_ENDIAN);
		map.setRegister(reg, bb.getInt());
	}

}
