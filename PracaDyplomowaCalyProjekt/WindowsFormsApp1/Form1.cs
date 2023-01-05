using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlTypes;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using static System.Net.Mime.MediaTypeNames;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        SerialPort mySerialPort;
        String data="";
        String previousData = "rt";

        delegate void serialCalback(string data);
        private string incomingData;
        public Form1()
        {
            InitializeComponent();
            SerialPort mySerialPort = new SerialPort("COM11");
            
            mySerialPort.BaudRate = 19200;
            mySerialPort.Parity = Parity.None;
            mySerialPort.StopBits = StopBits.One;
            mySerialPort.DataBits = 8;
            mySerialPort.Handshake = Handshake.None;
            mySerialPort.RtsEnable = true;
            mySerialPort.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
            mySerialPort.Open();



        }
        private void show(string d)
        {
            
            data += d;
            Console.WriteLine("przyszly dane : {0}",data);
            data = "";
        }

        private void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            //Console.WriteLine("poprzednie dane {0} : ", previousData);
            SerialPort sp = (SerialPort)sender;
            string indata = sp.ReadExisting();
            Console.WriteLine("Data Received: {0}", indata);
           // data += indata;
           //if (data.Contains("*"))
           // {
           //     var threadParameters = new ThreadStart(delegate { setString(data); });
           //     var thread2 = new Thread(threadParameters);
           //     thread2.Start();
           //     setString(data);
           //     data = "";
           // }


        }      
        private void setString(string data)
        {

            if (this.InvokeRequired)
            {
                Action safeWrite = delegate { setString(data); };
                textBox1.Invoke(safeWrite);
            }
            else
            { 
                Console.WriteLine("obecne dane  {0}", data);
                previousData = data;    
            }  
        }
        private void btnConnect_Click(object sender, EventArgs e)
        {
            mySerialPort.Open();
        }

        private void btnDisconnect_Click(object sender, EventArgs e)
        {
            mySerialPort.Close();
        }
    }
}
