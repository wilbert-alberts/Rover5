package com.asml.innovationteam.rover;

import java.io.IOException;

public interface IRoverReader {

	void readRover(RegisterMap map) throws IOException;

	void getInSync() throws IOException;

	void disconnect();

}