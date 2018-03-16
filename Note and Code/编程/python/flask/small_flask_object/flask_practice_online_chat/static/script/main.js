window.timestamp = 0;

function show_chat_list(result){
$.each(result.message, function(idx,msg)
            {
                $("ul.chat_list").append("<li>"+
                msg.time+"<br>"+msg.name+":"+msg.content+"</li>");
            }
       );
return create_comet(window.timestamp);
}


function create_comet(timestamp){
    $(document).ready($.getJSON("/update",{"timestamp":timestamp},
    function(result)
    {
        window.timestamp = result.latest_time;
        show_chat_list(result);
    }
    )
    );
}

function on_submit(){
$(document).ready(function(){
  $("#send").click(function(){
    $.ajax({url:"/add",data:$('#chat').serialize(),type:"POST",
    success:function(data){
   //因为在服务器端并没有在访问/add后返回任何东西,所以success的函数不会被执行
   //success中函数参数的data可以是json对象,然后用json的形式使用它 如data.attr...
    }});
    $("input[name='message']").val("").focus();//先发送数据,才能清空
  });
});
}
