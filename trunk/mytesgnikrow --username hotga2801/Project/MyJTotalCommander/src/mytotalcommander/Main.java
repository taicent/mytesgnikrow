/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package mytotalcommander;

import java.awt.EventQueue;
import javax.swing.UIManager;

/**
 *
 * @author VanLong
 */
public class Main {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        try
        {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());

            MyFrame myFrame = new MyFrame();
            myFrame.validate();
            myFrame.setSize(800,600);
            myFrame.setVisible(true);
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
        EventQueue.invokeLater(new Runnable(){
    	public void run() {
    	
    	}
    });
    }

}
