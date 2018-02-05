(function ($) {

	$(document).ready(function () {
		console.log('Loading eAtlas Map Client');

		var mapClientDiv = $('.eatlas-map-client-map')[0];
		var mapConfig = $(mapClientDiv).data('map-config');

		var mapClient = new aimsMap.MapClient($(mapClientDiv).attr('id'), {
			projection: mapConfig.projection,
			centre: mapConfig.centre,
			zoom: mapConfig.zoom,
			mapConfigHost: mapConfig.configHost,
			mapConfigURL: mapConfig.configUrl
		});

		mapClient.init()
			// add layers
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
			})
			// arrange controls
			.then(function() {
				var $controlsContainer = $('.eatlas-map-client-map-controls-container')[0];
				if (mapConfig.show_button_select_layers === true) {
					$('.aims-map-show-layers-button').appendTo($controlsContainer);
				} else {
					$('.aims-map-show-layers-button').hide();
				}

				if (mapConfig.show_button_open_map_url === true) {
					$('.open_map_url').show();
				}
			})
			// zoom into feature bounding box
			.then(function () {
				if (typeof mapConfig.bbox !== 'undefined') {
					var extent = [mapConfig.bbox.lonMin, mapConfig.bbox.latMin, mapConfig.bbox.lonMax, mapConfig.bbox.latMax];

					// Transform extent if necessary
					if (mapConfig.bbox.projection !== mapConfig.projection) {
						extent = ol.extent.applyTransform(extent, ol.proj.getTransform(mapConfig.bbox.projection, mapConfig.projection));
					}

					// mapClient.getOlMap().getView().fit(extent);
					mapClient.getOlMap().getView().fit(extent, {padding: [50, 50, 50, 50]});
				}
			});
	})
})(jQuery);
