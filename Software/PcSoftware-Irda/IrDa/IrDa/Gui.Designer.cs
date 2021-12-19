using System;
using System.Windows.Forms;



namespace IrDa
{


    partial class Gui
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Gui));
            this.label1 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.buttonIrCommResetIndex = new IrDa.RoundButton();
            this.buttonIrCommYear = new IrDa.RoundButton();
            this.buttonIrCommMonth = new IrDa.RoundButton();
            this.buttonIrCommDay = new IrDa.RoundButton();
            this.buttonIrCommSeconds = new IrDa.RoundButton();
            this.buttonIrCommMinutes = new IrDa.RoundButton();
            this.buttonIrCommHours = new IrDa.RoundButton();
            this.buttonIrCommTime = new IrDa.RoundButton();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.btnConnect = new IrDa.RoundButton();
            this.cboxComs = new IrDa.RoundComboBox();
            this.tboxCommandline = new IrDa.RoundText();
            this.btnRefresh = new IrDa.RoundButton();
            this.panel3 = new System.Windows.Forms.Panel();
            this.btnClear = new IrDa.RoundButton();
            this.tboxOutput = new System.Windows.Forms.RichTextBox();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.panel3.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(232, 17);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(54, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Command";
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.panel1.Controls.Add(this.buttonIrCommResetIndex);
            this.panel1.Controls.Add(this.buttonIrCommYear);
            this.panel1.Controls.Add(this.buttonIrCommMonth);
            this.panel1.Controls.Add(this.buttonIrCommDay);
            this.panel1.Controls.Add(this.buttonIrCommSeconds);
            this.panel1.Controls.Add(this.buttonIrCommMinutes);
            this.panel1.Controls.Add(this.buttonIrCommHours);
            this.panel1.Controls.Add(this.buttonIrCommTime);
            this.panel1.Controls.Add(this.pictureBox1);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.btnConnect);
            this.panel1.Controls.Add(this.cboxComs);
            this.panel1.Controls.Add(this.tboxCommandline);
            this.panel1.Controls.Add(this.btnRefresh);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1584, 74);
            this.panel1.TabIndex = 3;
            // 
            // buttonIrCommResetIndex
            // 
            this.buttonIrCommResetIndex.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(128)))));
            this.buttonIrCommResetIndex.FlatAppearance.BorderSize = 0;
            this.buttonIrCommResetIndex.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonIrCommResetIndex.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonIrCommResetIndex.Location = new System.Drawing.Point(606, 4);
            this.buttonIrCommResetIndex.Name = "buttonIrCommResetIndex";
            this.buttonIrCommResetIndex.Size = new System.Drawing.Size(99, 63);
            this.buttonIrCommResetIndex.TabIndex = 13;
            this.buttonIrCommResetIndex.Text = "IRCOMM: Reset Index";
            this.buttonIrCommResetIndex.UseVisualStyleBackColor = false;
            this.buttonIrCommResetIndex.Click += new System.EventHandler(this.HandlerButtonsIrcomm);
            // 
            // buttonIrCommYear
            // 
            this.buttonIrCommYear.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.buttonIrCommYear.FlatAppearance.BorderSize = 0;
            this.buttonIrCommYear.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonIrCommYear.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonIrCommYear.Location = new System.Drawing.Point(939, 6);
            this.buttonIrCommYear.Name = "buttonIrCommYear";
            this.buttonIrCommYear.Size = new System.Drawing.Size(98, 32);
            this.buttonIrCommYear.TabIndex = 12;
            this.buttonIrCommYear.Text = "IRCOMM: Year";
            this.buttonIrCommYear.UseVisualStyleBackColor = false;
            this.buttonIrCommYear.Click += new System.EventHandler(this.HandlerButtonsIrcomm);
            // 
            // buttonIrCommMonth
            // 
            this.buttonIrCommMonth.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.buttonIrCommMonth.FlatAppearance.BorderSize = 0;
            this.buttonIrCommMonth.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonIrCommMonth.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonIrCommMonth.Location = new System.Drawing.Point(1043, 6);
            this.buttonIrCommMonth.Name = "buttonIrCommMonth";
            this.buttonIrCommMonth.Size = new System.Drawing.Size(98, 32);
            this.buttonIrCommMonth.TabIndex = 11;
            this.buttonIrCommMonth.Text = "IRCOMM: Month";
            this.buttonIrCommMonth.UseVisualStyleBackColor = false;
            this.buttonIrCommMonth.Click += new System.EventHandler(this.HandlerButtonsIrcomm);
            // 
            // buttonIrCommDay
            // 
            this.buttonIrCommDay.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.buttonIrCommDay.FlatAppearance.BorderSize = 0;
            this.buttonIrCommDay.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonIrCommDay.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonIrCommDay.Location = new System.Drawing.Point(1144, 6);
            this.buttonIrCommDay.Name = "buttonIrCommDay";
            this.buttonIrCommDay.Size = new System.Drawing.Size(94, 32);
            this.buttonIrCommDay.TabIndex = 10;
            this.buttonIrCommDay.Text = "IRCOMM: Day";
            this.buttonIrCommDay.UseVisualStyleBackColor = false;
            this.buttonIrCommDay.Click += new System.EventHandler(this.HandlerButtonsIrcomm);
            // 
            // buttonIrCommSeconds
            // 
            this.buttonIrCommSeconds.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.buttonIrCommSeconds.FlatAppearance.BorderSize = 0;
            this.buttonIrCommSeconds.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonIrCommSeconds.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonIrCommSeconds.Location = new System.Drawing.Point(1455, 6);
            this.buttonIrCommSeconds.Name = "buttonIrCommSeconds";
            this.buttonIrCommSeconds.Size = new System.Drawing.Size(114, 32);
            this.buttonIrCommSeconds.TabIndex = 9;
            this.buttonIrCommSeconds.Text = "IRCOMM: Seconds";
            this.buttonIrCommSeconds.UseVisualStyleBackColor = false;
            this.buttonIrCommSeconds.Click += new System.EventHandler(this.HandlerButtonsIrcomm);
            // 
            // buttonIrCommMinutes
            // 
            this.buttonIrCommMinutes.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.buttonIrCommMinutes.FlatAppearance.BorderSize = 0;
            this.buttonIrCommMinutes.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonIrCommMinutes.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonIrCommMinutes.Location = new System.Drawing.Point(1344, 6);
            this.buttonIrCommMinutes.Name = "buttonIrCommMinutes";
            this.buttonIrCommMinutes.Size = new System.Drawing.Size(106, 32);
            this.buttonIrCommMinutes.TabIndex = 8;
            this.buttonIrCommMinutes.Text = "IRCOMM: Minutes";
            this.buttonIrCommMinutes.UseVisualStyleBackColor = false;
            this.buttonIrCommMinutes.Click += new System.EventHandler(this.HandlerButtonsIrcomm);
            // 
            // buttonIrCommHours
            // 
            this.buttonIrCommHours.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.buttonIrCommHours.FlatAppearance.BorderSize = 0;
            this.buttonIrCommHours.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonIrCommHours.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonIrCommHours.Location = new System.Drawing.Point(1244, 6);
            this.buttonIrCommHours.Name = "buttonIrCommHours";
            this.buttonIrCommHours.Size = new System.Drawing.Size(94, 32);
            this.buttonIrCommHours.TabIndex = 7;
            this.buttonIrCommHours.Text = "IRCOMM: Hours";
            this.buttonIrCommHours.UseVisualStyleBackColor = false;
            this.buttonIrCommHours.Click += new System.EventHandler(this.HandlerButtonsIrcomm);
            // 
            // buttonIrCommTime
            // 
            this.buttonIrCommTime.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.buttonIrCommTime.FlatAppearance.BorderSize = 0;
            this.buttonIrCommTime.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonIrCommTime.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonIrCommTime.Location = new System.Drawing.Point(711, 4);
            this.buttonIrCommTime.Name = "buttonIrCommTime";
            this.buttonIrCommTime.Size = new System.Drawing.Size(105, 62);
            this.buttonIrCommTime.TabIndex = 6;
            this.buttonIrCommTime.Text = "IRCOMM: Enable Clock Reading";
            this.buttonIrCommTime.UseVisualStyleBackColor = false;
            this.buttonIrCommTime.Click += new System.EventHandler(this.HandlerButtonsIrcomm);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
            this.pictureBox1.Location = new System.Drawing.Point(12, 6);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(107, 62);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox1.TabIndex = 5;
            this.pictureBox1.TabStop = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(385, 11);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(31, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Ports";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(502, 45);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(98, 20);
            this.label2.TabIndex = 3;
            this.label2.Text = "Commands";
            // 
            // btnConnect
            // 
            this.btnConnect.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.btnConnect.FlatAppearance.BorderSize = 0;
            this.btnConnect.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnConnect.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnConnect.Location = new System.Drawing.Point(137, 0);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(76, 34);
            this.btnConnect.TabIndex = 0;
            this.btnConnect.Text = "Connect";
            this.btnConnect.UseVisualStyleBackColor = false;
            this.btnConnect.Click += new System.EventHandler(this.HandlerConnectButton);
            // 
            // cboxComs
            // 
            this.cboxComs.BackColor = System.Drawing.SystemColors.HighlightText;
            this.cboxComs.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cboxComs.ForeColor = System.Drawing.SystemColors.ControlText;
            this.cboxComs.FormattingEnabled = true;
            this.cboxComs.Location = new System.Drawing.Point(219, 4);
            this.cboxComs.Name = "cboxComs";
            this.cboxComs.Size = new System.Drawing.Size(160, 24);
            this.cboxComs.TabIndex = 1;
            // 
            // tboxCommandline
            // 
            this.tboxCommandline.AllowDrop = true;
            this.tboxCommandline.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
            this.tboxCommandline.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.tboxCommandline.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tboxCommandline.Location = new System.Drawing.Point(219, 34);
            this.tboxCommandline.Name = "tboxCommandline";
            this.tboxCommandline.Size = new System.Drawing.Size(277, 35);
            this.tboxCommandline.TabIndex = 2;
            this.tboxCommandline.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.HandlerKeyPressed);
            // 
            // btnRefresh
            // 
            this.btnRefresh.BackColor = System.Drawing.SystemColors.InactiveBorder;
            this.btnRefresh.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnRefresh.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnRefresh.Location = new System.Drawing.Point(135, 37);
            this.btnRefresh.Name = "btnRefresh";
            this.btnRefresh.Size = new System.Drawing.Size(78, 34);
            this.btnRefresh.TabIndex = 0;
            this.btnRefresh.Text = "Refresh";
            this.btnRefresh.UseVisualStyleBackColor = false;
            this.btnRefresh.Click += new System.EventHandler(this.HandlerRefreshButton);
            // 
            // panel3
            // 
            this.panel3.AutoScroll = true;
            this.panel3.AutoScrollMargin = new System.Drawing.Size(5, 5);
            this.panel3.AutoSize = true;
            this.panel3.Controls.Add(this.btnClear);
            this.panel3.Controls.Add(this.tboxOutput);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel3.Location = new System.Drawing.Point(0, 74);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(1584, 687);
            this.panel3.TabIndex = 4;
            // 
            // btnClear
            // 
            this.btnClear.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnClear.BackColor = System.Drawing.Color.Coral;
            this.btnClear.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnClear.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnClear.Location = new System.Drawing.Point(1484, 643);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(77, 41);
            this.btnClear.TabIndex = 3;
            this.btnClear.Text = "Clear ";
            this.btnClear.UseVisualStyleBackColor = false;
            this.btnClear.Click += new System.EventHandler(this.HandlerButtonClear);
            // 
            // tboxOutput
            // 
            this.tboxOutput.BackColor = System.Drawing.Color.White;
            this.tboxOutput.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tboxOutput.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tboxOutput.Location = new System.Drawing.Point(0, 0);
            this.tboxOutput.Name = "tboxOutput";
            this.tboxOutput.Size = new System.Drawing.Size(1584, 687);
            this.tboxOutput.TabIndex = 4;
            this.tboxOutput.Text = "";
            // 
            // Gui
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1584, 761);
            this.Controls.Add(this.panel3);
            this.Controls.Add(this.panel1);
            this.Name = "Gui";
            this.Text = "IrDa Communicator";
            this.Load += new System.EventHandler(this.HandlerOnFormLoad);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.panel3.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label label1;
        private RoundButton btnConnect;
        private RoundComboBox cboxComs;
        private RoundText tboxCommandline;
        private RoundButton btnRefresh;
        private Panel panel1;
        private Panel panel2;
        private Panel panel3;
        private RoundButton btnClear;
        private RichTextBox tboxOutput;
        private PictureBox pictureBox1;
        private Label label3;
        private Label label2;
        private RoundButton buttonIrCommTime;
        private RoundButton buttonIrCommYear;
        private RoundButton buttonIrCommMonth;
        private RoundButton buttonIrCommDay;
        private RoundButton buttonIrCommSeconds;
        private RoundButton buttonIrCommMinutes;
        private RoundButton buttonIrCommHours;
        private RoundButton buttonIrCommResetIndex;
    }
  

}



