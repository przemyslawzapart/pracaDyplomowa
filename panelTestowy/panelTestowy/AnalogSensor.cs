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
        public float value { get; set; }   
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
            value = 0;
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
            pbProgres.Value = (int)value;

        }
        public void setValue(double value)
        {
            if(value < minValue)
                pbProgres.ForeColor = Color.Yellow;
            else if(value > maxValue)
                pbProgres.ForeColor = Color.Red;
            else 
                pbProgres.ForeColor = Color.Green;
            
            pbProgres.Value = (int) ((100/maxRange) *value);
            lblValue.Text = value.ToString("c1");

        }
        public void setNewInstelingen(int _maxValue, int _minValue, int _range, string _name, string _unit)
        {
            maxRange = maxRange;
            maxValue = _maxValue;
            minValue = _minValue;
            lblName.Text = _name;
            unit = _unit;
            lblUnit.Text = unit;
            pbProgres.Maximum = _range;
        }

           

    }
}
