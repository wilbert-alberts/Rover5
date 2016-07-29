package com.asml.innovationteam.rover;

import java.awt.CardLayout;
import java.awt.Color;
import java.awt.EventQueue;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.util.ArrayList;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.JTabbedPane;
import javax.swing.JTextField;
import javax.swing.JToggleButton;
import javax.swing.SwingConstants;
import javax.swing.SwingUtilities;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import com.asml.innovationteam.rover.RoverClient.CollisionDirection;
import com.asml.innovationteam.rover.RoverClient.LineSensorID;

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
	private Color lblLineSBG;
	private Color lblLineEBG;
	private Color lblLineWBG;
	private Color lblLineNBG;
	private Color grays[];
	private JPanel panel_1;
	private JPanel panel_3;
	private JLabel lblLineN;
	private JLabel lblLineW;
	private JLabel lblLineE;
	private JLabel lblLineS;
	private JLabel lblLineAmbN;
	private JLabel lblLineAmbW;
	private JLabel lblLineAmbS;
	private JLabel lblLineAmbE;
	private ArrayList<SensorAndUIBind> lineSensors = null;
	private ArrayList<SensorAndUIBind> lineAmbientSensors = null;
	private JLabel lblNewLabel;
	private JLabel lblNewLabel_1;
	private JLabel lblNewLabel_2;
	private JLabel lblNewLabel_3;
	private JLabel lblN;
	private JLabel lblE;
	private JLabel lblS;
	private JLabel lblW;
	private MinMaxSlider minmaxAmbient;
	private MinMaxSlider minmaxLine;

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
		initializeSUI();
	}

	private void initializeSUI() {
		// TODO Auto-generated method stub
		lineSensors = new ArrayList<MainWindow.SensorAndUIBind>();
		lineSensors.add(new SensorAndUIBind(lblLineN, minmaxLine, "N", LineSensorID.N));
		lineSensors.add(new SensorAndUIBind(lblLineE, minmaxLine, "E", LineSensorID.E));
		lineSensors.add(new SensorAndUIBind(lblLineS, minmaxLine, "S", LineSensorID.S));
		lineSensors.add(new SensorAndUIBind(lblLineW, minmaxLine, "W", LineSensorID.W));

		lineAmbientSensors = new ArrayList<MainWindow.SensorAndUIBind>();
		lineSensors.add(new SensorAndUIBind(lblLineAmbN, minmaxAmbient, "N", LineSensorID.N));
		lineSensors.add(new SensorAndUIBind(lblLineAmbE, minmaxAmbient, "E", LineSensorID.E));
		lineSensors.add(new SensorAndUIBind(lblLineAmbS, minmaxAmbient, "S", LineSensorID.S));
		lineSensors.add(new SensorAndUIBind(lblLineAmbW, minmaxAmbient, "W", LineSensorID.W));
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
		frame = new JFrame();
		frame.setBounds(100, 100, 586, 476);
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
					setAddressAndPort();
					rover.connect();
					btnConnect.setText("Connected");
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
		centerPanel.setBounds(0, 33, 570, 405);
		frame.getContentPane().add(centerPanel);
		centerPanel.setLayout(null);

		lblColNW = new JLabel("New label");
		lblColNW.setHorizontalAlignment(SwingConstants.CENTER);
		lblColNW.setBounds(10, 11, 100, 50);
		lblColNW.setOpaque(true);
		centerPanel.add(lblColNW);

		lblColNE = new JLabel(" NE");
		lblColNE.setHorizontalAlignment(SwingConstants.CENTER);
		lblColNE.setBounds(460, 11, 100, 50);
		lblColNE.setOpaque(true);
		centerPanel.add(lblColNE);

		lblPosLeft = new JLabel("New label");
		lblPosLeft.setHorizontalAlignment(SwingConstants.CENTER);
		lblPosLeft.setBounds(10, 151, 100, 14);
		centerPanel.add(lblPosLeft);

		JPanel panel_2 = new JPanel();
		panel_2.setBounds(123, 58, 320, 251);
		centerPanel.add(panel_2);
		lblLineNBG = new Color(200, 100, 100);
		panel_2.setLayout(new CardLayout(0, 0));

		JTabbedPane tabbedPane = new JTabbedPane(JTabbedPane.TOP);
		panel_2.add(tabbedPane, "name_1216252609521");
		// tabbedPane.setSelectedIndex(0);

		panel_1 = new JPanel();
		tabbedPane.addTab("Illuminated", null, panel_1, null);
		tabbedPane.setEnabledAt(0, true);
		GridBagLayout gbl_panel_1 = new GridBagLayout();
		gbl_panel_1.columnWidths = new int[] { 0, 0, 0, 0, 0, 0 };
		gbl_panel_1.rowHeights = new int[] { 0, 0, 0, 0, 0, 0, 0 };
		gbl_panel_1.columnWeights = new double[] { 0.0, 0.0, 0.0, 0.0, 0.0, Double.MIN_VALUE };
		gbl_panel_1.rowWeights = new double[] { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, Double.MIN_VALUE };
		panel_1.setLayout(gbl_panel_1);

		lblLineN = new JLabel("New label");
		lblLineN.setHorizontalAlignment(SwingConstants.CENTER);
		GridBagConstraints gbc_lblLineN = new GridBagConstraints();
		gbc_lblLineN.fill = GridBagConstraints.BOTH;
		gbc_lblLineN.weighty = 1.0;
		gbc_lblLineN.weightx = 1.0;
		gbc_lblLineN.insets = new Insets(0, 0, 5, 5);
		gbc_lblLineN.gridx = 2;
		gbc_lblLineN.gridy = 0;
		panel_1.add(lblLineN, gbc_lblLineN);

		lblN = new JLabel("N");
		GridBagConstraints gbc_lblN = new GridBagConstraints();
		gbc_lblN.insets = new Insets(0, 0, 5, 5);
		gbc_lblN.gridx = 2;
		gbc_lblN.gridy = 1;
		panel_1.add(lblN, gbc_lblN);

		lblLineW = new JLabel("New label");
		lblLineW.setHorizontalAlignment(SwingConstants.CENTER);
		GridBagConstraints gbc_lblLineW = new GridBagConstraints();
		gbc_lblLineW.fill = GridBagConstraints.BOTH;
		gbc_lblLineW.weighty = 1.0;
		gbc_lblLineW.weightx = 1.0;
		gbc_lblLineW.insets = new Insets(0, 0, 5, 5);
		gbc_lblLineW.gridx = 0;
		gbc_lblLineW.gridy = 2;
		panel_1.add(lblLineW, gbc_lblLineW);

		lblW = new JLabel("W");
		GridBagConstraints gbc_lblW = new GridBagConstraints();
		gbc_lblW.insets = new Insets(0, 0, 5, 5);
		gbc_lblW.gridx = 1;
		gbc_lblW.gridy = 2;
		panel_1.add(lblW, gbc_lblW);

		lblE = new JLabel("E");
		GridBagConstraints gbc_lblE = new GridBagConstraints();
		gbc_lblE.insets = new Insets(0, 0, 5, 5);
		gbc_lblE.gridx = 3;
		gbc_lblE.gridy = 2;
		panel_1.add(lblE, gbc_lblE);

		lblLineE = new JLabel("New label");
		lblLineE.setHorizontalAlignment(SwingConstants.CENTER);
		GridBagConstraints gbc_lblLineE = new GridBagConstraints();
		gbc_lblLineE.fill = GridBagConstraints.BOTH;
		gbc_lblLineE.weighty = 1.0;
		gbc_lblLineE.weightx = 1.0;
		gbc_lblLineE.insets = new Insets(0, 0, 5, 0);
		gbc_lblLineE.gridx = 4;
		gbc_lblLineE.gridy = 2;
		panel_1.add(lblLineE, gbc_lblLineE);

		lblS = new JLabel("S");
		GridBagConstraints gbc_lblS = new GridBagConstraints();
		gbc_lblS.insets = new Insets(0, 0, 5, 5);
		gbc_lblS.gridx = 2;
		gbc_lblS.gridy = 3;
		panel_1.add(lblS, gbc_lblS);

		panel_3 = new JPanel();
		tabbedPane.addTab("Ambient", null, panel_3, null);
		GridBagLayout gbl_panel_3 = new GridBagLayout();
		gbl_panel_3.columnWidths = new int[] { 0, 0, 0, 0, 0, 0 };
		gbl_panel_3.rowHeights = new int[] { 0, 0, 0, 0, 0, 0, 0 };
		gbl_panel_3.columnWeights = new double[] { 0.0, 0.0, 0.0, 1.0, 0.0, Double.MIN_VALUE };
		gbl_panel_3.rowWeights = new double[] { 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, Double.MIN_VALUE };
		panel_3.setLayout(gbl_panel_3);

		lblLineAmbN = new JLabel("New label");
		lblLineAmbN.setHorizontalAlignment(SwingConstants.CENTER);
		GridBagConstraints gbc_lblLineAmbN = new GridBagConstraints();
		gbc_lblLineAmbN.fill = GridBagConstraints.BOTH;
		gbc_lblLineAmbN.weighty = 1.0;
		gbc_lblLineAmbN.weightx = 1.0;
		gbc_lblLineAmbN.insets = new Insets(0, 0, 5, 5);
		gbc_lblLineAmbN.gridx = 2;
		gbc_lblLineAmbN.gridy = 0;
		panel_3.add(lblLineAmbN, gbc_lblLineAmbN);

		lblLineAmbN.setOpaque(true);

		lblNewLabel = new JLabel("N");
		GridBagConstraints gbc_lblNewLabel = new GridBagConstraints();
		gbc_lblNewLabel.insets = new Insets(0, 0, 5, 5);
		gbc_lblNewLabel.gridx = 2;
		gbc_lblNewLabel.gridy = 1;
		panel_3.add(lblNewLabel, gbc_lblNewLabel);

		lblLineAmbW = new JLabel("New label");
		lblLineAmbW.setHorizontalAlignment(SwingConstants.CENTER);
		GridBagConstraints gbc_lblLineAmbW = new GridBagConstraints();
		gbc_lblLineAmbW.fill = GridBagConstraints.BOTH;
		gbc_lblLineAmbW.weighty = 1.0;
		gbc_lblLineAmbW.weightx = 1.0;
		gbc_lblLineAmbW.insets = new Insets(0, 0, 5, 5);
		gbc_lblLineAmbW.gridx = 0;
		gbc_lblLineAmbW.gridy = 2;
		panel_3.add(lblLineAmbW, gbc_lblLineAmbW);

		lblNewLabel_1 = new JLabel("W");
		GridBagConstraints gbc_lblNewLabel_1 = new GridBagConstraints();
		gbc_lblNewLabel_1.insets = new Insets(0, 0, 5, 5);
		gbc_lblNewLabel_1.gridx = 1;
		gbc_lblNewLabel_1.gridy = 2;
		panel_3.add(lblNewLabel_1, gbc_lblNewLabel_1);

		lblNewLabel_2 = new JLabel("E");
		GridBagConstraints gbc_lblNewLabel_2 = new GridBagConstraints();
		gbc_lblNewLabel_2.insets = new Insets(0, 0, 5, 5);
		gbc_lblNewLabel_2.gridx = 3;
		gbc_lblNewLabel_2.gridy = 2;
		panel_3.add(lblNewLabel_2, gbc_lblNewLabel_2);

		lblLineAmbE = new JLabel("New label");
		lblLineAmbE.setHorizontalAlignment(SwingConstants.CENTER);
		GridBagConstraints gbc_lblLineAmbE = new GridBagConstraints();
		gbc_lblLineAmbE.fill = GridBagConstraints.BOTH;
		gbc_lblLineAmbE.weighty = 1.0;
		gbc_lblLineAmbE.weightx = 1.0;
		gbc_lblLineAmbE.insets = new Insets(0, 0, 5, 0);
		gbc_lblLineAmbE.gridx = 4;
		gbc_lblLineAmbE.gridy = 2;
		panel_3.add(lblLineAmbE, gbc_lblLineAmbE);

		lblNewLabel_3 = new JLabel("S");
		GridBagConstraints gbc_lblNewLabel_3 = new GridBagConstraints();
		gbc_lblNewLabel_3.insets = new Insets(0, 0, 5, 5);
		gbc_lblNewLabel_3.gridx = 2;
		gbc_lblNewLabel_3.gridy = 3;
		panel_3.add(lblNewLabel_3, gbc_lblNewLabel_3);
		lblLineWBG = new Color(200, 100, 100);
		lblLineEBG = new Color(200, 100, 100);
		lblLineSBG = new Color(200, 100, 100);

		lblPosRight = new JLabel("New label");
		lblPosRight.setHorizontalAlignment(SwingConstants.CENTER);
		lblPosRight.setBounds(460, 151, 100, 14);
		centerPanel.add(lblPosRight);

		lblColSW = new JLabel("SW");
		lblColSW.setHorizontalAlignment(SwingConstants.CENTER);
		lblColSW.setBounds(10, 315, 100, 50);
		lblColSW.setOpaque(true);
		centerPanel.add(lblColSW);

		lblColSE = new JLabel("SE");
		lblColSE.setHorizontalAlignment(SwingConstants.CENTER);
		lblColSE.setBounds(460, 315, 100, 50);
		lblColSE.setOpaque(true);
		centerPanel.add(lblColSE);
		lblLineAmbE.setOpaque(true);
		lblLineAmbW.setOpaque(true);

		lblLineAmbS = new JLabel("New label");
		lblLineAmbS.setHorizontalAlignment(SwingConstants.CENTER);
		GridBagConstraints gbc_lblLineAmbS = new GridBagConstraints();
		gbc_lblLineAmbS.fill = GridBagConstraints.BOTH;
		gbc_lblLineAmbS.weightx = 1.0;
		gbc_lblLineAmbS.weighty = 1.0;
		gbc_lblLineAmbS.insets = new Insets(0, 0, 5, 5);
		gbc_lblLineAmbS.gridx = 2;
		gbc_lblLineAmbS.gridy = 4;
		panel_3.add(lblLineAmbS, gbc_lblLineAmbS);
		lblLineAmbS.setOpaque(true);

		minmaxAmbient = new MinMaxSlider(0, 1023);
		GridBagLayout gbl_minmaxAmbient = (GridBagLayout) minmaxAmbient.getLayout();
		gbl_minmaxAmbient.rowWeights = new double[] { 0.0, 0.0 };
		gbl_minmaxAmbient.rowHeights = new int[] { 0, 0 };
		gbl_minmaxAmbient.columnWeights = new double[] { 0.0, 0.0, 1.0 };
		gbl_minmaxAmbient.columnWidths = new int[] { 0, 0, 0 };
		GridBagConstraints gbc_minmaxAmbient = new GridBagConstraints();
		gbc_minmaxAmbient.gridwidth = 5;
		gbc_minmaxAmbient.insets = new Insets(0, 0, 0, 5);
		gbc_minmaxAmbient.fill = GridBagConstraints.BOTH;
		gbc_minmaxAmbient.gridx = 0;
		gbc_minmaxAmbient.gridy = 5;
		panel_3.add(minmaxAmbient, gbc_minmaxAmbient);
		lblLineN.setOpaque(true);
		lblLineE.setOpaque(true);
		lblLineW.setOpaque(true);

		lblLineS = new JLabel("New label");
		lblLineS.setHorizontalAlignment(SwingConstants.CENTER);
		GridBagConstraints gbc_lblLineS = new GridBagConstraints();
		gbc_lblLineS.fill = GridBagConstraints.BOTH;
		gbc_lblLineS.weighty = 1.0;
		gbc_lblLineS.weightx = 1.0;
		gbc_lblLineS.insets = new Insets(0, 0, 5, 5);
		gbc_lblLineS.gridx = 2;
		gbc_lblLineS.gridy = 4;
		panel_1.add(lblLineS, gbc_lblLineS);
		lblLineS.setOpaque(true);

		minmaxLine = new MinMaxSlider(0, 1023);
		GridBagConstraints gbc_minmaxLine = new GridBagConstraints();
		gbc_minmaxLine.fill = GridBagConstraints.BOTH;
		gbc_minmaxLine.gridwidth = 5;
		gbc_minmaxLine.gridx = 0;
		gbc_minmaxLine.gridy = 5;
		panel_1.add(minmaxLine, gbc_minmaxLine);
		
		
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

			public Color getCollisionColor(boolean collision) {
				return collision ? Color.red : Color.green;
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
}
