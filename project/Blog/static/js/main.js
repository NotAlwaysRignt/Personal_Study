$(document).ready(function() {

var menu_api;//导航条控件
$("nav").hide();
$("#post-container").hide();
//初始时文章和导航条面板并无内容,隐藏面板,以免样式的干扰

//hightlight.js 和markedjs初始化
var rendererMD = new marked.Renderer();
    marked.setOptions({
      renderer: rendererMD,
      gfm: true,
      tables: true,
      breaks: false,
      pedantic: false,
      sanitize: false,
      smartLists: true,
      smartypants: false
    });
    var markdownString =
    marked.setOptions({
        highlight: function (code) {
        return hljs.highlightAuto(code).value;
      }
    });


//获取导航条所需数据
$.getJSON("/menu",function(menu_data){
    for(var latest_article of menu_data["latest_articles"]){
        $("#latest_articles").append('<li><a class="article" href="#" article_url="/article?article_id=' +
                                    latest_article.id+ '">'+ latest_article.title +'</a></li>')
    }

    var category_list="";
    for(var category_data of menu_data.category_list){
        category_list = category_list +'<li><span>'+category_data.category.name+'</span><ul>'

        for(var series_data of category_data.series_list){
            category_list = category_list + '<li><span>'+series_data.series.name+"</span><ul>"

            for(var article of series_data.article_list){
                category_list = category_list + '<li><a class="article" href="#" article_url="/article?article_id='+
                article.id +'">'+ article.title + '</a></li>'
            }

            category_list = category_list + '</ul></li>'
        }

        category_list = category_list + '</ul></li>'
    }
    $("#categories").append(category_list);



//数据初始化完毕后,创建导航条面板
$("#menu").mmenu({
    extensions 	: [ "shadow-panels", "fx-panels-slide-100", "border-none", "theme-black", "fullscreen" ],
    counters:true,
    navbars		: {
    content : [ "prev", "searchfield","close" ],
    height 	: 2
    },
    setSelected: true,
    searchfield: {
    resultsPanel: true
    }}, { });
    $(".mh-head.mm-sticky").mhead({
        scroll: {
            hide: 200
        }
});

$(".mh-head:not(.mm-sticky)").mhead({
    scroll: false
});

//导航条加载完毕可以显示
$("nav").show();

//获取导航条对象以便开启和关闭

menu_api = $("#menu").data("mmenu");
//按钮事件应该在动态添加完按钮后执行!

$("a.article").click(function(){
    menu_api.close();
    $("profile").hide();
    $("#post-container").show();
    $.getJSON($(this).attr("article_url"),function(article){
        text_data =article;
        $(".panel-heading").text(new Date(article["datetime"]).toLocaleString());
        $("#article_title").text(article["title"]);
        $("#article_content").html(marked(article["content"]));
    })
});

});

});
