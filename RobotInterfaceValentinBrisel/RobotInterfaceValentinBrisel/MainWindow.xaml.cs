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

namespace RobotInterfaceValentinBrisel
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        int n = 0;
        public MainWindow()
        {
            InitializeComponent();
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
            textBoxReception.Text += "\nReçu : " + textBoxEmission.Text;
            textBoxEmission.Text = " ";

        }

        private void textBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                textBoxReception.Text += "\nReçu : " + textBoxEmission.Text;
                textBoxEmission.Text = " ";
            }
        }
    }
}
