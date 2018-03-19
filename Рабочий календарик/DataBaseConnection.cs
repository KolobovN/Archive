using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.SqlClient;
using System.Configuration;
using System.Data;

namespace SphereCraft
{
    class DataBaseConnection    //Предоставляет сконфигурированный адаптер для доступа к БД
    {
        public static SqlDataAdapter DBadapter;
        public static string connectionString;    
        public static SqlConnection DBconnection;
        public static SqlCommand cmd_delete;
        public static SqlCommand cmd_update;
        public static void Initialize() //Настраивает адаптер на работу с БД: задаёт строку подключения и готовит SQL команды.
        {
            DBadapter = new SqlDataAdapter();            
            connectionString = ConfigurationManager.ConnectionStrings["SphereCraft.Properties.Settings.SphereCraftConnectionString"].ConnectionString;
            DBconnection = new SqlConnection(connectionString);
            DBadapter.InsertCommand = new SqlCommand("dbo.InsertWorkDayData", DBconnection);
            DBadapter.InsertCommand.CommandType = CommandType.StoredProcedure;
            DBadapter.InsertCommand.Parameters.Add(new SqlParameter("@Date", SqlDbType.Date, 0, "Date"));
            DBadapter.InsertCommand.Parameters.Add(new SqlParameter("@PSname", SqlDbType.NVarChar, 50, "PSname"));
            DBadapter.InsertCommand.Parameters.Add(new SqlParameter("@PSaddr", SqlDbType.NVarChar, 50, "PSaddr"));
            DBadapter.InsertCommand.Parameters.Add(new SqlParameter("@PSworktime", SqlDbType.NVarChar, 50, "PSworktime"));
            DBadapter.InsertCommand.Parameters.Add(new SqlParameter("@Сost", SqlDbType.Int, 0, "Cost"));
            DBadapter.InsertCommand.Parameters.Add(new SqlParameter("@IsSubUrban", SqlDbType.Bit, 0, "IsSubUrban"));
            //
            DBadapter.SelectCommand = new SqlCommand("dbo.SelectMonthWorkdays", DBconnection);
            DBadapter.SelectCommand.CommandType = CommandType.StoredProcedure;
            DBadapter.SelectCommand.Parameters.Add(new SqlParameter("@FirstDay", SqlDbType.Date));
            DBadapter.SelectCommand.Parameters.Add(new SqlParameter("@LastDay", SqlDbType.Date));
            //
            cmd_update = new SqlCommand("dbo.UpdateWorkDayData", DBconnection);
            cmd_update.CommandType = CommandType.StoredProcedure;
            cmd_update.Parameters.Add(new SqlParameter("@Date", SqlDbType.Date, 0, "Date"));
            cmd_update.Parameters.Add(new SqlParameter("@PSname", SqlDbType.NVarChar, 50, "PSname"));
            cmd_update.Parameters.Add(new SqlParameter("@PSaddr", SqlDbType.NVarChar, 50, "PSaddr"));
            cmd_update.Parameters.Add(new SqlParameter("@PSworktime", SqlDbType.NVarChar, 50, "PSworktime"));
            cmd_update.Parameters.Add(new SqlParameter("@Сost", SqlDbType.Int, 0, "Cost"));
            cmd_update.Parameters.Add(new SqlParameter("@IsSubUrban", SqlDbType.Bit, 0, "IsSubUrban"));
            //
            cmd_delete = new SqlCommand("dbo.DeleteWorkDayData", DBconnection);
            cmd_delete.CommandType = CommandType.StoredProcedure;
            cmd_delete.Parameters.Add(new SqlParameter("@Date", SqlDbType.Date, 0, "Date"));
        }

        public static void CloseConnection()
        {
            if (DBconnection != null)
                DBconnection.Close();
        }

        public static void DeleteWorkDay(DateTime DeletingDate)
        {
            cmd_delete.Parameters["@Date"].Value = DeletingDate;
            DBconnection.Open();
            cmd_delete.ExecuteNonQuery();
            DBconnection.Close();
        }

        public static void InsertWorkDay(DataTable SourceTable)
        {
            DBconnection.Open();
            DBadapter.Update(SourceTable);
            DBconnection.Close();
        }

        public static void UpdateWorkDay(DayInfo values)
        {
            cmd_update.Parameters["@Date"].Value = values.getDate();
            cmd_update.Parameters["@PSname"].Value = values.getPSname();
            cmd_update.Parameters["@PSaddr"].Value = values.getPSaddress();
            cmd_update.Parameters["@PSworktime"].Value = values.getPSworkTime();
            cmd_update.Parameters["@Сost"].Value = values.getCost();
            cmd_update.Parameters["@IsSubUrban"].Value = values.getSubUrbFlag();
            DBconnection.Open();
            cmd_update.ExecuteNonQuery();
            DBconnection.Close();
        }
    }
}
