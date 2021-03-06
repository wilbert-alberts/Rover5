package com.asml.innovationteam.rover;

import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.EventQueue;
import java.awt.FlowLayout;
import java.awt.GridBagLayout;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import javax.swing.JTextField;
import javax.swing.JToggleButton;
import javax.swing.SpringLayout;
import javax.swing.SwingConstants;
import javax.swing.SwingUtilities;

import com.asml.innovationteam.rover.RoverClient.CollisionDirection;
import com.asml.innovationteam.rover.RoverClient.LineSensorID;

import eu.hansolo.steelseries.gauges.Radial;
import javax.swing.JSlider;
import javax.swing.event.ChangeListener;
import javax.swing.event.ChangeEvent;

public class MainWindow implements RoverClient.IRoverChanged {

	private JFrame frmRoverMonitor;
	private JTextField txtAddress;
	private RoverClient rover;
	private JLabel lblPosRight;
	private JLabel lblColNW;
	private JLabel lblColNE;
	private JLabel lblColSE;
	private JLabel lblColSW;
	private Color lblLineSBG;
	private Color lblLineEBG;
	private Color lblLineWBG;
	private Color lblLineNBG;
	private Color grays[];
	private JPanel tabIlluminated;
	private JPanel tabAmbient;
	private JLabel lblLineN;
	private JLabel lblLineW;
	private JLabel lblLineE;
	private JLabel lblLineS;
	private JLabel lblLineAmbN;
	private JLabel lblLineAmbW;
	private JLabel lblLineAmbS;
	private JLabel lblLineAmbE;
	private List<SensorAndUIBind> lineSensors = null;
	private List<SensorAndUIBind> lineAmbientSensors = null;
	private List<CollisionSensorBind> collisionSensors =null;
	private JLabel lblNameN;
	private JLabel lblNameW;
	private JLabel lblNameE;
	private JLabel lblNameS;
	private JLabel lblN;
	private JLabel lblE;
	private JLabel lblS;
	private JLabel lblW;
	private MinMaxSlider minmaxAmbient;
	private MinMaxSlider minmaxLine;
	private JPanel panel_4;
	private JPanel panel_5;
	private Radial radLeft;
	private Radial radRight;
	private JToggleButton btnConnect;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {

		loadDefaults();
		
		if (args.length == 1) {
			RoverProperties.setProperty("address", args[0]);
			RoverProperties.setProperty("port", "34343");
		}
		if (args.length == 2) {
			RoverProperties.setProperty("address", args[0]);
			RoverProperties.setProperty("port", Integer.toString(Integer.parseInt(args[1])));
		}

		RoverClient rover = new RoverClient();

		EventQueue.invokeLater(new Runnable() {
			public void run() {

				try {
					MainWindow window = new MainWindow();
					window.setRover(rover);
					window.frmRoverMonitor.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
		saveDefaults();
	}

	private static void saveDefaults() {
		// TODO Auto-generated method stub
		
	}

	private static void loadDefaults() {
		// TODO Auto-generated method stub
		
	}

	/**
	 * Create the application.
	 */
	public MainWindow() {
		initialize();
		initializeGrays();
		initializeSUI();
	}

	private void initializeSUI() {
		lineSensors = new ArrayList<MainWindow.SensorAndUIBind>();
		lineSensors.add(new SensorAndUIBind(lblLineN, minmaxLine, "N", LineSensorID.N));
		lineSensors.add(new SensorAndUIBind(lblLineE, minmaxLine, "E", LineSensorID.E));
		lineSensors.add(new SensorAndUIBind(lblLineS, minmaxLine, "S", LineSensorID.S));
		lineSensors.add(new SensorAndUIBind(lblLineW, minmaxLine, "W", LineSensorID.W));

		lineAmbientSensors = new ArrayList<MainWindow.SensorAndUIBind>();
		lineAmbientSensors.add(new SensorAndUIBind(lblLineAmbN, minmaxAmbient, "N", LineSensorID.N));
		lineAmbientSensors.add(new SensorAndUIBind(lblLineAmbE, minmaxAmbient, "E", LineSensorID.E));
		lineAmbientSensors.add(new SensorAndUIBind(lblLineAmbS, minmaxAmbient, "S", LineSensorID.S));
		lineAmbientSensors.add(new SensorAndUIBind(lblLineAmbW, minmaxAmbient, "W", LineSensorID.W));
		
		collisionSensors =  new ArrayList<CollisionSensorBind>();
		collisionSensors.add(new CollisionSensorBind(lblColNE, CollisionDirection.NE));
		collisionSensors.add(new CollisionSensorBind(lblColSE, CollisionDirection.SE));
		collisionSensors.add(new CollisionSensorBind(lblColSW, CollisionDirection.SW));
		collisionSensors.add(new CollisionSensorBind(lblColNW, CollisionDirection.NW));
	}

	private void initializeGrays() {
		// TODO Auto-generated method stub
		grays = new Color[100];
		for (int i = 0; i < 100; i++)
			grays[i] = new Color(i / 100.0f, i / 100.0f, i / 100.0f);
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
		frmRoverMonitor = new JFrame();
		frmRoverMonitor.setTitle("Rover monitor");
		frmRoverMonitor.setBounds(100, 100, 761, 474);
		frmRoverMonitor.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frmRoverMonitor.getContentPane().setLayout(new BorderLayout(0, 0));

		JPanel panel = new JPanel();
		frmRoverMonitor.getContentPane().add(panel, BorderLayout.NORTH);
		panel.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));

		JLabel lblRoverServer = new JLabel("Address: ");
		panel.add(lblRoverServer);

		txtAddress = new JTextField();
		panel.add(txtAddress);
		txtAddress.setColumns(10);

		btnConnect = new JToggleButton("Connect");
		btnConnect.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent arg0) {
				if (btnConnect.isSelected()) {
					setAddressAndPort();
					btnConnect.setText("Connected");
					rover.connect();
				} else {
					rover.disconnect();
					btnConnect.setText("Disconnected");
				}
			}

			private void setAddressAndPort() {
				String[] addrAndPort = txtAddress.getText().split("\\s*:\\s*");
				if (addrAndPort.length >= 2) {
					String addr = addrAndPort[0];
					int port = Integer.parseInt(addrAndPort[1]);
					rover.setAddress(addr);
					rover.setPort(port);
					txtAddress.setText(addr + ':' + port);
				}
			}
		});
		panel.add(btnConnect);

		JPanel centerPanel = new JPanel();
		frmRoverMonitor.getContentPane().add(centerPanel);
		SpringLayout sl_centerPanel = new SpringLayout();
		centerPanel.setLayout(sl_centerPanel);

		radLeft = new Radial();
		radLeft.setLedVisible(false);
		sl_centerPanel.putConstraint(SpringLayout.EAST, radLeft, 200, SpringLayout.WEST, centerPanel);
		radLeft.setMinValue(-1);
		radLeft.setMaxValue(1);
		radLeft.setValue(0.0);
		sl_centerPanel.putConstraint(SpringLayout.NORTH, radLeft, 50, SpringLayout.NORTH, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.SOUTH, radLeft, -50, SpringLayout.SOUTH, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.WEST, radLeft, 0, SpringLayout.WEST, centerPanel);
		centerPanel.add(radLeft);

		radRight = new Radial();
		radRight.setLedVisible(false);
		sl_centerPanel.putConstraint(SpringLayout.EAST, radRight, 0, SpringLayout.EAST, centerPanel);
		radRight.setMinValue(-1);
		radRight.setMaxValue(1);
		radRight.setValue(0.0);
		sl_centerPanel.putConstraint(SpringLayout.NORTH, radRight, 50, SpringLayout.NORTH, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.SOUTH, radRight, -50, SpringLayout.SOUTH, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.WEST, radRight, -200, SpringLayout.EAST, centerPanel);
		centerPanel.add(radRight);

		lblColNW = new JLabel("New label");
		sl_centerPanel.putConstraint(SpringLayout.NORTH, lblColNW, 0, SpringLayout.NORTH, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.WEST, lblColNW, 0, SpringLayout.WEST, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.SOUTH, lblColNW, 50, SpringLayout.NORTH, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.EAST, lblColNW, 100, SpringLayout.WEST, centerPanel);
		lblColNW.setHorizontalAlignment(SwingConstants.CENTER);
		lblColNW.setOpaque(true);
		centerPanel.add(lblColNW);

		JPanel panel_2 = new JPanel();
		sl_centerPanel.putConstraint(SpringLayout.NORTH, panel_2, 50, SpringLayout.NORTH, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.WEST, panel_2, 200, SpringLayout.WEST, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.SOUTH, panel_2, -50, SpringLayout.SOUTH, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.EAST, panel_2, -200, SpringLayout.EAST, centerPanel);
		centerPanel.add(panel_2);
		panel_2.setLayout(new CardLayout(0, 0));

		JTabbedPane tabbedPane = new JTabbedPane(JTabbedPane.TOP);
		panel_2.add(tabbedPane, "name_1216252609521");
		// tabbedPane.setSelectedIndex(0);

		tabIlluminated = new JPanel();
		tabbedPane.addTab("Illuminated", null, tabIlluminated, null);
		tabbedPane.setEnabledAt(0, true);
		tabIlluminated.setLayout(new BorderLayout(0, 0));

		minmaxLine = new MinMaxSlider(0, 1023, "line");
		tabIlluminated.add(minmaxLine, BorderLayout.SOUTH);

		panel_5 = new JPanel();
		tabIlluminated.add(panel_5);
		SpringLayout sl_panel_5 = new SpringLayout();
		panel_5.setLayout(sl_panel_5);

		lblN = new JLabel("N");
		lblN.setHorizontalAlignment(SwingConstants.CENTER);
		panel_5.add(lblN);

		lblE = new JLabel("E");
		lblE.setHorizontalAlignment(SwingConstants.CENTER);
		panel_5.add(lblE);

		lblS = new JLabel("S");
		lblS.setHorizontalAlignment(SwingConstants.CENTER);
		panel_5.add(lblS);

		lblW = new JLabel("W");
		lblW.setHorizontalAlignment(SwingConstants.CENTER);
		panel_5.add(lblW);

		lblLineN = new JLabel("x-N");
		sl_panel_5.putConstraint(SpringLayout.NORTH, lblN, 0, SpringLayout.SOUTH, lblLineN);
		sl_panel_5.putConstraint(SpringLayout.WEST, lblN, 0, SpringLayout.WEST, lblLineN);
		sl_panel_5.putConstraint(SpringLayout.EAST, lblN, 0, SpringLayout.EAST, lblLineN);
		sl_panel_5.putConstraint(SpringLayout.NORTH, lblLineN, 0, SpringLayout.NORTH, panel_5);
		sl_panel_5.putConstraint(SpringLayout.HORIZONTAL_CENTER, lblLineN, 0, SpringLayout.HORIZONTAL_CENTER, panel_5);
		lblLineN.setPreferredSize(new Dimension(100, 50));
		lblLineN.setMinimumSize(new Dimension(100, 50));
		lblLineN.setMaximumSize(new Dimension(100, 50));
		panel_5.add(lblLineN);
		lblLineN.setHorizontalAlignment(SwingConstants.CENTER);
		lblLineN.setOpaque(true);

		lblLineE = new JLabel("x-E");
		sl_panel_5.putConstraint(SpringLayout.NORTH, lblE, 0, SpringLayout.NORTH, lblLineE);
		sl_panel_5.putConstraint(SpringLayout.SOUTH, lblE, 0, SpringLayout.SOUTH, lblLineE);
		sl_panel_5.putConstraint(SpringLayout.EAST, lblE, 0, SpringLayout.WEST, lblLineE);
		sl_panel_5.putConstraint(SpringLayout.EAST, lblLineE, 0, SpringLayout.EAST, panel_5);
		sl_panel_5.putConstraint(SpringLayout.VERTICAL_CENTER, lblLineE, 0, SpringLayout.VERTICAL_CENTER, panel_5);
		lblLineE.setPreferredSize(new Dimension(100, 50));
		lblLineE.setMinimumSize(new Dimension(100, 50));
		lblLineE.setMaximumSize(new Dimension(100, 50));
		panel_5.add(lblLineE);
		lblLineE.setHorizontalAlignment(SwingConstants.CENTER);
		lblLineE.setOpaque(true);

		lblLineS = new JLabel("x-S");
		sl_panel_5.putConstraint(SpringLayout.WEST, lblS, 0, SpringLayout.WEST, lblLineS);
		sl_panel_5.putConstraint(SpringLayout.SOUTH, lblS, 0, SpringLayout.NORTH, lblLineS);
		sl_panel_5.putConstraint(SpringLayout.EAST, lblS, 0, SpringLayout.EAST, lblLineS);
		sl_panel_5.putConstraint(SpringLayout.SOUTH, lblLineS, 0, SpringLayout.SOUTH, panel_5);
		sl_panel_5.putConstraint(SpringLayout.HORIZONTAL_CENTER, lblLineS, 0, SpringLayout.HORIZONTAL_CENTER, panel_5);
		lblLineS.setPreferredSize(new Dimension(100, 50));
		lblLineS.setMinimumSize(new Dimension(100, 50));
		lblLineS.setMaximumSize(new Dimension(100, 50));
		panel_5.add(lblLineS);
		lblLineS.setHorizontalAlignment(SwingConstants.CENTER);
		lblLineS.setOpaque(true);

		lblLineW = new JLabel("x-W");
		sl_panel_5.putConstraint(SpringLayout.NORTH, lblW, 0, SpringLayout.NORTH, lblLineW);
		sl_panel_5.putConstraint(SpringLayout.WEST, lblW, 0, SpringLayout.EAST, lblLineW);
		sl_panel_5.putConstraint(SpringLayout.SOUTH, lblW, 0, SpringLayout.SOUTH, lblLineW);
		sl_panel_5.putConstraint(SpringLayout.WEST, lblLineW, 0, SpringLayout.WEST, panel_5);
		sl_panel_5.putConstraint(SpringLayout.VERTICAL_CENTER, lblLineW, 0, SpringLayout.VERTICAL_CENTER, panel_5);
		lblLineW.setPreferredSize(new Dimension(100, 50));
		lblLineW.setMinimumSize(new Dimension(100, 50));
		lblLineW.setMaximumSize(new Dimension(100, 50));
		panel_5.add(lblLineW);
		lblLineW.setHorizontalAlignment(SwingConstants.CENTER);
		lblLineW.setOpaque(true);

		tabAmbient = new JPanel();
		tabbedPane.addTab("Ambient", null, tabAmbient, null);
		tabAmbient.setLayout(new BorderLayout(0, 0));

		minmaxAmbient = new MinMaxSlider(0, 1023, "ambient");
		GridBagLayout gbl_minmaxAmbient = (GridBagLayout) minmaxAmbient.getLayout();
		gbl_minmaxAmbient.rowWeights = new double[] { 0.0, 0.0 };
		gbl_minmaxAmbient.rowHeights = new int[] { 0, 0 };
		gbl_minmaxAmbient.columnWeights = new double[] { 0.0, 0.0, 1.0 };
		gbl_minmaxAmbient.columnWidths = new int[] { 0, 0, 0 };
		tabAmbient.add(minmaxAmbient, BorderLayout.SOUTH);

		panel_4 = new JPanel();
		tabAmbient.add(panel_4);
		SpringLayout sl_panel_4 = new SpringLayout();
		panel_4.setLayout(sl_panel_4);

		lblNameN = new JLabel("N");
		lblNameN.setHorizontalAlignment(SwingConstants.CENTER);
		panel_4.add(lblNameN);

		lblNameE = new JLabel("E");
		panel_4.add(lblNameE);

		lblNameS = new JLabel("S");
		lblNameS.setHorizontalAlignment(SwingConstants.CENTER);
		panel_4.add(lblNameS);

		lblNameW = new JLabel("W");
		panel_4.add(lblNameW);

		lblLineAmbN = new JLabel("x-N");
		sl_panel_4.putConstraint(SpringLayout.NORTH, lblNameN, 0, SpringLayout.SOUTH, lblLineAmbN);
		sl_panel_4.putConstraint(SpringLayout.WEST, lblNameN, 0, SpringLayout.WEST, lblLineAmbN);
		sl_panel_4.putConstraint(SpringLayout.EAST, lblNameN, 0, SpringLayout.EAST, lblLineAmbN);
		sl_panel_4.putConstraint(SpringLayout.NORTH, lblLineAmbN, 0, SpringLayout.NORTH, panel_4);
		sl_panel_4.putConstraint(SpringLayout.HORIZONTAL_CENTER, lblLineAmbN, 0, SpringLayout.HORIZONTAL_CENTER,
				panel_4);
		lblLineAmbN.setMaximumSize(new Dimension(100, 50));
		lblLineAmbN.setMinimumSize(new Dimension(100, 50));
		panel_4.add(lblLineAmbN);
		lblLineAmbN.setPreferredSize(new Dimension(100, 50));
		lblLineAmbN.setHorizontalAlignment(SwingConstants.CENTER);

		lblLineAmbN.setOpaque(true);

		lblLineAmbE = new JLabel("x-E");
		sl_panel_4.putConstraint(SpringLayout.NORTH, lblNameE, 0, SpringLayout.NORTH, lblLineAmbE);
		sl_panel_4.putConstraint(SpringLayout.SOUTH, lblNameE, 0, SpringLayout.SOUTH, lblLineAmbE);
		sl_panel_4.putConstraint(SpringLayout.EAST, lblNameE, 0, SpringLayout.WEST, lblLineAmbE);
		sl_panel_4.putConstraint(SpringLayout.EAST, lblLineAmbE, 0, SpringLayout.EAST, panel_4);
		sl_panel_4.putConstraint(SpringLayout.VERTICAL_CENTER, lblLineAmbE, 0, SpringLayout.VERTICAL_CENTER, panel_4);
		lblLineAmbE.setMaximumSize(new Dimension(100, 50));
		lblLineAmbE.setMinimumSize(new Dimension(100, 50));
		panel_4.add(lblLineAmbE);
		lblLineAmbE.setPreferredSize(new Dimension(100, 50));
		lblLineAmbE.setHorizontalAlignment(SwingConstants.CENTER);
		lblLineAmbE.setOpaque(true);

		lblLineAmbS = new JLabel("x-S");
		sl_panel_4.putConstraint(SpringLayout.WEST, lblNameS, 0, SpringLayout.WEST, lblLineAmbS);
		sl_panel_4.putConstraint(SpringLayout.SOUTH, lblNameS, 0, SpringLayout.NORTH, lblLineAmbS);
		sl_panel_4.putConstraint(SpringLayout.EAST, lblNameS, 0, SpringLayout.EAST, lblLineAmbS);
		sl_panel_4.putConstraint(SpringLayout.SOUTH, lblLineAmbS, 0, SpringLayout.SOUTH, panel_4);
		sl_panel_4.putConstraint(SpringLayout.HORIZONTAL_CENTER, lblLineAmbS, 0, SpringLayout.HORIZONTAL_CENTER,
				panel_4);
		lblLineAmbS.setMaximumSize(new Dimension(100, 50));
		lblLineAmbS.setMinimumSize(new Dimension(100, 50));
		panel_4.add(lblLineAmbS);
		lblLineAmbS.setPreferredSize(new Dimension(100, 50));
		lblLineAmbS.setHorizontalAlignment(SwingConstants.CENTER);
		lblLineAmbS.setOpaque(true);

		lblLineAmbW = new JLabel("x-W");
		sl_panel_4.putConstraint(SpringLayout.NORTH, lblNameW, 0, SpringLayout.NORTH, lblLineAmbW);
		sl_panel_4.putConstraint(SpringLayout.WEST, lblNameW, 0, SpringLayout.EAST, lblLineAmbW);
		sl_panel_4.putConstraint(SpringLayout.SOUTH, lblNameW, 0, SpringLayout.SOUTH, lblLineAmbW);
		sl_panel_4.putConstraint(SpringLayout.WEST, lblLineAmbW, 0, SpringLayout.WEST, panel_4);
		sl_panel_4.putConstraint(SpringLayout.VERTICAL_CENTER, lblLineAmbW, 0, SpringLayout.VERTICAL_CENTER, panel_4);
		panel_4.add(lblLineAmbW);
		lblLineAmbW.setMaximumSize(new Dimension(100, 50));
		lblLineAmbW.setMinimumSize(new Dimension(100, 50));
		lblLineAmbW.setPreferredSize(new Dimension(100, 50));
		lblLineAmbW.setHorizontalAlignment(SwingConstants.CENTER);
		lblLineAmbW.setOpaque(true);

		lblColNE = new JLabel(" NE");
		sl_centerPanel.putConstraint(SpringLayout.NORTH, lblColNE, 0, SpringLayout.NORTH, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.WEST, lblColNE, -100, SpringLayout.EAST, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.SOUTH, lblColNE, 50, SpringLayout.NORTH, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.EAST, lblColNE, 0, SpringLayout.EAST, centerPanel);
		lblColNE.setHorizontalAlignment(SwingConstants.CENTER);
		lblColNE.setOpaque(true);
		centerPanel.add(lblColNE);

		lblPosRight = new JLabel("New label");
		sl_centerPanel.putConstraint(SpringLayout.NORTH, lblPosRight, 0, SpringLayout.SOUTH, lblColNE);
		sl_centerPanel.putConstraint(SpringLayout.WEST, lblPosRight, 0, SpringLayout.EAST, panel_2);
		sl_centerPanel.putConstraint(SpringLayout.EAST, lblPosRight, 0, SpringLayout.EAST, centerPanel);
		lblPosRight.setHorizontalAlignment(SwingConstants.CENTER);
		centerPanel.add(lblPosRight);

		lblColSW = new JLabel("SW");
		sl_centerPanel.putConstraint(SpringLayout.NORTH, lblColSW, -50, SpringLayout.SOUTH, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.WEST, lblColSW, 0, SpringLayout.WEST, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.SOUTH, lblColSW, 0, SpringLayout.SOUTH, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.EAST, lblColSW, 100, SpringLayout.WEST, centerPanel);
		lblColSW.setHorizontalAlignment(SwingConstants.CENTER);
		lblColSW.setOpaque(true);
		centerPanel.add(lblColSW);

		lblColSE = new JLabel("SE");
		sl_centerPanel.putConstraint(SpringLayout.SOUTH, lblPosRight, 0, SpringLayout.NORTH, lblColSE);
		sl_centerPanel.putConstraint(SpringLayout.NORTH, lblColSE, -50, SpringLayout.SOUTH, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.WEST, lblColSE, -100, SpringLayout.EAST, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.SOUTH, lblColSE, 0, SpringLayout.SOUTH, centerPanel);
		sl_centerPanel.putConstraint(SpringLayout.EAST, lblColSE, 0, SpringLayout.EAST, centerPanel);
		lblColSE.setHorizontalAlignment(SwingConstants.CENTER);
		lblColSE.setOpaque(true);
		centerPanel.add(lblColSE);
		
		JLabel lblCollision = new JLabel("collision");
		JSlider sldrCollisionThr = new JSlider();
		sldrCollisionThr.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent arg0) {
				int v = sldrCollisionThr.getValue();
				
				if ((v>=0) && (v<1024)) {
					RoverProperties.setProperty("collision_threshold", Integer.toString(v));
					lblCollision.setText(Integer.toString(v));
				}
			}
		});
		sldrCollisionThr.setMaximum(1024);
		sl_centerPanel.putConstraint(SpringLayout.NORTH, sldrCollisionThr, 0, SpringLayout.SOUTH, lblColNW);
		sl_centerPanel.putConstraint(SpringLayout.EAST, sldrCollisionThr, 0, SpringLayout.EAST, lblColNW);
		sl_centerPanel.putConstraint(SpringLayout.WEST, sldrCollisionThr, 0, SpringLayout.WEST, lblColNW);
		centerPanel.add(sldrCollisionThr);
		
		lblCollision.setHorizontalAlignment(SwingConstants.CENTER);
		sl_centerPanel.putConstraint(SpringLayout.NORTH, lblCollision, 0, SpringLayout.SOUTH, sldrCollisionThr);
		sl_centerPanel.putConstraint(SpringLayout.EAST, lblCollision, 0, SpringLayout.EAST, sldrCollisionThr);
		sl_centerPanel.putConstraint(SpringLayout.WEST, lblCollision, 0, SpringLayout.WEST, sldrCollisionThr);
		centerPanel.add(lblCollision);
		lblLineNBG = new Color(200, 100, 100);
		lblLineWBG = new Color(200, 100, 100);
		lblLineEBG = new Color(200, 100, 100);
		lblLineSBG = new Color(200, 100, 100);

	}

	@Override
	public void changed() {
		SwingUtilities.invokeLater(new Runnable() {

			@Override
			public void run() {
				radLeft.setValue(rover.getLeftTorque());
				radLeft.setLcdValue(rover.getLeftPosition());
				radRight.setValue(rover.getRightTorque());
				radRight.setLcdValue(rover.getRightPosition());
							
				for (CollisionSensorBind cdb : collisionSensors) {
					int v = rover.getCollision(cdb.cd);
					setCollisionColor(cdb.fld, v);
					setCollisionLbl(cdb.fld,  v);
				}
				for (SensorAndUIBind sui : lineSensors) {
					int v = rover.getLine(sui.sensor);
					setLineColor(sui.fld, sui.minmax, v);
					setLinebl(sui.fld, sui.name, v);
				}
				for (SensorAndUIBind sui : lineAmbientSensors) {
					int v = rover.getLineAmbient(sui.sensor);
					setLineColor(sui.fld, sui.minmax, v);
					setLinebl(sui.fld, sui.name, v);
				}

			}


			public void setLineColor(JLabel fld, MinMaxSlider minmax, int v) {
				double range = minmax.getMax() - minmax.getMin();
				double d = (v - minmax.getMin()) / range;
				if (d < 0.0)
					d = 0.0;
				if (d > 1.0)
					d = 1.0;

				int idx = (int) (d * 99);
				fld.setBackground(grays[idx]);
			}

			public void setLinebl(JLabel fld, String n, int v) {
				fld.setText(Integer.toString(v));
			}
			
			public void setCollisionColor(JLabel fld, int v) {
				int collisionThr = Integer.parseInt(RoverProperties.getProperty("collision_threshold", Integer.toString(400)));

				if (v > collisionThr)
					fld.setBackground(Color.red);
				else
					fld.setBackground(Color.green);
			}


			public void setCollisionLbl(JLabel fld, int v) {
				fld.setText(Integer.toString(v));
			}
		});

	}

	@Override
	public void disconnected() {
		SwingUtilities.invokeLater(new Runnable() {
			
			@Override
			public void run() {
				btnConnect.setSelected(false);
				btnConnect.setText("Disconnected");
			}
		});
	}

	class SensorAndUIBind {
		public SensorAndUIBind(JLabel l, MinMaxSlider mm, String n, LineSensorID s) {
			fld = l;
			name = n;
			sensor = s;
			minmax = mm;
		}

		public JLabel fld;
		public String name;
		public LineSensorID sensor;
		public MinMaxSlider minmax;
	}
	
	class CollisionSensorBind {
		public CollisionSensorBind(JLabel fld, CollisionDirection cd) {
			this.fld = fld;
			this.cd = cd;
		}
		public JLabel fld;
		public CollisionDirection cd;
	}
}
