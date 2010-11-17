/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package mytotalcommander;

import java.awt.AWTEvent;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Toolkit;
import java.io.PrintWriter;
import javax.swing.JFrame;
import javax.swing.JPanel;

/**
 *
 * @author VanLong
 */
public class MyFrame extends JFrame{

    private BorderLayout _borderLayout;
    private MyJPane _myJPane;

    public MyFrame()
    {
        enableEvents(AWTEvent.WINDOW_EVENT_MASK);
        Init();
    }

    public void Init()
    {
        try
        {

            this.setSize(Toolkit.getDefaultToolkit().getScreenSize());
            this.setTitle("My Total Commander - 0712263");

            _myJPane = new MyJPane(this);
            this.setContentPane((JPanel)_myJPane);
            
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
    }
}
