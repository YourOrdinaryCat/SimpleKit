using SimpleKit.WindowsRuntime.UI.Navigation;
using Windows.UI.Xaml;

namespace SimpleKit.Galleries.CS.WinRT.Views
{
    /// <summary>
    /// A page showcasing NavigationHelper's capabilities
    /// that explains how the gestures work.
    /// </summary>
    public sealed partial class NavigationHelperPage : NavPage
    {
        public NavigationHelperPage()
        {
            this.InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            _ = this.Frame.Navigate(typeof(NavigationHelperPage));
        }
    }
}
