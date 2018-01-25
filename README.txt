//////////////////////////////////
// JS dependencies
//////////////////////////////////

This module has dependencies on the following JS libraries:
- aims-map-withdeps.js
    place file in "sites/all/libraries/mapping-client/js/aims-map-withdeps.js"
- openlayers (> 4.6.4)
    place file in "sites/all/libraries/openlayers/ol.js"
- jquery (> 3.2.1)
    To use multiple jQuery versions the jqmulti module (https://www.drupal.org/project/jqmulti) can be used. When
    installed:
        - the jquery-VERSION.min.js file needs to be placed in the folder "sites/all/jquery"
        - under "admin/config/system/jqmulti" check the "openlayers" and "mapping-client" checkboxes and save