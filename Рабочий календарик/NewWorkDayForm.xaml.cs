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
    /// Логика взаимодействия для NewWorkDayForm.xaml
    /// </summary>
    public partial class NewWorkDayForm : Page
    {
        /* У данной формы 2 конструктора и 2 разных кнопки "Сохранить" с предустановленным свойством Visibility = "Collapsed"
         * В зависимости от конструктора одна из кнопок становится активной. 
         * Если передана только дата - то кнопка подразумевает добавление смены, если полный набор информации - редактирование смены
         * Возможно, в будущем стоит проверить возможность переназначения события в кнопке во время исполнения.
         */
        public NewWorkDayForm(DateTime SelectedDate)    //Форма добавления новой смены
        {
            InitializeComponent();
            wDateBlock.Text = SelectedDate.GetDateTimeFormats('D')[1];
            NewWorkDayButton.Visibility = System.Windows.Visibility.Visible;
        }

        public NewWorkDayForm(DayInfo SelectedDateInfo)     //Форма редактирования смены
        {
            InitializeComponent();
            wDateBlock.Text = SelectedDateInfo.getDate().GetDateTimeFormats('D')[1];
            wPSnameBox.Text = SelectedDateInfo.getPSname();
            wPSaddressBox.Text = SelectedDateInfo.getPSaddress();
            wPSworkTimeBox.Text = SelectedDateInfo.getPSworkTime();
            wCostBox.Text = SelectedDateInfo.getCost().ToString();
            wIsSubUrbFlag.IsChecked = SelectedDateInfo.getSubUrbFlag();
            UpdWorkDayButton.Visibility = System.Windows.Visibility.Visible;
            wTitle.Content = "Редактирование смены";
        }

        private void NewWorkDay_Click(object sender, RoutedEventArgs e)
        {
            object[] rowVals = new object[6];
            rowVals[0] = wDateBlock.Text;
            rowVals[1] = wPSnameBox.Text;
            rowVals[2] = wPSaddressBox.Text;
            rowVals[3] = wPSworkTimeBox.Text;
            rowVals[4] = Convert.ToString(wCostBox.Text);
            rowVals[5] = Convert.ToString(wIsSubUrbFlag.IsChecked);   
            WorkDayDataTables.WorkDayInfoTable.Rows.Add(rowVals);
            try
            {
                DataBaseConnection.InsertWorkDay(WorkDayDataTables.WorkDayInfoTable);
                MessageBox.Show("Смена сохранена", "", MessageBoxButton.OK);
                NavigationService.Navigate(new Uri("StartWindow.xaml", UriKind.Relative));
            }
            catch(Exception error)
            {
                MessageBox.Show(error.Message, "Ошибка при сохранении смены в БД", MessageBoxButton.OK);
            }
            finally
            {
                DataBaseConnection.CloseConnection();
            }
        }

        private void UpdWorkDay_Click(object sender, RoutedEventArgs e)
        {
            DayInfo NewValues = new DayInfo();
            NewValues.FillDayInfo(Convert.ToDateTime(wDateBlock.Text), wPSnameBox.Text, wPSaddressBox.Text, wPSworkTimeBox.Text,
                Convert.ToInt32(wCostBox.Text), Convert.ToBoolean(wIsSubUrbFlag.IsChecked));
            try
            {
                DataBaseConnection.UpdateWorkDay(NewValues);
                MessageBox.Show("Смена сохранена", "", MessageBoxButton.OK);
                NavigationService.Navigate(new Uri("StartWindow.xaml", UriKind.Relative));
            }
            catch(Exception error)
            {
                MessageBox.Show(error.Message, "Ошибка при сохранении смены в БД", MessageBoxButton.OK);
            }
            finally
            {
                DataBaseConnection.CloseConnection();
            }
        }

        private void ReturnButton_Click(object sender, RoutedEventArgs e)
        {
            StartWindow hWnd = new StartWindow();
            NavigationService.Navigate(hWnd);
        }

        private void TextBoxPreview(object sender, TextCompositionEventArgs e)
        {
            e.Handled = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-.,АБВГДЕЁЖЗИКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзиклмнопрстуфхцчшщъыьэюя".IndexOf(e.Text) < 0;
        }

        private void TextBoxWorkTimePreview(object sender, TextCompositionEventArgs e)
        {
            e.Handled = "0123456789-:".IndexOf(e.Text) < 0;
        }

        private void TextBoxCostPreview(object sender, TextCompositionEventArgs e)
        {
            e.Handled = "0123456789".IndexOf(e.Text) < 0;
        }

        private void PSNameFieldGotFocus(object sender, RoutedEventArgs e)
        {
            if (wPSnameBox.Text == "Название ПС")
                wPSnameBox.Text = "";
        }

        private void PSAddrFieldGotFocus(object sender, RoutedEventArgs e)
        {
            if (wPSaddressBox.Text == "Адрес ПC")
                wPSaddressBox.Text = "";
        }

        private void PSWorkTimeFieldGotFocus(object sender, RoutedEventArgs e)
        {
            if (wPSworkTimeBox.Text == "Время работы ПС")
                wPSworkTimeBox.Text = "";
        }

        private void PSCostFieldGotFocus(object sender, RoutedEventArgs e)
        {
            if (wCostBox.Text == "Стоимость проезда туда-обратно")
            {
                wCostBox.Text = "";
                wCostBox.MaxLength = 5;
            }
        }

        private void PSNameFieldLostFocus(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrWhiteSpace(wPSnameBox.Text))
            {
                wPSnameBox.Text = "Название ПС";
            }
        }

        private void PSAddrFieldLostFocus(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrWhiteSpace(wPSaddressBox.Text))
            {
                wPSaddressBox.Text = "Адрес ПC";
            }
        }

        private void PSWorkTimeLostFocus(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrWhiteSpace(wPSworkTimeBox.Text))
            {
                wPSworkTimeBox.Text = "Время работы ПС";
            }
        }

        private void PSCostFieldLostFocus(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrWhiteSpace(wCostBox.Text))
            {
                wCostBox.MaxLength = 50;
                wCostBox.Text = "Стоимость проезда туда-обратно";
            }
        }
    }
}
