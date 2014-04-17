<?php

/**
 * @file
 * strongarm.module components hook implementations for Unfeaturize module.
 */

/**
 * Implements hook_unfeaturize_restore() for variable component.
 *
 * We can not pass our unfeaturized defaults to variable_features_rebuild()
 * directly, because it assumes they should be from features_get_default().
 * The code is exactly the same otherwise.
 *
 * @see variable_features_rebuild()
 */
function variable_unfeaturize_restore($module, $defaults) {
  if (empty($defaults)) {
    return;
  }

  $vars = strongarm_vars_load(TRUE, TRUE);
  foreach ($defaults as $name => $default) {
    if (!empty($vars[$name]->in_code_only) || (drupal_installation_attempted() && $vars[$name]->export_type & EXPORT_IN_CODE)) {
      variable_set($name, $default->value);
    }
  }
}
