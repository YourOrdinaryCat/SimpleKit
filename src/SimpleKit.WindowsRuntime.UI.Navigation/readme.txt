Welcome! If you're coming from a previous version, there is a small number of breaking
changes in this release that might give you some trouble. Read the summary below to fix
any problems caused by updating:

- The SaveState and LoadState events in NavigationHelper have been replaced by methods
you're intended to call within the page's `OnNavigated*` overrides. As an example:
	```C#
		private NavigationHelper PageHelper;
		public MyPage()
		{
			InitializeComponent();
			this.PageHelper = new NavigationHelper(this);
			this.PageHelper.LoadingState += PageHelper_LoadingState;
			this.PageHelper.SavingState += PageHelper_SavingState;
		}
		private void PageHelper_LoadingState(object sender, LoadStateEventArgs args)
		{
			if (args.HasState)
			{
				var myValue = args.PageState["MyKey"];
			}
		}
		private void PageHelper_SavingState(object sender, NavigationEventArgs args)
		{
			var dict = new Dictionary<string, object>();
            dict["MyKey"] = "MyValue";
            PageHelper.SaveState(dict);
		}
	```

	The above should be changed to:
	```C#
		private NavigationHelper PageHelper;
		public MyPage()
		{
			InitializeComponent();
			this.PageHelper = new NavigationHelper(this);
		}
		protected override void OnNavigatedTo(NavigationEventArgs e)
		{
			var state = this.PageHelper.LoadState(e.NavigationMode);
			if (state != null)
			{
				var myValue = state["MyKey"];
			}
		}
		protected override void OnNavigatedFrom(NavigationEventArgs args)
		{
			var dict = new Dictionary<string, object>();
            dict["MyKey"] = "MyValue";
            PageHelper.SaveState(dict);
		}
	```
	
- In case you use NavPage, the above applies to you as well, but you get to write even less code:
	```C#
		public MyPage()
		{
			InitializeComponent();
		}
		protected override void OnNavigatedTo(NavigationEventArgs e)
		{
			bool hasState = TryLoadState(e.NavigationMode);
            if (hasState)
            {
                var myValue = PageState["MyKey"];
            }
		}
		protected override void OnNavigatedFrom(NavigationEventArgs args)
		{
            PageState["MyKey"] = "MyValue";
            SaveState();
		}
	```

	The state dictionary is automatically loaded and saved if you don't override
	`OnNavigatedTo` or `OnNavigatedFrom`.


That is all. Happy coding!