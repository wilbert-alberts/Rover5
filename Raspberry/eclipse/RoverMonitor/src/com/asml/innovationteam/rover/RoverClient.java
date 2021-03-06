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

	public int getPort() {
		return port;
	}

	public void setPort(int port) {
		this.port = port;
		RoverProperties.setProperty("port", Integer.toString(port));
	}

	public void setAddress(String address) {
		this.address = address;
		RoverProperties.setProperty("address", address);
	}

	interface IRoverChanged {
		public void changed();
		public void disconnected();
	}

	enum CollisionDirection {
		NE, SE, SW, NW
	}

	enum LineSensorID {
		N, E, S, W
	}

	RoverClient() {
		address = RoverProperties.getProperty("address", "localhost");
		port = Integer.parseInt(RoverProperties.getProperty("port", "34343"));
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

	public double getLeftTorque() {
		double dc;
		synchronized (lock) {
			dc = map.getRegister(RegisterMap.REG_LEFTDC) / 256.0;
			return (map.getRegister(RegisterMap.REG_LEFTDIR) == 0) ? dc : -1.0 * dc; 
		}		
	}
	
	public double getRightTorque() {
		double dc;
		synchronized (lock) {
			dc = map.getRegister(RegisterMap.REG_RIGHTDC) / 256.0;
			return (map.getRegister(RegisterMap.REG_RIGHTDIR) == 0) ? dc : -1.0 * dc; 
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

	public int getCollisionAmbient(CollisionDirection cd) {
		synchronized (lock) {
			switch (cd) {
			case NE:
				return map.getRegister(RegisterMap.REG_AMB_COL_NE);
			case SE:
				return map.getRegister(RegisterMap.REG_AMB_COL_SE);
			case SW:
				return map.getRegister(RegisterMap.REG_AMB_COL_SW);
			case NW:
				return map.getRegister(RegisterMap.REG_AMB_COL_NW);

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

	public int getLineAmbient(LineSensorID ls) {
		synchronized (lock) {
			switch (ls) {
			case N:
				return map.getRegister(RegisterMap.REG_AMB_LINE_N);
			case E:
				return map.getRegister(RegisterMap.REG_AMB_LINE_E);
			case S:
				return map.getRegister(RegisterMap.REG_AMB_LINE_S);
			case W:
				return map.getRegister(RegisterMap.REG_AMB_LINE_W);
			default:
				return -1;
			}
		}
	}

	class ClientThread extends Thread {
		IRoverReader rr;
		boolean connected = false;

		public ClientThread() {

		}

		public void disconnect() {
			if (rr != null)
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
				rr.disconnect();
			} catch (UnknownHostException e) {
				// TODO Auto-generated catch block
				//e.printStackTrace();
				for (IRoverChanged l : listeners) {
					l.disconnected();
				}
				rr.disconnect();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				//e.printStackTrace();
				for (IRoverChanged l : listeners) {
					l.disconnected();
				}
			}
		}

	}
}
