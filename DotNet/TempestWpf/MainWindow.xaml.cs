﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

using TempestDotNET;

namespace TempestWpf
{
   /// <summary>
   /// Interaction logic for MainWindow.xaml
   /// </summary>
   public partial class MainWindow : Window
   {
      #region Private Fields

      private TDNWin32TempestSoundIO tempestSoundIO;
      private Tempest tempest;

      private DispatcherTimer timer;
      private DispatcherTimer vectorTimer;
      private DispatcherTimer spinnerTimer;
      private List<Line> lines = new List<Line>();
      private DateTime startTime;
      private Debug6502Window debug6502;
      private SolidColorBrush[] vectorBrush = new SolidColorBrush[16];
      private bool leftKeyDown = false;
      private bool rightKeyDown = false;

      private SolidColorBrush ledOnBrush = new SolidColorBrush(Colors.Red);
      private SolidColorBrush ledOffBrush = new SolidColorBrush(Colors.Black);

      #endregion

      /// <summary>
      /// Initializes a new instance of class MainWindow
      /// </summary>
      public MainWindow()
      {
         // normal initialization
         InitializeComponent();

         // set up our vector brushes
         vectorBrush[0] = new SolidColorBrush(Colors.White);
         vectorBrush[1] = new SolidColorBrush(Colors.Yellow);
         vectorBrush[2] = new SolidColorBrush(Colors.Purple);
         vectorBrush[3] = new SolidColorBrush(Colors.Red);
         vectorBrush[4] = new SolidColorBrush(Colors.Orange);
         vectorBrush[5] = new SolidColorBrush(Colors.Lime);
         vectorBrush[6] = new SolidColorBrush(Colors.Blue);
         vectorBrush[7] = new SolidColorBrush(Colors.Pink);
         vectorBrush[8] = new SolidColorBrush(Colors.Gray);
         vectorBrush[9] = new SolidColorBrush(Colors.Maroon);
         vectorBrush[10] = new SolidColorBrush(Colors.Gold);
         vectorBrush[11] = new SolidColorBrush(Colors.Silver);
         vectorBrush[12] = new SolidColorBrush(Colors.Fuchsia);
         vectorBrush[13] = new SolidColorBrush(Colors.LightBlue);
         vectorBrush[14] = new SolidColorBrush(Colors.LightGreen);
         vectorBrush[15] = new SolidColorBrush(Colors.LightGray);

         // event handlers
         this.Loaded += MainWindow_Loaded;
         this.Closed += MainWindow_Closed;
         this.Closing += MainWindow_Closing;
         this.KeyDown += MainWindow_KeyDown;
         this.KeyUp += MainWindow_KeyUp;
         buttonOnePlayerStart.MouseLeftButtonDown += buttonOnePlayerStart_MouseLeftButtonDown;
         buttonTwoPlayerStart.MouseLeftButtonDown += buttonTwoPlayerStart_MouseLeftButtonDown;
         view6502DebugWindowItem.Click += view6502DebugWindowItem_Click;
      }

      void MainWindow_KeyDown(object sender, KeyEventArgs e)
      {
         switch (e.Key)
         {
            case Key.Left:
               tempestSoundIO.MoveWheel(-4);
               leftKeyDown = true;
               rightKeyDown = false;
               break;

            case Key.Right:
               tempestSoundIO.MoveWheel(4);
               leftKeyDown = false;
               rightKeyDown = true;
               break;

            case Key.F:
               tempestSoundIO.Fire(true);
               break;

            case Key.V:
               tempestSoundIO.Zap(true);
               break;
         }
      }

      void MainWindow_KeyUp(object sender, KeyEventArgs e)
      {
         switch (e.Key)
         {
            case Key.Left:
               leftKeyDown = false;
               break;

            case Key.Right:
               rightKeyDown = false;
               break;

            case Key.F:
               tempestSoundIO.Fire(false);
               break;

            case Key.V:
               tempestSoundIO.Zap(false);
               break;
         }
      }

      void view6502DebugWindowItem_Click(object sender, RoutedEventArgs e)
      {
         // create the window if we haven't yet
         if (debug6502 == null)
         {
            debug6502 = new Debug6502Window();
            debug6502.Tempest = tempest;
         }

         // show it
         debug6502.Show();
         debug6502.Activate();
      }

      void buttonOnePlayerStart_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
      {
         tempestSoundIO.OnePlayer(true);
         System.Threading.Thread.Sleep(100);
         tempestSoundIO.OnePlayer(false);
      }

      void buttonTwoPlayerStart_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
      {
         tempestSoundIO.TwoPlayer(true);
         System.Threading.Thread.Sleep(100);
         tempestSoundIO.TwoPlayer(false);
      }

      void MainWindow_Closing(object sender, System.ComponentModel.CancelEventArgs e)
      {
         timer.IsEnabled = false;
         vectorTimer.IsEnabled = false;
         e.Cancel = false;
      }

      void MainWindow_Closed(object sender, EventArgs e)
      {
         tempest.Dispose();
      }

      void MainWindow_Loaded(object sender, RoutedEventArgs e)
      {
         // create the IO object that we represent
         tempestSoundIO = new TDNWin32TempestSoundIO();

         // create our tempest, connected to the IO object
         tempest = new Tempest(tempestSoundIO);

         // see if we're in Demo mode
         if (Environment.CommandLine.ToLower().IndexOf(" demo ") >= 0)
            tempest.SetDemoMode();

         // set it to running
         startTime = DateTime.Now;
         tempest.Start();

         timer = new DispatcherTimer();
         timer.Interval = TimeSpan.FromMilliseconds(200);
         timer.IsEnabled = true;
         timer.Tick += timer_Tick;

         vectorTimer = new DispatcherTimer();
         vectorTimer.Interval = TimeSpan.FromMilliseconds(50);
         vectorTimer.IsEnabled = true;
         vectorTimer.Tick += vectorTimer_Tick;

         spinnerTimer = new DispatcherTimer();
         spinnerTimer.Interval = TimeSpan.FromMilliseconds(10);
         spinnerTimer.IsEnabled = true;
         spinnerTimer.Tick += spinnerTimer_Tick;
      }

      void spinnerTimer_Tick(object sender, EventArgs e)
      {
         // update the spinner if we should
         if (leftKeyDown)
            tempestSoundIO.MoveWheel(-1);
         else if (rightKeyDown)
            tempestSoundIO.MoveWheel(1);

         // update our LED's
         if (tempestSoundIO.OnePlayerLED())
            buttonOnePlayerStart.Fill = ledOnBrush;
         else
            buttonOnePlayerStart.Fill = ledOffBrush;
         if (tempestSoundIO.TwoPlayerLED())
            buttonTwoPlayerStart.Fill = ledOnBrush;
         else
            buttonTwoPlayerStart.Fill = ledOffBrush;
      }

      void timer_Tick(object sender, EventArgs e)
      {
         processorStatus.Text = tempest.GetProcessorStatus();

         double processorSpeed = tempest.GetTotalClockCycles();
         processorSpeed /= (DateTime.Now - startTime).TotalSeconds;
         processorSpeed /= 1000000;
         processorSpeedText.Text = processorSpeed.ToString("F1") + " MHz";
      }

      void vectorTimer_Tick(object sender, EventArgs e)
      {
         // get a vector enumerator
         VectorEnumerator enumerator = tempest.GetVectorEnumerator();
         if (enumerator != null)
         {
            int index = 0;
            for (; ; )
            {
               // get the vector
               Int16 startX16, startY16, endX16, endY16;
               byte r, g, b;
               if (!enumerator.GetNextVector(out startX16, out startY16, out endX16, out endY16, out r, out g, out b))
                  break;

               // switch to longer than 16 bits
               int startX = startX16;
               int startY = startY16;
               int endX = endX16;
               int endY = endY16;

               Line line;
               if (index >= lines.Count)
               {
                  line = new Line();
                  lines.Add(line);
                  canvas.Children.Add(line);
               }
               else
               {
                  line = lines[index];
               }

               int strokeThickness = 100;
               
               // if we have a zero length line we need to fudge it... this is how
               // Tempest draws points
               if (endX == startX && endY==startY)
               {
                  endX += strokeThickness / 2;
                  startX += strokeThickness / 2;
                  endY -= strokeThickness / 2;
                  startY += strokeThickness / 2;
               }

               line.Stroke = new SolidColorBrush(Color.FromRgb(r, g, b));
               line.X1 = canvas.ActualWidth / 2 + startX;
               line.X2 = canvas.ActualWidth / 2 + endX;
               line.Y1 = canvas.ActualHeight / 2 - startY;
               line.Y2 = canvas.ActualHeight / 2 - endY;
               line.StrokeThickness = 100;
               line.Visibility = System.Windows.Visibility.Visible;

               ++index;
            }

            while (index < lines.Count)
               lines[index++].Visibility = System.Windows.Visibility.Hidden;
         }
      }

      private byte[] GetROM(string name)
      {
         Assembly assembly = Assembly.GetExecutingAssembly();
         string[] names = assembly.GetManifestResourceNames();
         for (int i = 0; i < names.Length; ++i)
         {
            if (names[i].IndexOf(name) >= 0)
            {
               Stream stream = assembly.GetManifestResourceStream(names[i]);
               List<byte> rom = new List<byte>();
               for (; ; )
               {
                  int b = stream.ReadByte();
                  if (b < 0)
                     break;
                  rom.Add((byte)b);
               }
               return rom.ToArray();
            }
         }

         throw new Exception("ROM resource not found: " + name);
      }
   }
}
