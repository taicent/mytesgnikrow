/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package mytotalcommander;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Insets;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import javax.swing.BoxLayout;
import javax.swing.DefaultListCellRenderer;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;
import javax.swing.filechooser.FileSystemView;
import sun.font.Font2D;

/**
 *
 * @author VanLong
 */
public class TopPane extends JPanel{
    private JPanel _drivesPane; // chua cac button hien thi o dia
    private JPanel _miscPane;
    private JLabel _currentPath;

    private MyView _parentPane;
    private boolean _hasFireComboEvent;

    private JButton button;

    public JLabel getCurrentPath() {
        return _currentPath;
    }

    public void setCurrentPath(JLabel _currentPath) {
        this._currentPath = _currentPath;
    }

    private JComboBox _drivesCombo;
    private JLabel _statusDrive;
    private DefaultListCellRenderer _cmbCellRenderer;

    public TopPane(MyView parent)
    {
        _parentPane = parent;
        _drivesPane = new JPanel();
        _miscPane = new JPanel(new BorderLayout());
        _drivesPane.setLayout(new BoxLayout(_drivesPane,BoxLayout.X_AXIS));

        _drivesCombo = new JComboBox();
        _cmbCellRenderer = new DefaultListCellRenderer();
        _currentPath = new JLabel();
        _statusDrive = new JLabel();
        _currentPath.setForeground(Color.BLUE);
        _hasFireComboEvent = false;
        this.setLayout(new BorderLayout());
        Init();
    }

    public void Init()
    {

        this.add(_drivesPane, BorderLayout.NORTH);
        this.add(_miscPane, BorderLayout.SOUTH);

        _miscPane.add(_drivesCombo, BorderLayout.WEST);
        _miscPane.add(_statusDrive/*, BorderLayout.EAST*/);
        _miscPane.add(_currentPath,BorderLayout.SOUTH);

        _drivesCombo.setRenderer(_cmbCellRenderer);
        _drivesCombo.setEditable(false);

        File[] drives = File.listRoots();
        for(int i = 0;i < drives.length;i++)
        {
            _drivesCombo.addItem(drives[i].getPath());
            button = new JButton(drives[i].getPath(), FileSystemView.getFileSystemView().getSystemIcon(drives[i]));
            button.setToolTipText(FileSystemView.getFileSystemView().getSystemDisplayName(drives[i]));

            button.setMargin(new Insets(2, 2, 2, 2));
            button.setFont(new Font("Default", Font.BOLD, 10));
            button.addMouseListener(new MouseAdapter() {

                @Override
                public void mouseClicked(MouseEvent e) {
                    super.mouseClicked(e);
                    JButton source = (JButton)e.getSource();
                    File dir = new File(source.getText());
                    _parentPane.Navigate(dir, false);

                }
            });
            _drivesPane.add(button);

        }
        _drivesPane.setAutoscrolls(true);
       // _drivesCombo.setSelectedIndex(3);
        _drivesCombo.addItemListener(new ItemListener(){
    		public void itemStateChanged(ItemEvent e){
    			if(e.getStateChange() == ItemEvent.DESELECTED){
    				return;
    			}
    			if(!_hasFireComboEvent)
    			{
    				_hasFireComboEvent = true;
    				return;
    			}
    			
                        _parentPane.Navigate(new File((String)_drivesCombo.getSelectedItem()), true);
                        
    		}
    	});

    }
    public void setStatus(char letter, boolean isComboCall)
    {
        for(int i = 0;i < _drivesCombo.getItemCount();i++)
        {
            String item = (String)_drivesCombo.getItemAt(i);
            JButton button = (JButton)_drivesPane.getComponent(i);
            if(item.charAt(0) == letter)
            {
                File f = new File(item);
                double freeSpace = Double.parseDouble(String.valueOf(f.getUsableSpace())) / 1000000000;
                double totalSpace = Double.parseDouble(String.valueOf(f.getTotalSpace())) / 1000000000;

                _statusDrive.setText(" [" + FileSystemView.getFileSystemView().getSystemDisplayName(f) + "] " +
                       String.format("%.2f", freeSpace)  + " GB free of " +
                        String.format("%.2f", totalSpace) + " GB");
                if(!isComboCall)
                {
                    _drivesCombo.setSelectedIndex(i);
                }
                button.setSelected(true);

            }
            else
            {
                button.setSelected(false);
            }
        }
    }
}
