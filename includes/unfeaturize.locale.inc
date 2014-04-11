<?php

/**
 * @file
 * locale.module components hook implementations for Unfeaturize module.
 */

/**
 * Implements hook_unfeaturize_restore() for language component.
 *
 * We can not pass our unfeaturized defaults to language_features_rebuild()
 * directly, because it assumes they should be from features_get_default().
 * The code is almost exactly the same otherwise.
 *
 * @see language_features_rebuild()
 */
function language_unfeaturize_restore($module, $defaults) {
  foreach ($defaults as $key => $language) {
    _features_language_save((object) $language);
  }

  // Set correct language count.
  $enabled_languages = db_select('languages')
    ->condition('enabled', 1)
    ->fields('languages')
    ->execute()
    ->rowCount();
  variable_set('language_count', $enabled_languages);
}
