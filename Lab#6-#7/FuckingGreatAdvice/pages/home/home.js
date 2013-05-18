(function () {
    "use strict";

    WinJS.UI.Pages.define("/pages/home/home.html", {
        // This function is called whenever a user navigates to this page. It
        // populates the page elements with the app's data.
        ready: function (element, options) {
            var adviceListView = element.querySelector("#adviceListView").winControl;
            adviceListView.oniteminvoked = this._itemInvoked.bind(this);
        },

        _itemInvoked: function (args) {
            var selectedItem = Data.allAdvices.getAt(args.detail.itemIndex);

            WinJS.Navigation.navigate("/pages/detail/detail.html",
                { item: selectedItem });
        }
    });
})();
