using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BaseCreater
{
    internal class Record
    {
        public Record()
        { }

        public Record(String text)
        {
            String[] values = text.Split(';');
            PhoneNumber = Int32.Parse(values[0]);
            Surname = values[1];
            Name = values[2];
            SecName = values[3];
            Streat = values[4];
            House = values[5] == "" ?0: Int32.Parse(values[5]);
            Building = values[6] == "" ? 0 : Int32.Parse(values[6]);
            Flat = values[7] == "" ? 0 : Int32.Parse(values[7]);
        }

        public string Surname { get; set; }
        public string Name { get; set; }
        public string SecName { get; set; }
        public int PhoneNumber { get; set; }
        public string Streat { get; set; }
        public int House { get; set; }
        public int Building { get; set; }
        public int Flat { get; set; }

        public override string ToString()
        {
            return String.Format("{0};{1};{2};{3};{4};{5};{6};{7}", PhoneNumber, Surname, Name,
                SecName, Streat, House, Building, Flat);
        }
    }


    class Program
    {
        static void Main(string[] args)
        {
            List<Record> baseRecords = new List<Record>();
            List<Record> result = new List<Record>();
            var random = new Random();

            using (var reader= new StreamReader(new FileStream("telbase.txt",FileMode.Open)))
            {
                while (!reader.EndOfStream)
                {
                    string readLine = reader.ReadLine();
                    if (readLine != String.Empty)
                    {
                        baseRecords.Add(new Record(readLine));
                    }
                }
            }

            for (int phoneNumber = 100000; phoneNumber < 999900; phoneNumber++)
            {
                result.Add(new Record()
                {
                    PhoneNumber = phoneNumber,
                    Surname = baseRecords[random.Next(baseRecords.Count)].Surname,
                    Name = baseRecords[random.Next(baseRecords.Count)].Name,
                    SecName = baseRecords[random.Next(baseRecords.Count)].SecName,
                    Streat = baseRecords[random.Next(baseRecords.Count)].Streat,
                    House =random.Next(1000)+1,
                    Building = random.Next(10) + 1,
                    Flat = random.Next(300) + 1,
                });
            }

            using (var writer = new StreamWriter(new FileStream("telbase2.txt", FileMode.Create), Encoding.Unicode))
            {
                foreach (Record record in result)
                {
                    writer.WriteLine(record.ToString());
                }
            }

        }

    }
}
