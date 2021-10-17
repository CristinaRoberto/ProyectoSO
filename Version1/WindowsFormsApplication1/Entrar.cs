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
    public partial class Entrar : Form
    {
        Socket server;
        public Entrar()
        {
            InitializeComponent();
        }
        public void setServidor(Socket Servidor)
        {
            this.server = Servidor;
        
        }
        private void button3_Click(object sender, EventArgs e)
        {
            IPAddress direc = IPAddress.Parse("192.168.56.102");
            IPEndPoint ipep = new IPEndPoint(direc, 9000);

            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                server.Connect(ipep);//Intentamos conectar el socket
                this.BackColor = Color.Green;
                MessageBox.Show("Conectado");

            }
            catch (SocketException ex)
            {
                //Si hay excepcion imprimimos error y salimos del programa con return 
                MessageBox.Show("No he podido conectar con el servidor");
                return;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Registro r = new Registro();
            r.setServer(server);
            r.ShowDialog();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            string mensaje = "1/" + textBox1.Text + "/" + textBox2.Text + "";
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
           

            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split(',')[0];

            if (mensaje == "SI")
            {
                MessageBox.Show("Acceso a la aplicación");
                Consultas2 r = new Consultas2();
                r.setServer(server);
                r.ShowDialog();

            }
            else if (mensaje == "NO")
            {
                MessageBox.Show("Acceso denegado. Revise los parámetros de entrada o regístrese");
            }
           // this.BackColor = Color.Gray;
            //server.Shutdown(SocketShutdown.Both);
            //server.Close();
        }

        
    }
}
