using panelSymulacjiSygnalow;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace symulacja_sygnalow_Windows
{
    public partial class Form1 : Form
    {
        delegate void serialCalback(string data);
        Serial mySerialPort;

        List<Button> digitalButtonsList = new List<Button>();
        List<DigitalSensor> digitalSensorList = new List<DigitalSensor>();

        List<int> analogValueList = new List<int>();
        List<ProgressBar> progressBarAnalogValueList = new List<ProgressBar>();
        List<Label> analogValuesLabelList = new List<Label>();

        Int64 digitalInGlobal = 0;

        public Form1()
        {
            InitializeComponent();
            mySerialPort = new Serial(cbxSelectCom, cbxSelectBaudRate, btnSerialRefresh,
                    btnSerialConnect, btnSerialDisConnect, gbConnection, lblSerialStatus, serialPort1);
            mySerialPort.getSerialPorts();

            makeSensorsList(digitalButtonsList, gbDigitalSensors);

            for (int i = 0; i < digitalButtonsList.Count; i++)
            {
                digitalSensorList.Add(new DigitalSensor(digitalButtonsList[i]));
                digitalButtonsList[i].Text = i.ToString();
            }

            analogValueList.Add(pBarAnalogValue1.Value);
            analogValueList.Add(pBarAnalogValue2.Value);
            analogValueList.Add(pBarAnalogValue3.Value);
            analogValueList.Add(pBarAnalogValue4.Value);

            progressBarAnalogValueList.Add(pBarAnalogValue1);
            progressBarAnalogValueList.Add(pBarAnalogValue2);
            progressBarAnalogValueList.Add(pBarAnalogValue3);
            progressBarAnalogValueList.Add(pBarAnalogValue4);

            analogValuesLabelList.Add(lblAnalogValue1);
            analogValuesLabelList.Add(lblAnalogValue2);
            analogValuesLabelList.Add(lblAnalogValue3);
            analogValuesLabelList.Add(lblAnalogValue4);

        }
        private void makeSensorsList<T>(List<T> list, GroupBox groupBox)
        {
            List<T> tempList = groupBox.Controls.OfType<T>().ToList();
            int size = tempList.Count() - 1; // list 9 to 1
            for (int i = 0; i < tempList.Count(); i++) //reversing list (1 to 9)
            {
                list.Add(tempList[size - i]);
            }
        }
        private void btnSerialRefresh_Click(object sender, EventArgs e)
        {
            mySerialPort.getSerialPorts();
        }

        private void btnSerialDisConnect_Click(object sender, EventArgs e)
        {
            mySerialPort.closeConnection();
            if (!mySerialPort.connected)
                timersendData.Enabled = false;
        }

        private void btnSerialConnect_Click(object sender, EventArgs e)
        {
            mySerialPort.makeConnestion();
            if (mySerialPort.connected)
                timersendData.Enabled = true;
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {         
               // char sign  = (char)serialPort1.ReadChar();

            String d = serialPort1.ReadExisting();
            Console.WriteLine(d);
            String data = d.Trim('@','*');
            Console.WriteLine("got data {0} : ",data);
            chekcIncomingString(data);
        }
        int getIntValue(string value)
        {
            try
            {
                int number = Convert.ToInt32(value, 16);
                Console.WriteLine("{0} --> {1}", value, number);
                return number;
            }
            catch (FormatException)
            {
                Console.WriteLine("{0}: FormatException", value);
            }
            catch (OverflowException)
            {
                Console.WriteLine("{0}: OverflowException", value);
            }
            return -1;
        }
        private void chekcIncomingString(String data)
        {
            data = data.Trim();
            Console.WriteLine("data : {0} : ", data);
            char[] separator = { '#', '/','*'};
            String[] dataList = data.Split(separator);
            if (dataList.Count() ==0 )
                return;

            if (String.IsNullOrEmpty(dataList[1]))
                return;
            Console.WriteLine("digital in  : {0}  ", dataList[1]);//digital in
            if (String.IsNullOrEmpty(dataList[2]))
                return;
            Console.WriteLine("sigital out : {0}  ", dataList[2]);//digital out
            if (String.IsNullOrEmpty(dataList[3]))
                return;
            Console.WriteLine("check sum   : {0}  ", dataList[3]);//check sum
            Console.WriteLine("hex to int  : {0}  ", Convert.ToInt32(dataList[3], 16));

            //Int64 digitalIn = Convert.ToInt32(dataList[1],16);
            //foreach (var item in dataList)
            //{
            //    Console.WriteLine("item : ", item);
            //}
            Console.WriteLine("list size {0} : ", dataList.Count());
            
            if (String.IsNullOrEmpty(dataList[1]))
                return;
            Int64 digitalIn = getIntValue(dataList[1]);
            if (digitalIn < 0)
                return;
            //Console.WriteLine("inncoming {0} : ", digitalIn);
            //Console.WriteLine("digital in  {0} , global in {1}", digitalIn, digitalInGlobal);
            for (int i = 0; i <8; i++)
            {
                //if( (digitalIn & (1<<i)) != (digitalInGlobal & (1<<i)))
                //{
                //    Console.WriteLine("sa rozne {0} , {1} ,{2} , {3}   ", digitalIn, digitalInGlobal, (digitalIn & (1 << i)), (digitalInGlobal & (1 << i)));
                //    changeDigitalState(16 +i);               
                //}

                if ((digitalIn & (1 << i)) == 0)
                {
                    digitalSensorList[16 + i].setState(true);


                }

                else
                    digitalSensorList[16 + i].setState(false);

                digitalSensorList[16 + i].changeState();


            }
             digitalInGlobal = digitalIn;                       
        }

        private void timersendData_Tick(object sender, EventArgs e)
        {
            makeStringToSend();
        }
        private void makeStringToSend()
        {
            String data = "#";

            //for (int i = 0; i < 16; i++)
            //{
            //    if (digitalSensorList[i].getState())
            //        data += "1";
            //    else
            //        data += "0";

            //}

            for (int i = 16; i > 0; i--)
            {
                int pos = i - 1;
                if (digitalSensorList[pos].getState())
                    data += "1";
                else
                    data += "0";

            }
            data += "/";
            //sendSerialdata(data);

           // data = "$";

            for (int i = 0; i < progressBarAnalogValueList.Count; i++)
            {
                data += analogValueList[i].ToString();
                data += "/";
            }

            if(radioButton1.Checked)
                data += "0/";
            else if(radioButton2.Checked)   
                data += "500/";
            else if(radioButton3.Checked)
                data += "200/";
            else if(radioButton4.Checked)
                data += "100/";

            data += "*";
            sendSerialdata(data);
        }
        private void sendSerialdata(string data)
        {
            Console.Write("sending : ");
            Console.WriteLine(data);
            if (mySerialPort.connected)
            {
                try
                {
                    serialPort1.Write(data);
                }
                catch (TimeoutException)
                {
                    Console.WriteLine("Sending error");
                }

            }
            else
                MessageBox.Show("No  conection !", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);

        }

        private void increaseAnalog(int position)
        {
            if (analogValueList[position] < 100)
            {
                analogValueList[position] += 5;
                updateProgresBarAnalog(position);
            }
        }
        private void decreaseAnalog(int position)
        {

            if (analogValueList[position] > 0)
            {
                analogValueList[position] -= 5;
                updateProgresBarAnalog(position);
            }
        }
        private void updateProgresBarAnalog(int position)
        {

            progressBarAnalogValueList[position].Value = analogValueList[position];
            double v = (4.98 * progressBarAnalogValueList[position].Value) / 100;
            analogValuesLabelList[position].Text = v.ToString("0.00");
        }

        private void changeDigitalState(int position)
        {
            digitalSensorList[position].changeState();           
        }

        private void button4_Click(object sender, EventArgs e)
        {
            changeDigitalState(0);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            changeDigitalState(1);   
        }

        private void button6_Click(object sender, EventArgs e)
        {
            changeDigitalState(2);            
        }

        private void button7_Click(object sender, EventArgs e)
        {
            changeDigitalState(3);            
        }

        private void button8_Click(object sender, EventArgs e)
        {
            changeDigitalState(4);
        }

        private void button9_Click(object sender, EventArgs e)
        {
            changeDigitalState(5);
        }

        private void button10_Click(object sender, EventArgs e)
        {
            changeDigitalState(6);
        }

        private void button11_Click(object sender, EventArgs e)
        {
            changeDigitalState(7);
        }

        private void button12_Click(object sender, EventArgs e)
        {
            changeDigitalState(8);
        }

        private void button13_Click(object sender, EventArgs e)
        {
            changeDigitalState(9);
        }
        private void button14_Click(object sender, EventArgs e)
        {
            changeDigitalState(10);
        }

        private void button15_Click(object sender, EventArgs e)
        {
            changeDigitalState(11);
        }
        private void button16_Click(object sender, EventArgs e)
        {
           //changeDigitalState(19);
        }

        private void button17_Click(object sender, EventArgs e)
        {
            //changeDigitalState(18);
        }

        private void button18_Click(object sender, EventArgs e)
        {
            //changeDigitalState(17);
        }

        private void button19_Click(object sender, EventArgs e)
        {
            //changeDigitalState(16);
        }
        private void btnAnalog1min_Click(object sender, EventArgs e)
        {
            decreaseAnalog(0);
        }

        private void btnAnalog1plus_Click(object sender, EventArgs e)
        {
             increaseAnalog(0);
        }

        private void btnAnalog2min_Click(object sender, EventArgs e)
        {
            decreaseAnalog(1);
        }

        private void btnAnalog2plus_Click(object sender, EventArgs e)
        {
            increaseAnalog(1);
        }

        private void btnAnalog3min_Click(object sender, EventArgs e)
        {
            decreaseAnalog(2);
        }

        private void btnAnalog3plus_Click(object sender, EventArgs e)
        {
            increaseAnalog(2);
        }

        private void btnAnalog4min_Click(object sender, EventArgs e)
        {
            decreaseAnalog(3);
        }

        private void btnAnalog4plus_Click(object sender, EventArgs e)
        {
            increaseAnalog(3);
        }

        private void button20_Click(object sender, EventArgs e)
        {
            changeDigitalState(12);
        }

        private void button21_Click(object sender, EventArgs e)
        {
            changeDigitalState(13);
        }

        private void button22_Click(object sender, EventArgs e)
        {
            changeDigitalState(14);
        }

        private void button23_Click(object sender, EventArgs e)
        {
            changeDigitalState(15);
        }
    }

}
