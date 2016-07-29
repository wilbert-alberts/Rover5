package com.asml.innovationteam.rover;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.util.HashSet;
import java.util.Set;

import javax.swing.BoundedRangeModel;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.JTextField;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class MinMaxSlider extends JPanel {
	/**
	 * 
	 */
	private static final long serialVersionUID = 7262984433121987432L;

	public int getMin() {
		return min;
	}

	public void setMin(int min) {
		this.min = min;
		updateMin(min);
	}

	public int getMax() {
		return max;
	}

	public void setMax(int max) {
		this.max = max;
		updateMax(max);
	}

	public void addChangeListener(ChangeListener l) 
	{
		listeners.add(l);
	}
	
	public void removeChangeListener(ChangeListener l)
	{
		listeners.remove(l);
	}
	
	private void notifyListeners()
	{
		for (ChangeListener l: listeners) {
			l.stateChanged(new ChangeEvent(this));
		}
	}
	
	private JTextField fldMin;
	private JTextField fldMax;

	private int min;
	private int max;

	private int ubound;
	private int lbound;
	private BoundedRangeModel minModel;
	private BoundedRangeModel maxModel;
	private JSlider sldrMax;
	private JSlider sldrMin;
	private Set<ChangeListener> listeners = new HashSet<ChangeListener>(); 

	/**
	 * Create the panel.
	 */
	public MinMaxSlider(int l, int u) {
		if (lbound > ubound) {
			ubound = lbound + 1;
		}

		this.ubound = u;
		this.lbound = l;
		min = lbound;
		max = ubound;

		GridBagLayout gridBagLayout = new GridBagLayout();
		gridBagLayout.columnWidths = new int[] { 0, 0, 0, 0 };
		gridBagLayout.rowHeights = new int[] { 0, 0, 0 };
		gridBagLayout.columnWeights = new double[] { 0.0, 0.0, 1.0, Double.MIN_VALUE };
		gridBagLayout.rowWeights = new double[] { 0.0, 0.0, Double.MIN_VALUE };
		setLayout(gridBagLayout);

		sldrMin = new JSlider();
		sldrMin.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				int v = sldrMin.getValue();
				if ((v >= lbound) && (v < ubound - 1)) {
					updateMin(v);
				}
			}
		});
		GridBagConstraints gbc_sldrMin = new GridBagConstraints();
		gbc_sldrMin.weighty = 1.0;
		gbc_sldrMin.fill = GridBagConstraints.HORIZONTAL;
		gbc_sldrMin.weightx = 1.0;
		gbc_sldrMin.insets = new Insets(0, 0, 5, 5);
		gbc_sldrMin.gridx = 0;
		gbc_sldrMin.gridy = 0;
		add(sldrMin, gbc_sldrMin);

		JLabel lblMin = new JLabel("min");
		GridBagConstraints gbc_lblMin = new GridBagConstraints();
		gbc_lblMin.ipadx = 2;
		gbc_lblMin.anchor = GridBagConstraints.EAST;
		gbc_lblMin.gridx = 1;
		gbc_lblMin.gridy = 0;
		add(lblMin, gbc_lblMin);

		fldMin = new JTextField();
		fldMin.addKeyListener(new KeyAdapter() {
			@Override
			public void keyTyped(KeyEvent e) {
				if (e.getKeyChar() == '\n') {
					int v = Integer.parseInt(fldMin.getText());
					if ((v >= lbound) && (v < ubound - 1)) {
						updateMin(v);
						notifyListeners();
					}
				}
			}
		});
		fldMin.addFocusListener(new FocusAdapter() {
			@Override
			public void focusLost(FocusEvent e) {
				int v = Integer.parseInt(fldMin.getText());
				if ((v >= lbound) && (v < ubound - 1)) {
					updateMin(v);
					notifyListeners();
				}
			}

		});
		GridBagConstraints gbc_fldMin = new GridBagConstraints();
		gbc_fldMin.insets = new Insets(0, 0, 5, 0);
		gbc_fldMin.fill = GridBagConstraints.BOTH;
		gbc_fldMin.gridx = 2;
		gbc_fldMin.gridy = 0;
		add(fldMin, gbc_fldMin);
		fldMin.setColumns(10);

		sldrMax = new JSlider();
		sldrMax.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				int v = sldrMax.getValue();
				if ((v > lbound) && (v < ubound)) {
					updateMax(v);
					notifyListeners();
				}
			}
		});
		GridBagConstraints gbc_sldrMax = new GridBagConstraints();
		gbc_sldrMax.weighty = 1.0;
		gbc_sldrMax.fill = GridBagConstraints.HORIZONTAL;
		gbc_sldrMax.weightx = 1.0;
		gbc_sldrMax.insets = new Insets(0, 0, 0, 5);
		gbc_sldrMax.gridx = 0;
		gbc_sldrMax.gridy = 1;
		add(sldrMax, gbc_sldrMax);

		JLabel lblMax = new JLabel("max");
		GridBagConstraints gbc_lblMax = new GridBagConstraints();
		gbc_lblMax.ipadx = 2;
		gbc_lblMax.anchor = GridBagConstraints.EAST;
		gbc_lblMax.gridx = 1;
		gbc_lblMax.gridy = 1;
		add(lblMax, gbc_lblMax);

		fldMax = new JTextField();
		fldMax.addKeyListener(new KeyAdapter() {
			@Override
			public void keyTyped(KeyEvent e) {
				if (e.getKeyChar() == '\n') {
					int v = Integer.parseInt(fldMax.getText());
					if ((v > lbound) && (v < ubound)) {
						updateMax(v);
						notifyListeners();
					}
				}
			}
		});
		fldMax.addFocusListener(new FocusAdapter() {
			@Override
			public void focusLost(FocusEvent e) {
				int v = Integer.parseInt(fldMax.getText());
				if ((v > lbound) && (v < ubound)) {
					updateMax(v);
					notifyListeners();
				}
			}

		});
		GridBagConstraints gbc_fldMax = new GridBagConstraints();
		gbc_fldMax.fill = GridBagConstraints.BOTH;
		gbc_fldMax.gridx = 2;
		gbc_fldMax.gridy = 1;
		add(fldMax, gbc_fldMax);
		fldMax.setColumns(10);

		minModel = sldrMin.getModel();
		minModel.setRangeProperties(lbound, 0, lbound, ubound, false);
		maxModel = sldrMax.getModel();
		maxModel.setRangeProperties(ubound, 0, lbound, ubound, false);

	}

	private void updateMax(int v) {
		max = v;
		sldrMax.setValue(v);
		fldMax.setText(Integer.toString(v));
		
		if (min >= max) {
			min = max-1;
			updateMin(min);
		}
	}

	private void updateMin(int v) {
		min = v;
		
		sldrMin.setValue(v);
		fldMin.setText(Integer.toString(v));
		
		if (min >= max) {
			max = min+1;
			updateMax(max);
		}
	}
}
