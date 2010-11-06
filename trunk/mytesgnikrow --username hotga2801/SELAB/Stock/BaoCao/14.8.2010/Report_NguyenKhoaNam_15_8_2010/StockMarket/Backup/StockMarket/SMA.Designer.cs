namespace StockMarket
{
    partial class SMA
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
            this.SMA_FilePath = new System.Windows.Forms.TextBox();
            this.SMA_SaveFile = new System.Windows.Forms.Button();
            this.SMA_Ok = new System.Windows.Forms.Button();
            this.SMA_SaveFileDlg = new System.Windows.Forms.SaveFileDialog();
            this.SuspendLayout();
            // 
            // SMA_FilePath
            // 
            this.SMA_FilePath.Location = new System.Drawing.Point(42, 35);
            this.SMA_FilePath.Name = "SMA_FilePath";
            this.SMA_FilePath.Size = new System.Drawing.Size(246, 20);
            this.SMA_FilePath.TabIndex = 0;
            // 
            // SMA_SaveFile
            // 
            this.SMA_SaveFile.Location = new System.Drawing.Point(294, 32);
            this.SMA_SaveFile.Name = "SMA_SaveFile";
            this.SMA_SaveFile.Size = new System.Drawing.Size(75, 23);
            this.SMA_SaveFile.TabIndex = 1;
            this.SMA_SaveFile.Text = "Save";
            this.SMA_SaveFile.UseVisualStyleBackColor = true;
            this.SMA_SaveFile.Click += new System.EventHandler(this.SMA_SaveFile_Click);
            // 
            // SMA_Ok
            // 
            this.SMA_Ok.Location = new System.Drawing.Point(155, 78);
            this.SMA_Ok.Name = "SMA_Ok";
            this.SMA_Ok.Size = new System.Drawing.Size(75, 23);
            this.SMA_Ok.TabIndex = 2;
            this.SMA_Ok.Text = "Ok";
            this.SMA_Ok.UseVisualStyleBackColor = true;
            this.SMA_Ok.Click += new System.EventHandler(this.SMA_Ok_Click);
            // 
            // SMA
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(407, 126);
            this.Controls.Add(this.SMA_Ok);
            this.Controls.Add(this.SMA_SaveFile);
            this.Controls.Add(this.SMA_FilePath);
            this.Name = "SMA";
            this.Text = "SMA";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox SMA_FilePath;
        private System.Windows.Forms.Button SMA_SaveFile;
        private System.Windows.Forms.Button SMA_Ok;
        private System.Windows.Forms.SaveFileDialog SMA_SaveFileDlg;
    }
}