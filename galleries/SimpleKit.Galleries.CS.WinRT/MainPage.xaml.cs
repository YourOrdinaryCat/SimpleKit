using SimpleKit.Galleries.CS.WinRT.Views;
using SimpleKit.WindowsRuntime.UI.Navigation;
using Windows.UI.Xaml;
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

            this._navigationHelper = new NavigationHelper(this);
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            _ = this.Frame.Navigate(typeof(NavigationHelperPage));
        }
    }
}
