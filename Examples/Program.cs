﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Examples
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Bloomberg API Emulator Examples");
            Console.WriteLine("http://bemu.codeplex.com/");
            Console.WriteLine("By: Robinson664");
            Console.WriteLine();
            Console.WriteLine("To send a reference data request, push 1");
            Console.WriteLine("To send a market data request, push 2");
            Console.WriteLine("To send a historical data request, push 3");

            string input = Console.ReadLine();
            Console.WriteLine();
            Console.WriteLine();
            bool informationReturned = true;

            switch (input)
            {
                case "1":
                    ReferenceDataRequest.RunExample();
                    break;
                case "2":
                    MarketDataRequest.RunExample();
                    break;
                case "3":
                    HistoricalDataRequest.RunExample();
                    break;
                default:
                    informationReturned = false;
                    break;
            }

            if (informationReturned)
            {
                Console.WriteLine();
                Console.WriteLine("Please note that the data in this request is completely random: it is not real data.");
                Console.WriteLine("Do not make any trading or investment decisions using the data shown above.");
            }
            Console.WriteLine();
            Console.WriteLine("Push enter to quit the application.");
            Console.ReadLine();
        }

    }
}