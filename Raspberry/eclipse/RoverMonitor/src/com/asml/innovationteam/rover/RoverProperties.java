package com.asml.innovationteam.rover;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.Properties;
import java.util.Timer;
import java.util.TimerTask;

public class RoverProperties {
	static final String filename = "RoverMonitor.ini";
	static Properties props = null;
	static boolean saveScheduled = false;

	static void setProperty(String key, String value) {
		getProps().setProperty(key, value);
		scheduleSave();
	}

	private static Properties getProps() {
		// TODO Auto-generated method stub
		if (props == null) {
			props = new Properties();
			try {
				InputStream is = new FileInputStream(filename);
				props.load(is);
			} catch (Exception e) {
				// TODO: handle exception
			}
		}
		return props;
	}

	static String getProperty(String key, String v) {
		String result = getProps().getProperty(key, v);
		scheduleSave();

		return result;
	}

	private static void scheduleSave() {
		// TODO Auto-generated method stub
		if (!saveScheduled) {
			Timer tmr = new Timer();
			tmr.schedule(new TimerTask() {

				@Override
				public void run() {
					// TODO Auto-generated method stub
					save();
					saveScheduled = false;
				}
			}, 5000);
		}
	}

	static void save() {
		try {
			FileOutputStream os = new FileOutputStream(filename);
			getProps().store(os, "Rover properties");
		} catch (Exception e) {
			// TODO: handle exception
			System.err.println(e);
		}
	}

}
