<?php

/**
 * @file
 * Unfeaturize roles.
 */

/**
 * Refactors an implementation of hook_user_default_roles().
 *
 * @param string $module_name
 *   The name of the implementing module being refactored.
 *
 * @return string
 *   A refactored function string.
 */
function unfeaturize_refactor_hook_user_default_roles($module_name) {
  $old = $module_name . '_user_default_roles';
  // Assumes the function is loaded. If not, bail.
  if (!function_exists($old)) {
    return;
  }

  // Begin refactored function string output.
  $new = array();
  $new []= '/**';
  $new []= ' * Defines default roles.';
  $new []= ' *';
  $new []= ' * Role info from deprecated ' . $module_name . '_user_default_roles().';
  $new []= ' *';
  $new []= ' * @return array';
  $new []= ' *   An array of role objects.';
  $new []= ' */';
  $new []= 'function ' . $module_name . '_roles() {';
  $new []= '  $roles = array();';
  $new []= '';

  foreach ($old() as $role) {
    $new []= '  $role = new stdClass();';
    $new []= '  $role->name = \'' . $role['name'] . '\';';
    $new []= '  $role->weight = ' . $role['weight'] . ';';
    $new []= '  $roles[] = $role;';
    $new []= '';
  }

  $new []= '  return $roles;';
  $new []= "}";

  return implode("\n", $new);
}
