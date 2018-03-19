using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Forms;
using System.IO;

namespace SphereCraft
{
    /// <summary>
    /// Логика взаимодействия для Settings.xaml
    /// </summary>
    public partial class pSettings : Page
    {
        public pSettings()
        {
            InitializeComponent();
            UserNameBox.Text = SphereCraft.Properties.Settings.Default["UserFIO"].ToString();
            ReportPath.Content = SphereCraft.Properties.Settings.Default["ReportPath"].ToString();
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrWhiteSpace(ReportPath.Content.ToString()))
            {
                System.Windows.MessageBox.Show("Отчёт будет создаваться в папке с программой");
                SphereCraft.Properties.Settings.Default["UserFIO"] = UserNameBox.Text;
                SphereCraft.Properties.Settings.Default["ReportPath"] = Directory.GetCurrentDirectory();
                StartWindow pg = new StartWindow();
                NavigationService.Navigate(pg);
            }
            else
            {
                SphereCraft.Properties.Settings.Default["UserFIO"] = UserNameBox.Text;
                SphereCraft.Properties.Settings.Default["ReportPath"] = ReportPath.Content;
                StartWindow pg = new StartWindow();
                NavigationService.Navigate(pg);
            }
        }

        private void ReportPath_Click(object sender, RoutedEventArgs e)
        {
            FolderBrowserDialog ChoosePathDialog = new FolderBrowserDialog();
            ChoosePathDialog.ShowDialog();
            if (string.IsNullOrWhiteSpace(ChoosePathDialog.SelectedPath))
            {
                ReportPath.Content = Directory.GetCurrentDirectory();                
            }
            else
            {
                ReportPath.Content = ChoosePathDialog.SelectedPath;
            }
        }

        private void UserNameBoxGotFocus(object sender, RoutedEventArgs e)
        {
            if (UserNameBox.Text == "DefaultUserName")
            {
                UserNameBox.Text = "";
            }
        }

        private void UserNameBoxLostFocus(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrWhiteSpace(UserNameBox.Text))
            {
                UserNameBox.Text = "DefaultUserName";
            }
        }
    }
}
