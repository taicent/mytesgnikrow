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
            string[] arrCode = System.IO.Directory.GetFiles("StochasticData");
            for (int i = 0; i < arrCode.Length; i++)
            {
                string temp = arrCode[i];
                temp = temp.Remove(0, temp.LastIndexOf('\\')+1);
                temp = temp.Replace(".csv", "");
                cbStockCode.Items.Add(temp);
            }
            cbStockCode.SelectedIndex = 0;

            
        }

        private void HandleCode(string code)
        {
            tbAdvice.Text = "";
            List<string> date = new List<string>();
            List<double> highPrice = new List<double>();
            List<double> lowPrice = new List<double>();
            List<double> closePrice = new List<double>();
            List<double> percentK = new List<double>();
            TextReader reader = new StreamReader("StochasticData" + "\\" + code + ".csv");
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

            //Dựa vào Slow % K để đưa ra lời khuyên
            //+ %K đi xuống qua mức 80 thì bán ra.
            //+ %K đi lên qua mức 20 thì mua vào.

            int okCases = 0;
            int neutralCases = 0;
            int notOkCases = 0;

            for (int i = 0; i < slowPercentK.Count - 1; i++)
            {
                string advice = "";
                if (slowPercentK[i] < 80 && slowPercentK[i + 1] >= 80)
                {
                    if (i == 0)
                    {
                        DateTime prevDate = DateTime.Parse(date[0]);
                        DateTime nextDate = prevDate.AddDays(1);
                        advice = nextDate.Month.ToString() + "//" + nextDate.Day.ToString() + "//" + nextDate.Year.ToString();
                        advice = advice + ": sell\r\n";
                    }
                    else
                    {
                        advice = date[i - 1];
                        double profit = closePrice[i] - closePrice[i - 1];
                        advice = advice + ": sell";
                        if (profit > 0)
                        {
                            advice = advice + "   Ok, decrease " + profit.ToString()+"\r\n";
                            okCases++;
                        }
                        else if (profit == 0)
                        {
                            advice = advice + "   Neutral\r\n";
                            neutralCases++;
                        }
                        else
                        {
                            advice = advice + "   Not ok, increase " + (-profit).ToString()+"\r\n";
                            notOkCases++;
                        }
                    }

                    //advice = advice + ": sell\r\n";
                }
                else if (slowPercentK[i] > 20 && slowPercentK[i + 1] <= 20)
                {
                    if (i == 0)
                    {
                        DateTime prevDate = DateTime.Parse(date[0]);
                        DateTime nextDate = prevDate.AddDays(1);
                        advice = nextDate.Month.ToString() + "//" + nextDate.Day.ToString() + "//" + nextDate.Year.ToString();
                        advice = advice + ": buy\r\n";
                    }
                    else
                    {
                        advice = date[i - 1];
                        double profit = closePrice[i-1] - closePrice[i];
                        advice = advice + ": buy";
                        if (profit > 0)
                        {
                            advice = advice + "   Ok, increase " + profit.ToString() + "\r\n";
                            okCases++;
                        }
                        else if (profit == 0)
                        {
                            advice = advice + "   Neutral\r\n";
                            neutralCases++;
                        }
                        else
                        {
                            advice = advice + "   Not ok, decrease " + (-profit).ToString() + "\r\n";
                            notOkCases++;
                        }
                    }
                    
                }
                if (advice != "")
                {
                    tbAdvice.Text = tbAdvice.Text + advice;
                }
                tbOk.Text = okCases.ToString();
                tbNeutral.Text = neutralCases.ToString();
                tbNotOk.Text = notOkCases.ToString();
            }
        }

        private void cbStockCode_SelectedIndexChanged(object sender, EventArgs e)
        {
            string selectedCode = cbStockCode.SelectedItem.ToString();
            HandleCode(selectedCode);
        }
    }
}