namespace Momentum_Oscilator
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.btnBrowse = new System.Windows.Forms.Button();
            this.cmbStockID = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.nmNumDaysPredicted = new System.Windows.Forms.NumericUpDown();
            this.nmNumPeriods = new System.Windows.Forms.NumericUpDown();
            this.nmNumDaysPeriod = new System.Windows.Forms.NumericUpDown();
            this.btnRun = new System.Windows.Forms.Button();
            this.tbxResult = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.nmNumDaysPredicted)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nmNumPeriods)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nmNumDaysPeriod)).BeginInit();
            this.SuspendLayout();
            // 
            // btnBrowse
            // 
            this.btnBrowse.Location = new System.Drawing.Point(54, 57);
            this.btnBrowse.Name = "btnBrowse";
            this.btnBrowse.Size = new System.Drawing.Size(89, 23);
            this.btnBrowse.TabIndex = 0;
            this.btnBrowse.Text = "Browse";
            this.btnBrowse.UseVisualStyleBackColor = true;
            this.btnBrowse.Click += new System.EventHandler(this.btnBrowse_Click);
            // 
            // cmbStockID
            // 
            this.cmbStockID.FormattingEnabled = true;
            this.cmbStockID.Location = new System.Drawing.Point(184, 59);
            this.cmbStockID.Name = "cmbStockID";
            this.cmbStockID.Size = new System.Drawing.Size(121, 21);
            this.cmbStockID.TabIndex = 1;
            this.cmbStockID.SelectedIndexChanged += new System.EventHandler(this.cmbStockID_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(54, 109);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(89, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Số ngày dự đoán";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(54, 145);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(124, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Số chu kỳ trong quá khứ";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(54, 181);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(84, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Số ngày/Chu kỳ";
            // 
            // nmNumDaysPredicted
            // 
            this.nmNumDaysPredicted.Location = new System.Drawing.Point(184, 107);
            this.nmNumDaysPredicted.Minimum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.nmNumDaysPredicted.Name = "nmNumDaysPredicted";
            this.nmNumDaysPredicted.Size = new System.Drawing.Size(120, 20);
            this.nmNumDaysPredicted.TabIndex = 5;
            this.nmNumDaysPredicted.Value = new decimal(new int[] {
            2,
            0,
            0,
            0});
            // 
            // nmNumPeriods
            // 
            this.nmNumPeriods.Location = new System.Drawing.Point(184, 143);
            this.nmNumPeriods.Maximum = new decimal(new int[] {
            20,
            0,
            0,
            0});
            this.nmNumPeriods.Minimum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.nmNumPeriods.Name = "nmNumPeriods";
            this.nmNumPeriods.Size = new System.Drawing.Size(120, 20);
            this.nmNumPeriods.TabIndex = 6;
            this.nmNumPeriods.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            // 
            // nmNumDaysPeriod
            // 
            this.nmNumDaysPeriod.Location = new System.Drawing.Point(184, 174);
            this.nmNumDaysPeriod.Minimum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.nmNumDaysPeriod.Name = "nmNumDaysPeriod";
            this.nmNumDaysPeriod.Size = new System.Drawing.Size(120, 20);
            this.nmNumDaysPeriod.TabIndex = 7;
            this.nmNumDaysPeriod.Value = new decimal(new int[] {
            2,
            0,
            0,
            0});
            // 
            // btnRun
            // 
            this.btnRun.Location = new System.Drawing.Point(134, 217);
            this.btnRun.Name = "btnRun";
            this.btnRun.Size = new System.Drawing.Size(75, 23);
            this.btnRun.TabIndex = 8;
            this.btnRun.Text = "Thực Hiện";
            this.btnRun.UseVisualStyleBackColor = true;
            this.btnRun.Click += new System.EventHandler(this.btnRun_Click);
            // 
            // tbxResult
            // 
            this.tbxResult.AcceptsReturn = true;
            this.tbxResult.AcceptsTab = true;
            this.tbxResult.Location = new System.Drawing.Point(54, 259);
            this.tbxResult.Multiline = true;
            this.tbxResult.Name = "tbxResult";
            this.tbxResult.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.tbxResult.Size = new System.Drawing.Size(250, 114);
            this.tbxResult.TabIndex = 9;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(364, 385);
            this.Controls.Add(this.tbxResult);
            this.Controls.Add(this.btnRun);
            this.Controls.Add(this.nmNumDaysPeriod);
            this.Controls.Add(this.nmNumPeriods);
            this.Controls.Add(this.nmNumDaysPredicted);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cmbStockID);
            this.Controls.Add(this.btnBrowse);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.nmNumDaysPredicted)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nmNumPeriods)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nmNumDaysPeriod)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnBrowse;
        private System.Windows.Forms.ComboBox cmbStockID;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown nmNumDaysPredicted;
        private System.Windows.Forms.NumericUpDown nmNumPeriods;
        private System.Windows.Forms.NumericUpDown nmNumDaysPeriod;
        private System.Windows.Forms.Button btnRun;
        private System.Windows.Forms.TextBox tbxResult;
    }
}

