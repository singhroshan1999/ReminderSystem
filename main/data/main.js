function ajaxRequest(fn){
    var ajax = new XMLHttpRequest();
    ajax.onreadystatechange = function (){
        if(this.readyState == 4 && this.status == 200){
            document.getElementById("display") == ajax.responseText;
        }
    };
ajax.open("GET","/?"+fn,true);
ajax.send()
}

function toggleStopWatch(){
ajaxRequest("toggleStopWatch=1");
}