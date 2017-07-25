using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ffcSerial1
{
    class Program
    {
        static void Main(string[] args)
        {
            var N = int.Parse(args[0]);
            for (int i = 0; i < N; i++)
            {
                System.Console.Write(i);
            }
        }
    }
}
