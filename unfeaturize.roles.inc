<?php

/**
 * @file
 * Unfeaturize roles.
 */

/**
 * Generates a function to install default roles.
 *
 * @param string $module_name
 *   The name of the implementing module being refactored.
 *
 * @return string
 *   A generated installation function named MODULE_install_roles() for
 *   MODULE.install. Then call this generated function in MODULE_install().
 */
function unfeaturize_generate_install_roles_function($module_name) {
  $new = array();
  $new[] = '/**';
  $new[] = ' * Installs default roles.';
  $new[] = ' */';
  $new[] = 'function ' . $module_name . '_install_roles() {';
  $new[] = '  foreach (' . $module_name . '_roles() as $role) {';
  $new[] = '    user_role_save($role);';
  $new[] = '  }';
  $new[] = '}';

  return implode("\n", $new);
}

/**
 * Generates a function to define default roles.
 *
 * Refactors an implementation of hook_user_default_roles().
 *
 * @param string $module_name
 *   The name of the implementing module being refactored.
 *
 * @return string
 *   A generated function named MODULE_roles() for MODULE.install.
 */
function unfeaturize_generate_roles_function($module_name) {
  $old = $module_name . '_user_default_roles';
  // Assumes the function is loaded. If not, bail.
  if (!function_exists($old)) {
    return;
  }

  // Begin refactored function string output.
  $new = array();
  $new[] = '/**';
  $new[] = ' * Defines default roles.';
  $new[] = ' *';
  $new[] = ' * Role info from deprecated ' . $module_name . '_user_default_roles().';
  $new[] = ' *';
  $new[] = ' * @return array';
  $new[] = ' *   An array of role objects.';
  $new[] = ' */';
  $new[] = 'function ' . $module_name . '_roles() {';
  $new[] = '  $roles = array();';
  $new[] = '';

  foreach ($old() as $role) {
    $new[] = '  $role = new stdClass();';
    $new[] = '  $role->name = \'' . $role['name'] . '\';';
    $new[] = '  $role->weight = ' . $role['weight'] . ';';
    $new[] = '  $roles[] = $role;';
    $new[] = '';
  }

  $new[] = '  return $roles;';
  $new[] = "}";

  return implode("\n", $new);
}
