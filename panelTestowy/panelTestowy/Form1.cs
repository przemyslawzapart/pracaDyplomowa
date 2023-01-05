using System;
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
using ComboBox = System.Windows.Forms.ComboBox;
using TextBox = System.Windows.Forms.TextBox;

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

        string date;
        string time;

        bool firstStart = true;
        

        //bool stateHand = false;
        //bool stateOff = true;
        //bool stateAuto = false;


        List<GroupBox> gbPanelList = new List<GroupBox>();   
        List<Button> btnPosList = new List<Button>();

        List<TextBox> tbxGeneralSettingsList = new List<TextBox>();
        List<TextBox> tbxAnalogSettingsList = new List<TextBox>();
        List<TextBox> tbxDigitalSettingsList = new List<TextBox>();

        List<GroupBox> settingsPanelList = new List<GroupBox>();

        List<Button>buttonStateList = new List<Button>();

        int analogSelectedIndex = 0;

        //int timerStart_a = 0;
        bool startButtonStatus = false;
        bool stopButtonStatus = false;
        bool resetButtonStatus = false;
        // List<bool>buttonState = new List<bool>();
        bool[] buttonState = new bool[3];//start stop reset
        bool[] keyStateArray = new bool[3];

        public Form1()
        {
            InitializeComponent();
            //Form1 form = new Form1();
            //form.Size = new Size(100, 100);
            //form.ShowDialog();
            //form.Show();
            //form.Visible = true;
            this.Size = new Size(680,740);

            mySerialPort = new Serial(cbxSelectCom, cbxSelectBaudRate, btnSerialRefresh,
                    btnSerialConnect, btnSerialDisConnect, gbConnection, lblSerialStatus, serialPort1);
            mySerialPort.getSerialPorts();

            //mySerialPort.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);

            pbAnalog1.ForeColor = Color.Yellow;
                      
            sensorList.Add(new AnalogSensor(lblName1, lblAnalog1, lblUnit1, pbAnalog1));
            sensorList.Add(new AnalogSensor(lblName2, lblAnalog2, lblUnit2, pbAnalog2));
            sensorList.Add(new AnalogSensor(lblName3, lblAnalog3, lblUnit3, pbAnalog3));
            sensorList.Add(new AnalogSensor(lblName4, lblAnalog4, lblUnit4, pbAnalog4));
            sensorList.Add(new AnalogSensor(lblName5, lblAnalog5, lblUnit5, pbAnalog5));
            sensorList.Add(new AnalogSensor(lblName6, lblAnalog6, lblUnit6, pbAnalog6));            
            sensorList.Add(new AnalogSensor(lblName7, lblAnalog7, lblUnit7, pbAnalog7));
            sensorList.Add(new AnalogSensor(lblName8, lblAnalog8, lblUnit8, pbAnalog8));

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

            gbPanelList.Add(gbValue);
            gbPanelList.Add(gbSettings);
            gbPanelList.Add(gbMessage);
            gbPanelList.Add(gbConnectionSettings);
            gbPanelList.Add(gbPswd);

            settingsPanelList.Add(gbSettingsGeneral);
            settingsPanelList.Add(gbSettingsAnalog);
            settingsPanelList.Add(gbSettingsDigital);
            settingsPanelList.Add(gbSettingsExtra);


            btnPosList.Add(btnPosHand);
            btnPosList.Add(btnPosOff);
            btnPosList.Add(btnPosAuto);


            buttonStateList.Add(btnPosHand);
            buttonStateList.Add(btnPosOff);
            buttonStateList.Add(btnPosAuto);
            buttonStateList.Add(btnStart);
            buttonStateList.Add(btnStop);   
            buttonStateList.Add(btnReset);

            setBtnPos(1);
            int a = 0;
            makeControlsList(tbxGeneralSettingsList, gbSettingsGeneral);
            makeControlsList(tbxAnalogSettingsList, gbSettingsAnalog);
            makeControlsList(tbxDigitalSettingsList, gbSettingsDigital);

            cbSettingsDigitalNoNc.SelectedIndex = 0;
            foreach (var item in tbxGeneralSettingsList)
            {
                item.Text = a.ToString();
                item.Enabled = false;
                a++;
            }
            a= 0;
            foreach (var item in tbxAnalogSettingsList)
            {
                item.Text = a.ToString();
                item.Enabled = false;
                a++;
            }
            a = 0;
            foreach (var item in tbxDigitalSettingsList)
            {
                item.Text = a.ToString();
                item.Enabled = false;
                a++;
            }

            getSensrosAnalogName(comboBox1);
        }


       

        private void makeControlsList<T>(List<T> list, GroupBox groupBox)
        {
            List<T> tempList = groupBox.Controls.OfType<T>().ToList();
            int size = tempList.Count() - 1; // list 9 to 1
            for (int i = 0; i < tempList.Count(); i++) //reversing list (1 to 9)
            {
                list.Add(tempList[size - i]);                
            }
        }

        private void sendData(String dataToSend)
        {
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
            buttonState[0] = true;  
        }

        private void btnStop_Click(object sender, EventArgs e)
        {           
            buttonState[1] = true;
        }

        private void btnReset_Click(object sender, EventArgs e)
        {            
            buttonState[2] = true;
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
            Console.WriteLine("data przysly : {0}",data);
            //data.TrimEnd();
            //data.TrimStart();
            //Console.WriteLine("1: {0} 2: {1} 3: {2} 4: {3}", data[0], data[1], data[2], data[3]);
            //char[] p = { '\n',' ' };
            //data.Trim(p);
            //Console.WriteLine(data);
            //Console.WriteLine("1: {0} 2: {1} 3: {2} 4: {3}", data[0], data[1], data[2], data[3]);
            //char[] c = { '.', '*', '!', '%' };
            //string[] d = data.Split(c);
            //foreach (var item in d)
            //{
            //    Console.WriteLine("item : {0}", item);
            //}




            string res = data.Substring(0, 1);
            Console.WriteLine("pierwszy znak : {0} , {1}", res, data);
            Console.WriteLine("pierwszy wyraz to 1: {0}, 2: {1} , 3: {2} , 4: {3}  : ", data[0], data[1], data[2], data[3]);
            if (String.Compare(res, "$") == 0)
            {
                digitalSettings(data);
                return;
            }
            else if (String.Compare(res, "@") == 0)
            {
                analogSettings(data);
            }
            else if (String.Compare(res, "#") == 0)
            {
                setValues(data);
            }

        }

        private void setValues(string data)
        {
            Console.WriteLine("incoming  data : {0}", data);
            char[] separator = { '#', '/', '*' };
            string[] splitString = data.Split(separator);
            foreach (var item in splitString)
            {
                Console.WriteLine("value    = {0}", item);
            }
            //setDate(splitString[1]);
            //setTime(splitString[2]);
            setDateTime(splitString[1], splitString[2]);
            setRpm(splitString[3]);
            setEnginMessage(splitString[4]);
            setDigitalIn(splitString[5]);
            setDigitalOut(splitString[6]);

            sensorList[0].setValue(splitString[7]);
            sensorList[1].setValue(splitString[8]);
            sensorList[2].setValue(splitString[9]); 
            sensorList[3].setValue(splitString[10]);                
            //sensorList[4].setValue(splitString[11]);
            //sensorList[5].setValue(splitString[12]);
            //sensorList[6].setValue(splitString[13]);
            //sensorList[7].setValue(splitString[14]);

        }

        private void setDateTime(string v1, string v2)
        {
            String date = v1 + " / " + v2;
           // btnDateTime.Text = date;
        }

        private void setAnalogValue(string v)
        {
            Console.WriteLine("anlog {0}",v);
        }

        private void setDigitalOut(string v)
        {

            Console.WriteLine("digital out {0}: ",v);
          //  Int32.TryParse(v, out int val);

            //int val = Convert.ToInt32(v, 16);
            //Console.WriteLine("val = {0}", val);
            //for (int i = 0; i < 16; i++)
            //{
            //    if ((val & (1 << i)) != 0)
            //    {
            //        digitalSensorList[i].changeState(true);
            //        Console.Write("1");
            //    }

            //    else
            //    {
            //        digitalSensorList[i].changeState(false);
            //        Console.Write("0");
            //    }

            //}

            //Console.WriteLine();
        }

        private void setDigitalIn(string v)
        {

            Console.WriteLine("digial in : {0}",v);
            int val = Convert.ToInt32(v, 16);
            Console.WriteLine("val = {0}", val);
            for (int i = 0; i < 16; i++)
            {
                if ((val & (1 << i)) != 0)
                {
                    digitalSensorList[i].changeState(false);
                    Console.Write("1");
                }

                else
                {
                    digitalSensorList[i].changeState(true);
                    Console.Write("0");
                }

            }

            Console.WriteLine();

        }

        private void setEnginMessage(string v)
        {

            Console.WriteLine(v);
        }

        private void setTime(string v)
        {
            btnDateTime.Text = 
            time = v;
            Console.WriteLine(v);
        }

        private void setDate(string v)
        {
            date = v;
            Console.WriteLine(v);
        }

        private void setRpm(string rpm)
        {
            Console.WriteLine(rpm);
        }

        private void analogSettings(string data)
        {
            Console.WriteLine("analog data : {0}", data);
            char[] separator = { '@', '/', '*' };
            string[] splitString = data.Split(separator);

            foreach (var item in splitString)
            {
                Console.WriteLine("data    = {0}", item);
            }

            int id = Int32.Parse(splitString[2]);
            sensorList[id].setNewInstelingen(splitString[3], Int32.Parse(splitString[5]), Int32.Parse(splitString[6]), Int32.Parse(splitString[7]), splitString[4]);
            setAnalogSettings(id);
        }

        private void digitalSettings(string data)
        {
            Console.WriteLine("digital data : {0}",data);
            char[] separator= { '$', '/', '*'};
            string[] splitString = data.Split(separator);

            Console.WriteLine("id   = {0}, name = {1}, state = {2} ", splitString[2], splitString[3], splitString[4]);

            int id = Int32.Parse(splitString[2]);
            digitalSensorList[id].setName(splitString[3]);            
        }

        private void changeDigitalSensorState(DigitalSensor sensor, bool state)
        {
            if (state)
                makeLog(sensor);
            sensor.changeState(state);
        }
        private void makeLog(DigitalSensor sensor)
        {
            string data = getDateTime()+sensor.name;
            tbxLog.AppendText(data); 
            tbxLog.AppendText(Environment.NewLine);

        }
        private string getDateTime()
        {
            //return "2029/11/25  19:45:88  ";
            return "   " + date + " " + time + "\t";
        }
        private void btnGetAnalogValues_Click(object sender, EventArgs e)
        {

            getAllData();
            //if (comboBox1.SelectedIndex >0)
            //{
            //    string data = "@";
            //    data += comboBox1.SelectedIndex.ToString();
            //    data += "/value*";
            //    sendData(data);
            //}


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

            setSettingsPanel(0);
            getSensrosAnalogName(comboBox1);//sensorList

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
                    item.Location = new Point(0, 100);
                }
                else
                {
                    item.Visible = false;
                }   
            }
            if(loginState && index == 1)//zsalogowany
            {  
                lblLogin.Visible = false; 
            }  
            else//nastapilo wylogowanie
            {
                loginState = false;
                makeSettingsPanelState(false);
                lblLogin.Visible = false;
            }
                
        }
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {          
            

            if (comboBox1.SelectedIndex > 0)
            {
                analogSelectedIndex = comboBox1.SelectedIndex-1;

                btnSendAnalogSettings.Enabled = true;
                int a = analogSelectedIndex;
               //a--;
                //string dataToSend = "#/G/A/" + analogSelectedIndex.ToString() + "*";
                string dataToSend = "@/A/" + a.ToString() + "*";
               // Console.WriteLine(dataToSend);
                sendData(dataToSend);
                comboBox1.SelectedIndex = 0;
            }
           // else 
               //btnSendAnalogSettings.Enabled = false;


        }

        
        private void groupBox5_Enter(object sender, EventArgs e)
        {

        }

        private void btnPswdReset_Click(object sender, EventArgs e)
        {
            tbPswd.Text = "";
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
                tbPswd.Text = "123";
                setloginSettingsState(true);
                setPanel(1);
            }
            else
            {
                lblPswd.Visible = true;
            }        
        }
        private void setloginSettingsState(bool state)
        {
            if(state)
                loginTimer.Enabled = true;
            loginState = state;
            makeSettingsPanelState(state);
        }
        private void makeSettingsPanelState(bool state)
        {
            makeCollectionState(tbxGeneralSettingsList, state);
            makeCollectionState(tbxAnalogSettingsList, state);
            makeCollectionState(tbxDigitalSettingsList, state);
        }
        private void makeCollectionState(List<TextBox> list, bool state)
        {
            foreach (var item in list)
            {
                item.Enabled = state;
            }

        }
        private void setLoginState(bool state){

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
           // gbTime = tbx;    
        }

        private void button5_Click(object sender, EventArgs e)
        {
            tbx.Text = "a";   
        }

        private void tbxPisz_TextChanged(object sender, EventArgs e)
        {
            
        }

        private void btnSerialConnect_Click(object sender, EventArgs e)
        {
            mySerialPort.makeConnestion();
            if (mySerialPort.connected)
            {
                timerSendData.Enabled = true;
                getAllData();

            }
                
        }

        private void btnSerialDisConnect_Click(object sender, EventArgs e)
        {
            mySerialPort.closeConnection();
            if (!mySerialPort.connected)
                timerSendData.Enabled =false;

        }

        private void btnSerialRefresh_Click(object sender, EventArgs e)
        {
            mySerialPort.getSerialPorts();
        }

        private void chekcIncomingString(String data)
        {
            Console.WriteLine("data : {0} : ", data);

            char[] separator = { '$', '/', '*' };
            string[] splitString = data.Split(separator);

            foreach (var item in splitString)
            {
                Console.WriteLine(item);
            }

            //Console.WriteLine("id   = {0}, name = {1}, state = {2} ", splitString[2], splitString[3], splitString[4]);

            //int id = Int32.Parse(splitString[2]);
            //digitalSensorList[id].setName(splitString[3]);
        }
        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
             
            incomingData = serialPort1.ReadTo("*");
            

                Console.WriteLine(incomingData);
                this.Invoke(new EventHandler(ShowData)); 
                




           ////String data = d.Trim('@', '*');
           // Console.WriteLine("got data {0} : ", incomingData);
           //  //chekcIncomingString(incomingData);
           //  



            // String d = serialPort1.ReadExisting();
            // Console.WriteLine(d);
            // String data = d.Trim('@', '*');
            // Console.WriteLine("got data {0} : ", data);
            //// chekcIncomingString(data);
            //// this.Invoke(new EventHandler(ShowData));

            //incomingData = serialPort1.ReadExisting();
            //incomingData = serialPort1.ReadTo("*");  // to zmienic zeby nie czekalo bo wywali blad
            //data += "*";
            //Console.WriteLine(data);
            //String data = "";
            //char d = (char)serialPort1.ReadChar();

            //data += d;

            //while (d!='*')
            //{
            //    d = (char)serialPort1.ReadChar();
            //    data += d;

            //}
            //Console.WriteLine(data);

            // incomingData += "*";
            //char incomingData = serialPort1.ReadExisting();
            // char data = (char)serialPort1.ReadChar();
            // incomingData += data;
            // if(data == '*')

            // incomingData = "";


            // String d = serialPort1.ReadExisting();
            // Console.WriteLine(d);
            //// String data = d.Trim('@', '*');
            // Console.WriteLine(d);
            //checkIncomingString(data);

        }
        private void ShowData(object sender, EventArgs e)
        {

           // chekcIncomingString(incomingData);

            sendText(incomingData);
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
              //  sendText(1, data);
               Console.WriteLine("send data {0}",data);
            }
        }
        private void sendText( int a ,string text)
        {


            //textBox.AppendText(text);
            //if (string.Compare(text, "OK") > 0 || text.Contains("ERROR"))
            //    textBox.AppendText("\n");
            //textBox.AppendText(Environment.NewLine);
            Console.WriteLine(incomingData);
           // incomingData = "";
            //checkIncomingString(incomingData);

            //if (firstStart)
            //{
            //    getAllData();
            //    firstStart = false;
            //}

        }    
        //} private void sendText(System.Windows.Forms.TextBox textBox, string text)
        //{
            

        //    textBox.AppendText(text);
        //    if (string.Compare(text, "OK") > 0 || text.Contains("ERROR"))
        //        textBox.AppendText("\n");
        //    textBox.AppendText(Environment.NewLine);
        //    Console.WriteLine(incomingData);
        //    checkIncomingString(incomingData);

        //    if (firstStart)
        //    {
        //        getAllData();
        //        firstStart = false;
        //    }
            
            
        //}
        private void getAllData()
        {
            //tutaj wyslij zapytanie do czujki +
            sendData("@Q*");
        }

        private void sendSerialdata(string data)
        {
            //Console.WriteLine("przyszlo");
           // Console.WriteLine(data);
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
            // sendData("!H*");
            

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
                if(a == pos)
                {
                    item.Enabled = false;
                    item.Size = new Size(110,75);
                    item.BackColor = Color.DodgerBlue;
                    keyStateArray[a] = true;
                }
                else
                {
                    item.Enabled = true;
                    item.Size = new Size(100, 65);
                    item.BackColor = Color.LightSkyBlue;
                    keyStateArray[a] = false;
                }
                a++;
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

       

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBox2.SelectedIndex != 0)
            {
                string data = "@/D/" + comboBox2.SelectedIndex.ToString() + "*";
                sendData(data);
                Console.WriteLine(data);
            }
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
                
            
        }

        private void tabPage1_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button5_Click_1(object sender, EventArgs e)
        {

            //ustawieni fabryczne sa na procku wiec nie trzeba wysyłac


            //setSettings("#A/3/new/22/88/100/VQ*");

            //setSettings("#A/0/Battery 1/16/30/100/V*");
            //setSettings("#A/1/Battery 2/16/30/100/V*");
            //setSettings("#A/2/Press pomp/5/10/100/Bar*");
            //setSettings("#A/3/Oil press/4/10/100/Bar*");
            ////setSettings("#A/4/new/22/88/100/VQ*");
            ////setSettings("#A/5/new/22/88/100/VQ*");
            ////setSettings("#A/6/new/22/88/100/VQ*");
            ////setSettings("#A/7/new/22/88/100/VQ*");
            ////setSettings("#A/8/new/22/88/100/VQ*");

            //sendData("#/S/A/0/Battery 1/16/30/100/V*");
            //sendData("#/S/A/1/Battery 2/16/30/100/V*");
            //sendData("#/S/A/2/Press pomp/5/10/100/Bar*");
            //sendData("#/S/A/3/Oil press/4/10/100/Bar*");



        }
        private void setSettings(string data)
        {
            
            char[] c = { '#', '/', '*' };
            string[] splitString = data.Split(c);
            foreach (string s in splitString)
                Console.WriteLine("data : {0} ",s);
            if (String.Compare(splitString[1], "A") == 0)
            {
                if (Int32.TryParse(splitString[2], out int id) && Int32.TryParse(splitString[4], out int min) &&
                    Int32.TryParse(splitString[5], out int max) && Int32.TryParse(splitString[6], out int range) && checkRange(min, max, range))
                {
                    sensorList[id].setNewInstelingen(splitString[3], min, max, range, splitString[7]);
                    setAnalogSettings(id);
                    getSensrosAnalogName(comboBox1);
                    //getSensrosDigitalName(comboBox1);
                }
            }
            else if (String.Compare(splitString[1], "D") == 0)
            {
                
                if (Int32.TryParse(splitString[2], out int id))
                {
                    digitalSensorList[id].setName(splitString[3]);
                    
                }
            }


        }
        private void setDigitalSettings(string data)
        {

        }
        private void setAnalogSettings(int id)
        {
            tbxAnalogSettingsList[0].Text = sensorList[id].name;
            tbxAnalogSettingsList[1].Text = sensorList[id].unit;
            tbxAnalogSettingsList[2].Text = sensorList[id].minValue.ToString();
            tbxAnalogSettingsList[3].Text = sensorList[id].maxValue.ToString();
            tbxAnalogSettingsList[4].Text = sensorList[id].maxRange.ToString();
           
        }

       

        private void btnAnalogSettings_Click(object sender, EventArgs e)
        {
            setSettingsPanel(1);
            getSensrosAnalogName(comboBox1);//sensorList

        }      

        private void btnDigitalSettings_Click(object sender, EventArgs e)
        {
            setSettingsPanel(2);
            getSensrosDigitalName(comboBox2);
            string data = "@/D/0*";
            sendData(data);


        }

        private void btnExtraSettings_Click(object sender, EventArgs e)
        {
            setSettingsPanel(3);
        }

        
        private void setSettingsPanel(int pos) 
        {
            Console.WriteLine(pos);
            int a = 0;
            foreach (var item in settingsPanelList)
            {
                if (a++ == pos)
                {
                    gbSettings.Controls.Add(item);
                    item.Location = new Point(155, 65);
                    item.Visible = true;
                    
                }
                else
                {
                    item.Visible = false;
                }
            }
        }
        private void btnSendAnalogSettings_Click(object sender, EventArgs e)
        {
            int min = checkDataDecimal(tbxAnalogSettingsList[2]);            
            int max = checkDataDecimal(tbxAnalogSettingsList[3]);
            int range = checkDataDecimal(tbxAnalogSettingsList[4]);

            Console.WriteLine(" {0} , {1} , {2} ", min, max, range);
            if(min>=0 && max >= 0 && range >= 0 && checkRange(min, max, range))
            {
                
                sensorList[analogSelectedIndex].setNewInstelingen(tbxAnalogSettingsList[0].Text,min,max,range , tbxAnalogSettingsList[1].Text);
                lblSettingsMessage.Visible = true;
                lblSettingsMessage.Text = "ok "; 
                getSensrosAnalogName(comboBox1);
                Console.WriteLine("tutaj 1");
                //comboBox1.SelectedIndex = analogSelectedIndex;
                Console.WriteLine("tutaj 2");
                //$S/A/<id>/name/min/max/unit*

                string dataToSend = "#A/"+analogSelectedIndex.ToString() + "/";
                int a = 0;
                
                foreach (var item in tbxAnalogSettingsList)
                {
                    dataToSend += item.Text;
                    if(++a < tbxAnalogSettingsList.Count)
                        dataToSend += "/";
                }
                dataToSend += "*";

                foreach (var item in tbxAnalogSettingsList)
                {
                    item.Text = "";
                }



                Console.WriteLine(dataToSend);  
                sendData(dataToSend);
                comboBox1.SelectedIndex = 0;


            }
            else
            {
                lblSettingsMessage.Visible = true;
                lblSettingsMessage.Text = "something wrong ";
            }
                       
        }

        private bool checkRange(int min, int max, int range)
        {
            bool state = true;
            if (max < min || range < max)
                state = false;
            return state;

        }
        private int checkDataDecimal(TextBox tbx)
        {
            //int value = 0;
            bool state = Int32.TryParse(tbx.Text ,out int value);
            if(state && (value >= 0))
                return value;
            else
                return -1;
        }

        private void getSensrosAnalogName(ComboBox cbox)
        {   cbox.Items.Clear();
            cbox.Items.Add("Select analog Sensor");
            int a = 0;
            foreach (var item in sensorList)
            {
                cbox.Items.Add(item.name);
                Console.WriteLine("ale lipa {0}",a++);
            }     
            

        }
        private void getSensrosDigitalName(ComboBox cbox)
        {
            cbox.Items.Clear();
            foreach (var item in digitalSensorList)
                cbox.Items.Add(item.name);
        }

        private void btnSendDigitalSettings_Click(object sender, EventArgs e)
        {
            getSensrosDigitalName(comboBox2);
            Console.WriteLine();
        }

        private void comboBox3_SelectedIndexChanged(object sender, EventArgs e)
        {
            ;
        }

        private void loginTimer_Tick(object sender, EventArgs e)
        {
            loginTimer.Enabled = false;
            setloginSettingsState(false);
        }

        private void panel1_MouseHover(object sender, EventArgs e)
        {
            panel1.BackColor = Color.Tomato;
        }

        private void panel1_MouseLeave(object sender, EventArgs e)
        {
            panel1.BackColor = Color.Violet;
        }

        private void btnGetAllSettings_Click(object sender, EventArgs e)
        {
            getAllData();
        }

        private void button4_Click(object sender, EventArgs e)
        {

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            //tutaj wysylaj dane do kasty
            //@start, stop, reset,h,o,a*
            sendButtonState();


        }
        private void sendButtonState()
        {
            String data = "!";
            data += setState(buttonState);
            data += setState(keyStateArray);
            //foreach (var item in buttonState)
            //{
            //    if (item)
            //        data += "1";
            //    else
            //        data += "0";
            //}

            //foreach (var item in keyStateArray)
            //{

            //}
            data += "*";
            Console.WriteLine(data);
            sendData(data);
        }
        private String setState(bool[]buttons)
        {
            int value = 0;
            String data = "";


            int i = 0;
            foreach (var item in buttons)
            {
                if (item)
                    value |= (1 << i);
                // data += "1";

                //else
                //    // data += "0";
                    i++;
            }     data = value.ToString("X");    
            return data;
        } 
        private void btnStart_MouseDown(object sender, MouseEventArgs e)
        {
            // sendData("@statr/1*");
            // timerStart.Enabled = true;
            startButtonStatus = true;
            buttonState[0] = true;
            sendButtonState();
        }

        private void btnStart_MouseUp(object sender, MouseEventArgs e)
        {
            //sendData("@start/0*");
            //timerStart.Enabled=false;
            //startButtonStatus = false;
            buttonState[0] = false;
            sendButtonState();
        }
        private void btnStop_MouseDown(object sender, MouseEventArgs e)
        {
            buttonState[1] = true;
            sendButtonState();
        }

        private void btnStop_MouseUp(object sender, MouseEventArgs e)
        {
            buttonState[1] = false;
            sendButtonState();
        }

        private void btnReset_MouseDown(object sender, MouseEventArgs e)
        {
            buttonState[2] = true;
            sendButtonState();
        }

        private void btnReset_MouseUp(object sender, MouseEventArgs e)
        {
            buttonState[2] = false;
            sendButtonState();
        }

        private void setStatus(int state)
        {
            if (state == 0)
                chaneButton(Color.Red, "Error");
            else if (state == 1)
                chaneButton(Color.Green, "Running");
            else if (state == 2)
                chaneButton(Color.LightBlue, "Ready");

            
        }

        private void chaneButton(Color color, string text)
        {
            String status = "Status : ";
            status += text;
            button3.BackColor = color;
            button3.Text = status;
        }

        private void btnSendGeneralSettings_Click(object sender, EventArgs e)
        {
            setSettingsPanel(0);
            getSensrosAnalogName(comboBox1);//sensorList
        }

        private void dateTimePicker1_ValueChanged(object sender, EventArgs e)
        {
            string data = dateTimePicker1.Value.ToString();
            string time = dateTimePicker1.MinDate.ToString();
            Console.WriteLine(data);
            Console.WriteLine(time);    
        }
    }
}
