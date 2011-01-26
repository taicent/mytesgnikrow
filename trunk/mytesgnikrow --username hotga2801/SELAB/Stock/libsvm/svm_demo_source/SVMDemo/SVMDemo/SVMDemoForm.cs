//Copyright (C) 2007 Matthew Johnson

//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License along
//with this program; if not, write to the Free Software Foundation, Inc.,
//51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace SVMDemo
{
    public partial class SVMDemoForm : Form
    {
        public SVMDemoForm()
        {
            InitializeComponent();

            redToolStripMenuItem.Checked = true;
        }

        private void redToolStripMenuItem_Click(object sender, EventArgs e)
        {
            redToolStripMenuItem.Checked = true;
            greenToolStripMenuItem.Checked = false;
            blueToolStripMenuItem.Checked = false;
            cyanToolStripMenuItem.Checked = false;
            magentaToolStripMenuItem.Checked = false;
            yellowToolStripMenuItem.Checked = false;
            svmCanvas1.Class = 0;
        }

        private void greenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            redToolStripMenuItem.Checked = false;
            greenToolStripMenuItem.Checked = true;
            blueToolStripMenuItem.Checked = false;
            cyanToolStripMenuItem.Checked = false;
            magentaToolStripMenuItem.Checked = false;
            yellowToolStripMenuItem.Checked = false;
            svmCanvas1.Class = 1;
        }

        private void blueToolStripMenuItem_Click(object sender, EventArgs e)
        {
            redToolStripMenuItem.Checked = false;
            greenToolStripMenuItem.Checked = false;
            blueToolStripMenuItem.Checked = true;
            cyanToolStripMenuItem.Checked = false;
            magentaToolStripMenuItem.Checked = false;
            yellowToolStripMenuItem.Checked = false;
            svmCanvas1.Class = 2;
        }

        private void cyanToolStripMenuItem_Click(object sender, EventArgs e)
        {
            redToolStripMenuItem.Checked = false;
            greenToolStripMenuItem.Checked = false;
            blueToolStripMenuItem.Checked = false;
            cyanToolStripMenuItem.Checked = true;
            magentaToolStripMenuItem.Checked = false;
            yellowToolStripMenuItem.Checked = false;
            svmCanvas1.Class = 3;
        }

        private void magentaToolStripMenuItem_Click(object sender, EventArgs e)
        {
            redToolStripMenuItem.Checked = false;
            greenToolStripMenuItem.Checked = false;
            blueToolStripMenuItem.Checked = false;
            cyanToolStripMenuItem.Checked = false;
            magentaToolStripMenuItem.Checked = true;
            yellowToolStripMenuItem.Checked = false;
            svmCanvas1.Class = 4;
        }

        private void yellowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            redToolStripMenuItem.Checked = false;
            greenToolStripMenuItem.Checked = false;
            blueToolStripMenuItem.Checked = false;
            cyanToolStripMenuItem.Checked = false;
            magentaToolStripMenuItem.Checked = false;
            yellowToolStripMenuItem.Checked = true;
            svmCanvas1.Class = 5;
        }

        private void computeToolStripButton_Click(object sender, EventArgs e)
        {
            svmCanvas1.PerformRecognition();
        }

        private void resetToolStripButton_Click(object sender, EventArgs e)
        {
            svmCanvas1.Reset();
        }

        private void singleToolStripMenuItem_Click(object sender, EventArgs e)
        {
            svmCanvas1.Density = PointDensity.Single;
            singleToolStripMenuItem.Checked = true;
            sparseToolStripMenuItem.Checked = false;
            denseToolStripMenuItem.Checked = false;
        }

        private void sparseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            svmCanvas1.Density = PointDensity.Sparse;
            singleToolStripMenuItem.Checked = false;
            sparseToolStripMenuItem.Checked = true;
            denseToolStripMenuItem.Checked = false;
        }

        private void denseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            svmCanvas1.Density = PointDensity.Dense;
            singleToolStripMenuItem.Checked = false;
            sparseToolStripMenuItem.Checked = false;
            denseToolStripMenuItem.Checked = true;
        }
    }
}