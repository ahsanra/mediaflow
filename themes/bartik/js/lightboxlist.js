var $ = jQuery.noConflict();

$( document ).ready(function() {

	$('#edit-lightboxselector').on('change', function (e) {
	    var optionSelected = $("option:selected", this);
	    var valueSelected = this.value;
	   	$("#edit-parameter-lightboxnodeid-settings-lightboxnodeid").val( valueSelected );	
	});
	
	$("#edit-parameter-lightboxnodeid-settings-lightboxnodeid").val( 	$('#edit-lightboxselector  option:selected').val() );	
	
});