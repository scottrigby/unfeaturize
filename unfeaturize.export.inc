<?php

/**
 * @file
 * Export functions for Unfeaturize module.
 */

/**
 * Exports a newly unfeaturized module.
 *
 * @param string $module_name
 *   The original Feature module name.
 * @param null|string $path
 *   Optional module export path.
 *
 * @return bool
 *
 * @see _drush_features_export()
 */
function unfeaturize_export($module_name, $path = NULL) {
  $path = isset($path) ? $path : unfeaturize_export_path($module_name);
  if (!unfeaturize_prepare_directory($path)) {
    return FALSE;
  };

  $files = unfeaturize_export_render($module_name);
  foreach ($files as $extension => $data) {
    if (!in_array($extension, array('module', 'info'))) {
      $extension .= '.inc';
    }
    $destination = "{$path}/{$module_name}.$extension";
    if (!file_unmanaged_save_data($data, $destination, FILE_EXISTS_REPLACE)) {
      features_log(t('The file @destination can not be saved.', array('@destination' => $destination)), 'warning');
      break;
    }
  }

  return TRUE;
}

/**
 * Gets the default module export path.
 *
 * @param string $module_name
 *   The original Feature module name.
 *
 * @return string
 *   The default module export path.
 */
function unfeaturize_export_path($module_name) {
  $wrapper = file_stream_wrapper_get_instance_by_scheme('public');
  $files_directory = $wrapper->getDirectoryPath();

  // Standardize the file path location.
  $path = $files_directory . '/unfeaturized/' . $module_name;

  return $path;
}

/**
 * Renders an exported feature.
 *
 * @param $module_name
 *   The original Feature module name.
 *
 * @return array
 *   An unfeaturized, Feature render array.
 *
 * @see features_export_render()
 */
function unfeaturize_export_render($module_name) {
  // Load Features export functions.
  module_load_include('inc', 'features', 'features.export');

  // Get Feature info.
  $load = features_load_feature($module_name);
  $info = $load->info;

  // Generate export definition.
  $export = unfeaturize_export_generate($info, $module_name);

  // Strip Features identification.
  unfeaturize_export_strip($export);

  // Render files.
  $files = features_export_render($export, $module_name, TRUE);

  // We must also modify the rendered info file.
  unfeaturize_export_render_modify($files);

  return $files;
}

/**
 * Generates a Features export array.
 *
 * We don't bother incrementing the version, because distributions may have
 * their own method for doing so.
 *
 * @param $info
 *   The Feature info associative array.
 * @param $module_name
 *   The original Feature module name.
 *
 * @return array $export
 *   A Features export array.
 *
 * @see _drush_features_generate_export()
 */
function unfeaturize_export_generate($info, $module_name) {
  module_load_include('inc', 'features', 'features.export');
  $export = features_populate($info, $module_name);
  if (!features_load_feature($module_name)) {
    $export['name'] = $module_name;
  }

  return $export;
}

/**
 * Strips a Features export array of any Features-identifying info.
 *
 * Note we do not want to rename any files/callback names, as long as we remove
 * the API discovery hooks. The only concern is modules that use path/file-based
 * discovery, in which case we'd need to change those files manually anyway.
 *
 * @param array $export
 *   By reference. An (almost) fully populated export array, minus information
 *   identifying this as a Feature.
 */
function unfeaturize_export_strip(&$export) {
  // Any module with this entry in the .info file  will be treated as a Feature
  // and included in the admin/build/features UI. So we want to remove this.
  // @see features_populate()
  if (isset($export['features']['features_api'])) {
    unset($export['features']['features_api']);
  }

  // Remove API discovery hooks handled by ctools (example: hook_views_api()).
  if (isset($export['features']['ctools'])) {
    unset($export['features']['ctools']);
  }
}

/**
 * Modifies the rendered file contents.
 *
 * @todo Must we still remove 'features[]' lines in order for Features to not
 *   recognize a module as a Feature? Isn't unfeaturize_export_strip() enough?
 *   If we do need to do this, in order to avoid creating our own variant of
 *   features_export_render() (just to omit features_export_prepare()), we must
 *   modify the return value of that function - the rendered files.
 *
 * @param array $files
 *   By reference. An array of info file and module file contents.
 *
 */
function unfeaturize_export_render_modify(&$files) {
  $info = drupal_parse_info_format($files['info']);
  // We want to keep this info so we can parse and restore later.
  if (!empty($info['features'])) {
    foreach ($info['features'] as $type => $component) {
      // Initialize type if needed.
      if (!isset($info['unfeaturize'][$type])) {
        $info['unfeaturize'][$type] = array();
      }
      $info['unfeaturize'][$type] = $component;
    }
    unset($info['features']);
  }
  // Change package.
  if (isset($info['package']) && $info['package'] == 'Features') {
    $info['package'] = 'Unfeaturize';
  }
  // Now recreate the string in Drupal .info format.
  $files['info'] = features_export_info($info);
}

/**
 * Creates a directory for the unfeaturized module, with status logging.
 *
 * @param $directory
 *   See the $directory param for file_prepare_directory().
 *
 * @return bool
 *   See return value of file_prepare_directory().
 */
function unfeaturize_prepare_directory($directory) {
  if (!file_prepare_directory($directory, FILE_MODIFY_PERMISSIONS | FILE_CREATE_DIRECTORY)) {
    features_log(t('The directory @directory can not be created.', array('@directory' => $directory)), 'warning');
    return FALSE;
  }
  else {
    features_log(t('The directory @directory was created.', array('@directory' => $directory)), 'status');
    return TRUE;
  }
}
