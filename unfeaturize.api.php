<?php

/**
 * @file
 * Hook and callback signature definitions for Unfeaturize module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Restores a module's unfeaturized components of a given type to the database.
 *
 * This is a component type hook. So the hook should be implemented using the
 * component type, not the module name: COMPONENT_TYPE_unfeaturize_restore()
 * rather than MODULE_features_export().
 *
 * @param string $module
 *   The name of the module storing the unfeaturized component.
 * @param array $defaults
 *   An associative array of the module's component defaults only of that type,
 *   keyed by component name.
 */
function hook_unfeaturize_restore($module, $defaults) {
  // Example: filter component.
  foreach ($defaults as $format) {
    $format = (object) $format;
    filter_format_save($format);
  }
}

/**
 * @} End of "addtogroup hooks".
 */
