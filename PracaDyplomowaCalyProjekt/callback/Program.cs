using System;
using System.IO.Ports;



class PortDataReceived
{
    int data = 0;
    string d;
    
    public static void Main()
    {
        SerialPort mySerialPort = new SerialPort("COM11");

        mySerialPort.BaudRate = 19200;
        mySerialPort.Parity = Parity.None;
        mySerialPort.StopBits = StopBits.One;
        mySerialPort.DataBits = 8;
        mySerialPort.Handshake = Handshake.None;
        mySerialPort.RtsEnable = true;
        mySerialPort.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
        mySerialPort.Open();  
        Console.WriteLine("Press any key to continue...");
        Console.WriteLine();
        Console.ReadKey();
        mySerialPort.Close();
    }

    private static void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
    {
        SerialPort sp = (SerialPort)sender;
        //string indata = sp.ReadExisting();
        string indata = sp.ReadTo("*");
        Console.WriteLine("Data Received: {0}", indata);
        
      
        //incomingData(indata);
    }
    //static private void incomingData(String data)
    //{
    //    char[] separator = { '$', '/', '*' };
    //    string[] splitString = data.Split(separator);

    //    foreach (var item in splitString)
    //    {
    //        Console.WriteLine(item);
    //    }
    //}

}