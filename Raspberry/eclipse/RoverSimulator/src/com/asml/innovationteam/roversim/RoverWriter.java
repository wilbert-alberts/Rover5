package com.asml.innovationteam.roversim;

import java.io.IOException;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Collection;
import java.util.Vector;

public class RoverWriter {
	private ServerSocket socket;
	private int port;
	private Socket clientSocket;
	private boolean running;
	private IRover rover;

	public RoverWriter(int port, IRover r) {
		this.port = port;
		this.rover = r;
	}

	public void startSimulation() throws Exception {
		socket = new ServerSocket(port);
		clientSocket = socket.accept();

		OutputStream os = clientSocket.getOutputStream();
		RegisterMap map = new RegisterMap();
		running = true;
		while (running) {
			rover.fillRegisterMap(map);
			try {
				writeRover(map, os);
			} catch (SocketException ex) {
				os.close();
				clientSocket.close();
				clientSocket = socket.accept();
				os = clientSocket.getOutputStream();
			}
			Thread.sleep(500);
		}
	}

	private void writeRover(RegisterMap map, OutputStream is) throws IOException {
		writeRegisterU32(is, map, RegisterMap.REG_HEADER);
		writeRegister32(is, map, RegisterMap.REG_MICROS);
		writeRegister32(is, map, RegisterMap.REG_MILLIS);
		writeRegister8(is, map, RegisterMap.REG_LEFTDIR);
		writeRegister8(is, map, RegisterMap.REG_LEFTDC);
		writeRegister8(is, map, RegisterMap.REG_RIGHTDIR);
		writeRegister8(is, map, RegisterMap.REG_RIGHTDC);
		writeRegister32(is, map, RegisterMap.REG_LEFTPOS);
		writeRegister32(is, map, RegisterMap.REG_RIGHTPOS);
		writeRegister16(is, map, RegisterMap.REG_AMB_LINE_N);
		writeRegister16(is, map, RegisterMap.REG_AMB_LINE_E);
		writeRegister16(is, map, RegisterMap.REG_AMB_LINE_S);
		writeRegister16(is, map, RegisterMap.REG_AMB_LINE_W);
		writeRegister16(is, map, RegisterMap.REG_AMB_COL_NE);
		writeRegister16(is, map, RegisterMap.REG_AMB_COL_SE);
		writeRegister16(is, map, RegisterMap.REG_AMB_COL_SW);
		writeRegister16(is, map, RegisterMap.REG_AMB_COL_NW);
		writeRegister16(is, map, RegisterMap.REG_IR_LINE_N);
		writeRegister16(is, map, RegisterMap.REG_IR_LINE_E);
		writeRegister16(is, map, RegisterMap.REG_IR_LINE_S);
		writeRegister16(is, map, RegisterMap.REG_IR_LINE_W);
		writeRegister16(is, map, RegisterMap.REG_IR_COL_NE);
		writeRegister16(is, map, RegisterMap.REG_IR_COL_SE);
		writeRegister16(is, map, RegisterMap.REG_IR_COL_SW);
		writeRegister16(is, map, RegisterMap.REG_IR_COL_NW);
		writeRegisterU32(is, map, RegisterMap.REG_TRAILER);

	}

	private void writeRegister8(OutputStream os, RegisterMap map, int reg) throws IOException {
		byte v = (byte) map.getRegister(reg);
		os.write(v);
	}

	private void writeRegister16(OutputStream os, RegisterMap map, int reg) throws IOException {
		short v = (short) map.getRegister(reg);
		byte[] msg = new byte[2];
		ByteBuffer bb = ByteBuffer.wrap(msg);
		bb.order(ByteOrder.LITTLE_ENDIAN);
		bb.putShort(v);
		os.write(msg);
	}

	private void writeRegister32(OutputStream os, RegisterMap map, int reg) throws IOException {
		int v = map.getRegister(reg);
		byte[] msg = new byte[4];
		ByteBuffer bb = ByteBuffer.wrap(msg);
		bb.order(ByteOrder.LITTLE_ENDIAN);
		bb.putInt(v);
		os.write(msg);
	}

	private void writeRegisterU32(OutputStream os, RegisterMap map, int reg) throws IOException {
		int v = map.getRegister(reg);
		byte[] msg = new byte[4];
		msg[0] = (byte) (v % 256);
		v = v / 256;
		msg[1] = (byte) (v % 256);
		v = v / 256;
		msg[2] = (byte) (v % 256);
		v = v / 256;
		msg[3] = (byte) (v % 256);
		v = v / 256;

		os.write(msg);

	}

	public static void main(String[] args) throws Exception {
		RoverWriter writer = new RoverWriter(34343, new Rover());
		writer.startSimulation();
	}

}
