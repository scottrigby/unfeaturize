<?php

/**
 * @file
 * Unfeaturize Drush integration.
 */

/**
 * Implements hook_drush_command().
 */
function unfeaturize_drush_command() {
  $items = array();

  $items['unfeaturize-export'] = array(
    'description' => "Export unfeaturized modules.",
    'arguments' => array(
      'feature' => 'A space delimited list of features.',
    ),
    'drupal dependencies' => array('unfeaturize'),
    'aliases' => array('ufe'),
  );
  $items['unfeaturize-restore'] = array(
    'description' => "Restores a module's unfeaturized components to the database.",
    'arguments' => array(
      'feature' => 'A space delimited list of modules or MODULE.COMPONENT pairs.',
    ),
    'examples' => array(
      'drush fr foo.node foo.taxonomy bar' => 'Restore node and taxonomy components of feature "foo". Restore all components of feature "bar".',
    ),
    'drupal dependencies' => array('unfeaturize'),
    'aliases' => array('ufr'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function unfeaturize_drush_help($section) {
  switch ($section) {
    case 'drush:unfeaturize-export':
      return dt("Export unfeaturized modules to the files directory.");
  }
}

/**
 * Exports unfeaturized modules.
 *
 * @todo Consolidate this logic with unfeaturize_export() even more.
 *
 * @see drush_features_update()
 * @see _drush_features_export()
 */
function drush_unfeaturize_export() {
  if ($args = func_get_args()) {
    module_load_include('inc', 'unfeaturize', 'unfeaturize.export');
    foreach ($args as $module) {
      if (($feature = features_load_feature($module, TRUE)) && module_exists($module)) {
        $directory = unfeaturize_export_path($module);
        if (unfeaturize_export($module, $directory)) {
          drush_log(dt("Created module: !module in !directory", array('!module' => $module, '!directory' => $directory)), 'ok');
        }
        else {
          drush_die(dt("Couldn't create module: !module in !directory", array('!module' => $module, '!directory' => $directory)));
        }
      }
      elseif ($feature) {
        _features_drush_set_error($module, 'FEATURES_FEATURE_NOT_ENABLED');
      }
      else {
        _features_drush_set_error($module);
      }
    }
  }
  else {
    // By default just show contexts that are available.
    $rows = array(array(dt('Available features')));
    foreach (features_get_features(NULL, TRUE) as $name => $info) {
      $rows[] = array($name);
    }
    drush_print_table($rows, TRUE);
  }
}

/**
 * Restores a module's unfeaturized components to the database.
 *
 * @todo Consolidate this logic with unfeaturize_restore().
 */
function drush_unfeaturize_restore() {
  if ($args = func_get_args()) {
    unfeaturize_include();

    // Parse list of arguments.
    // @see drush_features_revert().
    $modules = array();
    foreach ($args as $arg) {
      $arg = explode('.', $arg);
      $module = array_shift($arg);
      $component_type = array_shift($arg);
      if (isset($module)) {
        if (empty($component_type)) {
          // If we received just a module name, get all unfeaturized components.
          $modules[$module] = TRUE;
        }
        elseif ($modules[$module] !== TRUE) {
          if (!isset($modules[$module])) {
            $modules[$module] = array();
          }
          $modules[$module][] = $component_type;
        }
      }
    }

    foreach ($modules as $module => $component_types_needed) {
      $dt_args['@module'] = $module;
      $component_types = array_keys(unfeaturize_get_components($module));
      if (!empty($component_types_needed) && is_array($component_types_needed)) {
        $component_types = array_intersect($component_types, $component_types_needed);
      }
      if (empty($component_types)) {
        drush_log(dt('There are no unfeaturized components for @module.', $dt_args), 'ok');
      }
      else {
        foreach ($component_types as $component_type) {
          $dt_args['@component_type'] = $component_type;
          if ($defaults = unfeaturize_get_defaults($module, $component_type)) {
            features_invoke($component_type, 'unfeaturize_restore', $module, $defaults);
            if (features_hook($component_type, 'unfeaturize_restore')) {
              drush_log(dt("Restoring unfeaturized component type @module.@component_type", $dt_args), 'ok');
            }
            else {
              features_log(t('No callback to restore unfeaturized component type @module.@component_type.', $dt_args), 'warning');
            }
          }
          else {
            drush_log(dt("Could not restore unfeaturized component type @module.@component_type", $dt_args), 'error');
          }
        }
      }
    }
  }
  else {
    drush_features_list();
  }
}
