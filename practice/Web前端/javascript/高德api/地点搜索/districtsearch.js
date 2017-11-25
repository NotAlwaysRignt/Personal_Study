/*
行政区域查询对象使用步骤:
AMap.plugin[Array，function] Array里面是要用插件的名称，字符串形式，如 AMap.DistrictSearch
new 一个插件对象new AMap.PlaceSearch()
调用这个对象的相关方法，详细见文档
*/

var d,result;//全局变量，d为AMap.PlaceSearch对象，result为AutocompleteResult对象吗，方便在控制台访问
AMap.plugin(['AMap.DistrictSearch'],function(){
    var districtSearch = new AMap.DistrictSearch();
    d = districtSearch;
    //关键字查询
    districtSearch.search('北京',function (status,result){
        if(status=='complete')
        {
            //result 为 DistrictSearchResult，详见文档
            //自己可以在控制台调用查看其内部的属性
            window.result = result;  //变量同名时为了区分，记得全局变量前加个window

        }
    });
});
