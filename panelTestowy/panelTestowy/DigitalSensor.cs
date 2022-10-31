using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace panelTestowy
{
    internal class DigitalSensor
    {
        public string name { get; set; }
        public bool state { get; set; } 
        public bool enabled { get; set; }
        public Color color { get; set; }
        public Label label { get; set; }
        public Button button { get; set; }

        public DigitalSensor(Label label, Button button)
        {
            this.button = button;
            this.label = label;            

            name = "digital ingang";
            state = false;
            enabled = true;   
            color = Color.Red;
            this.button.BackColor = color;
            this.label.Text = name;
            //galaz wtorek
            
        }
        public void chaneState(bool buttonState)
        {
            if (buttonState)
                button.BackColor = color;
            else
                button.BackColor = Color.White;
        }
    }
}
