/*
查询当前城市的信息：
new 一个 AMap.CitySearch 类
调用 getLocalCity 方法，参数为回调函数，这个回调函数接收两个参数 status 和 result
当status为complete时，result为 CitySearchResult；
当status为error时，result为错误信息info；
当status为no_data时，代表检索返回0结果

CitySearchResult的对象有两个属性
city （类型为String）城市名称，如“广州市”
bounds，为Bounds类的实例(详细见文档) 这个对象用于地图展示该城市时使用的矩形区域
*/

    var result;//自定义的全局变量，我们把CitySearchResult对象赋值给它，方便在控制台调用查看
    var map = new AMap.Map("container");
    showCityInfo();
    //获取用户所在城市信息
    function showCityInfo() {
        //实例化城市查询类
        var citysearch = new AMap.CitySearch();
        //自动获取用户IP，返回当前城市
        citysearch.getLocalCity(function(status, result) {
            if (status === 'complete' && result.info === 'OK') {
                if (result && result.city && result.bounds) {
                    window.result = result;
                    var cityinfo = result.city;
                    var citybounds = result.bounds;
                    document.getElementById('tip').innerHTML = '您当前所在城市：'+cityinfo;
                    //地图显示当前城市
                    map.setBounds(citybounds);
                }
            } else {
                document.getElementById('tip').innerHTML = result.info;
                console.log("failed");
            }
        });
    }


/*
//对上面例子的简化，去除地图显示部分
//在控制台处 可以调用下面自定义result的全局变量，查看里面的相关信息
var result;
showCityInfo();
    //获取用户所在城市信息
    function showCityInfo() {
        //实例化城市查询类
        var citysearch = new AMap.CitySearch();
        //自动获取用户IP，返回当前城市
        citysearch.getLocalCity(function(status, result) {
            if (status === 'complete' && result.info === 'OK') {
                if (result && result.city && result.bounds) {
                    window.result = result;
                    var cityinfo = result.city;
                    var citybounds = result.bounds;
                    console.log(result.city);
                }
            } else {
                console.log("failed");
            }
        });
    }
*/
