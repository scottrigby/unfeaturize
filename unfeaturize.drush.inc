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
    'description' => "Export unfeaturized modules to the files directory.",
    'arguments' => array(
      'feature' => 'A space delimited list of features.',
    ),
    'drupal dependencies' => array('unfeaturize'),
    'aliases' => array('ufe'),
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
