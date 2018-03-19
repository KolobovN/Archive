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
using System.Windows.Shapes;
using System.Globalization;
using System.Data;
using System.Configuration;


namespace SphereCraft
{
    /// <summary>
    /// Логика взаимодействия для StartWindow.xaml
    /// </summary>
    public partial class StartWindow : Page
    {
        
        public StartWindow()
        {
            InitializeComponent();
            DateTime FirstDayOfMonth = new DateTime(DateTime.Today.Year, DateTime.Today.Month, 1);
            DateTime LastDayOfMonth = new DateTime(DateTime.Today.Year, DateTime.Today.Month, DateTime.DaysInMonth(DateTime.Today.Year, DateTime.Today.Month));
            DataBaseConnection.DBadapter.SelectCommand.Parameters["@FirstDay"].Value = FirstDayOfMonth;
            DataBaseConnection.DBadapter.SelectCommand.Parameters["@LastDay"].Value = LastDayOfMonth;
            WorkDayDataTables.WorkDayInfoTable.Clear();
            try
            {
                DataBaseConnection.DBconnection.Open();
                DataBaseConnection.DBadapter.Fill(WorkDayDataTables.WorkDayInfoTable);
                DataBaseConnection.DBconnection.Close();
            }
            catch (Exception error)
            {
                MessageBox.Show(error.Message, "Ошибка открытия БД", MessageBoxButton.OK);
            }
            finally
            {
                if (DataBaseConnection.DBconnection != null)
                    DataBaseConnection.DBconnection.Close();
            }
            iDate.Text = DateTime.Today.GetDateTimeFormats('D')[1];
         /*   foreach (DataRow row in WorkDayDataTables.WorkDayInfoTable.Rows)
            {
                wCalendar.SelectedDates.Add(row.Field<DateTime>("Date"));
            }*/
        }

        private void NewWorkDay_Click(object sender, RoutedEventArgs e)
        {
            DateTime SelectedDate;
            if (wCalendar.SelectedDate.HasValue) SelectedDate = wCalendar.SelectedDate.Value;
            else SelectedDate = DateTime.Today;
            NewWorkDayForm NewPage = new NewWorkDayForm(SelectedDate);
            this.NavigationService.Navigate(NewPage);
        }

        private void EditWorkDay_Click(object sender, RoutedEventArgs e)
        {
            DateTime SelectedDate;
            if (wCalendar.SelectedDate.HasValue) SelectedDate = wCalendar.SelectedDate.Value;
            else SelectedDate = DateTime.Today;
            DayInfo SelectedDayInfo = new DayInfo();
            SelectedDayInfo.FillDayInfo(SelectedDate, iPS.Text, iAddr.Text, iWorkTime.Text, Convert.ToInt32(iCost.Text), Convert.ToBoolean(IsGSM.IsChecked));
            NewWorkDayForm NewPage = new NewWorkDayForm(SelectedDayInfo);
            this.NavigationService.Navigate(NewPage);
        }

        private void DelWorkDay_Click(object sender, RoutedEventArgs e)
        {
            foreach (DataRow row in WorkDayDataTables.WorkDayInfoTable.Rows)
            {
                if (wCalendar.SelectedDate == row.Field<DateTime>("Date"))
                {
                    try
                    {
                        DataBaseConnection.DeleteWorkDay(row.Field<DateTime>("Date"));
                    }
                    catch (Exception error)
                    {
                        MessageBox.Show(error.Message, "Ошибка при удалении смены в БД", MessageBoxButton.OK);
                    }
                    finally
                    {
                        DataBaseConnection.CloseConnection();
                    }
                    break;
                }
            }
            StartWindow refresh = new StartWindow();
            this.NavigationService.Navigate(refresh);

        }

        private void CreateXLS_Click(object sender, RoutedEventArgs e)
        {
            ExcelAdapter ReportCreator = new ExcelAdapter();
            DateTime dt;
            if (wCalendar.SelectedDate.HasValue)
                dt = wCalendar.SelectedDate.Value;
            else
                dt = DateTime.Today;
            try
            {
                ReportCreator.CreateReport(dt);
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "GSM", MessageBoxButton.OK);
            }
            finally
            {
                ReportCreator.CloseExcel();
            }
        }

        private void Calendar_SelectedDatesChanged(object sender, SelectionChangedEventArgs e)
        {
            if (wCalendar.SelectedDate.HasValue)
            {
                iDate.Text = wCalendar.SelectedDate.Value.GetDateTimeFormats('D')[1];
            }
            else iDate.Text = DateTime.Today.GetDateTimeFormats('D')[1];
            bool IsValueFound = false;
            foreach (DataRow row in WorkDayDataTables.WorkDayInfoTable.Rows)
            {
                if (iDate.Text == row.Field<DateTime>("Date").GetDateTimeFormats('D')[1])
                {
                    iPS.Text = row.Field<string>("PSName");
                    iAddr.Text = row.Field<string>("PSaddr");
                    iWorkTime.Text = row.Field<string>("PSworktime");
                    iCost.Text = row.Field<int>("Cost").ToString();
                    IsGSM.IsChecked = row.Field<bool>("IsSubUrban");
                    IsValueFound = true;
                    DelWorkDay.Visibility = System.Windows.Visibility.Visible;
                    EditWorkDay.Visibility = System.Windows.Visibility.Visible;
                    NewWorkDay.Visibility = System.Windows.Visibility.Collapsed;
                    break;
                } 
            }
            if (!IsValueFound)
            {
                iPS.Text = "";
                iAddr.Text = "";
                iWorkTime.Text = "";
                iCost.Text = "";
                IsGSM.IsChecked = false;
                DelWorkDay.Visibility = System.Windows.Visibility.Collapsed;
                EditWorkDay.Visibility = System.Windows.Visibility.Collapsed;
                NewWorkDay.Visibility = System.Windows.Visibility.Visible;
            }

        }

        private void wCalendar_DisplayDateChanged(object sender, CalendarDateChangedEventArgs e)
        {

            DateTime FirstDay = new DateTime(this.wCalendar.DisplayDate.Year, this.wCalendar.DisplayDate.Month, 1);
            DateTime LastDay = new DateTime(this.wCalendar.DisplayDate.Year, this.wCalendar.DisplayDate.Month, DateTime.DaysInMonth(this.wCalendar.DisplayDate.Year, this.wCalendar.DisplayDate.Month));
            DataBaseConnection.DBadapter.SelectCommand.Parameters["@FirstDay"].Value = FirstDay;
            DataBaseConnection.DBadapter.SelectCommand.Parameters["@LastDay"].Value = LastDay;
            WorkDayDataTables.WorkDayInfoTable.Clear();
            try
            {
                DataBaseConnection.DBconnection.Open();
                DataBaseConnection.DBadapter.Fill(WorkDayDataTables.WorkDayInfoTable);
                DataBaseConnection.DBconnection.Close();
            }
            catch (Exception error)
            {
                MessageBox.Show(error.Message, "Ошибка открытия БД", MessageBoxButton.OK);
            }
            finally
            {
                if (DataBaseConnection.DBconnection != null)
                    DataBaseConnection.DBconnection.Close();
            }
            
        }

        private void Settings_Click(object sender, RoutedEventArgs e)
        {
            pSettings pg = new pSettings();
            NavigationService.Navigate(pg);
        }

        
    }
}
