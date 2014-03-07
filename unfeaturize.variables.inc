<?php

/**
 * @file
 * Unfeaturize variables.
 */

/**
 * Refactors an implementation of hook_strongarm().
 *
 * @param string $module_name
 *   The name of the implementing module being refactored.
 *
 * @return string
 *   A refactored function string.
 */
function unfeaturize_refactor_hook_strongarm($module_name) {
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
