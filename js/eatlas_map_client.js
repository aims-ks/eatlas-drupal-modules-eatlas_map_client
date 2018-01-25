(function ($) {
	// Execute when the page is ready
	$(document).ready(function(){
		console.log('Loading eAtlas Map Clients');

		$('.map_eatlas_map_client').each(function() {
			var mapClient = new aimsMap.MapClient($(this).attr('id'), {
				projection: 'EPSG:4326',
				configUrl: '//maps.eatlas.org.au/atlasmapper/client/gbrf/config/main.json'
			});
			mapClient.init();
		});

		console.log('eAtlas Map Clients successfully loaded');
	});

})(jQuery);
