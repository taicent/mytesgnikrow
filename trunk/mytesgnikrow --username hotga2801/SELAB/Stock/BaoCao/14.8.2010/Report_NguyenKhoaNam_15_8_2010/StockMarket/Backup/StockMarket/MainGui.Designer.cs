namespace StockMarket
{
    partial class MainGui
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
            this.MainGui_Menu = new System.Windows.Forms.MenuStrip();
            this.MainGui_Menu_TapTin = new System.Windows.Forms.ToolStripMenuItem();
            this.MainGui_Menu_TapTin_MoTapTin = new System.Windows.Forms.ToolStripMenuItem();
            this.lưuTậpTinToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.MainGui_Menu_TapTin_Thoat = new System.Windows.Forms.ToolStripMenuItem();
            this.MainGui_Menu_Bientap = new System.Windows.Forms.ToolStripMenuItem();
            this.MainGui_Menu_ChucNang = new System.Windows.Forms.ToolStripMenuItem();
            this.MainGui_Menu_ChucNang_XacDinhDuongXuThe = new System.Windows.Forms.ToolStripMenuItem();
            this.MainGui_Menu_GiupDo = new System.Windows.Forms.ToolStripMenuItem();
            this.MainGui_Menu_LienLac = new System.Windows.Forms.ToolStripMenuItem();
            this.MainGui_CB_MACK = new System.Windows.Forms.ComboBox();
            this.MainGui_OFDialog = new System.Windows.Forms.OpenFileDialog();
            this.label1 = new System.Windows.Forms.Label();
            this.sMAToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.MainGui_Menu.SuspendLayout();
            this.SuspendLayout();
            // 
            // MainGui_Menu
            // 
            this.MainGui_Menu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MainGui_Menu_TapTin,
            this.MainGui_Menu_Bientap,
            this.MainGui_Menu_ChucNang,
            this.MainGui_Menu_GiupDo,
            this.MainGui_Menu_LienLac});
            this.MainGui_Menu.Location = new System.Drawing.Point(0, 0);
            this.MainGui_Menu.Name = "MainGui_Menu";
            this.MainGui_Menu.Size = new System.Drawing.Size(816, 24);
            this.MainGui_Menu.TabIndex = 0;
            this.MainGui_Menu.Text = "Menu chính";
            // 
            // MainGui_Menu_TapTin
            // 
            this.MainGui_Menu_TapTin.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MainGui_Menu_TapTin_MoTapTin,
            this.lưuTậpTinToolStripMenuItem,
            this.MainGui_Menu_TapTin_Thoat});
            this.MainGui_Menu_TapTin.Name = "MainGui_Menu_TapTin";
            this.MainGui_Menu_TapTin.Size = new System.Drawing.Size(59, 20);
            this.MainGui_Menu_TapTin.Text = "Tập Tin";
            // 
            // MainGui_Menu_TapTin_MoTapTin
            // 
            this.MainGui_Menu_TapTin_MoTapTin.Name = "MainGui_Menu_TapTin_MoTapTin";
            this.MainGui_Menu_TapTin_MoTapTin.Size = new System.Drawing.Size(138, 22);
            this.MainGui_Menu_TapTin_MoTapTin.Text = "Mở tập tin...";
            this.MainGui_Menu_TapTin_MoTapTin.Click += new System.EventHandler(this.MainGui_Menu_TapTin_MoTapTin_Click);
            // 
            // lưuTậpTinToolStripMenuItem
            // 
            this.lưuTậpTinToolStripMenuItem.Name = "lưuTậpTinToolStripMenuItem";
            this.lưuTậpTinToolStripMenuItem.Size = new System.Drawing.Size(138, 22);
            this.lưuTậpTinToolStripMenuItem.Text = "Lưu tập tin";
            // 
            // MainGui_Menu_TapTin_Thoat
            // 
            this.MainGui_Menu_TapTin_Thoat.Name = "MainGui_Menu_TapTin_Thoat";
            this.MainGui_Menu_TapTin_Thoat.Size = new System.Drawing.Size(138, 22);
            this.MainGui_Menu_TapTin_Thoat.Text = "Thoát";
            this.MainGui_Menu_TapTin_Thoat.Click += new System.EventHandler(this.MainGui_Menu_TapTin_Thoat_Click);
            // 
            // MainGui_Menu_Bientap
            // 
            this.MainGui_Menu_Bientap.Name = "MainGui_Menu_Bientap";
            this.MainGui_Menu_Bientap.Size = new System.Drawing.Size(65, 20);
            this.MainGui_Menu_Bientap.Text = "Biên Tập";
            // 
            // MainGui_Menu_ChucNang
            // 
            this.MainGui_Menu_ChucNang.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MainGui_Menu_ChucNang_XacDinhDuongXuThe,
            this.sMAToolStripMenuItem});
            this.MainGui_Menu_ChucNang.Name = "MainGui_Menu_ChucNang";
            this.MainGui_Menu_ChucNang.Size = new System.Drawing.Size(79, 20);
            this.MainGui_Menu_ChucNang.Text = "Chức Năng";
            // 
            // MainGui_Menu_ChucNang_XacDinhDuongXuThe
            // 
            this.MainGui_Menu_ChucNang_XacDinhDuongXuThe.Name = "MainGui_Menu_ChucNang_XacDinhDuongXuThe";
            this.MainGui_Menu_ChucNang_XacDinhDuongXuThe.Size = new System.Drawing.Size(193, 22);
            this.MainGui_Menu_ChucNang_XacDinhDuongXuThe.Text = "Xác định đường xu thế";
            this.MainGui_Menu_ChucNang_XacDinhDuongXuThe.Click += new System.EventHandler(this.MainGui_Menu_ChucNang_XacDinhDuongXuThe_Click);
            // 
            // MainGui_Menu_GiupDo
            // 
            this.MainGui_Menu_GiupDo.Name = "MainGui_Menu_GiupDo";
            this.MainGui_Menu_GiupDo.Size = new System.Drawing.Size(62, 20);
            this.MainGui_Menu_GiupDo.Text = "Giúp Đỡ";
            // 
            // MainGui_Menu_LienLac
            // 
            this.MainGui_Menu_LienLac.Name = "MainGui_Menu_LienLac";
            this.MainGui_Menu_LienLac.Size = new System.Drawing.Size(62, 20);
            this.MainGui_Menu_LienLac.Text = "Liên Lạc";
            // 
            // MainGui_CB_MACK
            // 
            this.MainGui_CB_MACK.FormattingEnabled = true;
            this.MainGui_CB_MACK.Location = new System.Drawing.Point(118, 27);
            this.MainGui_CB_MACK.Name = "MainGui_CB_MACK";
            this.MainGui_CB_MACK.Size = new System.Drawing.Size(83, 21);
            this.MainGui_CB_MACK.TabIndex = 1;
            this.MainGui_CB_MACK.SelectedIndexChanged += new System.EventHandler(this.MainGui_CB_MACK_SelectedIndexChanged);
            // 
            // MainGui_OFDialog
            // 
            this.MainGui_OFDialog.DefaultExt = "svc";
            this.MainGui_OFDialog.FileName = "Mở Tập Tin...";
            this.MainGui_OFDialog.Title = "Mở Tập Tin...";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 30);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(100, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Mã chứng khoáng :";
            // 
            // sMAToolStripMenuItem
            // 
            this.sMAToolStripMenuItem.Name = "sMAToolStripMenuItem";
            this.sMAToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.sMAToolStripMenuItem.Text = "SMA";
            this.sMAToolStripMenuItem.Click += new System.EventHandler(this.sMAToolStripMenuItem_Click);
            // 
            // MainGui
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(816, 407);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.MainGui_CB_MACK);
            this.Controls.Add(this.MainGui_Menu);
            this.MainMenuStrip = this.MainGui_Menu;
            this.Name = "MainGui";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Dự Đoán Giá Chứng Khoán";
            this.MainGui_Menu.ResumeLayout(false);
            this.MainGui_Menu.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip MainGui_Menu;
        private System.Windows.Forms.ToolStripMenuItem MainGui_Menu_TapTin;
        private System.Windows.Forms.ToolStripMenuItem MainGui_Menu_TapTin_MoTapTin;
        private System.Windows.Forms.ToolStripMenuItem lưuTậpTinToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem MainGui_Menu_TapTin_Thoat;
        private System.Windows.Forms.ToolStripMenuItem MainGui_Menu_Bientap;
        private System.Windows.Forms.ToolStripMenuItem MainGui_Menu_ChucNang;
        private System.Windows.Forms.ToolStripMenuItem MainGui_Menu_ChucNang_XacDinhDuongXuThe;
        private System.Windows.Forms.ToolStripMenuItem MainGui_Menu_GiupDo;
        private System.Windows.Forms.ToolStripMenuItem MainGui_Menu_LienLac;
        private System.Windows.Forms.ComboBox MainGui_CB_MACK;
        public System.Windows.Forms.OpenFileDialog MainGui_OFDialog;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ToolStripMenuItem sMAToolStripMenuItem;
    }
}

