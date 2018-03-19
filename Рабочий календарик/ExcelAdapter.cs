using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Configuration;
using System.IO;
using Microsoft.Office.Interop.Excel;
using System.Windows.Data;
using System.Globalization;
using System.Diagnostics;

namespace SphereCraft
{
    public class ExcelAdapter
    {
        private Microsoft.Office.Interop.Excel.Application hExcel;
        private Workbook hWorkBook;
        private Worksheet hSheet; 
        public void CreateReport(System.DateTime SelectedDate)
        {
            hExcel = new Microsoft.Office.Interop.Excel.Application();
            hWorkBook = hExcel.Workbooks.Add(Directory.GetFiles(Directory.GetCurrentDirectory(), "ExcelGSMTemplate.xls").First());
            hSheet = (Worksheet)hWorkBook.Worksheets.get_Item(1);
            for (int i = 0; i < WorkDayDataTables.WorkDayInfoTable.Rows.Count; i++)
            {
                System.Data.DataRow row = WorkDayDataTables.WorkDayInfoTable.Rows[i];
                hSheet.Cells[2][i + 7] = i + 1;
                DateTime dt = (DateTime)row["Date"];
                hSheet.Cells[3][i + 7] = dt.GetDateTimeFormats('D')[1].ToString();
                hSheet.Cells[4][i + 7] = row["PSName"].ToString() + ", " + row["PSaddr"].ToString();
                hSheet.Cells[5][i + 7] = " ";
                if (row["IsSubUrban"].Equals(true))
                    hSheet.Cells[6][i + 7] = row["Cost"].ToString();
            }
            hSheet.Cells[4][2] = SphereCraft.Properties.Settings.Default["UserFIO"].ToString();
            hSheet.Cells[4][3] = SelectedDate;
            hSheet.Cells[4][4] = SelectedDate.Year.ToString();
            string ReportName;
            if (string.IsNullOrWhiteSpace(SphereCraft.Properties.Settings.Default["ReportPath"].ToString()))
            {
                ReportName = SphereCraft.Properties.Settings.Default["UserFIO"].ToString() + " " + CultureInfo.CurrentCulture.DateTimeFormat.GetMonthName(SelectedDate.Month) + " отчёт ГСМ.xlsx";
            }
            else if(SphereCraft.Properties.Settings.Default["ReportPath"].ToString().Length == 3)
            {
                ReportName = SphereCraft.Properties.Settings.Default["ReportPath"].ToString() + SphereCraft.Properties.Settings.Default["UserFIO"].ToString() + " " + CultureInfo.CurrentCulture.DateTimeFormat.GetMonthName(SelectedDate.Month) + " отчёт ГСМ.xlsx";
                //Временный костыль для случаев, когда путём выбран корень диска
            }
            else   
            {
                ReportName = SphereCraft.Properties.Settings.Default["ReportPath"].ToString() + "\\" + SphereCraft.Properties.Settings.Default["UserFIO"].ToString() + " " + CultureInfo.CurrentCulture.DateTimeFormat.GetMonthName(SelectedDate.Month) + " отчёт ГСМ.xlsx";
            }
            hWorkBook.SaveAs(ReportName, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Type.Missing, XlSaveAsAccessMode.xlNoChange, XlSaveConflictResolution.xlLocalSessionChanges);
            hWorkBook.Close();
            hExcel.Quit();
            Process ExcelProc = Process.GetProcessesByName("EXCEL")[0];
            ExcelProc.Kill();
            hWorkBook = null;
            hExcel = null;
            hSheet = null;
        }

        public void CloseExcel()
        {
            if (hExcel != null)
                hExcel.Quit();
        }
    }
}
