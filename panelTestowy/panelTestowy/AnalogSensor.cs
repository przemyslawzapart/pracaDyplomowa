using System;
using System.Collections.Generic;
using System.Configuration;
using System.Drawing;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace panelTestowy
{
    internal class AnalogSensor
    {
        public double value { get; set; }   
        public string name { get; set; }    
        public int minValue { get; set; }
        public int maxValue { get; set; }
        public string unit { get; set; }
        public int maxRange { get; set; }
        public Label lblValue { get; set; }
        public Label lblUnit { get; set; }
        public Label lblName { get; set; }
        public ProgressBar pbProgres{ get; set; }


        public bool enabled { get; set; }   

        public AnalogSensor(Label _name, Label _value, Label _unit, ProgressBar _progres)
        {
            this.value = 20;
            minValue = 20;
            maxValue = 70;
            name = "analog";
            unit = "V";
            enabled = true;
            lblValue = _value;
            lblUnit = _unit;    
            lblName = _name;
            pbProgres = _progres;
            maxRange = 100;

            lblName.Text = name;
            lblValue.Text = value.ToString();
            lblUnit.Text = unit.ToString();
            pbProgres.Maximum = maxRange;
            pbProgres.Value = (int)value;

        }
        public void setValue(double value)
        {
            this.value = value;
            double a = ((100 / (double)maxRange) * value);
            if (value < minValue)
                pbProgres.ForeColor = Color.Yellow;
            else if(value > maxValue)
                pbProgres.ForeColor = Color.Red;
            else 
                pbProgres.ForeColor = Color.Green;
            if(pbProgres.Value <= a)
                pbProgres.Value = (int) a;
            lblValue.Text = value.ToString();

        }
        public void setValue()
        {
             Console.Write("value ");
            Console.WriteLine(value);
            Console.Write("range ");
            Console.WriteLine(maxRange);
            Console.Write("progres bar ");
            double a = ((100 / (float)maxRange) * value);
            Console.WriteLine(a);

            if (value < minValue)
                pbProgres.ForeColor = Color.Yellow;
            else if (value > maxValue)
                pbProgres.ForeColor = Color.Red;
            else
                pbProgres.ForeColor = Color.Green;

            pbProgres.Value = (int)((100 / (float)maxRange) * value);
            pbProgres.Value = (int)a;
            lblValue.Text = value.ToString();
            //Console.WriteLine(value.ToString("c1"));


        }
        //public void setNewInstelingen(int _maxValue, int _minValue, int _range, string _name, string _unit)
        public void setNewInstelingen(string _name, int _minValue, int _maxValue, int _range,  string _unit)
        {
            maxRange = _range;
            maxValue = _maxValue;
            minValue = _minValue;
            name = _name;
            lblName.Text = name;
            unit = _unit;
            lblUnit.Text = unit;
            pbProgres.Maximum = _range;
            Console.WriteLine(value);
            setValue();
        }




           

    }
}
