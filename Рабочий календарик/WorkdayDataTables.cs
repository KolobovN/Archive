using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data;
using System.Windows.Data;
using System.Globalization;

/* IValueConverter - интерфейс, обеспечивающий возможность реализации пользовательской логики в привязке данных
 * Обязательна реализация метода Convert - для подсветки дней с назначенной сменой
 *
 * 
 */

namespace SphereCraft
{
    class WorkDayDataTables : IValueConverter    //Размеченные таблицы для SQL адаптера
    {
        public static DataTable WorkDayInfoTable;
        public static void Initialize() //Настраивает таблицы данных для работы с БД.
        {
            WorkDayInfoTable = new DataTable();
            WorkDayInfoTable.Columns.Add("Date", typeof(DateTime));
            WorkDayInfoTable.Columns.Add("PSName", typeof(string));
            WorkDayInfoTable.Columns.Add("PSaddr", typeof(string));
            WorkDayInfoTable.Columns.Add("PSworktime", typeof(string));
            WorkDayInfoTable.Columns.Add("Cost", typeof(int));
            WorkDayInfoTable.Columns.Add("IsSubUrban", typeof(bool));
        }
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            foreach (DataRow row in WorkDayInfoTable.Rows)
            {
                if ((DateTime)value == row.Field<DateTime>("Date")) return row.Field<string>("PSName");
            }
            return null;
        }
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return null;
        }
    }
}
