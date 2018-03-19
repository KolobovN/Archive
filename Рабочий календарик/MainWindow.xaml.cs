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
using System.Data.SqlClient;
using System.Configuration;
using System.Data;

namespace SphereCraft
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            DataBaseConnection.Initialize();    //Инициализация адаптера
            WorkDayDataTables.Initialize();     //Инициализация таблиц данных
            if (SphereCraft.Properties.Settings.Default["UserFIO"].ToString().Equals("DefaultUserName"))
            {
                mainframe.Navigate(new Uri("pSettings.xaml", UriKind.Relative));
            }
            else
            {
                mainframe.Navigate(new Uri("StartWindow.xaml", UriKind.Relative));
            }
        }
    }
}
