using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using ExtendedSerialPort_NS;
using System.IO.Ports;
using System.Windows.Threading;
using System.Runtime.ConstrainedExecution;

namespace RobotInterfaceValentinBrisel
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    
    

    public partial class MainWindow : Window
    {
        int n = 0;
        ExtendedSerialPort serialPort1;
        //string receivedText;
        DispatcherTimer timerAffichage;
        Robot robot = new Robot();



        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ExtendedSerialPort("COM10", 115200, Parity.None, 8, StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived; ;
            serialPort1.Open();

            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Start();

        }

        private void TimerAffichage_Tick(object? sender, EventArgs e)
        {           
            while (robot.byteListReceived.Count() > 0)
            {
                var c = robot.byteListReceived.Dequeue();
                textBoxReception.Text += c.ToString("X2")+" ";
                //textBoxReception.Text += Encoding.ASCII.GetString(new byte[] { c });
                
            }
        }

        public void SerialPort1_DataReceived(object? sender, DataReceivedArgs e)
        {
            //robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
            //textBoxReception.Text += "000";

            for (int i = 0; i < e.Data.Length; i++)
            {
                robot.byteListReceived.Enqueue(e.Data[i]);
            }
        }

        private void SendMessage()
        {
            //textBoxReception.Text += "\nReçu : " + textBoxEmission.Text;

            serialPort1.WriteLine(textBoxEmission.Text);
            textBoxEmission.Text = " ";
        }

        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            if (n == 0)
            {
                buttonEnvoyer.Background = Brushes.RoyalBlue;
                n = 1;
            }
            else
            {
                buttonEnvoyer.Background = Brushes.Beige;
                n = 0;
            }
            
            SendMessage();
        }

        private void textBoxEmission_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                e.Handled = true;
                SendMessage();
            }
        }

        private void boutonClear_Click(object sender, RoutedEventArgs e)
        {
            textBoxReception.Text = "";
        }

        private void boutonTest_Click(object sender, RoutedEventArgs e)
        {
            byte[] byteList = new byte[20];

            for (int i = 0; i < 20; i++)
            {
                byteList[i] = (byte)(2*i);
            }

            serialPort1.Write(byteList, 0, byteList.Length);

        }
    }
}
