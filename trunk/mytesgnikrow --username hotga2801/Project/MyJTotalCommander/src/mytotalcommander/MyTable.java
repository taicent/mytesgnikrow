/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package mytotalcommander;

import javax.swing.ActionMap;
import javax.swing.JComponent;
import javax.swing.JTable;
import javax.swing.table.TableCellRenderer;

/**
 *
 * @author VanLong
 */
public class MyTable extends JTable{

    private MyTableCellRenderer _tableCellRenderer;

    public MyTable()
    {
        _tableCellRenderer = new MyTableCellRenderer();
        this.getTableHeader().setReorderingAllowed(false);
        this.setShowGrid(false);
        this.setColumnSelectionAllowed(false);

        this.getInputMap(JTable.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT).clear();
        this.getInputMap().clear();
        this.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).clear();
        this.setActionMap(new ActionMap());
    }


    @Override
    public TableCellRenderer getCellRenderer(int row, int column) {
        return _tableCellRenderer;
    }

}
