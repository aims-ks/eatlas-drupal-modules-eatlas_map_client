(function ($) {

	$(document).ready(function () {
		console.log('Loading eAtlas Map Client');

		// declare namespace for map client
		window.eatlasMapClient = {};
		window.eatlasMapClient.instances = {};

		var $mapClientDiv = $('.eatlas-map-client-map');
		var mapConfig = $mapClientDiv.data('map-config');

		var mapClient = new aimsMap.MapClient($mapClientDiv.attr('id'), {
			projection: mapConfig.projection,
			centre: mapConfig.centre,
			zoom: mapConfig.zoom,
			mapConfigHost: mapConfig.configHost,
			mapConfigURL: mapConfig.configUrl,
			disableShowLayers: mapConfig.disableShowLayers,
			disableFeatureRequests: mapConfig.disableFeatureRequests,
			featureRequestsResultsPosition: mapConfig.featureRequestsResultsPosition,
			featureRequestsWmsParams: {
				BUFFER: 20,
				FEATURE_COUNT: 1
			}
		});

		// make this client globally accessible
		window.eatlasMapClient.instances[$mapClientDiv.attr('id')] = mapClient;

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
					overlayLayersAPI.setLayerStyle(layer.id, layer.style);
					overlayLayersAPI.addAvailableLayer(layer.id);
					if (layer.visible) {
						overlayLayersAPI.addActiveLayer(layer.id);
					}
				});
			})
			// add custom controls
			.then(function() {
				if (mapConfig.showButtonOpenMapUrl === true) {
					mapClient.getUserInterfaceAPI().getLeftControlsPanel().createButton(
						'open_map_url ol-control',
						'Open map to full extent',
						'zoom_out_map',
						function() {
							window.open($mapClientDiv.data('map-banner-url'));
						},
						null
					);
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

		$mapClientDiv.mappClient = mapClient;
	})
})(jQuery);
