﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.IO.Ports;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using static System.Windows.Forms.VisualStyles.VisualStyleElement;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.ToolBar;
using Button = System.Windows.Forms.Button;

namespace panelTestowy
{
    public partial class Form1 : Form
    {


        delegate void serialCalback(string data);
        private string incomingData;
        Serial mySerialPort;

        DateTime dt = new DateTime(2015, 12, 20);
        List<CheckBox> digitalState = new List<CheckBox>();       
        List<AnalogSensor> sensorList = new List<AnalogSensor>(); 
       
        List<DigitalSensor> digitalSensorList = new List<DigitalSensor>();
        string password = "123";
        bool loginState = false;
        System.Windows.Forms.TextBox tbx;
        

        bool stateHand = false;
        bool stateOff = true;
        bool stateAuto = false;


        List<GroupBox> gbPanelList = new List<GroupBox>();   
        List<Button> btnPosList = new List<Button>();

        int timerStart_a = 0;

        public Form1()
        {
            InitializeComponent();

            mySerialPort = new Serial(cbxSelectCom, cbxSelectBaudRate, btnSerialRefresh,
                    btnSerialConnect, btnSerialDisConnect, gbConnection, lblSerialStatus, serialPort1);
            mySerialPort.getSerialPorts();

            pbAnalog1.ForeColor = Color.Yellow;
            comboBox1.SelectedIndex = 0;
                      
            sensorList.Add(new AnalogSensor(lblName1, lblAnalog1, lblUnit1, pbAnalog1));
            sensorList.Add(new AnalogSensor(lblName2, lblAnalog2, lblUnit2, pbAnalog2));
            sensorList.Add(new AnalogSensor(lblName3, lblAnalog3, lblUnit3, pbAnalog3));
            sensorList.Add(new AnalogSensor(lblName4, lblAnalog4, lblUnit4, pbAnalog4));
            sensorList.Add(new AnalogSensor(lblName5, lblAnalog5, lblUnit5, pbAnalog5));

            //sensorList[0].lblName.Text = "Przemek";
            sensorList[0].setNewInstelingen(70, 50, 200, "pierwszy", "A");
            sensorList[1].setNewInstelingen(70, 50, 200, "drugi", "V");
            sensorList[2].setNewInstelingen(70, 50, 200, "trzeci", "Bar");
            
            //digitalSensor = new DigitalSensor(lblDigital1, btnDigital1);
            digitalSensorList.Add(new DigitalSensor(lblDigital1, btnDigital1));
            digitalSensorList.Add(new DigitalSensor(lblDigital2, btnDigital2));
            digitalSensorList.Add(new DigitalSensor(lblDigital3, btnDigital3));
            digitalSensorList.Add(new DigitalSensor(lblDigital4, btnDigital4));
            digitalSensorList.Add(new DigitalSensor(lblDigital5, btnDigital5));
            digitalSensorList.Add(new DigitalSensor(lblDigital6, btnDigital6));
            digitalSensorList.Add(new DigitalSensor(lblDigital7, btnDigital7));
            digitalSensorList.Add(new DigitalSensor(lblDigital8, btnDigital8));
            digitalSensorList.Add(new DigitalSensor(lblDigital9, btnDigital9));
            digitalSensorList.Add(new DigitalSensor(lblDigital10, btnDigital10));
            digitalSensorList.Add(new DigitalSensor(lblDigital11, btnDigital11));
            digitalSensorList.Add(new DigitalSensor(lblDigital12, btnDigital12));
            digitalSensorList.Add(new DigitalSensor(lblDigital13, btnDigital13));
            digitalSensorList.Add(new DigitalSensor(lblDigital14, btnDigital14));
            digitalSensorList.Add(new DigitalSensor(lblDigital15, btnDigital15));
            digitalSensorList.Add(new DigitalSensor(lblDigital16, btnDigital16));
            digitalSensorList.Add(new DigitalSensor(lblDigital17, btnDigital17));
            digitalSensorList.Add(new DigitalSensor(lblDigital18, btnDigital18));
            digitalSensorList.Add(new DigitalSensor(lblDigital19, btnDigital19));
            digitalSensorList.Add(new DigitalSensor(lblDigital20, btnDigital20));

            digitalSensorList[0].setName("pierwsza nazwa");
            digitalSensorList[0].changeState(false);

            tbx = new System.Windows.Forms.TextBox();

            //comboBox2.Items.Add("Select digital item");

            foreach (var item in digitalSensorList)
            {
                Console.WriteLine(item.name);
                comboBox2.Items.Add(item.name);
            }
            comboBox2.SelectedIndex = 0;

            gbPanelList.Add(gbValue);
            gbPanelList.Add(gbSettings);
            gbPanelList.Add(gbMessage);
            gbPanelList.Add(gbConnectionSettings);
            gbPanelList.Add(gbPswd);
            

            btnPosList.Add(btnPosHand);
            btnPosList.Add(btnPosOff);
            btnPosList.Add(btnPosAuto);

            setBtnPos(1);

        }

        private void sendData(String dataToSend)
        {
            Console.WriteLine(dataToSend);
            sendSerialdata(dataToSend);
        }
        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton1.Checked)
            {
                sendData("!0*");
                gbManual.Enabled = false;
            }
                
            
        }
        //private void unsetButton(Button btn)
        //{
        //    if(btn.Enabled == true)
        //        btn.Enabled = false;    
        //}

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            if(radioButton2.Checked)
            {
                sendData("!H*");
                gbManual.Enabled = true;
            }
             
            
        }

        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton3.Checked)
            {
                
                gbManual.Enabled = true;
            }
            
            
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
           // sendData("@start*");
            //string data = "%2022/11/03.10:52:33.22,5.100.3,9.44,9.24,0.FFFFF.";
            //data += makeCheckSum(data);            
            //data += "*";
            //checkIncomingString(data);

            //sensorList[0].setValue(20);
            //sensorList[1].setValue(80);
            //sensorList[2].setValue(50);
            //sensorList[3].setValue(10);
            //sensorList[4].setValue(95);
            
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            sendData("@stop*");
            //string data = "%2033/10/12.14:11:15.75,7.32,4.55.88,9.66,6.00000.";
            //data += makeCheckSum(data);
            //data += "*";
            //checkIncomingString(data);
        }

        private void btnReset_Click(object sender, EventArgs e)
        {
            sendData("@reset*");
            //string data = "%2049/01/12.04:44:23.50,8.22,4.26,8.10,8.23,9.AAAAB.";
            //data += makeCheckSum(data);
            //data += "*";
            //checkIncomingString(data);
        }
        private string makeCheckSum(string data)
        {
            int len = data.Length;
            len += 3;
            return len.ToString("X2");

        }
        public string ReverseString(string srtVarable)
        {
            return new string(srtVarable.Reverse().ToArray());
        }
        private void checkIncomingString(string data)
        {
            
            Console.WriteLine(data);
            char[] c = {'.','*','!','%'};
            string[]d = data.Split(c);
            Console.WriteLine(d.Length);
            Console.WriteLine(d[d.Length - 2]);
            long result;
            long.TryParse(d[d.Length - 2], System.Globalization.NumberStyles.HexNumber, null, out result);
            Console.WriteLine(result);
            Console.WriteLine();
            //if (data.Length == result)
            {
                
                // this returns 1322173
                //float intValue = int.Parse(d[3]);
                //float f = 0;
                //float.TryParse(d[3],out f);
                //string a = d[3].Replace('.','.');
                double a = double.Parse(d[3]);
                Console.WriteLine(a);
                
                lblDate.Text = d[1];
                lblTime.Text = d[2];
                //lblAnalog1.Text = d[3];
                //lblAnalog2.Text = d[4];
                //lblAnalog3.Text = d[5];
                //lblAnalog4.Text = d[6];
                //lblAnalog5.Text = d[7];
                // int a = TryParse(d[3]);
                
                sensorList[0].setValue(double.Parse(d[3]));
                sensorList[1].setValue(double.Parse(d[4]));
                sensorList[2].setValue(double.Parse(d[5]));
                sensorList[3].setValue(double.Parse(d[6]));
                sensorList[4].setValue(double.Parse(d[7]));

                // Console.WriteLine(d[8][0]);
                string revData = ReverseString(d[8]);
                Console.Write("obrcone ");
                Console.WriteLine(revData);

                long nr = Int64.Parse(revData, System.Globalization.NumberStyles.HexNumber);
                Console.WriteLine();
                int len = (4*d[8].Length) - 1;
                for (int i = 0; i <len; i++)
                //for (int i = len; i >= 0; i--)
                {

                    
                    if ((nr & (1 << i)) != 0)
                        digitalSensorList[i].changeState(true);
                    else
                        digitalSensorList[i].changeState(false);
                }
            }   
        }

        //private void checkBox1_CheckedChanged(object sender, EventArgs e)
        //{
        //    if (checkBox1.Checked == true)
        //        gbValues.Enabled = true;
        //    else
        //    {
        //        gbValues.Enabled = false;
        //        comboBox1.SelectedIndex = 0;
        //    }
                

        //    Console.WriteLine(comboBox1.SelectedIndex);
        //}

        private void btnGetAnalogValues_Click(object sender, EventArgs e)
        {
            if (comboBox1.SelectedIndex >0)
            {
                string data = "@";
                data += comboBox1.SelectedIndex.ToString();
                data += "/value*";
                sendData(data);
            }
            
        }

        private void pbAnalog1_Click(object sender, EventArgs e)
        {
            
        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        

        private void btnPanelSettings_Click(object sender, EventArgs e)
        {
            setPanel(1);
////            gbValue.Visible = false;
//            //gbValue.Size = new Size(0,0);
//            gbValue.Location = new Point(1000,1000);

////            gbSettings.Visible = true;
//            gbSettings.Size = new Size(680, 403);
//            gbSettings.Location = new Point(0, 47);
//            Console.WriteLine(gbValue.Location);

//            gbConnectionSettings.Visible =false;

        }

        private void btnPanelValues_Click(object sender, EventArgs e)
        {
            setPanel(0);
            
        }

        private void btnConnectionSettings_Click(object sender, EventArgs e)
        {
            setPanel(3);


        }

        private void setPanel(int index)
        {
            int a = 0;
            foreach (var item in gbPanelList)
            {
                if (a++ == index) 
                { 
                    item.Visible = true;
                    item.Location = new Point(0, 50);
                }
                else
                {
                    item.Visible = false;
                }   
            }
            if(loginState && index == 1)
            {
                gbSetAnalog.Enabled = true;
                lblLogin.Visible = false;
                

            }  
            else
            {
                gbSetAnalog.Enabled = false;
                loginState = false;
                
            }
                
        }
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            //"#A/id*
            if(comboBox1.SelectedIndex != 0)
            {
                int a = comboBox1.SelectedIndex;
                string data = "#";
                data += "A";
                data += "/";
                data += a.ToString();
                data += "*";
               sendData(data);
            }
        }

        
        private void groupBox5_Enter(object sender, EventArgs e)
        {

        }

        private void btnPswdReset_Click(object sender, EventArgs e)
        {
            tbPswd.Text = "";
            //gbPswd.BackColor = SystemColors.ActiveCaption;
            lblPswd.Visible = false;
        }

        private void btnPswdCancel_Click(object sender, EventArgs e)
        {
            setPanel(1);
        }

        private void bntLogin_Click(object sender, EventArgs e)
        {
            if (!loginState)
            {
                setPanel(4);                
            }
                
            else
                lblLogin.Visible = true;    
        }

        private void btnPswdLogin_Click(object sender, EventArgs e)
        {
            
            if(String.Compare(password,tbPswd.Text) == 0)
            {
                loginState = true;
                gbSetAnalog.Enabled = true;
                tbPswd.Text = "";
                setPanel(1);
            }
            else
            {
                lblPswd.Visible = true;
            }
            
                


        }
        private void setLoginState(bool state){

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            textBox1 = tbx;
            


        }

        private void button5_Click(object sender, EventArgs e)
        {
            tbx.Text = "a";
            tbxKey.Text = "text a";
            //tbxPisz.Text = "rrrr";


        }

        private void tbxPisz_TextChanged(object sender, EventArgs e)
        {
            tbx = tbxPisz;
        }

        private void btnSerialConnect_Click(object sender, EventArgs e)
        {
            mySerialPort.makeConnestion();
        }

        private void btnSerialDisConnect_Click(object sender, EventArgs e)
        {
            mySerialPort.closeConnection();
        }

        private void btnSerialRefresh_Click(object sender, EventArgs e)
        {
            mySerialPort.getSerialPorts();
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            //incomingData = serialPort1.ReadExisting();
            incomingData = serialPort1.ReadTo("*");
            incomingData += "*";
            this.Invoke(new EventHandler(ShowData));
        }
        private void ShowData(object sender, EventArgs e)
        {
            sendText(textBox1, incomingData);
            //Console.WriteLine("tutaj");
            //Console.WriteLine(incomingData);

        }

        

        private void sendText(string data)
        {
            if (this.InvokeRequired)
            {
                serialCalback calback = new serialCalback(sendText);
                this.Invoke(calback, new object[] { data });
            }
            else
            {
                sendText( data);
               Console.WriteLine(data);
            }
        }
        private void sendText(System.Windows.Forms.TextBox textBox, string text)
        {
            

            textBox.AppendText(text);
            if (string.Compare(text, "OK") > 0 || text.Contains("ERROR"))
                textBox.AppendText("\n");
            textBox.AppendText(Environment.NewLine);
            Console.WriteLine(incomingData);
            checkIncomingString(incomingData);
            
        }

       
        private void sendSerialdata(string data)
        {
            Console.WriteLine("przyszlo");
            Console.WriteLine(data);
            if (mySerialPort.connected)
            {
                try
                {
                    serialPort1.Write(data);
                    serialPort1.Write("\r");
                    // serialPort1.Write("\n");
                }
                catch (TimeoutException)
                {
                    //sendText(tbxData, "Sending error");
                    Console.WriteLine("nie mozna wysac");
                }

            }
            else
                MessageBox.Show("No  conection !", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);


        }

        private void btnPanelMessage_Click(object sender, EventArgs e)
        {
            setPanel(2);
        }

        private void btnPosHand_Click(object sender, EventArgs e)
        {
            setBtnPos(0);
            sendData("!H*");
        }

        private void btnPosOff_Click(object sender, EventArgs e)
        {
            setBtnPos(1);
            sendData("!0*");
        }

        private void btnPosAuto_Click(object sender, EventArgs e)
        {
            setBtnPos(2);
            sendData("!A*");
        }
        private void setBtnPos(int pos)
        {
            int a = 0;
            foreach (var item in btnPosList)
            {
                if(a++ == pos)
                {
                    item.Enabled = false;
                    item.Size = new Size(110,75);
                    item.BackColor = Color.DodgerBlue; 
                }
                else
                {
                    item.Enabled = true;
                    item.Size = new Size(100, 65);
                    item.BackColor = Color.LightSkyBlue;
                }
            }
            setState(pos);

        }
        private void setState(int pos)
        {   
            if (pos == 0)//hand
            {   
                setFontSize(btnStart, true);
                setFontSize(btnStop, true);
                
            }
            else if(pos == 1)//off
            {
                setFontSize(btnStart, false);
                setFontSize(btnStop, false);
            }
            else if (pos == 2)//auto
            {
                setFontSize(btnStart, false);
                setFontSize(btnStop, true);
            }
        }
        private void setFontSize(Button btn, bool state)
        {
            int newSizeSmall = 15;
            int newSizeLarge = 20;

            if (state)
            {
                btn.Enabled = true;
                btn.Font = new Font(btn.Font.FontFamily, newSizeLarge, FontStyle.Bold);
                btn.Size = new Size(220,70);
            }
            else
            {
                btn.Enabled = false;
                btn.Font = new Font(btn.Font.FontFamily, newSizeSmall, FontStyle.Regular);
                btn.Size = new Size(210, 60);
            }

        }

        private void btnStart_MouseDown(object sender, MouseEventArgs e)
        {
           sendData("@statr/1*");
            timerStart.Enabled = true;
        }

        private void btnStart_MouseUp(object sender, MouseEventArgs e)
        {
            sendData("@start/0*");
            timerStart.Enabled=false;
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBox2.SelectedIndex != 0)
            {
                string data = "#D/" + comboBox2.SelectedIndex.ToString() + "*";
                sendData(data);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {

        }

        private void btnPosAuto_MouseHover(object sender, EventArgs e)
        {
            //btnPosAuto.Size = new Size(110,75);           
        }

        private void btnPosAuto_MouseLeave(object sender, EventArgs e)
        {
            //btnPosAuto.Size = new Size(100, 65);
        }

        private void timerStart_Tick(object sender, EventArgs e)
        {
            sendData("@statr/1*");
            //if (timerStart_a > 1000)
            //{
            //    
            //    timerStart_a++;
            //}
            //else
            //{
            //    timerStart_a = 0;
            //    timerStart.Enabled = false;

            //}
                
            
        }
    }
}
