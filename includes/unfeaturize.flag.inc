<?php

/**
 * @file
 * flag.module components hook implementations for Unfeaturize module.
 */

/**
 * Implements hook_unfeaturize_restore() for flag component.
 *
 * We can not pass our unfeaturized defaults to flag_features_revert()
 * directly, because it assumes they should be from features_get_default().
 * The code is exactly the same otherwise.
 *
 * @see flag_features_revert()
 */
function flag_unfeaturize_restore($module, $defaults) {
  // Build up values for the cache clear.
  $entity_types = array();

  // Revert flags that are defined in code.
  foreach ($default_flags as $flag_name => $flag_info) {
    if (is_numeric($flag_name)) {
      // Backward compatibility.
      $flag_name = $flag_info['name'];
    }
    $flag = flag_load($flag_name, TRUE);
    if ($flag && $flag->revert() === FALSE) {
      features_log(t('Could not revert flag %flag-name to the state described in your code: Your flag was created by a different version of the Flag module than is now being used.', array('%flag-name' => $flag->name)), 'error');
    }
    else {
      features_log(t('The flag %flag-name has been restored.', array('%flag-name' => $flag->name)), 'status');
    }

    $entity_types[] = $flag->entity_type;
  }
  _flag_clear_cache($entity_types);
}
