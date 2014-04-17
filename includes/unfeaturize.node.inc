<?php

/**
 * @file
 * node.module components hook implementations for Unfeaturize module.
 */

/**
 * Implements hook_unfeaturize_restore() for node component.
 *
 * We can not pass our unfeaturized defaults to node_features_enable()
 * directly, because it assumes they should be from features_get_default(), and
 * also sets ownership in ways we don't want.
 *
 * @see node_features_enable()
 */
function node_unfeaturize_restore($module, $defaults) {
  foreach ($defaults as $type_name => $type_info) {
    // Ensure the type exists.
    if ($type_info = node_type_load($type_name)) {
      // Pass ownership back to node.module.
      $type_info->module = 'node';
      $type_info->custom = 1;
      $type_info->modified = 1;
      $type_info->locked = 0;
      node_type_save($type_info);
      features_log(t('The node type %type-name has been restored.', array('%type-name' => $type_name)), 'status');
    }
  }
}
