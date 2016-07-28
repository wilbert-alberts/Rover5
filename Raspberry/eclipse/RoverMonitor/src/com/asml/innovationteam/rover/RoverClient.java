package com.asml.innovationteam.rover;

import java.io.IOException;
import java.io.InputStream;
import java.io.Reader;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.HashSet;
import java.util.Set;

import javax.swing.SwingUtilities;

public class RoverClient {
	private RegisterMap map;
	private String address;
	private int port;
	private ClientThread thr;
	private Object lock = new Object();
	private Set<IRoverChanged> listeners;

	interface IRoverChanged {
		public void changed();
	}

	enum CollisionDirection {
		NE, SE, SW, NW
	}

	enum LineSensorID {
		N, E, S, W
	}

	RoverClient(String address, int port) {
		this.address = address;
		this.port = port;
		listeners = new HashSet<IRoverChanged>();
		map = new RegisterMap();
	}

	public void connect() {
		if (thr == null) {
			thr = new ClientThread();
			thr.start();
		}
	}

	public void disconnect() {
		if (thr != null) {
			thr.disconnect();
			try {
				thr.join();
				thr = null;
			} catch (InterruptedException e) {
				// Silently ignore
			}
		}
	}

	public void registerChangedListener(IRoverChanged l) {
		listeners.add(l);
	}

	public void unregisterChangedListener(IRoverChanged l) {
		listeners.remove(l);
	}

	public String getAddress() {
		return address + ":" + Integer.toString(port);
	}

	public int getLeftPosition() {
		synchronized (lock) {
			return map.getRegister(RegisterMap.REG_LEFTPOS);
		}
	}

	public int getRightPosition() {
		synchronized (lock) {
			return map.getRegister(RegisterMap.REG_RIGHTPOS);
		}
	}

	public int getCollision(CollisionDirection cd) {
		synchronized (lock) {
			switch (cd) {
			case NE:
				return map.getRegister(RegisterMap.REG_IR_COL_NE);
			case SE:
				return map.getRegister(RegisterMap.REG_IR_COL_SE);
			case SW:
				return map.getRegister(RegisterMap.REG_IR_COL_SW);
			case NW:
				return map.getRegister(RegisterMap.REG_IR_COL_NW);

			default:
				return -1;
			}
		}
	}

	public int getLine(LineSensorID ls) {
		synchronized (lock) {
			switch (ls) {
			case N:
				return map.getRegister(RegisterMap.REG_IR_LINE_N);
			case E:
				return map.getRegister(RegisterMap.REG_IR_LINE_E);
			case S:
				return map.getRegister(RegisterMap.REG_IR_LINE_S);
			case W:
				return map.getRegister(RegisterMap.REG_IR_LINE_W);
			default:
				return -1;
			}
		}
	}

	public boolean isLineVisible(LineSensorID ls) {
		int c;
		synchronized (lock) {
			c = map.getRegister(RegisterMap.REG_COLLISION);
		}
		switch (ls) {
		case N:
			return (c & (1 << 0)) != 0;
		case E:
			return (c & (1 << 1)) != 0;
		case S:
			return (c & (1 << 2)) != 0;
		case W:
			return (c & (1 << 3)) != 0;
		default:
			return false;
		}
	}

	public boolean isColliding(CollisionDirection cd) {
		int c;
		synchronized (lock) {
			c = map.getRegister(RegisterMap.REG_COLLISION);
		}
		switch (cd) {
		case NE:
			return (c & (1 << 0)) != 0;
		case SE:
			return (c & (1 << 1)) != 0;
		case SW:
			return (c & (1 << 2)) != 0;
		case NW:
			return (c & (1 << 3)) != 0;
		default:
			return false;
		}

	}

	class ClientThread extends Thread {
		IRoverReader rr;
		boolean connected = false;

		public ClientThread() {

		}

		public void disconnect() {
			rr.disconnect();
			connected = false;
		}

		@Override
		public void run() {
			RegisterMap m = new RegisterMap();
			try {
				rr = new RoverReader(address, port);
				connected = true;
				rr.getInSync();
				while (connected) {
					// Read from server
					rr.readRover(m);
					synchronized (lock) {
						m.cloneInto(map);
					}
					for (IRoverChanged l : listeners) {
						l.changed();
					}
				}
			} catch (UnknownHostException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}

	}
}
