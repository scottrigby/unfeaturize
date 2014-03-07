<?php

/**
 * @file
 * Unfeaturize variables.
 */

/**
 * Generates a function to install default variables.
 *
 * @param string $module_name
 *   The name of the implementing module being refactored.
 *
 * @return string
 *   A generated installation function named MODULE_install_variables() for
 *   MODULE.install. Then call this generated function in MODULE_install().
 */
function unfeaturize_generate_install_variables_function($module_name) {
  $new = array();
  $new[] = '/**';
  $new[] = ' * Installs default variables.';
  $new[] = ' */';
  $new[] = 'function ' . $module_name . '_install_variables() {';
  $new[] = '  foreach (' . $module_name . '_variables() as $name => $value) {';
  $new[] = '    variable_set($name, $value);';
  $new[] = '  }';
  $new[] = '}';

  return implode("\n", $new);
}

/**
 * Generates a function to define default variables.
 *
 * Refactors an implementation of hook_strongarm().
 *
 * @param string $module_name
 *   The name of the implementing module being refactored.
 *
 * @return string
 *   A generated function named MODULE_variables() for MODULE.install.
 */
function unfeaturize_generate_variables_function($module_name) {
  $old = $module_name . '_strongarm';
  // Assumes the function is loaded. If not, bail.
  if (!function_exists($old)) {
    return;
  }

  // Begin refactored function string output.
  $new = array();
  $new []= '/**';
  $new []= ' * Defines default variables.';
  $new []= ' *';
  $new []= ' * Variable info from deprecated ' . $module_name . '_strongarm().';
  $new []= ' *';
  $new []= ' * @return array';
  $new []= ' *   An associative array of variable values, keyed by name.';
  $new []= ' */';
  $new []= 'function ' . $module_name . '_variables() {';
  $new []= '  $variables = array();';
  $new []= '';

  foreach ($old() as $strongarm) {
    $new []= '  $variables[\'' . $strongarm->name . '\'] = ' . $strongarm->value . ';';
    $new []= '';
  }

  $new []= '  return $variables;';
  $new []= "}";

  return implode("\n", $new);
}
