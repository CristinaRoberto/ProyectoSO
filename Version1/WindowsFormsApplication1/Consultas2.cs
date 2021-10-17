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
    public partial class Consultas2 : Form
    {
        Socket server;
        public Consultas2()
        {
            InitializeComponent();
        }

        public void setServer(Socket servidor)
        {
            this.server = servidor;

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (radioButton1.Checked)
            {
                string mensaje = "3/" + textBox1.Text ;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
                

                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split(',')[0];



                MessageBox.Show("El jugador ganador de esta fecha es:" + mensaje);
                Close();

            }
            if (radioButton2.Checked)
            {
                string mensaje = "4/" + textBox1.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
                

                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split(',')[0];


                if (mensaje == "NO")
                {
                    MessageBox.Show("No existe ningún jugador con este nombre");

                }
                else
                {
                    MessageBox.Show("Puntuación total del jugador:" + mensaje);
                    Close();
                }

            }
            if (radioButton3.Checked)
            {
                string mensaje = "5/" + textBox1.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
                

                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split(',')[0];


                if (mensaje == "NO")
                {
                    MessageBox.Show("No existe ningún jugador con este nombre");

                }
                else
                {
                    MessageBox.Show("Fecha y hora:" + mensaje);
                    Close();
                }

            }
        }
    }
}
