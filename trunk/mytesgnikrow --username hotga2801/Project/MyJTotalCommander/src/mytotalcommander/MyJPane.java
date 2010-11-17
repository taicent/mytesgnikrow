/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package mytotalcommander;

import java.awt.BorderLayout;
import java.awt.Frame;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.util.Vector;
import javax.swing.ActionMap;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.KeyStroke;

/**
 *
 * @author VanLong
 */
public class MyJPane extends JPanel{

    private JPanel _btnPane;
    //private JPanel _progressPane;

    private JSplitPane _splitPane;
    private MyView _myLeftView;
    private MyView _myRightView;
    private BorderLayout _borderLayout;
    private FileManager _fileMan;

    public MyJPane(Frame frm)
    {
        
        _btnPane = new JPanel();
        //_progressPane = new JPanel();
        _splitPane = new JSplitPane();
        _borderLayout = new BorderLayout();
        _fileMan = new FileManager(frm);
        _myLeftView = new MyView(MyView.LEFT, getFileMan());
        _myRightView = new MyView(MyView.RIGHT, getFileMan());
        // thiet lap layout va add cac thanh phan
        this.setLayout(_borderLayout);
        this.add(_btnPane, BorderLayout.CENTER);
        //this.add(_progressPane, BorderLayout.SOUTH);
        //_progressPane.add(new JLabel("Progress"));
        //_progressPane.add(_fileMan.getProgressBar());
        //_progressPane.setVisible(false);
        this.add(_splitPane, BorderLayout.NORTH);
        _splitPane.add(_myLeftView, JSplitPane.LEFT);
        _splitPane.add(_myRightView, JSplitPane.RIGHT);
        _splitPane.setDividerLocation(400);
        _splitPane.setActionMap(new ActionMap());
        //_splitPane.getInputMap().clear();
        CreateCommandButtons();
    }

    private void CreateCommandButtons()
    {
        Vector buttons = new Vector();

        MyAction aboutAct = new MyAction(MyAction.ABOUT, this);
        JButton btnAbout = new JButton("About(F1)");
        btnAbout.getInputMap(WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("F1"), "aboutAct");
        btnAbout.addActionListener(aboutAct);
        btnAbout.getActionMap().put("aboutAct", aboutAct);
        buttons.addElement(btnAbout);

        MyAction renameAct = new MyAction(MyAction.RENAME, this);
        JButton btnRename = new JButton("Rename(F2)");
        btnRename.getInputMap(WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("F2"), "renameAct");
        btnRename.addActionListener(renameAct);
        btnRename.getActionMap().put("renameAct", renameAct);
        buttons.addElement(btnRename);

        MyAction viewAct = new MyAction(MyAction.VIEW, this);
        JButton btnView = new JButton("View(F3)");
        btnView.getInputMap(WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("F3"), "viewAct");
        btnView.addActionListener(viewAct);
        btnView.getActionMap().put("viewAct", viewAct);
        buttons.addElement(btnView);

        MyAction copyAct = new MyAction(MyAction.COPY, this);
        JButton btnCopy = new JButton("Copy(F5)");
        btnCopy.getInputMap(WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("F5"), "copyAct");
        btnCopy.getActionMap().put("copyAct", copyAct);
        btnCopy.addActionListener(copyAct);
        buttons.addElement(btnCopy);

        MyAction moveAct = new MyAction(MyAction.MOVE, this);
        JButton btnMove = new JButton("Move(F6)");
        btnMove.getInputMap(WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("F6"), "moveAct");
        btnMove.getActionMap().put("moveAct", moveAct);
        btnMove.addActionListener(moveAct);
        buttons.addElement(btnMove);

        MyAction mkdirAct = new MyAction(MyAction.NEWFOLDER,this);
        JButton btnMkdir = new JButton("New Folder(F7)");
        btnMkdir.getInputMap(WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("F7"), "mkdirAct");
        btnMkdir.getActionMap().put("mkdirAct", mkdirAct);
        btnMkdir.addActionListener(mkdirAct);
        buttons.addElement(btnMkdir);

        MyAction deleteAct = new MyAction(MyAction.DELETE,this);
        JButton btnDel = new JButton("Delete(F8)");
        btnDel.getInputMap(WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("F8"), "deleteAct");
        btnDel.getActionMap().put("deleteAct", deleteAct);
        btnDel.addActionListener(deleteAct);
        buttons.addElement(btnDel);

        MyAction zipAct = new MyAction(MyAction.ZIP,this);
        JButton btnZip = new JButton("Zip(F9)");
        btnZip.getInputMap(WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("F9"), "zipAct");
        btnZip.getActionMap().put("zipAct", zipAct);
        btnZip.addActionListener(zipAct);
        buttons.addElement(btnZip);

        MyAction unzipAct = new MyAction(MyAction.UNZIP,this);
        JButton btnUnzip = new JButton("Unzip(F10)");
        btnUnzip.getInputMap(WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("F10"), "unzipAct");
        btnUnzip.getActionMap().put("unzipAct", unzipAct);
        btnUnzip.addActionListener(unzipAct);
        buttons.addElement(btnUnzip);

        MyAction closeAct = new MyAction(MyAction.CLOSE, this);
        JButton btnClose = new JButton("Close(Atl+F4)");
        btnClose.getInputMap(WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(KeyEvent.VK_F4, InputEvent.ALT_MASK), "closeAct");
        btnClose.getActionMap().put("closeAct", closeAct);
        btnClose.addActionListener(closeAct);
        buttons.addElement(btnClose);

        //_btnPane.setLayout(BorderLayout.CENTER);
        for(int i = 0; i < buttons.size();i++)
        {
            getBtnPane().add((JButton)buttons.elementAt(i));

        }
    }

    /**
     * @return the _btnPane
     */
    public JPanel getBtnPane() {
        return _btnPane;
    }

    /**
     * @param btnPane the _btnPane to set
     */
    public void setBtnPane(JPanel btnPane) {
        this._btnPane = btnPane;
    }

    /**
     * @return the _splitPane
     */
    public JSplitPane getSplitPane() {
        return _splitPane;
    }

    /**
     * @param splitPane the _splitPane to set
     */
    public void setSplitPane(JSplitPane splitPane) {
        this._splitPane = splitPane;
    }

    /**
     * @return the _myLeftView
     */
    public MyView getLeftView() {
        return _myLeftView;
    }

    /**
     * @param myLeftView the _myLeftView to set
     */
    public void setLeftView(MyView myLeftView) {
        this._myLeftView = myLeftView;
    }

    /**
     * @return the _myRightView
     */
    public MyView getRightView() {
        return _myRightView;
    }

    /**
     * @param myRightView the _myRightView to set
     */
    public void setRightView(MyView myRightView) {
        this._myRightView = myRightView;
    }

    /**
     * @return the _borderLayout
     */
    public BorderLayout getBorderLayout() {
        return _borderLayout;
    }

    /**
     * @param borderLayout the _borderLayout to set
     */
    public void setBorderLayout(BorderLayout borderLayout) {
        this._borderLayout = borderLayout;
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

}
