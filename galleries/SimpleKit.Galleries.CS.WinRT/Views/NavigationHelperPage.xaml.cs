using SimpleKit.WindowsRuntime.UI.Navigation;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace SimpleKit.Galleries.CS.WinRT.Views
{
    /// <summary>
    /// A page showcasing NavigationHelper's capabilities
    /// that explains how the gestures work.
    /// </summary>
    public sealed partial class NavigationHelperPage : Page
    {
        private readonly NavigationHelper _navigationHelper;

        public NavigationHelperPage()
        {
            this.InitializeComponent();

            _navigationHelper = new NavigationHelper(this);
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
