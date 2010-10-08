$(document).ready(function(){
	$("body").fadeOut(0);
	$("#GuestBook").load("GetGuest");
	$("body").fadeIn(500);
});
