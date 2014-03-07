<?php

/**
 * @file
 * Unfeaturize permissions.
 */

/**
 * Refactors an implementation of hook_user_default_permissions().
 *
 * @param string $module_name
 *   The name of the implementing module being refactored.
 *
 * @return string
 *   A refactored function string.
 */
function unfeaturize_refactor_hook_user_default_permissions($module_name) {
  $old = $module_name . '_user_default_permissions';
  // Assumes the function is loaded. If not, bail.
  if (!function_exists($old)) {
    return;
  }

  // Begin refactored function string output.
  $new = array();
  $new []= '/**';
  $new []= ' * Defines default permissions.';
  $new []= ' *';
  $new []= ' * Permission info from deprecated ' . $module_name . '_user_default_permissions().';
  $new []= ' *';
  $new []= ' * @return array';
  $new []= ' *   An associative array of permission names, keyed by role name.';
  $new []= ' */';
  $new []= 'function ' . $module_name . '_permissions() {';
  $new []= '  $permissions = array();';
  $new []= '';

  $roles = array();
  foreach ($old() as $permission) {
    foreach (array_keys($permission['roles']) as $role_name) {
      // Instantiate the role name key if necessary.
      if (!isset($roles[$role_name])) {
        $roles[$role_name] = array();
      }
      // Add permission name to the array keyed by role name.
      $roles[$role_name][] = $permission['name'];
    }
  }

  foreach ($roles as $role_name => $permissions) {
    $new []= '  $permissions[\'' . $role_name . '\'] = array(';
    foreach ($permissions as $permission_name) {
      $new []= '    \'' . $permission_name . '\',';
    }
    $new []= '  );';
    $new []= '';
  }

  $new []= '  return $permissions;';
  $new []= "}";

  return implode("\n", $new);
}
