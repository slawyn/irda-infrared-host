using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace IrDa
{
    public partial class Gui : Form
    {
        private System.Windows.Forms.Timer formTimer;

        SerialPort clsSerialPort;
        Thread threadReadingThread;

        ConcurrentQueue<LogObject> clsLogBuffer;

        bool bContinue;

        static char[] hexChars = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9','A', 'B', 'C', 'D', 'E', 'F' };
        const int cintPaddingLength = 20;
        const string csPaddingColumnSupversory = "Vr:{0}     ";
        const string csPaddingColumgInformative = "Vr:{0} Vs:{1}     ";
        enum Commands:ushort{ CMD_IRDA_PARAM= 10, CMD_RESET_STATS= 11, CMD_START_DISCOVERY = 12, CMD_CALCULATE_CRC = 13, CMD_GET_STATISTIC = 14, CMD_GET_DEVICES = 15, CMD_READ_FRAME = 16, CMD_STATUS = 17, CMD_DEBUG = 18, CMD_IRDA_DISC=19,CMD_IRCOMM_PAYLOAD=20, STATE_TIMEOUT = 0xFFFE, STATE_CLEANUP = 0xFFFF };
        public Gui()
        {
            InitializeComponent();
            this.SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.UserPaint | ControlStyles.DoubleBuffer, true);
            this.formTimer = new System.Windows.Forms.Timer();
            this.formTimer.Tick += new EventHandler(HandlerTimerTick);
            this.formTimer.Interval = 17;
        }

        private void HandlerOnFormLoad(object sender, EventArgs e)
        {
            CboxUpdateList();


            clsLogBuffer = new ConcurrentQueue<LogObject>();
            clsSerialPort = new SerialPort();

            this.formTimer.Start();
        }
        private void HandlerButtonClear(object sender, EventArgs e)
        {
            tboxOutput.Clear();
        }

        private void HandlerButtonsIrcomm(object sender, EventArgs e)
        {
            string Command = "";
            if (sender == buttonIrCommTime)
            {
                Command = "5w0000#";
            }
            else if (sender == buttonIrCommSeconds)
            {
                Command = "5r0200#";
            }
            else if (sender == buttonIrCommMinutes)
            {
                Command = "5r0300#";
            }
            else if (sender == buttonIrCommHours)
            {
                Command = "5r0400#";
            }
            else if (sender == buttonIrCommDay)
            {
                Command = "5r0500#";
            }
            else if (sender == buttonIrCommMonth)
            {
                Command = "5r0600#";
            }
            else if (sender == buttonIrCommYear)
            {
                Command = "5rca00#";
            }
            else {
                Command = "/";
            }



            byte[] data = new byte[Command.Length + 4];
            data[0] = (byte)(Command.Length + 4);
            data[1] = (byte)(Commands.CMD_IRCOMM_PAYLOAD);
            data[2] = (byte)((int)(Commands.CMD_IRCOMM_PAYLOAD) >> 8);
            data[3] = (byte)Command.Length;
            for (int i = 0; i < Command.Length; ++i) {
                data[4 + i] = (byte)Command[i];
            }

 
            clsSerialPort.Write(data, 0, data.Length);
        }
        

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }



        private void CboxUpdateList() {
            // Get a list of serial port names.
            string[] ports = SerialPort.GetPortNames();

            Array.Sort(ports);

            // Display each port name to the console.
            this.cboxComs.Items.Clear();
            foreach (string port in ports)
            {
                this.cboxComs.Items.Add(port);
            }


            if (this.cboxComs.Items.Count > 0)
            {
                this.cboxComs.SelectedIndex = 0;
            }

        }

        private void HandlerConnectButton(object sender, EventArgs e)
        {
            try
            {
                if (!bContinue)
                {
                    string comPort = this.cboxComs.SelectedItem.ToString();
                    if (comPort != "")
                    {
                        bContinue = true;
                        threadReadingThread = new Thread(Read);

                        clsSerialPort.PortName = comPort;
                        clsSerialPort.BaudRate = 115200;
                        clsSerialPort.Parity = Parity.None;
                        clsSerialPort.DataBits = 8;
                        clsSerialPort.StopBits = StopBits.One;
                        clsSerialPort.Handshake = Handshake.None;
                        clsSerialPort.DtrEnable = true;

                        clsSerialPort.ReadTimeout = 500;
                        clsSerialPort.WriteTimeout = 500;
                        clsSerialPort.Open();

                        threadReadingThread.Start();
                        btnConnect.Text = "Disconnect";
                       btnConnect.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
                    }
                }
                else
                {
                    btnConnect.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
                    bContinue = false;
                    btnConnect.Text = "Connect";
                    threadReadingThread.Join();
                    clsSerialPort.Close();
                }
            }
            catch (Exception ex) {
                bContinue = false;
            }


        }


        private void HandlerKeyPressed(Object o, KeyPressEventArgs e)
        {
            byte[] data;
            // The keypressed method uses the KeyChar property to check 
            // whether the ENTER key is pressed. 

            // If the ENTER key is pressed, the Handled property is set to true, 
            // to indicate the event is handled.
            if (e.KeyChar == (char)Keys.Return)
            {   

                e.Handled = true;

                if (bContinue)
                {
                    string text = tboxCommandline.Text;
                    if (text != "")
                    {
                        string[] tsplit = text.Split();
                        int len = tsplit.Length;

                        try
                        {
                            if (tsplit[0] == "crc")
                            {
                                if (len == 2)
                                {
                                    byte[] payload = Helper.ConvertHexStringToByteArray(tsplit[1]);
                                    data = new byte[payload.Length + 3];
                                    data[0] = (byte)(payload.Length + 3);
                                    data[1] = (byte)(Commands.CMD_CALCULATE_CRC);
                                    data[2] = (byte)((int)(Commands.CMD_CALCULATE_CRC) >> 8);
                                    payload.CopyTo(data, 3);
                                    clsSerialPort.Write(data, 0, data.Length);
                                }
                            }
                            else if (tsplit[0] == "rstat")
                            {
                                data = new byte[3];
                                data[0] = (byte)(3);
                                data[1] = (byte)(Commands.CMD_RESET_STATS);
                                data[2] = (byte)((int)(Commands.CMD_RESET_STATS) >> 8);
                                clsSerialPort.Write(data, 0, data.Length);
                            }
                            else if (tsplit[0] == "listen" && len > 1)
                            {
                                data = new byte[4];
                                data[0] = (byte)(4);
                                data[1] = (byte)(Commands.CMD_IRDA_PARAM);
                                data[2] = (byte)((int)(Commands.CMD_IRDA_PARAM) >> 8);
                                data[3] = (byte)(Int16.Parse(tsplit[1]));
                                clsSerialPort.Write(data, 0, data.Length);
                            }
                            if (tsplit[0] == "ircomm")
                            {
                                if (len == 2)
                                {
                                    int payloadlength = tsplit[1].Length;

                                    System.Text.ASCIIEncoding encoding = new System.Text.ASCIIEncoding();
                                    data = new byte[payloadlength + 4];
                                    data[0] = (byte)(payloadlength + 4);
                                    data[1] = (byte)(Commands.CMD_IRCOMM_PAYLOAD);
                                    data[2] = (byte)((int)(Commands.CMD_IRCOMM_PAYLOAD) >> 8);
                                    data[3] = (byte)payloadlength;
                                    encoding.GetBytes(tsplit[1], 0, tsplit[1].Length, data, 4);
                                    clsSerialPort.Write(data, 0, data.Length);
                                }
                            }
                            else if (tsplit[0] == "gdevs")
                            {
                                data = new byte[3];
                                data[0] = (byte)(3);
                                data[1] = (byte)(Commands.CMD_GET_DEVICES);
                                data[2] = (byte)((int)(Commands.CMD_GET_DEVICES) >> 8);
                                clsSerialPort.Write(data, 0, data.Length);
                            }
                            else if (tsplit[0] == "debug")
                            {
                                data = new byte[3];
                                data[0] = (byte)(3);
                                data[1] = (byte)(Commands.CMD_DEBUG);
                                data[2] = (byte)((int)(Commands.CMD_DEBUG) >> 8);
                                clsSerialPort.Write(data, 0, data.Length);
                            }
                            else if (tsplit[0] == "gstat")
                            {
                                data = new byte[3];
                                data[0] = (byte)(3);
                                data[1] = (byte)(Commands.CMD_GET_STATISTIC);
                                data[2] = (byte)((int)(Commands.CMD_GET_STATISTIC) >> 8);
                                clsSerialPort.Write(data, 0, data.Length);
                            }
                            else if (tsplit[0] == "discovery")
                            {
                                data = new byte[3];
                                data[0] = (byte)(3);
                                data[1] = (byte)(Commands.CMD_START_DISCOVERY);
                                data[2] = (byte)((int)(Commands.CMD_START_DISCOVERY) >> 8);
                                clsSerialPort.Write(data, 0, data.Length);
                            }

                        }
                        catch (Exception ex) {
                            PrintError(ex.Message);
                        }
                    }
                
                    tboxCommandline.Text = "";
                }
            }
        }

        private void HandlerRefreshButton(object sender, EventArgs e)
        {
            CboxUpdateList();
        }


        public static string convertByteArrayToAsciiString(byte[] ba, int offset, int count) {
            StringBuilder buf = new StringBuilder(count);

            for (int i = 0; i < count; i++) {
                buf.Append((char)ba[offset + i]);
            }
            return buf.ToString();
        }

        private void HandlerTimerTick(object sender, EventArgs e)
        {
          
            int iStart = Environment.TickCount;

            do
            {
                if (clsLogBuffer.TryDequeue(out LogObject clsLogResult))
                {

                    RichTextBoxExtensions.AppendText(tboxOutput, clsLogResult.strTimestamp, Color.Tomato, Color.White);
                    while (clsLogResult.TryGetNextLineContent(out string strContent, out int iType))
                    {
                        switch (iType)
                        {
                            /* Normal Print */
                            case 0:
                                RichTextBoxExtensions.AppendText(tboxOutput, strContent, Color.DarkBlue, Color.White);
                                break;

                            /* Error Print */
                            case 1:
                                RichTextBoxExtensions.AppendText(tboxOutput, strContent, Color.Red, Color.PaleVioletRed);
                                break;

                            /* Lime-Green*/
                            case 2:
                                RichTextBoxExtensions.AppendText(tboxOutput, strContent, Color.DarkBlue, Color.LimeGreen);
                                break;

                            /* Red */
                            case 3:
                                RichTextBoxExtensions.AppendText(tboxOutput, strContent, Color.Red, Color.White);
                                break;
                            /* Dark green */
                            case 4:
                                RichTextBoxExtensions.AppendText(tboxOutput, strContent, Color.DarkGreen, Color.White);
                                break;

                            /* Black Aqua */
                            case 5:
                                RichTextBoxExtensions.AppendText(tboxOutput, strContent, Color.Black, Color.Aquamarine);
                                break;

                            /* Black Pink */
                            case 6:
                                RichTextBoxExtensions.AppendText(tboxOutput, strContent, Color.Black, Color.Pink);
                                break;
                            case 7:
                                RichTextBoxExtensions.AppendText(tboxOutput, strContent, Color.Tomato, Color.White);
                                break;

                            case 8:
                                RichTextBoxExtensions.AppendText(tboxOutput, strContent, Color.DarkGray, Color.White);
                                break;
                            case 9:
                                RichTextBoxExtensions.AppendText(tboxOutput, strContent, Color.Black, Color.White);
                                break;
                            case 10:
                                RichTextBoxExtensions.AppendText(tboxOutput, strContent, Color.Purple, Color.White);
                                break;
                            case 11:
                                RichTextBoxExtensions.AppendText(tboxOutput, strContent, Color.DarkCyan, Color.White);
                                break;
                            case 12:
                                RichTextBoxExtensions.AppendText(tboxOutput, strContent, Color.Magenta, Color.White);
                                break;
                            case 13:
                                RichTextBoxExtensions.AppendText(tboxOutput, strContent, Color.DarkBlue, Color.White);
                                break;
                            case 14:
                                RichTextBoxExtensions.AppendText(tboxOutput, strContent, Color.Orange, Color.White);
                                break;
                        }
                    }

                    RichTextBoxExtensions.AppendText(tboxOutput, Environment.NewLine, Color.DarkBlue, Color.White);
                    tboxOutput.SelectionStart = tboxOutput.Text.Length;
                    tboxOutput.ScrollToCaret();
                }
                else
                {
                    break;
                }
            } while (Environment.TickCount - iStart < 17);


        }

        public void PrintError(string message)
        {
            string timestamp = DateTime.UtcNow.ToString("HH:mm:ss.fff    ", CultureInfo.InvariantCulture);
            LogObject lo = new LogObject(timestamp);

            lo.AddLineContent(message, 0);
            clsLogBuffer.Enqueue(lo);
        }


        public void PrintBuffer(byte[] Buffer, int lOffset, int iLength)
        {
            string timestamp = DateTime.UtcNow.ToString("HH:mm:ss.fff    ", CultureInfo.InvariantCulture);
            LogObject lo = new LogObject(timestamp);

            lo.AddLineContent(Helper.ConvertByteArrayToHexString(Buffer, lOffset, iLength), 0);
            clsLogBuffer.Enqueue(lo); 
        }

        public void PrintCmdActive(byte[] Buffer, int ReadBytes)
        {
            string timestamp = DateTime.UtcNow.ToString("HH:mm:ss.fff    ", CultureInfo.InvariantCulture);
            LogObject lo = new LogObject(timestamp);

            lo.AddLineContent(Helper.ConvertByteArrayToHexString(Buffer, 0, 4), 3);
            lo.AddLineContent(Helper.ConvertByteArrayToHexString(Buffer, 4, Buffer.Length - 4), 0);

            int cmd = (Buffer[1]) | (((ushort)Buffer[2]) << 8);

            // Additional Text
            switch (cmd)
            {
                case (int)Commands.CMD_GET_STATISTIC:
                    PrintCmdGetStatistic(lo, Buffer);
                    break;
                case (int)Commands.CMD_GET_DEVICES:
                    PrintCmdGetDevices(lo, Buffer, ReadBytes);
                    break;
            }

            clsLogBuffer.Enqueue(lo);
        }

        public void PrintCmdGetStatistic(LogObject lo, byte[] Buffer) 
        {


            int sirrerror = (Buffer[21]) | (((int)Buffer[22]) << 8);
            int laperror = (((int)Buffer[23])) | (((int)Buffer[24]) << 8);
            string strerrors = "";
            if ((laperror == 0 && sirrerror == 0))
            {
                strerrors = " None";
            }
            else
            {
                if ((sirrerror & 0x000001) != 0)
                {
                    strerrors += " Preamble";
                }

                if ((sirrerror & 0x000002) != 0)
                {
                    strerrors += " BofFormat";
                }

                if ((sirrerror & 0x00004) != 0)
                {
                    strerrors += " FrameFormat";
                }

                if ((sirrerror & 0x00008) != 0)
                {
                    strerrors += " Fcs";
                }

                if ((sirrerror & 0x000010) != 0)
                {
                    strerrors += " WrongLength";
                }
                if ((sirrerror & 0x000020) != 0)
                {
                    strerrors += " Framing";
                }

                if ((sirrerror & 0x000040) != 0)
                {
                    strerrors += " OverRun";
                }

                if ((sirrerror & 0x000080) != 0)
                {
                    strerrors += " BufferOverRun";
                }

                if ((sirrerror & 0x000100) != 0)
                {
                    strerrors += " MediaTimedOut";
                }

                if ((laperror & 0x000001) != 0)
                {
                    strerrors += " UnknownXidFormat";
                }

                if ((laperror & 0x000002) != 0)
                {
                    strerrors += " WrongCmdLength";
                }

                if ((laperror & 0x000004) != 0)
                {
                    strerrors += " CommandFrame";
                }

                if ((laperror & 0x00008) != 0)
                {
                    strerrors += " WrongHintLength";
                }

                if ((laperror & 0x000010) != 0)
                {
                    strerrors += " UnknownCommand";
                }
            }

            lo.AddLineContent(String.Format("Tx: {0} ", (Buffer[4]) | (((int)Buffer[5]) << 8) | (((int)Buffer[6]) << 16) | (((int)Buffer[7]) << 24)), 4);
            lo.AddLineContent(String.Format("Rx: {0} ", (Buffer[8]) | (((int)Buffer[9]) << 8) | (((int)Buffer[10]) << 16) | (((int)Buffer[11]) << 24)), 4);
            lo.AddLineContent(String.Format("Echo: {0} ", (Buffer[12]) | (((int)Buffer[13]) << 8) | (((int)Buffer[14]) << 16) | (((int)Buffer[15]) << 24)), 4);
            lo.AddLineContent("Uptime: " + TimeSpan.FromMilliseconds((Buffer[16]) | (((int)Buffer[17]) << 8) | (((int)Buffer[18]) << 16) | (((int)Buffer[19]) << 24)).ToString(@"hh\:mm\:ss"), 4);
            lo.AddLineContent(String.Format(" Discovery Seq.: {0} ", (Buffer[20])), 4);

            lo.AddLineContent("Errors: [" + strerrors, 4);
            lo.AddLineContent(String.Format(" ] Count: {0} ", (Buffer[25])), 4);
            lo.AddLineContent("Erroneus Control: " + Helper.ConvertByteArrayToHexString(Buffer, 26, 1), 4);

 
        }


        public void PrintCmdGetDevices(LogObject lo, byte[] Buffer, int BytesRead) {


            lo.AddLineContent(String.Format("Device count: {0} ", (int)Buffer[4]), 4);

            if (BytesRead >= 5)
            {
                lo.AddLineContent(Helper.ConvertByteArrayToAsciiString(Buffer, 5, BytesRead - 5), 4);
            }
        }

        public void PrintCmdPassive(byte[] Buffer, int BytesRead) 
        {

            string timestamp = DateTime.UtcNow.ToString("HH:mm:ss.fff    ", CultureInfo.InvariantCulture);
            LogObject lo = new LogObject(timestamp);


            if (BytesRead > 9)
            {

                if ((Buffer[9] & 0x03) == 0x01)
                {
                    lo.AddLineContent(String.Format(csPaddingColumnSupversory, Buffer[9] >> 5).PadRight(cintPaddingLength, ' '), (Buffer[8] & 0x01)>0?5:6);
                }
                else if ((Buffer[9] & 0x01) == 0x00)
                {
                    lo.AddLineContent(String.Format(csPaddingColumgInformative, Buffer[9] >> 5, (Buffer[9] >> 1) & 0x07).PadRight(cintPaddingLength, ' '), (Buffer[8] & 0x01) > 0 ? 5 : 6);
                }
                else
                {

                    lo.AddLineContent("".PadLeft(cintPaddingLength, ' '), 7);
                }

            }
            else
            {
                lo.AddLineContent("".PadLeft(cintPaddingLength, ' '), 7);
            }

            lo.AddLineContent(Helper.ConvertByteArrayToHexString(Buffer, 0, 4), 3);
            lo.AddLineContent(Helper.ConvertByteArrayToHexString(Buffer, 4, 2), 8);
            lo.AddLineContent(Helper.ConvertByteArrayToHexString(Buffer, 6, 1), 9);
            lo.AddLineContent("[ " + Helper.ConvertByteArrayToHexString(Buffer, 7, 1) + "]", 9);

            string strIrcommPayload = "";
            if (Buffer[3] == 0)
            {
                int written = 8;
                int idx = 0;
                while (written < BytesRead)
                {
                    if (idx == 0 && (written + 2) < BytesRead)
                    {
                        ++idx;
                        lo.AddLineContent(Helper.ConvertByteArrayToHexString(Buffer, written, 2), 10);
                        written += 2;
                    }
                    else if (idx == 1 && (written + 2) < BytesRead)
                    {
                        ++idx;
                        lo.AddLineContent(Helper.ConvertByteArrayToHexString(Buffer, written, 2), 11);
                        written += 2;
                    }
                    else if ((BytesRead - written) == 2)
                    {
                        lo.AddLineContent(Helper.ConvertByteArrayToHexString(Buffer, written, 2), 12);
                        written += 2;
                    }
                    else if((BytesRead- written>=5) && (written == (4+8)) && (Buffer[4+8] == 0 && Buffer[5+8] == 0))    // Ircomm
                    {
                        lo.AddLineContent(Helper.ConvertByteArrayToHexString(Buffer, written, 2), 7);
                        strIrcommPayload = Helper.ConvertByteArrayToAsciiString(Buffer, written+2, BytesRead - written - 4);

                        written += 2;
                    }
                    else
                    {
                        lo.AddLineContent(Helper.ConvertByteArrayToHexString(Buffer, written, 1), 13);
                        ++written;
                    }
                }
            }
            else
            {
                lo.AddLineContent(Helper.ConvertByteArrayToHexString(Buffer, 8, BytesRead - 8), 14);
            }


            if (strIrcommPayload != "")
            {
                lo.AddLineContent(strIrcommPayload, 4);
            }
            clsLogBuffer.Enqueue(lo);

       
        }


        public void Read()
        {
            while (this.bContinue)
            {
                try
                {
                    byte[] ReceiveBuffer;
                    int BytesInBuffer = clsSerialPort.BytesToRead;
                    if (BytesInBuffer > 0)
                    {
                        int FrameLength = clsSerialPort.ReadByte();

                        // Wait 30 ms
                        if (FrameLength > BytesInBuffer)
                        {
                            Thread.Sleep(30);
                            BytesInBuffer = clsSerialPort.BytesToRead;
                        }


                        // Print Buffer Bytes
                        if (FrameLength > 0 && (FrameLength <= BytesInBuffer))
                        {

                            ReceiveBuffer = new byte[FrameLength];
                            ReceiveBuffer[0] = (byte)FrameLength;

                            // TODO add timeouts here
                            int ReadBytes = 1;
                            while (ReadBytes < FrameLength)
                            {
                                ReadBytes += clsSerialPort.Read(ReceiveBuffer, ReadBytes, FrameLength - ReadBytes);
                            }

                            // Print differently if frame is an active or a passive read
                            int cmd = (ReceiveBuffer[1]) | (((ushort)ReceiveBuffer[2]) << 8);
                            if (cmd != (int)Commands.CMD_READ_FRAME)
                            {
                                PrintCmdActive(ReceiveBuffer, ReadBytes);
                            }
                            else
                            {
                                PrintCmdPassive(ReceiveBuffer, ReadBytes);

                            }

                           
                        }
                        else
                        {
                          
                            ReceiveBuffer = new byte[BytesInBuffer + 1];
                            ReceiveBuffer[0] = (byte)FrameLength;
                            int idx = 1;
                            while (idx < BytesInBuffer)
                            {

                                ReceiveBuffer[idx] = (byte)clsSerialPort.ReadByte();
                                ++idx;
                            }

                            PrintBuffer(ReceiveBuffer, 0, BytesInBuffer);
                        }
                    }
                    else {
                        Thread.Sleep(2);
                    }
                }
                catch (Exception ex) {
                    PrintError(ex.Message);
                    this.bContinue = false;
                }
            }
        }

    }

    public class LogObject
    {
        public string strTimestamp;
        private ConcurrentQueue<string> clsStringList;
        private ConcurrentQueue<int> clsTypesList;

        public LogObject(string strTimestamp)
        {
            this.strTimestamp = strTimestamp;
            this.clsStringList = new ConcurrentQueue<string>();
            this.clsTypesList = new ConcurrentQueue<int>();
        }

        public void AddLineContent(string strLogSegment, int iType) 
        {
            clsStringList.Enqueue(strLogSegment);
            clsTypesList.Enqueue(iType);
        }

        public bool TryGetNextLineContent(out string content, out int type)
        {
            bool success = clsStringList.TryDequeue(out content) & clsTypesList.TryDequeue(out type);
            return success;
        }
    }
}
