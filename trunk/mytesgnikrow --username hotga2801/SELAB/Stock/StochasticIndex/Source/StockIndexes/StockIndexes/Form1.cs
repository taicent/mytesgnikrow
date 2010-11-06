using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace StockIndexes
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //string[] arrCode = System.IO.Directory.GetFiles("StochasticData");
            string[] arrCode = System.IO.Directory.GetFiles("ModifiedData");
            for (int i = 0; i < arrCode.Length; i++)
            {
                string temp = arrCode[i];
                temp = temp.Remove(0, temp.LastIndexOf('\\')+1);
                temp = temp.Replace(".csv", "");
                cbStockCode.Items.Add(temp);
            }
            cbStockCode.SelectedIndex = 0;

            
        }

        private void HandleCode(string code, out int okCases, out int neutralCases, out int notOkCases, string type)//type: cho biết là đang ở ngữ cảnh nào: report hay handle. Nếu là report thì ta không đụng chạm gì đến 2 vùng: Advice & Total
        {
            tbAdvice.Text = "";
            List<string> date = new List<string>();
            List<double> highPrice = new List<double>();
            List<double> lowPrice = new List<double>();
            List<double> closePrice = new List<double>();
            List<double> percentK = new List<double>();
            //TextReader reader = new StreamReader("StochasticData" + "\\" + code + ".csv");
            TextReader reader = new StreamReader("ModifiedData\\" + code + ".csv");
            //TextReader reader = new StreamReader("StochasticData\\test.csv");
            while (true)
            {
                string line = reader.ReadLine();
                if (line == null)
                    break;
                //line.Replace(" ", "");
                string[] arrStr = line.Split(',');
                date.Add(arrStr[0]);
                highPrice.Add(double.Parse(arrStr[1]));
                lowPrice.Add(double.Parse(arrStr[2]));
                closePrice.Add(double.Parse(arrStr[3]));
            }
            reader.Close();
            

            //1. Tính Fast %K
            int period = int.Parse(tbPeriod.Text);
            for (int i = 0; i < highPrice.Count - period + 1; i++)
            {
                //1.a. Tìm highest high & lowest high
                double highestHigh = highPrice[i];
                double lowestLow = lowPrice[i];
                for (int j = i+1; j < i + period; j++)
                {
                    if (highPrice[j] > highestHigh)
                    {
                        highestHigh = highPrice[j];
                    }
                    if (lowPrice[j] < lowestLow)
                    {
                        lowestLow = lowPrice[j];
                    }
                }
                //1.b. Tính Fast %K
                percentK.Add((closePrice[i] - lowestLow)*100 / (highestHigh - lowestLow));
            }


            


            //2. Tính Slow %K
            List<double> slowPercentK = new List<double>();
            for (int i = 0; i < percentK.Count - 2; i++)
            {
                slowPercentK.Add((percentK[i] + percentK[i + 1] + percentK[i + 2]) / 3);
            }

            /////////////////////////////////////////////////////////////
            //TextWriter writer = new StreamWriter("testResult.txt");
            //for (int i = 0; i < slowPercentK.Count; i++)
            //{
            //    writer.Write(date[i]);
            //    writer.Write(string.Format("{0, 10:F2}", percentK[i]));
            //    writer.Write(string.Format("{0, 10:F2}", slowPercentK[i]));
            //    writer.WriteLine(string.Format("{0, 10:F2}", closePrice[i]));

            //}
            //writer.Close();
            /////////////////////////////////////////////////////////

            //Dựa vào Slow % K để đưa ra lời khuyên
            //+ %K đi xuống qua mức 80 thì bán ra.
            //+ %K đi lên qua mức 20 thì mua vào.

            
            int confirmDays = int.Parse(tbConfirmDay.Text);//Số ngày để xác nhận lại lời khuyên là đúng hay sai

            okCases = 0;
            neutralCases = 0;
            notOkCases = 0;

            for (int i = 1; i < slowPercentK.Count - 1; i++)
            {
                string advice = "";
                
                    if (slowPercentK[i] < 80 && slowPercentK[i + 1] > 80)
                    {
                        //if (i == 0)
                        //{
                        //    DateTime prevDate = DateTime.Parse(date[0]);
                        //    DateTime nextDate = prevDate.AddDays(1);
                        //    advice = nextDate.Month.ToString() + "//" + nextDate.Day.ToString() + "//" + nextDate.Year.ToString();
                        //    advice = advice + ": sell\r\n";
                        //}
                        //else
                        //{

                        if (i - 1 - confirmDays >= 0)
                        {
                            advice = date[i - 1];
                            double profit = closePrice[i - 1] - closePrice[i - 1 - confirmDays];
                            advice = advice + ": sell";
                            if (profit > 0)
                            {
                                advice = advice + "   Ok, decrease " + string.Format("{0:F2}", profit) + "\r\n";
                                okCases++;
                            }
                            else if (profit == 0)
                            {
                                advice = advice + "   Neutral\r\n";
                                neutralCases++;
                            }
                            else
                            {
                                advice = advice + "   Not ok, increase " + string.Format("{0:F2}", -profit) + "\r\n";
                                notOkCases++;
                            }
                        }
                        //}

                        //advice = advice + ": sell\r\n";
                    }
                    if (cbIsBearish.CheckState == CheckState.Unchecked)
                    {
                        if (slowPercentK[i] > 20 && slowPercentK[i + 1] < 20)
                        {
                            //if (i == 0)
                            //{
                            //    DateTime prevDate = DateTime.Parse(date[0]);
                            //    DateTime nextDate = prevDate.AddDays(1);
                            //    advice = nextDate.Month.ToString() + "//" + nextDate.Day.ToString() + "//" + nextDate.Year.ToString();
                            //    advice = advice + ": buy\r\n";
                            //}
                            //else
                            //{
                            if (i - 1 - confirmDays > 0)
                            {
                                advice = date[i - 1];
                                double profit = closePrice[i - 1 - confirmDays] - closePrice[i - 1];
                                advice = advice + ": buy";
                                if (profit > 0)
                                {
                                    advice = advice + "   Ok, increase " + string.Format("{0:F2}", profit) + "\r\n";
                                    okCases++;
                                }
                                else if (profit == 0)
                                {
                                    advice = advice + "   Neutral\r\n";
                                    neutralCases++;
                                }
                                else
                                {
                                    advice = advice + "   Not ok, decrease " + string.Format("{0:F2}", -profit) + "\r\n";
                                    notOkCases++;
                                }
                            }
                            //}

                        }
                    }
                if (type == "handle")
                {
                    if (advice != "")
                    {
                        tbAdvice.Text = tbAdvice.Text + advice;
                    }
                    tbOk.Text = okCases.ToString();
                    tbNeutral.Text = neutralCases.ToString();
                    tbNotOk.Text = notOkCases.ToString();
                }
            }
        }

        private void cbStockCode_SelectedIndexChanged(object sender, EventArgs e)
        {
            string selectedCode = cbStockCode.SelectedItem.ToString();
            int okCases, neutralCases, notOkCases;
           
            HandleCode(selectedCode, out okCases, out neutralCases, out notOkCases, "handle");
        }

        private void tbHandle_Click(object sender, EventArgs e)
        {
            string selectedCode = cbStockCode.SelectedItem.ToString();
            int okCases, neutralCases, notOkCases;

            HandleCode(selectedCode, out okCases, out neutralCases, out notOkCases, "handle");
        }

        private void tbReport_Click(object sender, EventArgs e)
        {
            TextWriter writer = new StreamWriter("report.txt");
            string[] arrCode = System.IO.Directory.GetFiles("ModifiedData");
            int okCases, neutralCases, notOkCases;
            int sum;
            double avgPercentOk, avgPercentNeutral, avgPercentNotOk;
            avgPercentOk = avgPercentNeutral = 0;
            double percentOk, percentNeutral, percentNotOk;
            int exceptionCases = 0;//Ứng với trường hợp okCases = neutralCases = notOkCases = 0
            for (int i = 0; i < arrCode.Length; i++)
            {
                string temp = arrCode[i];
                temp = temp.Remove(0, temp.LastIndexOf('\\') + 1);
                temp = temp.Replace(".csv", "");

                HandleCode(temp, out okCases, out neutralCases, out notOkCases, "report");

                if (okCases == 0 && neutralCases == 0 && notOkCases == 0)
                {
                    exceptionCases++;
                }
                else
                {
                    sum = okCases + neutralCases + notOkCases;
                    percentOk = (double)okCases * 100 / sum;
                    percentNeutral = (double)neutralCases * 100 / sum;
                    avgPercentOk += percentOk;
                    avgPercentNeutral += percentNeutral;
                    percentNotOk = 100 - percentOk - percentNeutral;
                    writer.Write(temp + string.Format("{0, 10:F2}{1, 10:F2}{2, 10:F2}", percentOk, percentNeutral, percentNotOk) + "\r\n");
                }
            }
            avgPercentOk = avgPercentOk / (arrCode.Length-exceptionCases);
            avgPercentNeutral = avgPercentNeutral / (arrCode.Length-exceptionCases);
            avgPercentNotOk = 100 - avgPercentOk - avgPercentNeutral;
            MessageBox.Show("Ok: " + string.Format("{0:F2}",avgPercentOk) + "%\r\n" + "Neutral: " + string.Format("{0:F2}",avgPercentNeutral) + "%\r\n" + "Not Ok: " + string.Format("{0:F2}",avgPercentNotOk) + "%\r\n");
            writer.Close();
        }
    }
}