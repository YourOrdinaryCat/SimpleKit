using SimpleKit.Galleries.CS.WinRT.Views;
using SimpleKit.WindowsRuntime.UI.Navigation;
using System;
using System.Collections.Generic;
using Windows.ApplicationModel.Core;
using Windows.UI.Core;
using Windows.UI.ViewManagement;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace SimpleKit.Galleries.CS.WinRT
{
    /// <summary>
    /// The main gallery page, contains links to other
    /// pages that show off the features of SimpleKit.
    /// </summary>
    public sealed partial class MainPage : NavPage
    {
        private readonly Dictionary<int, WeakReference<Frame>> frames =
            new Dictionary<int, WeakReference<Frame>>();

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
            var window = CoreApplication.CreateNewView();
            ApplicationView newView = null;

            await window.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
            {
                Frame frame = new Frame();
                _ = frame.Navigate(typeof(MainPage));

                var curr = Window.Current;
                curr.Content = frame;
                curr.Activate();

                newView = ApplicationView.GetForCurrentView();
                newView.Consolidated += NewView_Consolidated;

                frames[newView.Id] = new WeakReference<Frame>(frame);
            });

            await ApplicationViewSwitcher.TryShowAsStandaloneAsync(newView.Id);
        }

        private void NewView_Consolidated(ApplicationView sender, ApplicationViewConsolidatedEventArgs args)
        {
            var id = sender.Id;

            var result = frames[id].TryGetTarget(out var frame);
            if (result)
                frame.Content = null;

            frames.Remove(id);
            sender.Consolidated -= NewView_Consolidated;
        }
    }
}
