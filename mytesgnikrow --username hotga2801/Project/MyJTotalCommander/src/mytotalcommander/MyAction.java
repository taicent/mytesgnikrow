/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package mytotalcommander;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Desktop;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.beans.PropertyChangeListener;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Enumeration;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipOutputStream;
import javax.swing.Action;
import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextPane;

/**
 *
 * @author VanLong
 */
public class MyAction implements Action{
    public static final short RENAME = 0;
    public static final short VIEW = 1;
    public static final short COPY = 2;
    public static final short MOVE = 3;
    public static final short NEWFOLDER = 4;
    public static final short DELETE = 5;
    public static final short CLOSE = 6;
    public static final short ZIP = 7;
    public static final short UNZIP = 8;
    public static final short ABOUT = 9;

    private short _type;
    private MyJPane _myJPane;
    

    public MyAction(short type, MyJPane pane)
    {
        setType(type);
        _myJPane = pane;
    }

    public Object getValue(String key) {
        return null;
    }

    public void putValue(String key, Object value) {
        
    }

    public void setEnabled(boolean b) {
        
    }

    public boolean isEnabled() {
        return true;
    }

    public void addPropertyChangeListener(PropertyChangeListener listener) {
        
    }

    public void removePropertyChangeListener(PropertyChangeListener listener) {
        
    }

    public void actionPerformed(ActionEvent e){
        int[] selectedRows;
        switch(_type)
        {
            case ABOUT:
                JOptionPane.showConfirmDialog(_myJPane, "Vạn Duy Thanh Long \n MSSV: 0712263", "About", JOptionPane.CLOSED_OPTION);
                break;
            case RENAME:
                selectedRows = GetSelectedRows();
                if(selectedRows.length != 1)
                {
                    JOptionPane.showConfirmDialog(_myJPane, "Choose ONLY ONE file to rename!", "Warning", JOptionPane.CLOSED_OPTION);
                }
                else
                {
                    String newName = JOptionPane.showInputDialog("Re name");
                    if(newName != null)
                    {
                        if(newName.equals(""))
                        {
                            JOptionPane.showConfirmDialog(_myJPane, "Invalid file name !", "Warning", JOptionPane.CLOSED_OPTION);
                        }
                        else
                        {
                            try
                            {
                                File selectedFile = (File)GetFocusedView().getTableModel().getValueAt(selectedRows[0], 0);
                                if(!selectedFile.isDirectory())
                                {
                                    String ext = selectedFile.getName().substring(selectedFile.getName().lastIndexOf("."));
                                    File resFile = new File(selectedFile.getParent() + "\\" + newName + ext);
                                    if(!selectedFile.renameTo(resFile))
                                    {
                                        JOptionPane.showConfirmDialog(_myJPane, newName + ext + " already existed!", "Warning",  JOptionPane.CLOSED_OPTION);
                                    }
                                }
                                else
                                {
                                    File resFile = new File(selectedFile.getParent() + "\\" + newName);
                                    if(!selectedFile.renameTo(resFile))
                                    {
                                        JOptionPane.showConfirmDialog(_myJPane, newName + " already existed!", "Warning",  JOptionPane.CLOSED_OPTION);
                                    }
                                }
                                RefreshView();
                            }
                            catch(Exception ex)
                            {
                                ex.printStackTrace();
                            }
                        }
                    }
                }
                break;
            case VIEW:
                selectedRows = GetSelectedRows();
                File selectedFile = null;
                if(selectedRows.length != 1)
                {

                    JOptionPane.showConfirmDialog(_myJPane, "Chose ONE file!", "Warning", JOptionPane.CLOSED_OPTION);
                }
                else
                {
                    selectedFile = (File)GetFocusedView().getTableModel().getValueAt(selectedRows[0], 0);
                    if(selectedFile.isDirectory())
                    {
                        return;
                    }
                    String[] buttons=new String[]{"Lister","Default program"};
                    int choice = JOptionPane.showOptionDialog(null, "Which do you want to use to view this file?",
                        "Option", JOptionPane.DEFAULT_OPTION,JOptionPane.INFORMATION_MESSAGE, null, buttons, buttons[0]);
                    if(choice == 0)
                    {
                        JFrame frame = new JFrame("Lister");
                        JEditorPane text = new JEditorPane();
                        JScrollPane panel = new JScrollPane(text);

                        panel.setSize(800,600);
                        frame.setSize(800,600);
                        frame.setContentPane(panel);
                        frame.validate();
                        frame.setVisible(true);
                        //panel.setBackground(Color.WHITE);
                        //panel.setAutoscrolls(true);
                        //text.setSize(800,600);
                        text.setAutoscrolls(true);
                        text.setEditable(false);
                        
                        
                        InputStreamReader in = null;
                        try
                        {
                            in = new InputStreamReader(new FileInputStream(selectedFile));
                            char[] data = new char[4096];
                            int bytesRead = 0;
                            String strText = "";
                            while((bytesRead = in.read(data)) >= 0)
                            {
                                String temp = "";
                                byte[] meta = new byte[bytesRead];
                                for(int i = 0;i < bytesRead;i++)
                                {
                                    meta[i] = (byte)data[i];
                                }
                                
                                temp = new String(meta, in.getEncoding());
                                strText += temp;
                                
                            }
                            text.setText(strText);
                            //text.setText(strText);
                            text.repaint();

                            //frame.repaint();
                        }
                        catch(IOException ex)
                        {
                            JOptionPane.showMessageDialog(null, "Error in reading "+ selectedFile.getPath(), "Error", JOptionPane.ERROR_MESSAGE);
                        }
                        finally
                        {
                            try
                            {
                                in.close();
                            }
                            catch (IOException ex)
                            {
                                Logger.getLogger(MyAction.class.getName()).log(Level.SEVERE, null, ex);
                            }
                            
                        }
                    }
                    else if(choice == 1)
                    {
                        try
                        {
                            Desktop.getDesktop().open(selectedFile);
                        }
                        catch (IOException ex)
                        {
                            Logger.getLogger(MyAction.class.getName()).log(Level.SEVERE, null, ex);
                        }
                    }
                }

                break;
            case COPY:
                selectedRows = GetSelectedRows();
                if(selectedRows.length == 0)
                {
                    JOptionPane.showConfirmDialog(_myJPane, "No file is selected!", "Warning", JOptionPane.CLOSED_OPTION);
                }
                else
                {
                    try
                    {
                        //_myJPane.getProgressPane().setVisible(true);

                        try
                        {
                            File[] selectedFiles = new File[selectedRows.length];
                            for(int i = 0;i < selectedRows.length;i++)
                            {
                                selectedFiles[i] = (File)GetFocusedView().getTableModel().getValueAt(selectedRows[i], 0);
                            }

                            _myJPane.getFileMan().Copy(selectedFiles,
                                    (_myJPane.getFileMan().getFocusedPane() == FileManager.LEFT) ?
                                        _myJPane.getFileMan().getRightPath() : _myJPane.getFileMan().getLeftPath(), false);
                        }
                        catch (IOException ex1)
                        {
                            Logger.getLogger(MyAction.class.getName()).log(Level.SEVERE, null, ex1);
                        }

                    }
                    catch(Exception ex)
                    {
                        ex.printStackTrace();
                    }
                    finally
                    {
                        //_myJPane.getProgressPane().setVisible(false);
                        RefreshNonFocusedView();
                    }

                }
                break;
            case MOVE:
                selectedRows = GetSelectedRows();
                if(selectedRows.length == 0)
                {
                    JOptionPane.showConfirmDialog(_myJPane, "No file is selected!", "Warning", JOptionPane.CLOSED_OPTION);
                }
                else
                {
                    try
                    {
                        //_myJPane.getProgressPane().setVisible(true);

                        try
                        {
                            File[] selectedFiles = new File[selectedRows.length];
                            for(int i = 0;i < selectedRows.length;i++)
                            {
                                selectedFiles[i] = (File)GetFocusedView().getTableModel().getValueAt(selectedRows[i], 0);
                            }

                            _myJPane.getFileMan().CopyFiles(selectedFiles,
                                    (_myJPane.getFileMan().getFocusedPane() == FileManager.LEFT) ?
                                        _myJPane.getFileMan().getRightPath() : _myJPane.getFileMan().getLeftPath(), true);
                        }
                        catch (IOException ex1)
                        {
                            Logger.getLogger(MyAction.class.getName()).log(Level.SEVERE, null, ex1);
                        }

                    }
                    catch(Exception ex)
                    {
                        ex.printStackTrace();
                    }
                    finally
                    {
                        //_myJPane.getProgressPane().setVisible(false);
                        RefreshView();
                    }

                }
                break;
            case DELETE:
                selectedRows = GetSelectedRows();
                if(selectedRows.length == 0)
                {
                    JOptionPane.showConfirmDialog(_myJPane, "No file is selected!", "Warning", JOptionPane.CLOSED_OPTION);
                }
                else
                {
                    try
                    {
                        File[] selectedFiles = new File[selectedRows.length];
                        for(int i = 0;i < selectedRows.length;i++)
                        {
                            selectedFiles[i] = (File)GetFocusedView().getTableModel().getValueAt(selectedRows[i], 0);
                        }
                        String message;
                        if(selectedFiles.length > 1)
                        {
                            message = "Do you want to delete these " + selectedFiles.length + " files?";
                        }
                        else
                        {
                            message = "Do you want to delete " + selectedFiles[0].getPath() + "?";
                        }

                        if(JOptionPane.showConfirmDialog(_myJPane, message, "Confirm", JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION)
                        {
                            _myJPane.getFileMan().Delete(selectedFiles);
                        }
                    }
                    catch(Exception ex)
                    {
                        ex.printStackTrace();
                    }
                    finally
                    {
                        RefreshView();
                    }
                }
                break;
            case NEWFOLDER:
                String newFolder = JOptionPane.showInputDialog("New Folder");
                if(newFolder != null)
                {
                    if(newFolder.equals(""))
                    {
                        JOptionPane.showConfirmDialog(_myJPane, "Invalid folder name !", "Warning", JOptionPane.CLOSED_OPTION);
                    }
                    else
                    {
                        if(!_myJPane.getFileMan().NewFolder(newFolder))
                        {
                            JOptionPane.showConfirmDialog(_myJPane, newFolder + " already existed!", "Warning",  JOptionPane.CLOSED_OPTION);
                        }
                        RefreshView();
                    }
                }
                break;
            case ZIP:
                if(JOptionPane.showConfirmDialog(_myJPane, "Do you want to zip selected file(s)?", "Confirm", JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION)
                {
                    selectedRows = GetSelectedRows();
                    if(selectedRows.length == 0)
                    {
                        JOptionPane.showConfirmDialog(_myJPane, "No file is selected!", "Warning", JOptionPane.CLOSED_OPTION);
                    }
                    else
                    {
                        try
                        {
                            File[] selectedFiles = new File[selectedRows.length];
                            for(int i = 0;i < selectedRows.length;i++)
                            {
                                selectedFiles[i] = (File)GetFocusedView().getTableModel().getValueAt(selectedRows[i], 0);
                            }

                            String workingDir = selectedFiles[0].getParent();

                            if (selectedFiles.length == 1)
                            {
                                if (selectedFiles[0].isDirectory())
                                {
                                    String outFilename = workingDir + File.separator + selectedFiles[0].getName() + ".zip";
                                    ZipOutputStream out = new ZipOutputStream(new FileOutputStream(outFilename));
                                    File zipFolder = selectedFiles[0];

                                    int len = zipFolder.getAbsolutePath().lastIndexOf(File.separator);
                                    String baseName = zipFolder.getAbsolutePath().substring(0, len + 1);
                                    _myJPane.getFileMan().ZipFolder(zipFolder, out, baseName);
                                    out.close();
                                }
                                else
                                {
                                    String fileName = selectedFiles[0].getName();
                                    int idx = fileName.lastIndexOf('.');
                                    if (idx != -1)
                                    {
                                        fileName = fileName.substring(0, idx);
                                    }
                                    String outFilename = workingDir + File.separator + fileName + ".zip";
                                    ZipOutputStream out = new ZipOutputStream(new FileOutputStream(outFilename));
                                    File zipF = selectedFiles[0];
                                    _myJPane.getFileMan().ZipFile(zipF, out, workingDir + File.separator);
                                    out.close();
                                }
                            }
                            else
                            {
                                String baseName = workingDir + File.separator;
                                String outFilename = workingDir + File.separator + new File(workingDir).getName() + ".zip";
                                ZipOutputStream out = new ZipOutputStream(new FileOutputStream(outFilename));
                                for (int i = 0; i < selectedFiles.length; i++) {
                                    if (selectedFiles[i].isDirectory()) {
                                        _myJPane.getFileMan().ZipFolder(selectedFiles[i], out, baseName);
                                    } else {
                                        _myJPane.getFileMan().ZipFile(selectedFiles[i], out, baseName);

                                    }
                                }
                                out.close();
                            }
                            RefreshView();
                        }
                        catch(Exception ex)
                        {
                            ex.printStackTrace();
                        }
                    }
                }
                break;
            case UNZIP:
                selectedRows = GetSelectedRows();
                if(selectedRows.length != 1)
                {
                    JOptionPane.showConfirmDialog(_myJPane, "Choose ONLY ONE file to unzip!", "Warning", JOptionPane.CLOSED_OPTION);
                }
                else
                {
                    try
                    {
                        File unZipFile = (File)GetFocusedView().getTableModel().getValueAt(selectedRows[0], 0);
                        if(!unZipFile.getName().substring(unZipFile.getName().lastIndexOf(".")).equals(".zip") || unZipFile.isDirectory())
                        {
                            JOptionPane.showConfirmDialog(_myJPane, "Cannot unzip this file!", "Warning", JOptionPane.CLOSED_OPTION);
                            return;
                        }

                        if(JOptionPane.showConfirmDialog(_myJPane, "Do you want to unzip " + unZipFile.getPath() + "?", "Confirm", JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION)
                        {
                            BufferedOutputStream buffOut = null;
                            BufferedInputStream buffIn = null;
                            ZipEntry entry;
                            ZipFile zipfile = new ZipFile(unZipFile);
                            Enumeration en = zipfile.entries();
                            while (en.hasMoreElements())
                            {
                                entry = (ZipEntry) en.nextElement();

                                if(!entry.isDirectory())
                                {
                                    buffIn = new BufferedInputStream(zipfile.getInputStream(entry));
                                    int count;
                                    byte data[] = new byte[4096];
                                    String outputFilename = unZipFile.getParent() + File.separator + entry.getName();
                                    //File outFile = new File(outputFilename);
                                    //outFile.createNewFile();
                                    FileOutputStream fos = new FileOutputStream(outputFilename);
                                    buffOut = new BufferedOutputStream(fos, 4096);
                                    while ((count = buffIn.read(data, 0, 4096)) != -1)
                                    {
                                        buffOut.write(data, 0, count);
                                    }
                                    buffOut.flush();
                                    buffOut.close();
                                    buffIn.close();
                                }
                                else
                                {
                                    File folder = new File(unZipFile.getParent() + File.separator + entry.getName());
                                    folder.mkdir();
                                }

                                RefreshView();
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        ex.printStackTrace();
                    }
                }
                break;
            case CLOSE:
                System.exit(0);
                break;
        }
    }

    public void RefreshView()
    {
        _myJPane.getLeftView().Navigate(_myJPane.getFileMan().getLeftPath(), false);
        _myJPane.getRightView().Navigate(_myJPane.getFileMan().getRightPath(), false);
    }

    public void RefreshNonFocusedView()
    {
        if(_myJPane.getFileMan().getFocusedPane() == FileManager.RIGHT)
        {
            _myJPane.getLeftView().Navigate(_myJPane.getFileMan().getLeftPath(), false);
        }
        else
        {
            _myJPane.getRightView().Navigate(_myJPane.getFileMan().getRightPath(), false);
        }
    }

    public int[] GetSelectedRows()
    {
        if(_myJPane.getFileMan().getFocusedPane() == FileManager.LEFT)
        {
            return _myJPane.getLeftView().getTable().getSelectedRows();
        }
        else
        {
            return _myJPane.getRightView().getTable().getSelectedRows();
        }
    }

    public MyView GetFocusedView()
    {
        return _myJPane.getFileMan().getFocusedPane() == FileManager.LEFT ? _myJPane.getLeftView() : _myJPane.getRightView();
    }

    public MyView GetNonFocusedView()
    {
        return _myJPane.getFileMan().getFocusedPane() == FileManager.RIGHT ? _myJPane.getLeftView() : _myJPane.getRightView();
    }
    /**
     * @return the _type
     */
    public short getType() {
        return _type;
    }

    /**
     * @param type the _type to set
     */
    public void setType(short type) {
        this._type = type;
    }

}
