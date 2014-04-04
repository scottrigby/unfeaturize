<?php

/**
 * @file
 * Field component hook implementations for Unfeaturize module.
 */

/**
 * Implements hook_unfeaturize() for field_base component.
 *
 * We can not pass our unfeaturized defaults to field_base_features_rebuild()
 * directly, because it assumes they should be from features_get_default().
 * The code is almost exactly the same otherwise.
 *
 * @see field_base_features_rebuild()
 */
function field_base_unfeaturize($module, $defaults) {
  if ($fields = $defaults) {
    field_info_cache_clear();

    // Load all the existing field bases up-front so that we don't
    // have to rebuild the cache all the time.
    $existing_fields = field_info_fields();

    foreach ($fields as $field) {
      // Create or update field.
      if (isset($existing_fields[$field['field_name']])) {
        $existing_field = $existing_fields[$field['field_name']];
        if ($field + $existing_field !== $existing_field) {
          field_update_field($field);
        }
      }
      else {
        field_create_field($field);
        $existing_fields[$field['field_name']] = $field;
      }
      variable_set('menu_rebuild_needed', TRUE);
    }
  }
}

/**
 * Implements hook_unfeaturize() for field_instance component.
 *
 * We can not pass our unfeaturized defaults to
 * field_instance_features_rebuild() directly, because it assumes they should be
 * from features_get_default(). We also replace watchdog() with features_log()
 * for drush compatibility. The code is almost exactly the same otherwise.
 *
 * @see field_instance_features_rebuild()
 */
function field_instance_unfeaturize($module, $defaults) {
  if ($instances = $defaults) {
    field_info_cache_clear();

    // Load all the existing instances up-front so that we don't
    // have to rebuild the cache all the time.
    $existing_instances = field_info_instances();

    foreach ($instances as $field_instance) {
      // If the field base information does not exist yet, cancel out.
      if (!field_info_field($field_instance['field_name'])) {
        continue;
      }

      // Create or update field instance.
      if (isset($existing_instances[$field_instance['entity_type']][$field_instance['bundle']][$field_instance['field_name']])) {
        $existing_instance = $existing_instances[$field_instance['entity_type']][$field_instance['bundle']][$field_instance['field_name']];
        if ($field_instance + $existing_instance !== $existing_instance) {
          try {
            field_update_instance($field_instance);
          }
          catch (FieldException $e) {
            features_log(t('Attempt to update field instance %label (in %entity entity type %bundle bundle) failed: %message', array('%label' => $field_instance['field_name'], '%entity' => $field_instance['entity_type'], '%bundle' => $field_instance['bundle'], '%message' => $e->getMessage())), 'error');
          }
        }
      }
      else {
        try {
          field_create_instance($field_instance);
        }
        catch (FieldException $e) {
          features_log(t('Attempt to create field instance %label (in %entity entity type %bundle bundle) failed: %message', array('%label' => $field_instance['field_name'], '%entity' => $field_instance['entity_type'], '%bundle' => $field_instance['bundle'], '%message' => $e->getMessage())), 'error');
        }
        $existing_instances[$field_instance['entity_type']][$field_instance['bundle']][$field_instance['field_name']] = $field_instance;
      }
    }

    if ($instances) {
      variable_set('menu_rebuild_needed', TRUE);
    }
  }
}
