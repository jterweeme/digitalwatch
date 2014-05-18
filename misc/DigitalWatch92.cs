using System;

namespace DigitalWatch92
{
    interface MyObserver
    {
        void update();
    }

    // State pattern
    abstract class WatchMode : MyObserver
    {
        protected static DateTime tijd { get; set; }
        public WatchMode next { get; set; }
        public abstract void button2Pressed();

        private void incrementSeconds()
        {
            if (tijd != null)
                tijd = tijd.AddSeconds(1);
        }

        public void update()
        {
            incrementSeconds();
            Console.Write("\r{0}", tijd);
        }
    }

    class DisplayTimeMode : WatchMode
    {
        public DisplayTimeMode()
        {
            WatchMode.tijd = DateTime.Now;
        }

        public override void button2Pressed()
        {
            Console.Write("\r{0}", tijd);
        }
    }

    class IncrementHoursMode : WatchMode
    {
        public override void button2Pressed()
        {
            tijd = tijd.AddHours(1);
            Console.Write("\r{0}", tijd);
        }
    }

    class IncrementMinutesMode : WatchMode
    {
        public override void button2Pressed()
        {
            tijd = tijd.AddMinutes(1);
            Console.Write("\r{0}", tijd);
        }
    }

    // Observable
    class Crystal : System.Timers.Timer
    {
        public MyObserver observer { get; set; }

        public Crystal(double interval) : base(interval)
        {
            Elapsed += new System.Timers.ElapsedEventHandler(pulse);
        }

        private void pulse(object source, System.Timers.ElapsedEventArgs e)
        {
            observer.update();
        }
    }

    // Singleton
    class DigitalWatch
    {
        static DigitalWatch instance;
        WatchMode currentMode;
        WatchMode displayTimeMode;
        WatchMode incrementHoursMode;
        WatchMode incrementMinutesMode;
        Crystal crystal1Hz;

        private DigitalWatch()
        {
            displayTimeMode = new DisplayTimeMode();
            incrementHoursMode = new IncrementHoursMode();
            incrementMinutesMode = new IncrementMinutesMode();
            displayTimeMode.next = incrementHoursMode;
            incrementHoursMode.next = incrementMinutesMode;
            incrementMinutesMode.next = displayTimeMode;
            currentMode = displayTimeMode;
            crystal1Hz = new Crystal(1000);
            crystal1Hz.observer = currentMode;
            crystal1Hz.Enabled = true;
        }

        public static DigitalWatch getInstance()
        {
            if (instance == null)
            {
                instance = new DigitalWatch();
            }

            return instance;
        }

        public void run()
        {
            ConsoleKeyInfo keyInfo;

            while (true)
            {
                keyInfo = Console.ReadKey();

                if (keyInfo.Key.Equals(ConsoleKey.D1))
                {
                    currentMode = currentMode.next;
                }
                else if (keyInfo.Key.Equals(ConsoleKey.D2))
                {
                    currentMode.button2Pressed();
                }
            }
        }

        static void Main(string[] args)
        {
            DigitalWatch dw = DigitalWatch.getInstance();
            dw.run();
        }
    }
}
