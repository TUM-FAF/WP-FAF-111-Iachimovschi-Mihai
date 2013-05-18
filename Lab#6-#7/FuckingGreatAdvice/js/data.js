/// <reference path="//Microsoft.WinJS.1.0/js/base.js" />
(function () {
    "use strict";

    var advices = new WinJS.Binding.List();

    var apiUrl = "http://fucking-great-advice.ru/api/latest/40";

    WinJS.xhr({ url: apiUrl }).then(function (xhr) {

        var items = JSON.parse(xhr.responseText);

        items.forEach(function (item) {
            item.text = item.text.replace('&nbsp;', ' ');
            item.teaser = item.text.substr(0, 10);
            item.teaser += " ...";
            item.textId = "Advice #" + item.id;
            advices.push(item);
        });
    });

    WinJS.Namespace.define("Data", {
        allAdvices: advices
    });

})();
