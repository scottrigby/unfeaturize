<?php

/**
 * @file
 * field_group.module components hook implementations for Unfeaturize module.
 */

/**
 * Implements hook_unfeaturize_restore() for field_group component.
 *
 * @see field_group_group_save()
 */
function field_group_unfeaturize_restore($module, $defaults) {
  foreach ($defaults as $default) {
    $id = $default->identifier;
    $options = array('@id' => $id);
    if ($return = ctools_export_crud_save('field_group', $default)) {
      features_log(t('The Field Group @id has been restored.', $options), 'status');
    }
    else {
      features_log(t('The Field Group @id could not be restored.', $options), 'error');
    }
  }
}
