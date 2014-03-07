<?php

/**
 * @file
 * Unfeaturize permissions.
 */

/**
 * Generates a function to install default permissions.
 *
 * @param string $module_name
 *   The name of the implementing module being refactored.
 *
 * @return string
 *   A generated installation function named MODULE_install_permissions() for
 *   MODULE.install. Then call this generated function in MODULE_install().
 */
function unfeaturize_generate_install_permissions_function($module_name) {
  $new = array();
  $new[] = '/**';
  $new[] = ' * Installs default permissions.';
  $new[] = ' */';
  $new[] = 'function ' . $module_name . '_install_permissions() {';
  $new[] = '  foreach (' . $module_name . '_permissions() as $role_name => $permissions) {';
  $new[] = '    if ($role = user_role_load_by_name($role_name)) {';
  $new[] = '      user_role_grant_permissions($role->rid, $permissions);';
  $new[] = '    }';
  $new[] = '  }';
  $new[] = '}';

  return implode("\n", $new);
}

/**
 * Generates a function to define default permissions.
 *
 * Refactors an implementation of hook_user_default_permissions().
 *
 * @param string $module_name
 *   The name of the implementing module being refactored.
 *
 * @return string
 *   A generated function named MODULE_permissions() for MODULE.install.
 */
function unfeaturize_generate_permissions_function($module_name) {
  $old = $module_name . '_user_default_permissions';
  // Assumes the function is loaded. If not, bail.
  if (!function_exists($old)) {
    return;
  }

  // Begin refactored function string output.
  $new = array();
  $new[] = '/**';
  $new[] = ' * Defines default permissions.';
  $new[] = ' *';
  $new[] = ' * Permission info from deprecated ' . $module_name . '_user_default_permissions().';
  $new[] = ' *';
  $new[] = ' * @return array';
  $new[] = ' *   An associative array of permission names, keyed by role name.';
  $new[] = ' */';
  $new[] = 'function ' . $module_name . '_permissions() {';
  $new[] = '  $permissions = array();';
  $new[] = '';

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
    $new[] = '  $permissions[\'' . $role_name . '\'] = array(';
    foreach ($permissions as $permission_name) {
      $new[] = '    \'' . $permission_name . '\',';
    }
    $new[] = '  );';
    $new[] = '';
  }

  $new[] = '  return $permissions;';
  $new[] = "}";

  return implode("\n", $new);
}
