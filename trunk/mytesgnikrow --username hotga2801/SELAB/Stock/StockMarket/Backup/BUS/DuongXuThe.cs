using System;
using System.Collections;
using System.Text;

namespace BUS
{

    public class DuongXuThe
    {
        public int XacDinhXuTheTangGiam(DateTime t1, DateTime t2,ArrayList dlck)
        {
            double Max = 0;
            double Min = 0;
            int index = 0;
            int _iMax = 0;
            int _iMin = 0;
            double value;
            DTO.DuLieuNgay  dln,_dlnMax,_dlnMin;

            //Search Max and Min
            for (int i = dlck.Count-1; i >= 0; i--)
            { 
                dln = (DTO.DuLieuNgay)dlck[i];
                if(t1 == dln.Ngay || t1 < dln.Ngay)
                {
                    index = i;
                    Max = dln.GiaDongCua;
                    Min = dln.GiaDongCua;
                    break;
                }
            }

            for (int i = index; ; i--)
            {
                dln = (DTO.DuLieuNgay)dlck[i];
                if (dln.Ngay >= t2)
                    break;

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
            dln = (DTO.DuLieuNgay)dlck[index];
            _dlnMax = (DTO.DuLieuNgay)dlck[_iMax];
            _dlnMin = (DTO.DuLieuNgay)dlck[_iMin];

            System.Windows.Forms.MessageBox.Show(_dlnMax.Ngay.ToString() + "--" + _dlnMax.GiaDongCua.ToString() + "--" + _dlnMin.Ngay.ToString() + "--" + _dlnMin.GiaDongCua.ToString());

            
            if (Convert.ToDouble(dln.GiaThapNhat) >= (1 / 2 * (Math.Pow(5, 1 / 2) + 1) / 2 * value))
            {
                if (_dlnMax.Ngay > _dlnMin.Ngay)
                    return 1;
                else
                    return 2;
            }
            else
            {
                if (Convert.ToDouble(dln.GiaThapNhat) >= (1 / 2 * (Math.Pow(5, 1 / 2) - 1) / 2 * value))
                {
                    if (_dlnMax.Ngay > _dlnMin.Ngay)
                        return 1;
                    else
                        return 2;
                }
                else
                {
                    if (_dlnMax.Ngay > _dlnMin.Ngay)
                        return 3;
                    else
                        return 4;
                }
            }
        }

    }
}
