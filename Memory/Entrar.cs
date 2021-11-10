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
            label3.Show();
            textBox3.Show();
            label4.Show();
            textBox4.Show();
            button4.Show();
            button6.Show();

            button3.Hide();
            label1.Hide();
            textBox1.Hide();
            label2.Hide();
            textBox2.Hide();
            button2.Hide();
            button1.Hide();

            

            
            
           
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
                textBox5.Show();
                radioButton1.Show();
                 radioButton2.Show();
                 radioButton3.Show();
                 button5.Show();
                 button7.Show();
                 button8.Show();
                

                 button3.Hide();
                 label3.Hide();
                label1.Hide();
                 textBox1.Hide();
                label2.Hide();
                 textBox2.Hide();
                 button2.Hide();
                  button1.Hide();
                
                
               

            }
            else if (mensaje == "NO")
            {
                MessageBox.Show("Acceso denegado. Revise los parámetros de entrada o regístrese");
            }
           // this.BackColor = Color.Gray;
            //server.Shutdown(SocketShutdown.Both);
            //server.Close();
        }

        private void Entrar_Load(object sender, EventArgs e)
        {
            label3.Hide();
            textBox3.Hide();
            label4.Hide();
            textBox4.Hide();
            button4.Hide();
            button6.Hide();
            button5.Hide();
            button7.Hide();
            button8.Hide();
          
            
            textBox5.Hide();

            radioButton1.Hide();
            radioButton2.Hide();
            radioButton3.Hide();
            button5.Hide();


        }

        private void button4_Click(object sender, EventArgs e)
        {
            string mensaje = "2/" + textBox3.Text + "/" + textBox4.Text + "";
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);


            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split(',')[0];

            if (mensaje == "SI")
            {
                MessageBox.Show("Registrado correctamente");
               

            }
            else if (mensaje == "NO")
            {
                MessageBox.Show("No se ha podido registrar");
            }
        }

        private void button6_Click(object sender, EventArgs e)
        {
            label1.Show();
            textBox1.Show();
            label2.Show();
            textBox2.Show();
            button2.Show();
            button1.Show();

            label3.Hide();
            textBox3.Hide();
            label4.Hide();
            textBox4.Hide();
            button4.Hide();
            button6.Hide();
        }

        private void button7_Click(object sender, EventArgs e)
        {
            if (radioButton1.Checked)
            {
                string mensaje = "3/" + textBox5.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);


                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split(',')[0];



                MessageBox.Show("El jugador ganador de esta fecha es:" + mensaje);
               

            }
            if (radioButton2.Checked)
            {
                string mensaje = "4/" + textBox5.Text;
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
                    
                }

            }
            if (radioButton3.Checked)
            {
                string mensaje = "5/" + textBox5.Text;
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
                    
                }

            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            string mensaje = "7/" + textBox1.Text;
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);


           
            
            
            
            textBox5.Hide();

            radioButton1.Hide();
            radioButton2.Hide();
            radioButton3.Hide();
            button7.Hide();
            button5.Hide();
            button8.Hide();

            label1.Show();
            textBox1.Show();
            label2.Show();
            textBox2.Show();
            button2.Show();
            button1.Show();


        }

        private void button8_Click(object sender, EventArgs e)
        {
            string mensaje = "6/" ;
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);


            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split(',')[0];

            string[] line = mensaje.Split('/');
            int i = 1;

            while (i < line.Length - 1)
            {
                dataGridView1[1, i].Value = line[i];
                i++;
            }

          


            MessageBox.Show( "Estos son los usuarios conectados: "+ line[1]);

        }




        

        
    }
}
