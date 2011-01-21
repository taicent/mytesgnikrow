namespace GUI
{
    partial class AppGUI
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
            this.cmbStockID = new System.Windows.Forms.ComboBox();
            this.dtpFrom = new System.Windows.Forms.DateTimePicker();
            this.dtpTo = new System.Windows.Forms.DateTimePicker();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.lblStockID = new System.Windows.Forms.Label();
            this.lblFrom = new System.Windows.Forms.Label();
            this.lblTo = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.lblChart = new System.Windows.Forms.Label();
            this.dtpPredictDay = new System.Windows.Forms.DateTimePicker();
            this.lblPredictDay = new System.Windows.Forms.Label();
            this.btnPredict = new System.Windows.Forms.Button();
            this.tbxANNPrice = new System.Windows.Forms.TextBox();
            this.lblANNPrice = new System.Windows.Forms.Label();
            this.lblSVMPrice = new System.Windows.Forms.Label();
            this.tbxSVMPrice = new System.Windows.Forms.TextBox();
            this.tbxSVMTrend = new System.Windows.Forms.TextBox();
            this.lblSVMTrend = new System.Windows.Forms.Label();
            this.lblANNTrend = new System.Windows.Forms.Label();
            this.tbxANNTrend = new System.Windows.Forms.TextBox();
            this.gbResult = new System.Windows.Forms.GroupBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.gbResult.SuspendLayout();
            this.SuspendLayout();
            // 
            // cmbStockID
            // 
            this.cmbStockID.FormattingEnabled = true;
            this.cmbStockID.Location = new System.Drawing.Point(126, 84);
            this.cmbStockID.Name = "cmbStockID";
            this.cmbStockID.Size = new System.Drawing.Size(91, 21);
            this.cmbStockID.TabIndex = 0;
            this.cmbStockID.SelectedIndexChanged += new System.EventHandler(this.cmbStockID_SelectedIndexChanged);
            // 
            // dtpFrom
            // 
            this.dtpFrom.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            this.dtpFrom.Location = new System.Drawing.Point(293, 84);
            this.dtpFrom.Name = "dtpFrom";
            this.dtpFrom.Size = new System.Drawing.Size(81, 20);
            this.dtpFrom.TabIndex = 1;
            // 
            // dtpTo
            // 
            this.dtpTo.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            this.dtpTo.Location = new System.Drawing.Point(425, 84);
            this.dtpTo.Name = "dtpTo";
            this.dtpTo.Size = new System.Drawing.Size(81, 20);
            this.dtpTo.TabIndex = 2;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.pictureBox1.Location = new System.Drawing.Point(63, 132);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(615, 183);
            this.pictureBox1.TabIndex = 3;
            this.pictureBox1.TabStop = false;
            // 
            // lblStockID
            // 
            this.lblStockID.AutoSize = true;
            this.lblStockID.Location = new System.Drawing.Point(60, 88);
            this.lblStockID.Name = "lblStockID";
            this.lblStockID.Size = new System.Drawing.Size(49, 13);
            this.lblStockID.TabIndex = 4;
            this.lblStockID.Text = "Stock ID";
            // 
            // lblFrom
            // 
            this.lblFrom.AutoSize = true;
            this.lblFrom.Location = new System.Drawing.Point(257, 87);
            this.lblFrom.Name = "lblFrom";
            this.lblFrom.Size = new System.Drawing.Size(30, 13);
            this.lblFrom.TabIndex = 5;
            this.lblFrom.Text = "From";
            // 
            // lblTo
            // 
            this.lblTo.AutoSize = true;
            this.lblTo.Location = new System.Drawing.Point(401, 86);
            this.lblTo.Name = "lblTo";
            this.lblTo.Size = new System.Drawing.Size(20, 13);
            this.lblTo.TabIndex = 6;
            this.lblTo.Text = "To";
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(192)))));
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(1, 1);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(735, 43);
            this.label1.TabIndex = 9;
            this.label1.Text = "STOCK PREDICTION";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblChart
            // 
            this.lblChart.AutoSize = true;
            this.lblChart.Location = new System.Drawing.Point(60, 116);
            this.lblChart.Name = "lblChart";
            this.lblChart.Size = new System.Drawing.Size(32, 13);
            this.lblChart.TabIndex = 10;
            this.lblChart.Text = "Chart";
            // 
            // dtpPredictDay
            // 
            this.dtpPredictDay.Format = System.Windows.Forms.DateTimePickerFormat.Short;
            this.dtpPredictDay.Location = new System.Drawing.Point(138, 345);
            this.dtpPredictDay.Name = "dtpPredictDay";
            this.dtpPredictDay.Size = new System.Drawing.Size(79, 20);
            this.dtpPredictDay.TabIndex = 11;
            // 
            // lblPredictDay
            // 
            this.lblPredictDay.AutoSize = true;
            this.lblPredictDay.Location = new System.Drawing.Point(60, 351);
            this.lblPredictDay.Name = "lblPredictDay";
            this.lblPredictDay.Size = new System.Drawing.Size(62, 13);
            this.lblPredictDay.TabIndex = 12;
            this.lblPredictDay.Text = "Predict Day";
            // 
            // btnPredict
            // 
            this.btnPredict.Location = new System.Drawing.Point(74, 383);
            this.btnPredict.Name = "btnPredict";
            this.btnPredict.Size = new System.Drawing.Size(129, 35);
            this.btnPredict.TabIndex = 13;
            this.btnPredict.Text = "Predict";
            this.btnPredict.UseVisualStyleBackColor = true;
            this.btnPredict.Click += new System.EventHandler(this.btnPredict_Click);
            // 
            // tbxANNPrice
            // 
            this.tbxANNPrice.Location = new System.Drawing.Point(98, 27);
            this.tbxANNPrice.Name = "tbxANNPrice";
            this.tbxANNPrice.ReadOnly = true;
            this.tbxANNPrice.Size = new System.Drawing.Size(100, 20);
            this.tbxANNPrice.TabIndex = 14;
            // 
            // lblANNPrice
            // 
            this.lblANNPrice.AutoSize = true;
            this.lblANNPrice.Location = new System.Drawing.Point(22, 30);
            this.lblANNPrice.Name = "lblANNPrice";
            this.lblANNPrice.Size = new System.Drawing.Size(57, 13);
            this.lblANNPrice.TabIndex = 15;
            this.lblANNPrice.Text = "ANN Price";
            // 
            // lblSVMPrice
            // 
            this.lblSVMPrice.AutoSize = true;
            this.lblSVMPrice.Location = new System.Drawing.Point(22, 64);
            this.lblSVMPrice.Name = "lblSVMPrice";
            this.lblSVMPrice.Size = new System.Drawing.Size(57, 13);
            this.lblSVMPrice.TabIndex = 16;
            this.lblSVMPrice.Text = "SVM Price";
            // 
            // tbxSVMPrice
            // 
            this.tbxSVMPrice.Location = new System.Drawing.Point(98, 61);
            this.tbxSVMPrice.Name = "tbxSVMPrice";
            this.tbxSVMPrice.ReadOnly = true;
            this.tbxSVMPrice.Size = new System.Drawing.Size(100, 20);
            this.tbxSVMPrice.TabIndex = 17;
            // 
            // tbxSVMTrend
            // 
            this.tbxSVMTrend.Location = new System.Drawing.Point(293, 61);
            this.tbxSVMTrend.Name = "tbxSVMTrend";
            this.tbxSVMTrend.ReadOnly = true;
            this.tbxSVMTrend.Size = new System.Drawing.Size(100, 20);
            this.tbxSVMTrend.TabIndex = 21;
            // 
            // lblSVMTrend
            // 
            this.lblSVMTrend.AutoSize = true;
            this.lblSVMTrend.Location = new System.Drawing.Point(217, 64);
            this.lblSVMTrend.Name = "lblSVMTrend";
            this.lblSVMTrend.Size = new System.Drawing.Size(61, 13);
            this.lblSVMTrend.TabIndex = 20;
            this.lblSVMTrend.Text = "SVM Trend";
            // 
            // lblANNTrend
            // 
            this.lblANNTrend.AutoSize = true;
            this.lblANNTrend.Location = new System.Drawing.Point(217, 30);
            this.lblANNTrend.Name = "lblANNTrend";
            this.lblANNTrend.Size = new System.Drawing.Size(61, 13);
            this.lblANNTrend.TabIndex = 19;
            this.lblANNTrend.Text = "ANN Trend";
            // 
            // tbxANNTrend
            // 
            this.tbxANNTrend.Location = new System.Drawing.Point(293, 27);
            this.tbxANNTrend.Name = "tbxANNTrend";
            this.tbxANNTrend.ReadOnly = true;
            this.tbxANNTrend.Size = new System.Drawing.Size(100, 20);
            this.tbxANNTrend.TabIndex = 18;
            // 
            // gbResult
            // 
            this.gbResult.Controls.Add(this.tbxSVMTrend);
            this.gbResult.Controls.Add(this.tbxANNPrice);
            this.gbResult.Controls.Add(this.lblSVMTrend);
            this.gbResult.Controls.Add(this.lblANNPrice);
            this.gbResult.Controls.Add(this.lblANNTrend);
            this.gbResult.Controls.Add(this.lblSVMPrice);
            this.gbResult.Controls.Add(this.tbxANNTrend);
            this.gbResult.Controls.Add(this.tbxSVMPrice);
            this.gbResult.Location = new System.Drawing.Point(264, 337);
            this.gbResult.Name = "gbResult";
            this.gbResult.Size = new System.Drawing.Size(414, 103);
            this.gbResult.TabIndex = 22;
            this.gbResult.TabStop = false;
            this.gbResult.Text = "Result";
            // 
            // AppGUI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(737, 478);
            this.Controls.Add(this.gbResult);
            this.Controls.Add(this.btnPredict);
            this.Controls.Add(this.lblPredictDay);
            this.Controls.Add(this.dtpPredictDay);
            this.Controls.Add(this.lblChart);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.lblTo);
            this.Controls.Add(this.lblFrom);
            this.Controls.Add(this.lblStockID);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.dtpTo);
            this.Controls.Add(this.dtpFrom);
            this.Controls.Add(this.cmbStockID);
            this.Name = "AppGUI";
            this.Text = "AppGUI";
            this.Load += new System.EventHandler(this.AppGUI_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.gbResult.ResumeLayout(false);
            this.gbResult.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox cmbStockID;
        private System.Windows.Forms.DateTimePicker dtpFrom;
        private System.Windows.Forms.DateTimePicker dtpTo;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label lblStockID;
        private System.Windows.Forms.Label lblFrom;
        private System.Windows.Forms.Label lblTo;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lblChart;
        private System.Windows.Forms.DateTimePicker dtpPredictDay;
        private System.Windows.Forms.Label lblPredictDay;
        private System.Windows.Forms.Button btnPredict;
        private System.Windows.Forms.TextBox tbxANNPrice;
        private System.Windows.Forms.Label lblANNPrice;
        private System.Windows.Forms.Label lblSVMPrice;
        private System.Windows.Forms.TextBox tbxSVMPrice;
        private System.Windows.Forms.TextBox tbxSVMTrend;
        private System.Windows.Forms.Label lblSVMTrend;
        private System.Windows.Forms.Label lblANNTrend;
        private System.Windows.Forms.TextBox tbxANNTrend;
        private System.Windows.Forms.GroupBox gbResult;
    }
}