/*
var p,result;//全局变量，p为AMap.PlaceSearch对象，result为AutocompleteResult对象吗，方便在控制台访问
var map = new AMap.Map("container", {
    resizeEnable: true
});

//可以用AMap.plugin（详见文档的参考手册“搜索服务”）， 也可以用Amap.service（详见文档示例中心“POI搜索”）
AMap.plugin(['AMap.PlaceSearch'],function(){
    var placeSearch = new AMap.PlaceSearch({ //构造地点查询类
        pageSize: 5,
        pageIndex: 1,
        city: "010", //城市
        map: map,
        panel: "panel"
    });
    p = placeSearch;
    //关键字查询
    placeSearch.search('北京大学',function (status,result){
        if(status=='complete')
        {
            window.result = result;
        }
    });
});
*/
//下面是不显示地图的简化版
var p,result;//全局变量，p为AMap.PlaceSearch对象，result为AutocompleteResult对象吗，方便在控制台访问
AMap.plugin(['AMap.PlaceSearch'],function(){
    var placeSearch = new AMap.PlaceSearch();
    p = placeSearch;
    //关键字查询
    placeSearch.search('北京',function (status,result){
        if(status=='complete')
        {
            window.result = result;  //变量同名时为了区分，记得全局变量前加个window
        }
    });
});
