package com.asml.innovationteam.rover;

import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.JPanel;
import java.awt.BorderLayout;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.JToggleButton;
import java.awt.GridBagLayout;
import javax.swing.JButton;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import java.awt.event.ItemListener;
import java.awt.event.ItemEvent;

public class MainWindow {

	private JFrame frame;
	private JTextField txtAddress;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					MainWindow window = new MainWindow();
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
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		frame = new JFrame();
		frame.setBounds(100, 100, 450, 300);
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
				System.err.println("Connect pressed");
			}
		});
		panel.add(btnConnect);
		
		JPanel lblTrackRight = new JPanel();
		frame.getContentPane().add(lblTrackRight, BorderLayout.CENTER);
		GridBagLayout gbl_lblTrackRight = new GridBagLayout();
		gbl_lblTrackRight.columnWidths = new int[] {0, 0, 0, 3};
		gbl_lblTrackRight.rowHeights = new int[] {0, 0, 0, 3};
		gbl_lblTrackRight.columnWeights = new double[]{0.0, 1.0, 0.0, Double.MIN_VALUE};
		gbl_lblTrackRight.rowWeights = new double[]{0.0, 1.0, 0.0, Double.MIN_VALUE};
		lblTrackRight.setLayout(gbl_lblTrackRight);
		
		JButton btnColNW = new JButton("NW");
		GridBagConstraints gbc_btnColNW = new GridBagConstraints();
		gbc_btnColNW.insets = new Insets(0, 0, 5, 5);
		gbc_btnColNW.gridx = 0;
		gbc_btnColNW.gridy = 0;
		lblTrackRight.add(btnColNW, gbc_btnColNW);
		
		JButton btnColNE = new JButton(" NE");
		GridBagConstraints gbc_btnColNE = new GridBagConstraints();
		gbc_btnColNE.insets = new Insets(0, 0, 5, 0);
		gbc_btnColNE.gridx = 2;
		gbc_btnColNE.gridy = 0;
		lblTrackRight.add(btnColNE, gbc_btnColNE);
		
		JLabel lblTrackLeft = new JLabel("New label");
		GridBagConstraints gbc_lblTrackLeft = new GridBagConstraints();
		gbc_lblTrackLeft.insets = new Insets(0, 0, 5, 5);
		gbc_lblTrackLeft.gridx = 0;
		gbc_lblTrackLeft.gridy = 1;
		lblTrackRight.add(lblTrackLeft, gbc_lblTrackLeft);
		
		JPanel panel_2 = new JPanel();
		GridBagConstraints gbc_panel_2 = new GridBagConstraints();
		gbc_panel_2.insets = new Insets(0, 0, 5, 5);
		gbc_panel_2.gridx = 1;
		gbc_panel_2.gridy = 1;
		lblTrackRight.add(panel_2, gbc_panel_2);
		GridBagLayout gbl_panel_2 = new GridBagLayout();
		gbl_panel_2.columnWidths = new int[] {0, 0, 0, 3};
		gbl_panel_2.rowHeights = new int[] {0, 0, 0, 3};
		gbl_panel_2.columnWeights = new double[]{0.0, 0.0, 0.0, Double.MIN_VALUE};
		gbl_panel_2.rowWeights = new double[]{0.0, 0.0, 0.0, Double.MIN_VALUE};
		panel_2.setLayout(gbl_panel_2);
		
		JButton btnLineN = new JButton("N");
		GridBagConstraints gbc_btnLineN = new GridBagConstraints();
		gbc_btnLineN.insets = new Insets(0, 0, 5, 5);
		gbc_btnLineN.gridx = 1;
		gbc_btnLineN.gridy = 0;
		panel_2.add(btnLineN, gbc_btnLineN);
		
		JButton btnLineW = new JButton("W");
		GridBagConstraints gbc_btnLineW = new GridBagConstraints();
		gbc_btnLineW.insets = new Insets(0, 0, 5, 5);
		gbc_btnLineW.gridx = 0;
		gbc_btnLineW.gridy = 1;
		panel_2.add(btnLineW, gbc_btnLineW);
		
		JButton btnLineE = new JButton("E");
		GridBagConstraints gbc_btnLineE = new GridBagConstraints();
		gbc_btnLineE.insets = new Insets(0, 0, 5, 0);
		gbc_btnLineE.gridx = 2;
		gbc_btnLineE.gridy = 1;
		panel_2.add(btnLineE, gbc_btnLineE);
		
		JButton btnLineS = new JButton("S");
		GridBagConstraints gbc_btnLineS = new GridBagConstraints();
		gbc_btnLineS.insets = new Insets(0, 0, 0, 5);
		gbc_btnLineS.gridx = 1;
		gbc_btnLineS.gridy = 2;
		panel_2.add(btnLineS, gbc_btnLineS);
		
		JLabel lblNewLabel_1 = new JLabel("New label");
		GridBagConstraints gbc_lblNewLabel_1 = new GridBagConstraints();
		gbc_lblNewLabel_1.insets = new Insets(0, 0, 5, 0);
		gbc_lblNewLabel_1.gridx = 2;
		gbc_lblNewLabel_1.gridy = 1;
		lblTrackRight.add(lblNewLabel_1, gbc_lblNewLabel_1);
		
		JButton btnColSW = new JButton("SW");
		GridBagConstraints gbc_btnColSW = new GridBagConstraints();
		gbc_btnColSW.insets = new Insets(0, 0, 0, 5);
		gbc_btnColSW.gridx = 0;
		gbc_btnColSW.gridy = 2;
		lblTrackRight.add(btnColSW, gbc_btnColSW);
		
		JButton btnColSE = new JButton("SE");
		GridBagConstraints gbc_btnColSE = new GridBagConstraints();
		gbc_btnColSE.gridx = 2;
		gbc_btnColSE.gridy = 2;
		lblTrackRight.add(btnColSE, gbc_btnColSE);
	}

}
