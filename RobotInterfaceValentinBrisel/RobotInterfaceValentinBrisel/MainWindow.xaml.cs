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
using System.Windows.Interop;
using static RobotInterfaceValentinBrisel.Robot;


namespace RobotInterfaceValentinBrisel
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>



    public partial class MainWindow : Window
    {
        ExtendedSerialPort serialPort1;
        //string receivedText;
        DispatcherTimer timerAffichage;
        Robot robot = new Robot();
        bool autoControlActivated = true;



        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ExtendedSerialPort("COM5", 115200, Parity.None, 8, StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived; ;
            serialPort1.Open();

            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Start();

            IR_gauche_display.Text = "0";
            IR_centre_display.Text = "0";
            IR_droit_display.Text = "0";
            vitesse_gauche_display.Text = "0";
            vitesse_droite_display.Text = "0";
        }



        private void TimerAffichage_Tick(object? sender, EventArgs e)
        {
            while (robot.byteListReceived.Count() > 0)
            {
                var c = robot.byteListReceived.Dequeue();
                DecodeMessage(c);
            }

        }

        public void SerialPort1_DataReceived(object? sender, DataReceivedArgs e)
        {
            //robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
            //textBoxReception.Text += "";

            for (int i = 0; i < e.Data.Length; i++)
            {
                robot.byteListReceived.Enqueue(e.Data[i]);
            }
        }

        private void SendMessage()
        {
            serialPort1.Write(textBoxEmission.Text);
            textBoxEmission.Text = "";
        }

        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            SendMessage();
        }

        private void textBoxEmission_KeyUp(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SendMessage();
            }

            switch (e.Key)
            {
                case Key.Left:
                    UartEncodeAndSendMessage(0x0051, 1, new byte[] { (byte)StateRobot.STATE_TOURNE_SUR_PLACE_GAUCHE });
                    break;

                case Key.Right:
                    UartEncodeAndSendMessage(0x0051, 1, new byte[]
                    {
                                (byte) StateRobot.STATE_TOURNE_SUR_PLACE_DROITE
                    });
                    break;

                case Key.Up:
                    UartEncodeAndSendMessage(0x0051, 1, new byte[]
                    { (byte) StateRobot.STATE_AVANCE});
                    break;

                case Key.Down:
                    UartEncodeAndSendMessage(0x0051, 1, new byte[]
                    { (byte) StateRobot.STATE_RECULE});
                    break;

                case Key.Escape:
                    UartEncodeAndSendMessage(0x0051, 1, new byte[]
                    { (byte) StateRobot.STATE_ARRET});
                    break;
            }
        }

        private void boutonClear_Click(object sender, RoutedEventArgs e)
        {
            textBoxReception.Text = "";
        }

        private void boutonTest_Click(object sender, RoutedEventArgs e)
        {
            if (autoControlActivated)
            {
                UartEncodeAndSendMessage((int)Command_ID.SET_ROBOT_AUTO_CONTROL, 1, new byte[] { 1 });
                autoControlActivated = false;
                

            }
            else
            {
                UartEncodeAndSendMessage((int)Command_ID.SET_ROBOT_AUTO_CONTROL, 1, new byte[] { 0 });
                autoControlActivated = true;

            }
        }

        private byte CalculateChecksum(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte checksum = 0xFE;

            checksum ^= (byte)msgFunction;
            checksum ^= (byte)(msgFunction >> 8);

            checksum ^= (byte)msgPayloadLength;
            checksum ^= (byte)(msgPayloadLength >> 8);

            for (int i = 0; i < msgPayloadLength; i++)
            {
                checksum ^= msgPayload[i];
            }

            return checksum;
        }

        private void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte[] trame1 = new byte[5];
            trame1[0] = 0xFE;
            trame1[1] = (byte)(msgFunction >> 8);
            trame1[2] = (byte)(msgFunction);
            trame1[3] = (byte)(msgPayloadLength >> 8);
            trame1[4] = (byte)(msgPayloadLength);

            byte[] trame = trame1.Concat(msgPayload).ToArray();
            trame = trame.Append(CalculateChecksum(msgFunction, msgPayloadLength, msgPayload)).ToArray();

            serialPort1.Write(trame, 0, trame.Length);
        }


        public enum StateReception
        {
            Waiting,
            FunctionMSB,
            FunctionLSB,
            PayloadLengthMSB,
            PayloadLengthLSB,
            Payload,
            CheckSum
        }

        StateReception rcvState = StateReception.Waiting;
        int msgDecodedFunction = 0;
        int msgDecodedPayloadLength = 0;
        byte[] msgDecodedPayload = new byte[0];
        int msgDecodedPayloadIndex = 0;

        private void DecodeMessage(byte c)
        {
            switch (rcvState)
            {
                case StateReception.Waiting:
                    if (c == 0xFE)
                    {
                        msgDecodedPayloadIndex = 0;
                        msgDecodedPayload = new byte[0];
                        rcvState = StateReception.FunctionMSB;
                    }
                    break;

                case StateReception.FunctionMSB:
                    msgDecodedFunction = c << 8;
                    rcvState = StateReception.FunctionLSB;
                    break;

                case StateReception.FunctionLSB:
                    msgDecodedFunction |= c;
                    rcvState = StateReception.PayloadLengthMSB;
                    break;

                case StateReception.PayloadLengthMSB:
                    msgDecodedPayloadLength = c << 8;
                    rcvState = StateReception.PayloadLengthLSB;
                    break;

                case StateReception.PayloadLengthLSB:
                    msgDecodedPayloadLength |= c;
                    rcvState = StateReception.Payload;
                    msgDecodedPayload = new byte[msgDecodedPayloadLength];
                    break;

                case StateReception.Payload:
                    msgDecodedPayload[msgDecodedPayloadIndex] = c;
                    rcvState = StateReception.Payload;

                    if ((msgDecodedPayloadLength - 1) == msgDecodedPayloadIndex)
                    {
                        rcvState = StateReception.CheckSum;
                    }
                    msgDecodedPayloadIndex++;
                    break;

                case StateReception.CheckSum:
                    if (CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload) == c)
                    {
                        ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    }
                    rcvState = StateReception.Waiting;
                    break;

                default:
                    rcvState = StateReception.Waiting;
                    break;
            }
        }

        private void displayMsg(byte[] msg)
        {
            textBoxReception.Text += Encoding.ASCII.GetString(msg);
            textBoxReception.Text += "\n";
        }

        public enum Command_ID
        {
            transmission_texte = 0x0080,
            reglage_led = 0x0020,
            distance_telemetre_IR = 0x0030,
            consigne_vitesse = 0x0040,
            RobotState = 0x0050,
            SET_ROBOT_STATE = 0x0051,
            SET_ROBOT_AUTO_CONTROL = 0x0052
        }

        private void ProcessDecodedMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            switch (msgFunction)
            {
                case (int)Command_ID.transmission_texte:
                    robot.receivedText = Encoding.ASCII.GetString(msgPayload);
                    displayMsg(msgPayload);
                    break;

                case (int)Command_ID.reglage_led:
                    if ((msgPayload[0] == (int)Robot.LED.led_rouge) && (msgPayload[1] == 1))
                    {
                        led_rouge.IsChecked = true;
                    }
                    else if ((msgPayload[0] == (int)Robot.LED.led_rouge) && (msgPayload[1] == 0))
                    {
                        led_rouge.IsChecked = false;
                    }

                    if ((msgPayload[0] == (int)Robot.LED.led_bleue) && (msgPayload[1] == 1))
                    {
                        led_bleue.IsChecked = true;
                    }
                    else if ((msgPayload[0] == (int)Robot.LED.led_bleue) && (msgPayload[1] == 0))
                    {
                        led_bleue.IsChecked = false;
                    }

                    if ((msgPayload[0] == (int)Robot.LED.led_blanc) && (msgPayload[1] == 1))
                    {
                        led_blanc.IsChecked = true;
                    }
                    else if ((msgPayload[0] == (int)Robot.LED.led_blanc) && (msgPayload[1] == 0))
                    {
                        led_blanc.IsChecked = false;
                    }

                    break;

                case (int)Command_ID.distance_telemetre_IR:
                    robot.distanceTelemetreGauche = msgPayload[0];
                    IR_gauche_display.Text = robot.distanceTelemetreGauche.ToString();

                    robot.distanceTelemetreCentre = msgPayload[1];
                    IR_centre_display.Text = robot.distanceTelemetreCentre.ToString();

                    robot.distanceTelemetreDroit = msgPayload[2];
                    IR_droit_display.Text = robot.distanceTelemetreDroit.ToString();
                    break;

                case (int)Command_ID.consigne_vitesse:
                    robot.vitesse_gauche = msgPayload[0];
                    vitesse_gauche_display.Text = robot.vitesse_gauche.ToString();

                    robot.vitesse_droit = msgPayload[1];
                    vitesse_droite_display.Text = robot.vitesse_droit.ToString();
                    break;

                case (int)Command_ID.RobotState:
                    int instant = (((int)msgPayload[1]) << 24) + (((int)msgPayload[2]) << 16)
                    + (((int)msgPayload[3]) << 8) + ((int)msgPayload[4]);

                    textBoxReception.Text += "\nRobot State : " + ((Robot.StateRobot)(msgPayload[0])).ToString() +
                    " - " + instant.ToString() + " ms";
                    break;

                default:
                    break;

            }
        }

    }
}
