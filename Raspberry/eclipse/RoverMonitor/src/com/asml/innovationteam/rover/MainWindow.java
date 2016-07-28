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
		
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				String servername = null;
				int port = 0;

				try {
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
					
 					MainWindow window = new MainWindow();
					RoverClient rover = new RoverClient(servername, port);
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
		
		JPanel panel = new JPanel();
		frame.getContentPane().add(panel, BorderLayout.NORTH);
		
		JLabel lblRoverServer = new JLabel("Address: ");
		panel.add(lblRoverServer);
		
		txtAddress = new JTextField();
		panel.add(txtAddress);
		txtAddress.setColumns(10);
		
		JToggleButton btnConnect = new JToggleButton("Connect");
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
		frame.getContentPane().add(centerPanel, BorderLayout.CENTER);
		GridBagLayout gbl_lblTrackRight = new GridBagLayout();
		gbl_lblTrackRight.columnWidths = new int[] {0, 0, 0, 3};
		gbl_lblTrackRight.rowHeights = new int[] {0, 0, 0, 3};
		gbl_lblTrackRight.columnWeights = new double[]{0.0, 1.0, 0.0, Double.MIN_VALUE};
		gbl_lblTrackRight.rowWeights = new double[]{0.0, 1.0, 0.0, Double.MIN_VALUE};
		centerPanel.setLayout(gbl_lblTrackRight);
		
		lblColNW = new JLabel("New label");
		lblColNW.setOpaque(true);
		GridBagConstraints gbc_lblColNW = new GridBagConstraints();
		gbc_lblColNW.weightx = 1.0;
		gbc_lblColNW.weighty = 1.0;
		gbc_lblColNW.insets = new Insets(0, 0, 5, 5);
		gbc_lblColNW.gridx = 0;
		gbc_lblColNW.gridy = 0;
		centerPanel.add(lblColNW, gbc_lblColNW);
		
		lblColNE = new JLabel(" NE");
		lblColNE.setOpaque(true);
		GridBagConstraints gbc_btnColNE = new GridBagConstraints();
		gbc_btnColNE.weighty = 1.0;
		gbc_btnColNE.weightx = 1.0;
		gbc_btnColNE.insets = new Insets(0, 0, 5, 0);
		gbc_btnColNE.gridx = 2;
		gbc_btnColNE.gridy = 0;
		centerPanel.add(lblColNE, gbc_btnColNE);
		
		lblPosLeft = new JLabel("New label");
		GridBagConstraints gbc_lblPosLeft = new GridBagConstraints();
		gbc_lblPosLeft.insets = new Insets(0, 0, 5, 5);
		gbc_lblPosLeft.gridx = 0;
		gbc_lblPosLeft.gridy = 1;
		centerPanel.add(lblPosLeft, gbc_lblPosLeft);
		
		JPanel panel_2 = new JPanel();
		GridBagConstraints gbc_panel_2 = new GridBagConstraints();
		gbc_panel_2.weighty = 4.0;
		gbc_panel_2.weightx = 4.0;
		gbc_panel_2.fill = GridBagConstraints.BOTH;
		gbc_panel_2.insets = new Insets(0, 0, 5, 5);
		gbc_panel_2.gridx = 1;
		gbc_panel_2.gridy = 1;
		centerPanel.add(panel_2, gbc_panel_2);
		
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
		GridBagConstraints gbc_lblPosRight = new GridBagConstraints();
		gbc_lblPosRight.insets = new Insets(0, 0, 5, 0);
		gbc_lblPosRight.gridx = 2;
		gbc_lblPosRight.gridy = 1;
		centerPanel.add(lblPosRight, gbc_lblPosRight);
		
		lblColSW = new JLabel("SW");
		lblColSW.setOpaque(true);
		GridBagConstraints gbc_btnColSW = new GridBagConstraints();
		gbc_btnColSW.weighty = 1.0;
		gbc_btnColSW.weightx = 1.0;
		gbc_btnColSW.insets = new Insets(0, 0, 0, 5);
		gbc_btnColSW.gridx = 0;
		gbc_btnColSW.gridy = 2;
		centerPanel.add(lblColSW, gbc_btnColSW);
		
		lblColSE = new JLabel("SE");
		lblColSE.setOpaque(true);
		GridBagConstraints gbc_btnColSE = new GridBagConstraints();
		gbc_btnColSE.weighty = 1.0;
		gbc_btnColSE.weightx = 1.0;
		gbc_btnColSE.gridx = 2;
		gbc_btnColSE.gridy = 2;
		centerPanel.add(lblColSE, gbc_btnColSE);
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
				
				System.out.println("SWT thread update)");
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
