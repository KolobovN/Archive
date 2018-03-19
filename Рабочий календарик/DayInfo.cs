using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SphereCraft
{
    public class DayInfo
    {
        DateTime wDate;
        string PSname; //название пункта самовывоза
        string PSaddress;
        string PSworkTime;
        int cost; //стоимость проезда туда-обратно
        bool IsSubUrban; //True - если пункт в области, false - если в Москве
        public DayInfo()
        {        }
        public void FillDayInfo(DateTime newDate, string newPSname, string newPSaddress, string newPSworkTime, int newCost, bool IsSubUrb)
        {
            wDate = newDate;
            PSname = newPSname;
            PSaddress = newPSaddress;
            PSworkTime = newPSworkTime;
            cost = newCost;
            IsSubUrban = IsSubUrb;
        }
        public DateTime getDate(){ return wDate; }
        public string getPSname() { return PSname; }
        public string getPSaddress() { return PSaddress; }
        public string getPSworkTime() { return PSworkTime; }
        public int getCost() { return cost; }
        public bool getSubUrbFlag() { return IsSubUrban; }

    }
}
