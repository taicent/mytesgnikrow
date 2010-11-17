package mytotalcommander;

import java.awt.Color;
import java.awt.Frame;
import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Arrays;
import java.util.Vector;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;
import javax.swing.JOptionPane;
import javax.swing.JProgressBar;
import javax.swing.filechooser.FileSystemView;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author VanLong
 */
public class FileManager {
    public static short LEFT = -1;
    public static short RIGHT = 1;

    private Frame _mainFrame;
    private MyJPane _myPane;
    private File _leftPath;
    private Vector _leftList;
    private Vector _rightList;
    private File _rightPath;
    private short _focusedPane;
    private FileSystemView _fileSys;

    private ProgressDialog _progressDialog;
    private long _totalSize;
    private long _writenSize;
    private boolean _isOverWriteAll;
    private boolean _isSkipAll;
    private boolean _isCancel;
    private int _numTotalDeleteFiles;
    private int _numDeletedFiles;

    public FileManager(Frame frm, MyJPane pane)
    {
        _mainFrame = frm;
        _myPane = pane;
        _fileSys = FileSystemView.getFileSystemView();
        _leftPath = _fileSys.getDefaultDirectory();
        _rightPath = _leftPath;
        _focusedPane = LEFT;
        //_progressDialog = new JProgressBar();
        //_progressDialog.setForeground(Color.red);
        _isOverWriteAll = false;
        _isSkipAll = false;
    }
    
    public Vector NavigateLeftList(File dir)
    {
        //File currentDir = new File(getLeftPath());
        File[] files = dir.listFiles();
        Vector vectorFiles = null;
        if(files != null)
        {
            vectorFiles = new Vector();
            Arrays.sort(files);
            int numDir = 0;
            for(int i = 0;i < files.length;i++)
            {
                if(files[i].isDirectory())
                {
                    vectorFiles.add(numDir++, files[i]);
                }
                else
                {
                    vectorFiles.add(files[i]);
                }
            }

            if(!_fileSys.isDrive(dir))
            {
                File upDir = new File(getRightPath() + "\\..");
                vectorFiles.add(0, upDir);
            }
            setLeftPath(dir);
        }
        else
        {
            if(!_fileSys.isDrive(dir))
            {
                vectorFiles = new Vector();
                File upDir = new File(getRightPath() + "\\..");
                vectorFiles.add(0, upDir);
                setLeftPath(dir);
            }

        }
        if(vectorFiles != null)
        {
            _leftList = vectorFiles;
        }
        return vectorFiles;
    }
    
    public Vector NavigateRightList(File dir)
    {
        //File currentDir = new File(getRightPath());
        File[] files = dir.listFiles();
        Vector vectorFiles = null;
        if(files != null)
        {
            vectorFiles = new Vector();
            Arrays.sort(files);
            int numDir = 0;
            for(int i = 0;i < files.length;i++)
            {
                if(files[i].isDirectory())
                {
                    vectorFiles.add(numDir++, files[i]);
                }
                else
                {
                    vectorFiles.add(files[i]);
                }
            }

            if(!_fileSys.isDrive(dir))
            {
                File upDir = new File(getRightPath() + "\\..");
                vectorFiles.add(0, upDir);
            }
            setRightPath(dir);
        }
        else
        {
            if(!_fileSys.isDrive(dir))
            {
                vectorFiles = new Vector();
                File upDir = new File(getRightPath() + "\\..");
                vectorFiles.add(0, upDir);
                setRightPath(dir);
            }

        }
        if(vectorFiles != null)
        {
            _rightList = vectorFiles;
        }
        return vectorFiles;
    }

    public void RefreshView()
    {
        _myPane.getLeftView().Navigate(getLeftPath(), false);
        _myPane.getRightView().Navigate(getRightPath(), false);
    }

    public void RefreshNonFocusedView()
    {
        if(_myPane.getFileMan().getFocusedPane() == FileManager.RIGHT)
        {
            _myPane.getLeftView().Navigate(getLeftPath(), false);
        }
        else
        {
            _myPane.getRightView().Navigate(getRightPath(), false);
        }
    }
    
    public boolean NewFolder(String newFolder)
    {
        if(_focusedPane == LEFT)
        {
            File newDir = new  File(getLeftPath().getPath() + "\\" + newFolder);
            return newDir.mkdir();
        }
        else
        {
            File newDir = new  File(getRightPath().getPath() + "\\" + newFolder);
            return newDir.mkdir();
        }
    }
    private void CopyDir(File tar,File dest, boolean isMove) throws IOException
    {
        File[] f = tar.listFiles();
        if(f == null)
        {
            File newDir = new  File(dest.getPath() + "\\" + tar.getName());
            newDir.mkdir();
            if(isMove && !isCancel())
            {
                tar.delete();
            }
            return;
        }
        for(int i = 0;i < f.length;i++)
        {
            if(f[i].isDirectory())
            {
                File newDir = new  File(dest.getPath() + "\\" + f[i].getName());
                newDir.mkdir();
                CopyDir(f[i], newDir, isMove);
            }
            else
            {
                File newFile = new  File(dest.getPath() + "\\" + f[i].getName());
                newFile = newFile.getAbsoluteFile();
                CopyFile(f[i], newFile);
            }
            if(isMove && !isCancel())
            {
                f[i].delete();
            }
        }
    }
    /**
     * @return the _leftPath
     */
    public File getLeftPath() {
        return _leftPath;
    }

    /**
     * @param leftPath the _leftPath to set
     */
    public void setLeftPath(File leftPath) {
        this._leftPath = leftPath;
    }

    /**
     * @return the _rightPath
     */
    public File getRightPath() {
        return _rightPath;
    }

    /**
     * @param rightPath the _rightPath to set
     */
    public void setRightPath(File rightPath) {
        this._rightPath = rightPath;
    }

    /**
     * @return the _focusedPane
     */
    public short getFocusedPane() {
        return _focusedPane;
    }

    /**
     * @param focusedPane the _focusedPane to set
     */
    public void setFocusedPane(short focusedPane) {
        this._focusedPane = focusedPane;
    }

    private void CopyFile(File tar, File newFile) throws IOException
    {
        if(newFile.exists() && !_isOverWriteAll && !_isSkipAll)
        {
            String[] buttons=new String[]{"Overwrite","Overwrite all", "Skip", "Skip all"};
            int choice = JOptionPane.showOptionDialog(null, "File " + newFile.getName() + " already exists",
                    "File exists", JOptionPane.DEFAULT_OPTION,JOptionPane.INFORMATION_MESSAGE,
                    null, buttons, buttons[1]);
            switch(choice)
            {
                case 0:
                    break;
                case 1:
                    _isOverWriteAll = true;
                    break;
                case 2:
                    _writenSize += tar.length();
                    int percentCompleted=(int)((_writenSize * 100) / _totalSize);
                    getProgressDialog().setValue(percentCompleted);
                    return;
                case 3:
                    _isSkipAll = true;
                    break;

            }
        }

        if(newFile.exists() && _isSkipAll)
        {
            _writenSize += tar.length();
            int percentCompleted=(int)((_writenSize * 100) / _totalSize);
            getProgressDialog().setValue(percentCompleted);
            return;
        }

        FileOutputStream out = null;
        FileInputStream in = null;
        try
        {
            out = new FileOutputStream(newFile);
            in = new FileInputStream(tar);
            byte[] data = new byte[4096];
            int bytesRead = 0;
            while((bytesRead = in.read(data)) >= 0)
            {
                if(isCancel())
                {
                    throw new Exception();
                }
                out.write(data,0,bytesRead);
                _writenSize += bytesRead;
                int percentCompleted=(int)((_writenSize * 100) / _totalSize);
                getProgressDialog().setValue(percentCompleted);
            }

        }
        catch(Exception ex)
        {
            if(!isCancel())
            {
                JOptionPane.showMessageDialog(null, "Error in writing "+ newFile.getPath(), "Error", JOptionPane.ERROR_MESSAGE);
            }
            if(out != null)
            {
                out.close();
            }
            newFile.delete();
        }
        finally
        {
            if(in != null)
            {
                in.close();
            }
            if(out != null)
            {
                out.close();
            }
        }
    }

    public void Copy(final File[] selectedFiles,final File destDir,final boolean isMove) throws IOException
    {
        _progressDialog = new ProgressDialog(_mainFrame, isMove ? "Move" : "Copy", true);
        _progressDialog.setLocationRelativeTo(_mainFrame);
        _progressDialog.startProgress(new MyImplementation() {

            public void start() {
                try
                {
                    CopyFiles(selectedFiles, destDir, isMove);
                }
                catch (IOException ex)
                {
                    Logger.getLogger(FileManager.class.getName()).log(Level.SEVERE, null, ex);
                }
            }

            public void cancel() {
                setCancel(true);
            }
        });
        
    }

    public void CopyFiles(File[] selectedFiles,File destDir, boolean isMove) throws IOException
    {
        String message;
        if(isMove)
        {
            message = "Do you want to move selected file(s) to " + destDir.getPath() + "?";
        }
        else
        {
            message = "Do you want to copy selected file(s) to " + destDir.getPath() + "?";
        }

        if(JOptionPane.showConfirmDialog(_progressDialog, message, "Confirm", JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION)
        {
            _totalSize = GetFilesSize(selectedFiles);
            _writenSize = 0;
            _isOverWriteAll = false;
            _isSkipAll = false;
            _isCancel = false;
            getProgressDialog().setValue(0);

            for(int i = 0;i < selectedFiles.length;i++)
            {
                File f = selectedFiles[i];
                if(FileSystemView.getFileSystemView().isParent(f, destDir) || f.getPath().equals(destDir.getPath()))
                {
                    JOptionPane.showConfirmDialog(_progressDialog, "Destination is subfolder!", "Warning", JOptionPane.CLOSED_OPTION);
                    continue;
                }
                if(f.isDirectory())
                {
                    File newDir = new  File(destDir.getPath() + "\\" + f.getName());
                    if(newDir.getPath().equals(f.getPath()))
                    {
                        JOptionPane.showConfirmDialog(_progressDialog, "Source file and destination file are the same: " + f.getPath() + " !", "Warning", JOptionPane.CLOSED_OPTION);
                    }
                    else
                    {
                        newDir.mkdir();
                        CopyDir(f, newDir, isMove);
                        if(isCancel())
                        {
                            //JOptionPane.showConfirmDialog(_mainFrame, "Cancel");
                            break;
                        }
                        if(isMove && !isCancel())
                        {
                            f.delete();
                        }
                    }

                }
                else
                {
                    File newFile = new  File(destDir.getPath() + "\\" + f.getName());
                    if(newFile.getPath().equals(f.getPath()))
                    {
                        JOptionPane.showConfirmDialog(_progressDialog, "Source file and destination file are the same: " + f.getPath() + " !", "Warning", JOptionPane.CLOSED_OPTION);
                    }
                    else
                    {
                        newFile = newFile.getAbsoluteFile();
                        CopyFile(f, newFile);
                        if(isCancel())
                        {
                            //JOptionPane.showConfirmDialog(_mainFrame, "Cancel");
                            break;
                        }
                        if(isMove && !isCancel())
                        {
                            //JOptionPane.showConfirmDialog(_mainFrame, "Cancel");
                            f.delete();
                        }
                    }
                }
            }
            RefreshView();
        }
        getProgressDialog().setValue(100);
        //getProgressBar().setVisible(false);
    }

    public void Delete(final File[] tarFiles)
    {
        _isCancel = false;
        _numTotalDeleteFiles = 0;
        _numDeletedFiles = 0;
        CheckNumDeletedFiles(tarFiles);
        
        _progressDialog = new ProgressDialog(_mainFrame, "Delete", true);
        _progressDialog.setLocationRelativeTo(_mainFrame);
        _progressDialog.startProgress(new MyImplementation() {

            public void start() {
                try
                {
                    DeleteFiles(tarFiles);
                    _progressDialog.setValue(100);
                }
                catch (Exception ex)
                {
                    ex.printStackTrace();
                }
                finally
                {
                    RefreshView();
                }
            }

            public void cancel() {
                setCancel(true);
            }
        });
        
    }

    private void CheckNumDeletedFiles(File[] tarFiles)
    {
        if(tarFiles != null)
        {
            _numTotalDeleteFiles += tarFiles.length;
            for(int i = 0;i < tarFiles.length;i++)
            {
                if(tarFiles[i].isDirectory())
                {
                    CheckNumDeletedFiles(tarFiles[i].listFiles());
                }
            }
        }
    }

    public void DeleteFiles(File[] tarFiles)
    {
        if(tarFiles != null && !isCancel())
        {
            for(int i = 0;i < tarFiles.length;i++)
            {
                if(tarFiles[i].isDirectory())
                {
                    DeleteFiles(tarFiles[i].listFiles());
                }
                tarFiles[i].delete();
                _numDeletedFiles++;
                int percentCompleted=(int)((_numDeletedFiles * 100) / _numTotalDeleteFiles);
                _progressDialog.setValue(percentCompleted);
            }
            
        }
    }

    private long GetFilesSize(File[] selectedFiles)
    {
        long totalSize = 0;
        if(selectedFiles != null)
        {
            for(int i = 0;i < selectedFiles.length;i++)
            {
                File f= selectedFiles[i];
                if(f.isDirectory())
                {
                    totalSize += GetFilesSize(f.listFiles());
                }
                else
                {
                    totalSize += f.length();
                }
            }
        }
        return totalSize;
    }

     public void ZipFile(File file, ZipOutputStream zip, String baseName) throws FileNotFoundException, IOException
     {
        BufferedInputStream buffIn = null;
        byte data[] = new byte[4096];
        String name = file.getAbsolutePath().substring(baseName.length());
        FileInputStream fi = new FileInputStream(file);
        buffIn = new BufferedInputStream(fi, 4096);
        ZipEntry zipEntry = new ZipEntry(name);
        zip.putNextEntry(zipEntry);
        int count;
        while ((count = buffIn.read(data, 0, 4096)) != -1)
        {
            zip.write(data, 0, count);
        }
        buffIn.close();

        zip.closeEntry();
    }

    public void ZipFolder(File folder, ZipOutputStream zip, String baseName) throws IOException {

        ZipEntry zipEntry = new ZipEntry(folder.getAbsolutePath().substring(baseName.length())+"/");
        zip.putNextEntry(zipEntry);

        File[] files = folder.listFiles();
        for (File file : files) {
            if (file.isDirectory())
            {
                ZipFolder(file, zip, baseName);
            }
            else
            {
                ZipFile(file, zip, baseName);
            }
        }
    }



    /**
     * @return the _progressDialog
     */
    public ProgressDialog getProgressDialog() {
        return _progressDialog;
    }

    /**
     * @param progressBar the _progressDialog to set
     */
    public void setProgressDialog(ProgressDialog progressBar) {
        this._progressDialog = progressBar;
    }

    /**
     * @return the _isCancel
     */
    public boolean isCancel() {
        return _isCancel;
    }

    /**
     * @param isCancel the _isCancel to set
     */
    public void setCancel(boolean isCancel) {
        this._isCancel = isCancel;
    }

}
