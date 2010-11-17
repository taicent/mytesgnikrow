/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package mytotalcommander;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Desktop;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.io.IOException;
import java.util.Date;
import java.util.Vector;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.SwingUtilities;
import javax.swing.table.DefaultTableModel;


/**
 *
 * @author HotGa
 */
public class MyView extends JPanel{
    public static short LEFT = -1;
    public static short RIGHT = 1;

    private BorderLayout _borderLayout;


    private MyTable _myTable;
    
    private DefaultTableModel _tableModel;
    private JScrollPane _scrollPane;
    private TopPane _topPane;
    private FileManager _fileMan;

    private short _position;

    public MyView(short pos, final FileManager fileMan)
    {
        _borderLayout = new BorderLayout();

        _myTable = new MyTable(){
            public boolean isCellEditable(int rowIndex, int colIndex) {
                return false;   
            }
        };
       
        _scrollPane = new JScrollPane(getTable());
        _topPane = new TopPane(this);
        _position = pos;

        _fileMan = fileMan;
        Init();
    }

    public void Init()
    {
        this.setLayout(_borderLayout);
        this.add(_topPane, BorderLayout.NORTH);
        this.add(_scrollPane,BorderLayout.CENTER);

        getTable().setBackground(Color.WHITE);
        _myTable.setFillsViewportHeight(true);
        Navigate(_position == LEFT ? getFileMan().getLeftPath() : getFileMan().getRightPath(), false);
        
        getTable().addMouseListener(new MouseAdapter()
        {
            public void mouseClicked(MouseEvent event)
            {
                if(SwingUtilities.isLeftMouseButton(event))
                {
                    int row = getTable().rowAtPoint(event.getPoint());
                    if(event.getClickCount()== 2 && row >= 0 && row < getTable().getModel().getRowCount())
                    {
                        File f = (File)getTable().getModel().getValueAt(row, 0);
                        if(f.isDirectory())
                        {
                            
                            if("..".equals(f.getName()))
                            {
                                if(_position == FileManager.LEFT)
                                {
                                    Navigate(new File(getFileMan().getLeftPath().getParent()), false);
                                }
                                else
                                {
                                    Navigate(new File(getFileMan().getRightPath().getParent()), false);
                                }
                            }
                            else
                            {
                                Navigate(f, false);
                            }

                        }
                        else
                        {
                            try
                            {
                                Desktop.getDesktop().open(f);
                            }
                            catch (IOException ex)
                            {
                                Logger.getLogger(MyView.class.getName()).log(Level.SEVERE, null, ex);
                            }
                        }
                    }
                }
            }
        });

        getTable().addFocusListener(new FocusListener() {

            public void focusGained(FocusEvent e) {
                getFileMan().setFocusedPane(_position);
            }

            public void focusLost(FocusEvent e) {
                int[] selectedRows = getTable().getSelectedRows();
                for(int i = 0;i < selectedRows.length;i++)
                {
                    MyTableCellRenderer cellRenderer = (MyTableCellRenderer)getTable().getCellRenderer(selectedRows[i], 0);
                    cellRenderer.setBorder(null);

                }
                getTable().repaint();
            }

        });

    }

    public void Navigate(File dir, boolean isComboCall)
    {

        Vector files;
        if(getPosition() == LEFT)
        {
            files = getFileMan().NavigateLeftList(dir);
            _topPane.getCurrentPath().setText(getFileMan().getLeftPath().getPath());
            _topPane.setStatus(getFileMan().getLeftPath().getPath().charAt(0), isComboCall);
        }
        else
        {
            files = getFileMan().NavigateRightList(dir);
            _topPane.getCurrentPath().setText(getFileMan().getRightPath().getPath());
            _topPane.setStatus(getFileMan().getRightPath().getPath().charAt(0), isComboCall);
        }
        if(files != null)
        {
            setTableModel(new DefaultTableModel(new Object[]{"Name", "Size", "Date Modified"}, 0));
            getTableModel().setNumRows(files.size());
            getTable().setModel(getTableModel());
            for(int i = 0;i < files.size();i++)
            {
                File f = (File)files.elementAt(i);
                getTableModel().setValueAt(f, i, 0);
                getTableModel().setValueAt(f.length(), i, 1);
                getTableModel().setValueAt(new Date(f.lastModified()), i, 2);

            }
        }
        else
        {
            JOptionPane.showConfirmDialog(this, "Cannot browse this drive !", "Warning", JOptionPane.CLOSED_OPTION);
        }
    }

    /**
     * @return the _fileMan
     */
    public FileManager getFileMan() {
        return _fileMan;
    }

    /**
     * @param fileMan the _fileMan to set
     */
    public void setFileMan(FileManager fileMan) {
        this._fileMan = fileMan;
    }

    /**
     * @return the _position
     */
    public short getPosition() {
        return _position;
    }

    /**
     * @param position the _position to set
     */
    public void setPosition(short position) {
        this._position = position;
    }

    /**
     * @return the _myTable
     */
    public MyTable getTable() {
        return _myTable;
    }

    /**
     * @param myTable the _myTable to set
     */
    public void setTable(MyTable myTable) {
        this._myTable = myTable;
    }

    /**
     * @return the _tableModel
     */
    public DefaultTableModel getTableModel() {
        return _tableModel;
    }

    /**
     * @param tableModel the _tableModel to set
     */
    public void setTableModel(DefaultTableModel tableModel) {
        this._tableModel = tableModel;
    }
}
