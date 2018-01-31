(function ($) {

	$(document).ready(function () {
		console.log('Loading eAtlas Map Client');

		var mapClientDiv = $('.eatlas-map-client-map')[0];
		var mapConfig = $(mapClientDiv).data('map-config');

		var mapClient = new aimsMap.MapClient($(mapClientDiv).attr('id'), {
			projection: 'EPSG:4326',
			centre: mapConfig.centre,
			zoom: mapConfig.zoom,
			mapConfigHost: mapConfig.configHost,
			mapConfigURL: mapConfig.configUrl
		});

		mapClient.init()
			.then(function () {
				var baseLayersAPI = mapClient.getBaseLayersAPI();
				mapConfig.layers.forEach(function(layer) {
					baseLayersAPI.addAvailableLayer(layer.id);
					if (layer.visible) {
						baseLayersAPI.addActiveLayer(layer.id);
					}
				});

				var overlayLayersAPI = mapClient.getOverlayLayersAPI();
				mapConfig.layers.forEach(function(layer) {
					overlayLayersAPI.addAvailableLayer(layer.id);
					if (layer.visible) {
						overlayLayersAPI.addActiveLayer(layer.id);
					}
				});
				console.log('eAtlas Map Clients successfully loaded');
		});
	})
})(jQuery);
