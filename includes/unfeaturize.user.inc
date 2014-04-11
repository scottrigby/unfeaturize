<?php

/**
 * @file
 * user.module components hook implementations for Unfeaturize module.
 */

/**
 * Implements hook_unfeaturize_restore() for user_permission component.
 *
 * We can not pass our unfeaturized defaults to
 * user_permission_features_rebuild() directly, because it assumes they should
 * be from features_get_default(). The code is exactly the same otherwise.
 *
 * @see field_base_features_rebuild()
 */
function user_permission_unfeaturize_restore($module, $defaults) {
  // Make sure the list of available node types is up to date, especially when
  // installing multiple features at once, for example from an install profile
  // or via drush.
  node_types_rebuild();

  $modules = user_permission_get_modules();
  $roles = _user_features_get_roles();
  $permissions_by_role = _user_features_get_permissions(FALSE);
  foreach ($defaults as $permission) {
    $perm = $permission['name'];
    _user_features_change_term_permission($perm, 'machine_name');
    if (empty($modules[$perm])) {
      $args = array('!name' => $perm, '!module' => $module,);
      $msg = t('Warning in features rebuild of !module. No module defines permission "!name".', $args);
      drupal_set_message($msg, 'warning');
      continue;
    }
    // Export vocabulary permissions using the machine name, instead of
    // vocabulary id.
    foreach ($roles as $role) {
      if (in_array($role, $permission['roles'])) {
        $permissions_by_role[$role][$perm] = TRUE;
      }
      else {
        $permissions_by_role[$role][$perm] = FALSE;
      }
    }
  }
  // Write the updated permissions.
  foreach ($roles as $rid => $role) {
    if (isset($permissions_by_role[$role])) {
      user_role_change_permissions($rid, $permissions_by_role[$role]);
    }
  }
}

/**
 * Implements hook_unfeaturize_restore() for user_role component.
 *
 * We can not pass our unfeaturized defaults to user_role_features_rebuild()
 * directly, because it assumes they should be from features_get_default(). The
 * code is exactly the same otherwise.
 *
 * @see user_role_features_rebuild()
 */
function user_role_unfeaturize_restore($module, $defaults) {
  foreach ($defaults as $role) {
    $role = (object) $role;
    if ($existing = user_role_load_by_name($role->name)) {
      $role->rid = $existing->rid;
    }
    user_role_save($role);
  }
}
