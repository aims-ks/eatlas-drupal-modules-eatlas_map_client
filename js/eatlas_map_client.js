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
      disableDefaultOverlayLayer: mapConfig.disableDefaultOverlayLayer,
			disableShowLayers: mapConfig.disableShowLayers,
			disableMetadata: mapConfig.disableMetadata,
			disableFeatureRequests: mapConfig.disableFeatureRequests,
			featureRequestsResultsPosition: mapConfig.featureRequestsResultsPosition,
			featureRequestsWmsParams: {
				BUFFER: 20,
				FEATURE_COUNT: 1
			},
			defaultActiveComponent: mapConfig.defaultActiveComponent
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
					overlayLayersAPI.setLayerProperty(layer.id, 'style', layer.style);
					if (layer.hasOwnProperty('opacity')) {
						overlayLayersAPI.setLayerProperty(layer.id, 'opacity', layer.opacity);
					}
					overlayLayersAPI.addAvailableLayer(layer.id);
					if (layer.visible) {
						overlayLayersAPI.addActiveLayer(layer.id);
					}
				});
			})
			// add custom controls
			.then(function() {
				if (mapConfig.showButtonOpenMapUrl === true) {
					var openMapUrlButton = mapClient.getUserInterfaceAPI().getLeftControlsPanel().createButton(
						'open_map_url',
						'Open map to full extent',
						'zoom_out_map',
						false
					);
					openMapUrlButton.on(openMapUrlButton.EVENT_CLICKED, function() {
					  window.open($mapClientDiv.data('map-banner-url'));
					});
				}

				var mapSizeClass = '';
				if ($mapClientDiv.hasClass('medium-map')) {
          mapSizeClass = 'medium-map';
        } else if ($mapClientDiv.hasClass('small-map')) {
          mapSizeClass = 'small-map';
        }

        // only add expand-button when it is either a medium or small map
        if (mapSizeClass !== '') {
          var expandButton = mapClient.getUserInterfaceAPI().getRightControlsPanel().createButton(
            'open_map_fullscreen',
            'Open map to full extent',
            'fullscreen',
            false,
            'top'
          );

          expandButton.on(expandButton.EVENT_CLICKED, function() {
            $mapClientDiv.toggleClass(mapSizeClass);
            $mapClientDiv.toggleClass('large-map');
            if ($mapClientDiv.hasClass('large-map')) {
              $mapClientDiv.find('.aims-map-controls-panel .open_map_fullscreen i').html('fullscreen_exit');
            } else {
              $mapClientDiv.find('.aims-map-controls-panel .open_map_fullscreen i').html('fullscreen');
            }
            mapClient.getOlMap().updateSize();
          });
        }
			})
			// zoom into feature bounding box
			.then(function () {
				if (typeof mapConfig.bbox !== 'undefined') {
					var extent = [mapConfig.bbox.lonMin, mapConfig.bbox.latMin, mapConfig.bbox.lonMax, mapConfig.bbox.latMax];

					// Transform extent if necessary
					if (mapConfig.bbox.projection !== mapClient.getOlMap().getView().getProjection().getCode()) {
						extent = ol.extent.applyTransform(extent,
              ol.proj.getTransform(mapConfig.bbox.projection, mapClient.getOlMap().getView().getProjection().getCode()));
					}

					// mapClient.getOlMap().getView().fit(extent);
					mapClient.getOlMap().getView().fit(extent, {padding: [50, 50, 50, 50]});
				}
			});

		$mapClientDiv.mappClient = mapClient;
	})
})(jQuery);
