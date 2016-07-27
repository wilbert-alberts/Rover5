package com.asml.innovationteam.rover;

import java.io.InputStream;
import java.io.Reader;
import java.net.Socket;

public class RoverClient {

	RoverClient(String address, int port) 
	{
		
	}
	
	 class ClientThread implements Runnable
	 {
		private Reader reader;
		boolean connected;

		public ClientThread(Reader rdr) {
			this.reader = rdr;
		}
		 
		@Override
		public void run() {
			Socket socket = new Socket(address, port);
			InputStream is = socket.getInputStream();

			connected = true;
			
			getInSync(is);
			// TODO Auto-generated method stub
			while (connected) {
				// Read from server
				// update state
				// refresh gui				
			}
		}

		private void getInSync(InputStream is) {
			int cursor = 0;
			char trailer[4];
			int c;
			
			trailer[0] = 0xFF;
			trailer[1] = 0xA5;
			trailer[2] = 0x00;
			trailer[3] = 0x5A;
			
			while(connected) {
				c = is.read();
				if (c == trailer[cursor]) {
					cursor ++;
					if (cursor == 4)
						return;
				}
				else {
					cursor = 0;
				}
			}
		}
		 
	 }
}
