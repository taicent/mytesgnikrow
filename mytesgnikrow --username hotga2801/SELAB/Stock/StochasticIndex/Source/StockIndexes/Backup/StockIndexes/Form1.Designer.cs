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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.tbAdvice = new System.Windows.Forms.TextBox();
            this.cbStockCode = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.tbPeriod = new System.Windows.Forms.TextBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.tbOk = new System.Windows.Forms.TextBox();
            this.tbNeutral = new System.Windows.Forms.TextBox();
            this.tbNotOk = new System.Windows.Forms.TextBox();
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
            this.label1.Size = new System.Drawing.Size(694, 31);
            this.label1.TabIndex = 0;
            this.label1.Text = "SLOW STOCHASTIC";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // groupBox1
            // 
            this.groupBox1.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.groupBox1.Location = new System.Drawing.Point(27, 54);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(432, 149);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Graph";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.tbAdvice);
            this.groupBox2.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.groupBox2.Location = new System.Drawing.Point(27, 223);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(432, 210);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Advice";
            // 
            // tbAdvice
            // 
            this.tbAdvice.Location = new System.Drawing.Point(48, 29);
            this.tbAdvice.Multiline = true;
            this.tbAdvice.Name = "tbAdvice";
            this.tbAdvice.ReadOnly = true;
            this.tbAdvice.Size = new System.Drawing.Size(333, 156);
            this.tbAdvice.TabIndex = 0;
            // 
            // cbStockCode
            // 
            this.cbStockCode.FormattingEnabled = true;
            this.cbStockCode.Location = new System.Drawing.Point(563, 97);
            this.cbStockCode.Name = "cbStockCode";
            this.cbStockCode.Size = new System.Drawing.Size(121, 21);
            this.cbStockCode.TabIndex = 3;
            this.cbStockCode.SelectedIndexChanged += new System.EventHandler(this.cbStockCode_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(474, 100);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(63, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Stock Code";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(474, 154);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(91, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Look-back Period";
            // 
            // tbPeriod
            // 
            this.tbPeriod.Location = new System.Drawing.Point(563, 151);
            this.tbPeriod.Name = "tbPeriod";
            this.tbPeriod.Size = new System.Drawing.Size(121, 20);
            this.tbPeriod.TabIndex = 6;
            this.tbPeriod.Text = "14";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.tbNotOk);
            this.groupBox3.Controls.Add(this.tbNeutral);
            this.groupBox3.Controls.Add(this.tbOk);
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Location = new System.Drawing.Point(484, 223);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(200, 210);
            this.groupBox3.TabIndex = 7;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Total";
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
            // tbOk
            // 
            this.tbOk.Location = new System.Drawing.Point(107, 47);
            this.tbOk.Name = "tbOk";
            this.tbOk.ReadOnly = true;
            this.tbOk.Size = new System.Drawing.Size(78, 20);
            this.tbOk.TabIndex = 3;
            // 
            // tbNeutral
            // 
            this.tbNeutral.Location = new System.Drawing.Point(107, 90);
            this.tbNeutral.Name = "tbNeutral";
            this.tbNeutral.ReadOnly = true;
            this.tbNeutral.Size = new System.Drawing.Size(78, 20);
            this.tbNeutral.TabIndex = 4;
            // 
            // tbNotOk
            // 
            this.tbNotOk.Location = new System.Drawing.Point(107, 135);
            this.tbNotOk.Name = "tbNotOk";
            this.tbNotOk.ReadOnly = true;
            this.tbNotOk.Size = new System.Drawing.Size(78, 20);
            this.tbNotOk.TabIndex = 5;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(694, 467);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.tbPeriod);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.cbStockCode);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
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
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox tbAdvice;
        private System.Windows.Forms.ComboBox cbStockCode;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox tbPeriod;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TextBox tbNotOk;
        private System.Windows.Forms.TextBox tbNeutral;
        private System.Windows.Forms.TextBox tbOk;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
    }
}

