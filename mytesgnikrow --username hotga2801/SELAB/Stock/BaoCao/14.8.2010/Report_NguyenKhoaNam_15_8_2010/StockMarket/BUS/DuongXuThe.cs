using System;
using System.Collections;
using System.Text;

namespace BUS
{

    public class DuongXuThe
    {
        public DTO.DataDuongXuThe XacDinhXuTheTangGiam(DateTime t1, DateTime t2,ArrayList dlck)
        {
            double Max = 0;
            double Min = 0;
            int index = 0;
            int _iMax = 0;
            int _iMin = 0;
            double value;
            TimeSpan Temp;
            int T2Data = 0;
            double DayDistance = 0;
            double MaxMinDistance = 0;
            double CosValue;
            DTO.DuLieuNgay  dln,_dlnMax,_dlnMin;
            DTO.DataDuongXuThe dataDXT = new DTO.DataDuongXuThe();

            //Search Max and Min
            for (int i = dlck.Count-1; i >= 0; i--)
            { 
                dln = (DTO.DuLieuNgay)dlck[i];
                if(t1 <= dln.Ngay)
                {
                    index = i;
                    Max = dln.GiaDongCua;
                    Min = dln.GiaDongCua;
                    _iMax = i;
                    _iMin = i;
                    break;
                }
            }

            for (int i = index; ; i--)
            {
                dln = (DTO.DuLieuNgay)dlck[i];
                if (dln.Ngay > t2)
                {
                    T2Data = i + 1;
                    break;
                }

                if (Max < dln.GiaDongCua)
                {
                    Max = dln.GiaDongCua;
                    _iMax = i;
                }

                if (Min > dln.GiaDongCua)
                {
                    Min = dln.GiaDongCua;
                    _iMin = i;
                }
            }

            //do value
            value = Max - Min;
            dln = (DTO.DuLieuNgay)dlck[T2Data];
            
            _dlnMax = (DTO.DuLieuNgay)dlck[_iMax];
            _dlnMin = (DTO.DuLieuNgay)dlck[_iMin];

            Temp = _dlnMax.Ngay.Subtract(_dlnMin.Ngay);
            DayDistance = Temp.Days;
            MaxMinDistance = Math.Pow(Math.Pow(DayDistance,2) + Math.Pow(_dlnMax.GiaDongCua - _dlnMin.GiaDongCua,2),0.5f);
            CosValue = DayDistance / MaxMinDistance;
           
            //System.Windows.Forms.MessageBox.Show(CosValue.ToString());
            //System.Windows.Forms.MessageBox.Show(_dlnMax.Ngay.ToString() + "--" + _dlnMax.GiaDongCua.ToString() + "--" + _dlnMin.Ngay.ToString() + "--" + _dlnMin.GiaDongCua.ToString());


            if (Convert.ToDouble(dln.GiaThapNhat) >= (1 / 2 * (Math.Pow(5, 1 / 2) + 1) / 2 * value))
            {
                if (CosValue > 0)
                {
                    if (CosValue <= 0.38f)
                        dataDXT.value = 1;
                    else
                    {
                        if (CosValue <= 0.61f)
                            dataDXT.value = 2;
                        else
                            dataDXT.value = 3;
                    }
                }
                else
                    dataDXT.value = 4;
            }
            else
            {
                if (Convert.ToDouble(dln.GiaThapNhat) >= (1 / 2 * (Math.Pow(5, 1 / 2) - 1) / 2 * value))
                {
                    if (CosValue > 0)
                    {
                        if (CosValue <= 0.38f)
                            dataDXT.value = 1;
                        else
                        {
                            if (CosValue <= 0.61f)
                                dataDXT.value = 2;
                            else
                                dataDXT.value = 3;
                        }
                    }
                    else
                        dataDXT.value = 4;
                }
                else
                {
                    if (CosValue > 0)
                    {
                        if (CosValue <= 0.38f)
                            dataDXT.value = 5;
                        else
                        {
                            if (CosValue <= 0.61f)
                                dataDXT.value = 6;
                            else
                                dataDXT.value = 7;
                        }
                    }
                    else
                        dataDXT.value = 8;
                }
            }
            dataDXT.iMax = _iMax;
            dataDXT.iMin = _iMin;
            return dataDXT;
           
        }
    }
}
