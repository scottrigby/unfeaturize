<?php

/**
 * @file
 * filter.module components hook implementations for Unfeaturize module.
 */

/**
 * Implements hook_unfeaturize_restore() for filter component.
 *
 * We can not pass our unfeaturized defaults to filter_features_rebuild()
 * directly, because it assumes they should be from features_get_default().
 * The code is almost exactly the same otherwise.
 *
 * @see filter_features_rebuild()
 */
function filter_unfeaturize_restore($module, $defaults) {
  foreach ($defaults as $format) {
    $format = (object) $format;
    filter_format_save($format);
  }
}
