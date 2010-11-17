/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package mytotalcommander;

import java.awt.Color;
import java.awt.Component;
import java.io.File;
import javax.swing.Icon;
import javax.swing.JLabel;
import javax.swing.JTable;
import javax.swing.UIManager;
import javax.swing.filechooser.FileSystemView;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
//import sun.net.dns.ResolverConfiguration.Options;

/**
 *
 * @author VanLong
 */
public class MyTableCellRenderer extends DefaultTableCellRenderer{

//    private LineBorder border = new LineBorder(Color.BLUE, 1, false);
    public MyTableCellRenderer() {
        super();
    }

    public Component getTableCellRendererComponent(JTable table, Object value,boolean isSelected, boolean hasFocus, int row, int column) {

  	setFont(table.getFont());
  	DefaultTableModel model= (DefaultTableModel) table.getModel();

  	setBackground(table.getBackground());
  	if(table.isRowSelected(row) && table.isFocusOwner())
        {
            setBackground(new Color(248, 227, 156));
        }
  	

  	try
        {
            if(column == table.getTableHeader().getColumnModel().getColumnIndex("Size"))
            {
                setHorizontalAlignment(JLabel.CENTER);
                if(row == 0)
                {
                    value = "<UP>";
                }
                else
                {
                    File f = (File)model.getValueAt(row, 0);
                    if (f.isDirectory())
                    {
                        value = "<DIR>";
                    }
                    else
                    {
                        value = Math.round(Double.parseDouble(value.toString())/1000) + " KB";
                    }
                }
            }
            else
            {
                setHorizontalAlignment(JLabel.LEFT);

            }
        }
        catch(Exception ignore)
        {

        }

        if(!(value instanceof File))
        {
            setIcon(null);
            setValue(value);
            
  	}
        else
        {
            File f=(File)value;
            if(!f.getName().equals(".."))
            {
                setIcon(FileSystemView.getFileSystemView().getSystemIcon(f));
            }
            else
            {
                setIcon((Icon)UIManager.get("FileChooser.upFolderIcon"));
            }
            setValue(f.getName());
  	}
  	return this;
  }
}
