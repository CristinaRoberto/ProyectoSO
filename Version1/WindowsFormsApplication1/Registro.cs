using System;
using System.Collections.Generic;
using System.ComponentModel;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;

namespace WindowsFormsApplication1
{
    public partial class Registro : Form
    {
        Socket server;
        
        public Registro()
        {
            InitializeComponent();
        }

        public void setServer(Socket Servidor)
        {
            this.server = Servidor;
        
        }

        

        private void button1_Click(object sender, EventArgs e)
        {
            
            string mensaje = "2/" + textBox1.Text + "/" + textBox2.Text + "";
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
           

            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split(',')[0];

            if (mensaje == "SI")
            {
                MessageBox.Show("Registrado correctamente");
                Close();

            }
            else if (mensaje == "NO")
            {
                MessageBox.Show("No se ha podido registrar");
            }
            
        }
    }
}
