using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace panelTestowy
{
    public partial class Form1 : Form
    {
        DateTime dt = new DateTime(2015, 12, 20);
        List<CheckBox> digitalState = new List<CheckBox>();       
        List<AnalogSensor> sensorList = new List<AnalogSensor>(); 
       
        List<DigitalSensor> digitalSensorList = new List<DigitalSensor>();                                                   
       
        
        public Form1()
        {
            InitializeComponent();
            digitalState.Add(cbDigital1);
            digitalState.Add(cbDigital2);
            digitalState.Add(cbDigital3);
            digitalState.Add(cbDigital4);
            digitalState.Add(cbDigital5);
            digitalState.Add(cbDigital6);
            digitalState.Add(cbDigital7);
            digitalState.Add(cbDigital8);
            digitalState.Add(cbDigital9);
            digitalState.Add(cbDigital10); 
            digitalState.Add(cbDigital11);
            digitalState.Add(cbDigital12);
            digitalState.Add(cbDigital13);
            digitalState.Add(cbDigital14);
            digitalState.Add(cbDigital15);
            digitalState.Add(cbDigital16);
            digitalState.Add(cbDigital17);
            digitalState.Add(cbDigital18);
            digitalState.Add(cbDigital19);
            digitalState.Add(cbDigital20);

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

          


        }

        private void sendData(String dataToSend)
        {
            Console.WriteLine(dataToSend);
        }
        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton1.Checked)
            {
                sendData("!0*");
                gbManual.Enabled = false;
            }
                
            
        }
        private void unsetButton(Button btn)
        {
            if(btn.Enabled == true)
                btn.Enabled = false;    
        }

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
                sendData("!A*");
                gbManual.Enabled = true;
            }
            
            
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            sendData("@start*");
            string data = "%2022/11/03.10:52:33.22,5.100.3,9.44,9.24,0.FFFFF.";
            data += makeCheckSum(data);            
            data += "*";
            checkIncomingString(data);

            //sensorList[0].setValue(20);
            //sensorList[1].setValue(80);
            //sensorList[2].setValue(50);
            //sensorList[3].setValue(10);
            //sensorList[4].setValue(95);
            
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            sendData("@stop*");
            string data = "%2033/10/12.14:11:15.75,7.32,4.55.88,9.66,6.00000.";
            data += makeCheckSum(data);
            data += "*";
            checkIncomingString(data);
        }

        private void btnReset_Click(object sender, EventArgs e)
        {
            sendData("@reset*");
            string data = "%2049/01/12.04:44:23.50,8.22,4.26,8.10,8.23,9.AAAAB.";
            data += makeCheckSum(data);
            data += "*";
            checkIncomingString(data);
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
            if (data.Length == result)
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
                Console.WriteLine(revData);

                long nr = Int64.Parse(d[8], System.Globalization.NumberStyles.HexNumber);
                Console.WriteLine();
                int len = (4*d[8].Length) - 1;
                for (int i = len; i >= 0; i--)
                {

                    //if ((nr & (1 << i)) != 0)
                    //    digitalState[i].Checked = true;
                    //else
                    //    digitalState[i].Checked = false;
                    if ((nr & (1 << i)) != 0)
                        digitalSensorList[i].changeState(true);
                    else
                        digitalSensorList[i].changeState(false);
                }
            }   
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked == true)
                gbValues.Enabled = true;
            else
            {
                gbValues.Enabled = false;
                comboBox1.SelectedIndex = 0;
            }
                

            Console.WriteLine(comboBox1.SelectedIndex);
        }

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

        private void btnPanelMain_Click(object sender, EventArgs e)
        {

        }

        private void btnPanelSettings_Click(object sender, EventArgs e)
        {
            
//            gbValue.Visible = false;
            //gbValue.Size = new Size(0,0);
            gbValue.Location = new Point(1000,1000);

//            gbSettings.Visible = true;
            gbSettings.Size = new Size(680, 403);
            gbSettings.Location = new Point(0, 47);
            Console.WriteLine(gbValue.Location);

        }

        private void button3_Click(object sender, EventArgs e)
        {      
            
            gbValue.Location = new Point(0, 47);

 //           gbSettings.Visible = false;
//            gbSettings.Size = new Size(0,0);
            gbSettings.Location = new Point(1000, 1000);

            //            gbValue.Visible = true;
            //            gbValue.Size = new Size(680,403);

            Console.WriteLine(gbSettings.Location);









        }

      
    }
}
