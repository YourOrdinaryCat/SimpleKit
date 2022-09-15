using SimpleKit.Galleries.CS.WinRT.Views;
using SimpleKit.WindowsRuntime.UI.Navigation;
using SimpleKit.WindowsRuntime.UI.ViewManagement;
using System;
using Windows.UI.Xaml;

namespace SimpleKit.Galleries.CS.WinRT
{
    /// <summary>
    /// The main gallery page, contains links to other
    /// pages that show off the features of SimpleKit.
    /// </summary>
    public sealed partial class MainPage : NavPage
    {
        public MainPage()
        {
            this.InitializeComponent();

            this.TitleBar.SetTitleBarForCurrentView();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            _ = this.Frame.Navigate(typeof(NavigationHelperPage));
        }

        private async void OpenWindowButton_Click(object sender, RoutedEventArgs e)
        {
            _ = await ViewHelpers.ShowNewViewAsync(typeof(MainPage));
        }
    }
}
