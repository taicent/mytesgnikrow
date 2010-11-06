namespace StockIndexes
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
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.tbAdvice = new System.Windows.Forms.TextBox();
            this.cbStockCode = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.tbPeriod = new System.Windows.Forms.TextBox();
            this.tbConfirmDay = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.tbHandle = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.tbNotOk = new System.Windows.Forms.TextBox();
            this.tbNeutral = new System.Windows.Forms.TextBox();
            this.tbOk = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.tbReport = new System.Windows.Forms.Button();
            this.cbIsBearish = new System.Windows.Forms.CheckBox();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.Color.Blue;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(0, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(736, 31);
            this.label1.TabIndex = 0;
            this.label1.Text = "SLOW STOCHASTIC";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.tbAdvice);
            this.groupBox2.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.groupBox2.Location = new System.Drawing.Point(23, 44);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(421, 213);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Advice";
            // 
            // tbAdvice
            // 
            this.tbAdvice.Location = new System.Drawing.Point(43, 29);
            this.tbAdvice.Multiline = true;
            this.tbAdvice.Name = "tbAdvice";
            this.tbAdvice.ReadOnly = true;
            this.tbAdvice.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tbAdvice.Size = new System.Drawing.Size(333, 156);
            this.tbAdvice.TabIndex = 0;
            // 
            // cbStockCode
            // 
            this.cbStockCode.FormattingEnabled = true;
            this.cbStockCode.Location = new System.Drawing.Point(595, 62);
            this.cbStockCode.Name = "cbStockCode";
            this.cbStockCode.Size = new System.Drawing.Size(89, 21);
            this.cbStockCode.TabIndex = 3;
            this.cbStockCode.SelectedIndexChanged += new System.EventHandler(this.cbStockCode_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(474, 65);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(63, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Stock Code";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(474, 119);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(91, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Look-back Period";
            // 
            // tbPeriod
            // 
            this.tbPeriod.Location = new System.Drawing.Point(595, 116);
            this.tbPeriod.Name = "tbPeriod";
            this.tbPeriod.Size = new System.Drawing.Size(89, 20);
            this.tbPeriod.TabIndex = 6;
            this.tbPeriod.Text = "14";
            // 
            // tbConfirmDay
            // 
            this.tbConfirmDay.Location = new System.Drawing.Point(595, 166);
            this.tbConfirmDay.Name = "tbConfirmDay";
            this.tbConfirmDay.Size = new System.Drawing.Size(89, 20);
            this.tbConfirmDay.TabIndex = 9;
            this.tbConfirmDay.Text = "5";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(474, 169);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(115, 13);
            this.label7.TabIndex = 8;
            this.label7.Text = "Num of days to confirm";
            // 
            // tbHandle
            // 
            this.tbHandle.Location = new System.Drawing.Point(477, 261);
            this.tbHandle.Name = "tbHandle";
            this.tbHandle.Size = new System.Drawing.Size(75, 23);
            this.tbHandle.TabIndex = 10;
            this.tbHandle.Text = "Handle";
            this.tbHandle.UseVisualStyleBackColor = true;
            this.tbHandle.Click += new System.EventHandler(this.tbHandle_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.tbNotOk);
            this.groupBox3.Controls.Add(this.tbNeutral);
            this.groupBox3.Controls.Add(this.tbOk);
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Location = new System.Drawing.Point(23, 277);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(421, 170);
            this.groupBox3.TabIndex = 12;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Total";
            // 
            // tbNotOk
            // 
            this.tbNotOk.Location = new System.Drawing.Point(107, 135);
            this.tbNotOk.Name = "tbNotOk";
            this.tbNotOk.ReadOnly = true;
            this.tbNotOk.Size = new System.Drawing.Size(78, 20);
            this.tbNotOk.TabIndex = 5;
            // 
            // tbNeutral
            // 
            this.tbNeutral.Location = new System.Drawing.Point(107, 90);
            this.tbNeutral.Name = "tbNeutral";
            this.tbNeutral.ReadOnly = true;
            this.tbNeutral.Size = new System.Drawing.Size(78, 20);
            this.tbNeutral.TabIndex = 4;
            // 
            // tbOk
            // 
            this.tbOk.Location = new System.Drawing.Point(107, 47);
            this.tbOk.Name = "tbOk";
            this.tbOk.ReadOnly = true;
            this.tbOk.Size = new System.Drawing.Size(78, 20);
            this.tbOk.TabIndex = 3;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.ForeColor = System.Drawing.Color.Maroon;
            this.label6.Location = new System.Drawing.Point(18, 138);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(85, 13);
            this.label6.TabIndex = 2;
            this.label6.Text = "Not Ok Cases";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.label5.Location = new System.Drawing.Point(18, 93);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(86, 13);
            this.label5.TabIndex = 1;
            this.label5.Text = "Neutral Cases";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.ForeColor = System.Drawing.Color.Green;
            this.label4.Location = new System.Drawing.Point(18, 50);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(61, 13);
            this.label4.TabIndex = 0;
            this.label4.Text = "Ok Cases";
            // 
            // tbReport
            // 
            this.tbReport.Location = new System.Drawing.Point(609, 262);
            this.tbReport.Name = "tbReport";
            this.tbReport.Size = new System.Drawing.Size(75, 23);
            this.tbReport.TabIndex = 11;
            this.tbReport.Text = "Collect All";
            this.tbReport.UseVisualStyleBackColor = true;
            this.tbReport.Click += new System.EventHandler(this.tbReport_Click);
            // 
            // cbIsBearish
            // 
            this.cbIsBearish.AutoSize = true;
            this.cbIsBearish.Location = new System.Drawing.Point(477, 212);
            this.cbIsBearish.Name = "cbIsBearish";
            this.cbIsBearish.Size = new System.Drawing.Size(70, 17);
            this.cbIsBearish.TabIndex = 13;
            this.cbIsBearish.Text = "is bearish";
            this.cbIsBearish.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(737, 510);
            this.Controls.Add(this.cbIsBearish);
            this.Controls.Add(this.tbReport);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.tbHandle);
            this.Controls.Add(this.tbConfirmDay);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.tbPeriod);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.cbStockCode);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.label1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox tbAdvice;
        private System.Windows.Forms.ComboBox cbStockCode;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox tbPeriod;
        private System.Windows.Forms.TextBox tbConfirmDay;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button tbHandle;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TextBox tbNotOk;
        private System.Windows.Forms.TextBox tbNeutral;
        private System.Windows.Forms.TextBox tbOk;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button tbReport;
        private System.Windows.Forms.CheckBox cbIsBearish;
    }
}

