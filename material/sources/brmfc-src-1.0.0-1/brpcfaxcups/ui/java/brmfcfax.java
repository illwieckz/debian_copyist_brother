/*
// Brother FAX share driver
// Copyright (C) 2005 Brother. Industries, Ltd.


// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place, Suite 330, Boston, MA  02111-1307  USA
*/

/***** VERSION 1.00 ****/


import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.*;
import java.math.BigDecimal;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.*;



class brFaxDialog extends JFrame implements ActionListener, KeyListener {

	JLabel display;			
	JButton button[];		
	JButton cbutton[];		
	String faxNumberStr;			
	String optionStr;			
	int not1stChar;			
	JRadioButton faxNumButton;
	JRadioButton broadcastButton;


	final String NBUTTON_NAME[] = {	
			"1" ,"2" ,"3" ,
			"4" ,"5" ,"6" ,
			"7" ,"8" ,"9" ,
			"0" ,"-" };
										// 

	final String CBUTTON_NAME[] = {
			"Send" ,"Clear" ,"Cancel"   };


        //final String DLGFONTNAME="Monospaced";
        final String DLGFONTNAME="Dialog";
        final int    DLGFONTSTYLE=Font.BOLD;

	brFaxDialog(String title) {
		super(title);
		initial();
	}
										//
	void initial() {
		JPanel dpanel = new JPanel();
		dpanel.setLayout(new GridLayout(3,1,4,4));


		display = new JLabel(" ", JLabel.RIGHT);
		display.setFont(new Font(DLGFONTNAME, DLGFONTSTYLE, 18));
		display.setForeground(Color.white);
		display.setBackground(Color.black);
		display.setOpaque(true);

		dpanel.add(display);
		

		ButtonGroup group = new ButtonGroup();
		faxNumButton    = new JRadioButton("Fax number",true); 
		broadcastButton = new JRadioButton("Group Send",false); 
		broadcastButton.setFont(new Font(DLGFONTNAME, DLGFONTSTYLE, 14));
		faxNumButton.setFont(new Font(DLGFONTNAME, DLGFONTSTYLE, 14));
		faxNumButton.addKeyListener(this);
		broadcastButton.addKeyListener(this);
		group.add(faxNumButton);
		group.add(broadcastButton);

		dpanel.add(faxNumButton);
		dpanel.add(broadcastButton);
		dpanel.addKeyListener(this);

		JPanel npanel = new JPanel();
		npanel.setLayout(new GridLayout(4,3, 4,4));
		button = new JButton[20];
		
		for (int i = 0 ; i < 11 ; i++) {
			button[i] = new JButton(NBUTTON_NAME[i]);
			button[i].setFont(new Font(DLGFONTNAME, DLGFONTSTYLE, 16));
			//			if ((i % 5) == 3) 
			//			if ((i % 5) == 4) button[i].setForeground(Color.red);
			button[i].setFocusPainted(false);
			button[i].addActionListener(this);
			button[i].addKeyListener(this);
			if (NBUTTON_NAME[i].equals(" ")) button[i].setVisible(false);
			npanel.add(button[i]);
			
			addKeyListener(this);
			
		}

		Container container = getContentPane();
		container.setLayout(new BorderLayout());
		container.add(dpanel, BorderLayout.NORTH);
		container.add(npanel, BorderLayout.CENTER);
			


	    	JPanel cpanel = new JPanel();
		cpanel.setLayout(new GridLayout(3,1, 4,4));
		cbutton = new JButton[20];
		
		for (int i = 0 ; i < 3 ; i++) {
			cbutton[i] = new JButton(CBUTTON_NAME[i]);
			cbutton[i].setFont(new Font(DLGFONTNAME, DLGFONTSTYLE, 16));
			cbutton[i].setFocusPainted(false);
			cbutton[i].addActionListener(this);
			cbutton[i].addKeyListener(this);
			cbutton[i].setForeground(Color.blue);
			//if (CBUTTON_NAME[i].equals(" ")) cbutton[i].setVisible(false);
			cpanel.add(cbutton[i]);
			
			addKeyListener(this);
			
		}
			
		faxNumberStr = "0";
		not1stChar = 0;

		container.add(cpanel, BorderLayout.EAST);
		container.addKeyListener(this);

	}
	
	/*
	 *
	 */

        boolean  check_control_key(String s){
		if (  s.equals("\b") ||
		      s.equals(CBUTTON_NAME[0]) ||
		      s.equals(CBUTTON_NAME[1]) ||
		      s.equals(CBUTTON_NAME[2]) ){
		    return true;
		}
		return false;
        }

	public void actionPerformed(ActionEvent arg0) {
		//

	    String s = arg0.getActionCommand();
	    if(faxNumButton.isSelected()){
		if (Character.isDigit(s.charAt(0))) faxNumberWork(s);
		if (s.equals("-")) faxNumberWork(s);
	    }
	    else{
		if (!(check_control_key(s))){
		    BroadcastDBWork(s);
		}
	    }

	    if (s.equals("Send")){
		SendWork(s);
	    }
	    else if (s.equals("Clear")){
	       	ClearWork(s);
	    }
	    else if (s.equals("Cancel")){
		CancelWork(s);
	    }
	}

	/*
	 *
	 */
	public void keyTyped(KeyEvent arg0) {
	    clickButton(String.valueOf(arg0.getKeyChar()));
	}

	/*
	 *
	 */
	public void keyPressed(KeyEvent arg0) {
		if (arg0.getKeyCode() == KeyEvent.VK_ENTER)  clickButton("Send");
		if (arg0.getKeyCode() == KeyEvent.VK_DELETE) clickButton("Cancel");
		if (arg0.getKeyCode() == KeyEvent.VK_BACK_SPACE) delete1char();
	}

	/*
	 *
	 */
	public void keyReleased(KeyEvent arg0) {


	}		



	void clickButton(String s) {
	    if(faxNumButton.isSelected()){
		for (int i = 0 ; i < 11 ; i++) {
			if (s.equals(NBUTTON_NAME[i]))  button[i].doClick(100);
		}
	    }
	    else{
		if (!(check_control_key(s))){
		    BroadcastDBWork(s);
		}
	    }
	    for (int i = 0 ; i < 3 ; i++) {
	    	if (s.equals(CBUTTON_NAME[i])) cbutton[i].doClick(100);
	    }
	}
										


	void faxDispString(String s) {
	    switch (not1stChar) {
	    case 0: 
	        faxNumberStr = s;
	        setFaxNumberDisplay(faxNumberStr);
	        not1stChar = 1;
	        break;
	    case 1: 
	        if (faxNumberStr.length() < 60) {
		    faxNumberStr += s;
		    setFaxNumberDisplay(faxNumberStr);
		}
	        break;
	    }
	}

        void delete1char(){
	    if(faxNumberStr.length() < 1) {
	        return ;
	    }
	    faxNumberStr = faxNumberStr.substring(0,faxNumberStr.length()-1);
	    setFaxNumberDisplay(faxNumberStr);
        }


	void faxNumberWork(String s) {
	    faxDispString(s);
	}

	void BroadcastDBWork(String s) {
	    faxDispString(s);
	}



	void setFaxNumberDisplay(String s) {
		display.setText(s);
	}




	void SendWork(String s){
	    if(faxNumButton.isSelected()){
		optionStr = "-o fax-number=" + faxNumberStr;
	    }
	    else{
		optionStr = "-o fax-broadcast=" + faxNumberStr;
	    }
	    System.out.println(optionStr);
	    System.exit(0);
	}

	void ClearWork(String s){
	    	faxNumberStr = "0";
	    	not1stChar = 0;
	    	setFaxNumberDisplay("");
        }
	
        void CancelWork(String s){      
	    System.exit(0);
	}

}





public class brmfcfax {

	public static void main(String[] args) {
		
		brFaxDialog faxUIFrame;
		
		faxUIFrame = new brFaxDialog("brpcfax");
		faxUIFrame.setDefaultCloseOperation(brFaxDialog.EXIT_ON_CLOSE);
		faxUIFrame.setSize(350, 200);
		faxUIFrame.setVisible(true);
	}
}
