(function ($) {
	// Execute when the page is ready
	$(document).ready(function(){
		console.log('Loading eAtlas Map Clients');

		$('.eatlas-map-client-map').each(function() {
			var mapClient = new aimsMap.MapClient($(this).attr('id'), {
				projection: 'EPSG:4326',
				mapConfigHost: $(this).data('map-config-host'),
				mapConfigURL: $(this).data('map-config-url')
			});
			mapClient.init();
		});

		console.log('eAtlas Map Clients successfully loaded');
	});

})(jQuery);
