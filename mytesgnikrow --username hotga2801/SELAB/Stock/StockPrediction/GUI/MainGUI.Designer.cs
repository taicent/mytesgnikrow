namespace GUI
{
    partial class MainGUI
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
            this.rdTrendPrediction = new System.Windows.Forms.RadioButton();
            this.rdPricePrediction = new System.Windows.Forms.RadioButton();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.rdSVR = new System.Windows.Forms.RadioButton();
            this.rdANN = new System.Windows.Forms.RadioButton();
            this.tbxPathTrain = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.btnBrowserTrain = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.btnBrowserTest = new System.Windows.Forms.Button();
            this.tbxPathTest = new System.Windows.Forms.TextBox();
            this.btnTest = new System.Windows.Forms.Button();
            this.btnTrain = new System.Windows.Forms.Button();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.ckbNMSE = new System.Windows.Forms.CheckBox();
            this.ckbMSE = new System.Windows.Forms.CheckBox();
            this.ckbMAPE = new System.Windows.Forms.CheckBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(192)))));
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(0, 1);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(744, 43);
            this.label1.TabIndex = 0;
            this.label1.Text = "STOCK PREDICTION";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.rdTrendPrediction);
            this.groupBox1.Controls.Add(this.rdPricePrediction);
            this.groupBox1.Location = new System.Drawing.Point(47, 62);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(200, 92);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Prediction Type";
            // 
            // rdTrendPrediction
            // 
            this.rdTrendPrediction.AutoSize = true;
            this.rdTrendPrediction.Location = new System.Drawing.Point(58, 54);
            this.rdTrendPrediction.Name = "rdTrendPrediction";
            this.rdTrendPrediction.Size = new System.Drawing.Size(103, 17);
            this.rdTrendPrediction.TabIndex = 1;
            this.rdTrendPrediction.TabStop = true;
            this.rdTrendPrediction.Text = "Trend Prediction";
            this.rdTrendPrediction.UseVisualStyleBackColor = true;
            // 
            // rdPricePrediction
            // 
            this.rdPricePrediction.AutoSize = true;
            this.rdPricePrediction.Location = new System.Drawing.Point(58, 27);
            this.rdPricePrediction.Name = "rdPricePrediction";
            this.rdPricePrediction.Size = new System.Drawing.Size(99, 17);
            this.rdPricePrediction.TabIndex = 0;
            this.rdPricePrediction.TabStop = true;
            this.rdPricePrediction.Text = "Price Prediction";
            this.rdPricePrediction.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btnTrain);
            this.groupBox2.Controls.Add(this.comboBox1);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.rdSVR);
            this.groupBox2.Controls.Add(this.rdANN);
            this.groupBox2.Controls.Add(this.btnBrowserTrain);
            this.groupBox2.Controls.Add(this.tbxPathTrain);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.ForeColor = System.Drawing.Color.Black;
            this.groupBox2.Location = new System.Drawing.Point(12, 175);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(337, 285);
            this.groupBox2.TabIndex = 4;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Training";
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(86, 147);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(121, 21);
            this.comboBox1.TabIndex = 5;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(17, 150);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(60, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Preprocess";
            // 
            // rdSVR
            // 
            this.rdSVR.AutoSize = true;
            this.rdSVR.Location = new System.Drawing.Point(177, 91);
            this.rdSVR.Name = "rdSVR";
            this.rdSVR.Size = new System.Drawing.Size(47, 17);
            this.rdSVR.TabIndex = 3;
            this.rdSVR.TabStop = true;
            this.rdSVR.Text = "SVR";
            this.rdSVR.UseVisualStyleBackColor = true;
            // 
            // rdANN
            // 
            this.rdANN.AutoSize = true;
            this.rdANN.Location = new System.Drawing.Point(86, 91);
            this.rdANN.Name = "rdANN";
            this.rdANN.Size = new System.Drawing.Size(48, 17);
            this.rdANN.TabIndex = 2;
            this.rdANN.TabStop = true;
            this.rdANN.Text = "ANN";
            this.rdANN.UseVisualStyleBackColor = true;
            // 
            // tbxPathTrain
            // 
            this.tbxPathTrain.Location = new System.Drawing.Point(20, 33);
            this.tbxPathTrain.Name = "tbxPathTrain";
            this.tbxPathTrain.Size = new System.Drawing.Size(199, 20);
            this.tbxPathTrain.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(17, 93);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(36, 13);
            this.label2.TabIndex = 0;
            this.label2.Text = "Model";
            // 
            // btnBrowserTrain
            // 
            this.btnBrowserTrain.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnBrowserTrain.Location = new System.Drawing.Point(234, 19);
            this.btnBrowserTrain.Name = "btnBrowserTrain";
            this.btnBrowserTrain.Size = new System.Drawing.Size(83, 34);
            this.btnBrowserTrain.TabIndex = 2;
            this.btnBrowserTrain.Text = "Browser...";
            this.btnBrowserTrain.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.groupBox4);
            this.groupBox3.Controls.Add(this.btnTest);
            this.groupBox3.Controls.Add(this.btnBrowserTest);
            this.groupBox3.Controls.Add(this.tbxPathTest);
            this.groupBox3.Location = new System.Drawing.Point(396, 175);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(337, 285);
            this.groupBox3.TabIndex = 7;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Testing";
            // 
            // btnBrowserTest
            // 
            this.btnBrowserTest.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnBrowserTest.Location = new System.Drawing.Point(234, 19);
            this.btnBrowserTest.Name = "btnBrowserTest";
            this.btnBrowserTest.Size = new System.Drawing.Size(83, 34);
            this.btnBrowserTest.TabIndex = 2;
            this.btnBrowserTest.Text = "Browser...";
            this.btnBrowserTest.UseVisualStyleBackColor = true;
            // 
            // tbxPathTest
            // 
            this.tbxPathTest.Location = new System.Drawing.Point(20, 33);
            this.tbxPathTest.Name = "tbxPathTest";
            this.tbxPathTest.Size = new System.Drawing.Size(199, 20);
            this.tbxPathTest.TabIndex = 1;
            // 
            // btnTest
            // 
            this.btnTest.Image = global::GUI.Properties.Resources.Test;
            this.btnTest.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnTest.Location = new System.Drawing.Point(106, 225);
            this.btnTest.Name = "btnTest";
            this.btnTest.Size = new System.Drawing.Size(124, 41);
            this.btnTest.TabIndex = 6;
            this.btnTest.Text = "Test";
            this.btnTest.UseVisualStyleBackColor = true;
            // 
            // btnTrain
            // 
            this.btnTrain.Image = global::GUI.Properties.Resources.Train;
            this.btnTrain.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnTrain.Location = new System.Drawing.Point(83, 225);
            this.btnTrain.Name = "btnTrain";
            this.btnTrain.Size = new System.Drawing.Size(124, 41);
            this.btnTrain.TabIndex = 6;
            this.btnTrain.Text = "Train";
            this.btnTrain.UseVisualStyleBackColor = true;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.ckbMAPE);
            this.groupBox4.Controls.Add(this.ckbMSE);
            this.groupBox4.Controls.Add(this.ckbNMSE);
            this.groupBox4.Location = new System.Drawing.Point(20, 61);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(297, 158);
            this.groupBox4.TabIndex = 7;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Performance Measures";
            // 
            // ckbNMSE
            // 
            this.ckbNMSE.AutoSize = true;
            this.ckbNMSE.Location = new System.Drawing.Point(12, 29);
            this.ckbNMSE.Name = "ckbNMSE";
            this.ckbNMSE.Size = new System.Drawing.Size(57, 17);
            this.ckbNMSE.TabIndex = 0;
            this.ckbNMSE.Text = "NMSE";
            this.ckbNMSE.UseVisualStyleBackColor = true;
            // 
            // ckbMSE
            // 
            this.ckbMSE.AutoSize = true;
            this.ckbMSE.Location = new System.Drawing.Point(109, 29);
            this.ckbMSE.Name = "ckbMSE";
            this.ckbMSE.Size = new System.Drawing.Size(49, 17);
            this.ckbMSE.TabIndex = 1;
            this.ckbMSE.Text = "MSE";
            this.ckbMSE.UseVisualStyleBackColor = true;
            // 
            // ckbMAPE
            // 
            this.ckbMAPE.AutoSize = true;
            this.ckbMAPE.Location = new System.Drawing.Point(206, 29);
            this.ckbMAPE.Name = "ckbMAPE";
            this.ckbMAPE.Size = new System.Drawing.Size(56, 17);
            this.ckbMAPE.TabIndex = 2;
            this.ckbMAPE.Text = "MAPE";
            this.ckbMAPE.UseVisualStyleBackColor = true;
            // 
            // MainGUI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(745, 472);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.label1);
            this.Name = "MainGUI";
            this.Text = "StockPrediction";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton rdTrendPrediction;
        private System.Windows.Forms.RadioButton rdPricePrediction;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btnTrain;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.RadioButton rdSVR;
        private System.Windows.Forms.RadioButton rdANN;
        private System.Windows.Forms.Button btnBrowserTrain;
        private System.Windows.Forms.TextBox tbxPathTrain;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button btnTest;
        private System.Windows.Forms.Button btnBrowserTest;
        private System.Windows.Forms.TextBox tbxPathTest;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.CheckBox ckbMAPE;
        private System.Windows.Forms.CheckBox ckbMSE;
        private System.Windows.Forms.CheckBox ckbNMSE;
    }
}

