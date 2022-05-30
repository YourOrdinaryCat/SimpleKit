﻿using SimpleKit.WindowsRuntime.UI.Navigation;
using Windows.UI.Xaml.Controls;

namespace SimpleKit.Galleries.CS.WinRT
{
    /// <summary>
    /// The main gallery page, contains links to other
    /// pages that show off the features of SimpleKit.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private readonly NavigationHelper _navigationHelper;

        public MainPage()
        {
            this.InitializeComponent();

            _navigationHelper = new NavigationHelper(this);
        }
    }
}