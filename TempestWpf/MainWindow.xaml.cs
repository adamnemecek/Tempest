﻿using System;
using System.Collections.Generic;
using System.Linq;
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
        private Tempest tempest;
        private DispatcherTimer timer = new DispatcherTimer();

        public MainWindow()
        {
            // normal initialization
            InitializeComponent();

            // event handlers
            this.Loaded += MainWindow_Loaded;
        }

        void timer_Tick(object sender, EventArgs e)
        {
            throw new NotImplementedException();
        }

        void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            tempest = new Tempest();
            tempest.Start();

            // initialize our timer
            timer.Interval = TimeSpan.FromMilliseconds(50);
            timer.Tick += timer_Tick;
            timer.IsEnabled = true;
        }
    }
}
