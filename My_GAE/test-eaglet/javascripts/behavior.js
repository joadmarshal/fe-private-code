$(document).ready(function(){
	$("#menu ul a").each(function(){
		$(this).click(function(){
			var html=$(this).attr("href");
			$("#content").attr({src:html});
			//�ѵ�<html>
			return false;
		})
        	$(this).hover(
			function(){
				$(this).css({color:'#00F0F0'});
				$(this).stop().animate({fontSize:'18px'},200);
			},
			function(){
				$(this).css({color:'#ffffff'});
				$(this).stop().animate({fontSize:'11px'},200);
			}
		)
        });
});


$(document).ready(function(){
});