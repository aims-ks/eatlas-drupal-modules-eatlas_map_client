Content fields

For this module to work it needs the following content fields:
- Map banner URL
    MACHINE NAME: field_map_banner
    FIELD TYPE: Text
    WIDGET: Text field
- Map banner Type
    MACHINE NAME: field_map_banner_type
    FIELD TYPE: List (text)
    WIDGET: Select list
    FIELD SETTINGS:
      small|Small (20% of the page)
      medium|Medium (50% of the page)
      large|Large (80% of the page)
- Map banner configuration
    MACHINE NAME: field_map_banner_configuration
    FIELD TYPE: Term reference
    WIDGET: Select list
    FIELD SETTINGS:
      Vocabulary: Map Client configurations

JS dependencies

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
