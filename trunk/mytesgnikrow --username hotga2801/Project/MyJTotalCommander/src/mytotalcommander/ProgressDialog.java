/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package mytotalcommander;

import java.awt.BorderLayout;
import java.awt.Frame;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

/**
 *
 * @author VanLong
 */
public class ProgressDialog extends JDialog{
    private JLabel _fileProgressLabel;
    private JPanel _btnPane;
    private JPanel _progressPane;
    private JPanel _mainPane;
    private JProgressBar _progressBar;
    private JButton _cancelButton;
    private MyImplementation _myImp;

    public ProgressDialog(Frame frame, String title, boolean modal)
    {
        super(frame, title, modal);
        try
        {
          Init();
          pack();
        }
        catch(Exception ex)
        {
          ex.printStackTrace();
        }
  }

    public void Init()
    {
        _myImp = null;
        _fileProgressLabel = new JLabel("Total progress");
        _progressBar = new JProgressBar();
        _cancelButton = new JButton("Cancel");
        _btnPane = new JPanel();
        _progressPane = new JPanel();
        _mainPane = new JPanel();

        _btnPane.setLayout(new BorderLayout());
        _progressPane.setLayout(new BorderLayout());
        _mainPane.setLayout(new BorderLayout());

        _btnPane.add(_cancelButton, BorderLayout.EAST);
        _progressPane.add(_fileProgressLabel, BorderLayout.WEST);
        _progressPane.add(_progressBar, BorderLayout.CENTER);
        _mainPane.add(_progressPane, BorderLayout.NORTH);
        _mainPane.add(_btnPane, BorderLayout.SOUTH);
        getContentPane().add(_mainPane);

        _progressBar.setStringPainted(true);
        _cancelButton.addActionListener(new ActionListener() {

            public void actionPerformed(ActionEvent e) {
                ActionCancel(e);
            }
        });

        _progressBar.addChangeListener(new ChangeListener(){
            public void stateChanged(ChangeEvent e)
            {
                if(_progressBar.getValue() >= 100)
                {
                  dispose();
                }
            }
        });
    }

    public void setValue(int val)
    {
        _progressBar.setValue(val);
    }

    public void startProgress(final MyImplementation imp)
    {
        _myImp = imp;
        Thread actionThread=new Thread(new Runnable(){
          public void run(){
              _myImp.start();
          }
        });
        actionThread.start();
        this.setVisible(true);
    }

    void ActionCancel(ActionEvent e)
    {
        //_fileMan.setCancel(true);
        _myImp.cancel();
        this.dispose();
    }
}
