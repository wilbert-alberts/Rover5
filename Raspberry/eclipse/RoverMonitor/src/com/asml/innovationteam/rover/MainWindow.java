package com.asml.innovationteam.rover;

import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.JPanel;
import java.awt.BorderLayout;
import java.awt.Color;

import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.JToggleButton;
import javax.swing.SwingUtilities;

import com.asml.innovationteam.rover.RoverClient.CollisionDirection;
import com.asml.innovationteam.rover.RoverClient.LineSensorID;

import java.awt.GridBagLayout;
import javax.swing.JButton;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import java.awt.event.ItemListener;
import java.lang.reflect.InvocationTargetException;
import java.awt.event.ItemEvent;
import java.awt.GridLayout;
import javax.swing.SwingConstants;

public class MainWindow implements RoverClient.IRoverChanged {

	private JFrame frame;
	private JTextField txtAddress;
	private RoverClient rover;
	private JLabel lblPosLeft;
	private JLabel lblPosRight;
	private JLabel lblColNW;
	private JLabel lblColNE;
	private JLabel lblColSE;
	private JLabel lblColSW;
	private JLabel lblLineS;
	private JLabel lblLineE;
	private JLabel lblLineW;
	private JLabel lblLineN;
	private Color lblLineSBG;
	private Color lblLineEBG;
	private Color lblLineWBG;
	private Color lblLineNBG;
	private JLabel label;
	private JLabel label_1;
	private JLabel label_2;
	private JLabel label_3;
	private JLabel label_4;
	private Color grays[];
	
	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		String servername = null;
		int port = 0;

		if (args.length == 0) {
			servername = "localhost";
			port = 34343;
		}
		if (args.length == 1) {
			servername = args[0];
			port = 34343;
		}
		if (args.length == 2) {
			servername = args[0];
			port = Integer.parseInt(args[1]);
		}
		
		RoverClient rover = new RoverClient(servername, port);

		EventQueue.invokeLater(new Runnable() {
			public void run() {

				try {
 					MainWindow window = new MainWindow();
					window.setRover(rover);
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public MainWindow() {
		initialize();
		initializeGrays();
	}
	
	private void initializeGrays() {
		// TODO Auto-generated method stub
		grays = new Color[100];
		for (int i=0; i<100; i++)
			grays[i] = new Color(i/100.0f, i/100.0f, i/100.0f);
	}

	public void setRover(RoverClient rover) {
		this.rover = rover;
		txtAddress.setText(rover.getAddress());
		rover.registerChangedListener(this);
	}
	
	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		frame = new JFrame();
		frame.setBounds(100, 100, 586, 391);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(null);
		
		JPanel panel = new JPanel();
		panel.setBounds(0, 0, 570, 33);
		frame.getContentPane().add(panel);
		panel.setLayout(null);
		
		JLabel lblRoverServer = new JLabel("Address: ");
		lblRoverServer.setBounds(104, 9, 80, 14);
		panel.add(lblRoverServer);
		
		txtAddress = new JTextField();
		txtAddress.setBounds(194, 6, 120, 20);
		panel.add(txtAddress);
		txtAddress.setColumns(10);
		
		JToggleButton btnConnect = new JToggleButton("Connect");
		btnConnect.setBounds(319, 5, 120, 23);
		btnConnect.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent arg0) {
				if (btnConnect.isSelected()) {
					rover.connect();
					btnConnect.setText("Connected");
				}
				else {
					rover.disconnect();					
					btnConnect.setText("Disconnected");
				}
			}
		});
		panel.add(btnConnect);
		
		JPanel centerPanel = new JPanel();
		centerPanel.setBounds(0, 33, 570, 320);
		frame.getContentPane().add(centerPanel);
		centerPanel.setLayout(null);
		
		lblColNW = new JLabel("New label");
		lblColNW.setBounds(10, 11, 100, 50);
		lblColNW.setOpaque(true);
		centerPanel.add(lblColNW);
		
		lblColNE = new JLabel(" NE");
		lblColNE.setBounds(460, 11, 100, 50);
		lblColNE.setOpaque(true);
		centerPanel.add(lblColNE);
		
		lblPosLeft = new JLabel("New label");
		lblPosLeft.setBounds(10, 151, 100, 14);
		centerPanel.add(lblPosLeft);
		
		JPanel panel_2 = new JPanel();
		panel_2.setBounds(123, 58, 320, 200);
		centerPanel.add(panel_2);
		
		lblLineN = new JLabel("N");
		lblLineN.setHorizontalAlignment(SwingConstants.CENTER);
		lblLineNBG = new Color(200,100,100);
		panel_2.setLayout(new GridLayout(0, 3, 0, 0));
		
		label = new JLabel("");
		panel_2.add(label);
		lblLineN.setBackground(lblLineNBG);
		lblLineN.setOpaque(true);
		panel_2.add(lblLineN);
		
		lblLineW = new JLabel("W");
		lblLineW.setHorizontalAlignment(SwingConstants.CENTER);
		lblLineWBG = new Color(200,100,100);
		
		label_1 = new JLabel("");
		panel_2.add(label_1);
		lblLineW.setBackground(lblLineWBG);
		lblLineW.setOpaque(true);
		panel_2.add(lblLineW);
		
		lblLineE = new JLabel("E");
		lblLineE.setHorizontalAlignment(SwingConstants.CENTER);
		lblLineEBG = new Color(200,100,100);
		
		label_2 = new JLabel("");
		panel_2.add(label_2);
		lblLineE.setBackground(lblLineEBG);
		lblLineE.setOpaque(true);
		panel_2.add(lblLineE);
		
		lblLineS = new JLabel("S");
		lblLineS.setHorizontalAlignment(SwingConstants.CENTER);
		lblLineSBG = new Color(200,100,100);
		
		label_3 = new JLabel("");
		panel_2.add(label_3);
		lblLineS.setBackground(lblLineSBG);
		lblLineS.setOpaque(true);
		panel_2.add(lblLineS);
		
		label_4 = new JLabel("");
		panel_2.add(label_4);
		
		lblPosRight = new JLabel("New label");
		lblPosRight.setBounds(460, 151, 100, 14);
		centerPanel.add(lblPosRight);
		
		lblColSW = new JLabel("SW");
		lblColSW.setBounds(10, 259, 100, 50);
		lblColSW.setOpaque(true);
		centerPanel.add(lblColSW);
		
		lblColSE = new JLabel("SE");
		lblColSE.setBounds(460, 259, 100, 50);
		lblColSE.setOpaque(true);
		centerPanel.add(lblColSE);
	}

	@Override
	public void changed() {
		SwingUtilities.invokeLater(new Runnable() {
			
			@Override
			public void run() {
				lblPosLeft.setText(Integer.toString(rover.getLeftPosition()));
				lblPosRight.setText(Integer.toString(rover.getRightPosition()));
				
				lblColNE.setBackground(getCollisionColor(rover.isColliding(CollisionDirection.NE)));
				lblColSE.setBackground(getCollisionColor(rover.isColliding(CollisionDirection.SE)));
				lblColSW.setBackground(getCollisionColor(rover.isColliding(CollisionDirection.SW)));
				lblColNW.setBackground(getCollisionColor(rover.isColliding(CollisionDirection.NW)));
				
				lblColNE.setText(Integer.toString(rover.getCollision(CollisionDirection.NE)));
				lblColSE.setText(Integer.toString(rover.getCollision(CollisionDirection.SE)));
				lblColSW.setText(Integer.toString(rover.getCollision(CollisionDirection.SW)));
				lblColNW.setText(Integer.toString(rover.getCollision(CollisionDirection.NW)));
				
				setLineColor(lblLineN, rover.getLine(LineSensorID.N));
				setLineColor(lblLineE, rover.getLine(LineSensorID.E));
				setLineColor(lblLineS, rover.getLine(LineSensorID.S));
				setLineColor(lblLineW, rover.getLine(LineSensorID.W));
			}
			
			public Color getCollisionColor(boolean collision) {
				return collision ? Color.red : Color.green;
			}
			
			public void setLineColor(JLabel fld, int v) {				
				double d = v;
				d = 100 * d / 65536;
				int idx = (int)d;
				fld.setBackground(grays[idx]);
			}
		});
		
	}

}
