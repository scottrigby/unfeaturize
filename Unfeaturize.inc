<?php
/**
 * @file
 * Contains Unfeaturize.
 */

/**
 * An object to help unfeaturize a module.
 */
class Unfeaturize {

  /**
   * The name of the old module to unfeaturize.
   *
   * @var string
   */
  public $old_name;

  /**
   * The name of the new unfeaturized module.
   *
   * @var string
   */
  public $name;

  /**
   * An optional array of Features components to exclude.
   *
   * @var array
   */
  public $exclude;

  /**
   * The exported unfeaturized module directory.
   *
   * @var null|string
   */
  public $export_path;

  public $old_object;
  public $component_types;
  public $old_components;
  public $defaults = array();
  public $classes = array();

  /**
   * Constructs the Unfeaturize object.
   *
   * @param string $old_name
   *   The name of the module to unfeaturize.
   * @param string $new_name
   *   An optional new name for the unfeaturized module. If not provided, the
   *   new module will be exported with the same name as $module.
   * @param array $exclude
   *   An optional array of Features components to exclude.
   */
  public function __construct($old_name, $new_name = NULL, $exclude = array()) {
    $this->old_name = $old_name;
    $this->name = isset($new_name) ? $new_name : $this->old_name;
    $this->exclude = $exclude;
  }

  /**
   * Exports a new unfeaturized module from an existing Features-tracked module.
   *
   * The new module will contain unfeaturized variants of (supported) components
   * in the original.
   */
  public function export() {
    if (!$files_directory = $this->getDirectoryPath()) {
      return FALSE;
    }

    $export_path = $files_directory . '/unfeaturized/' . $this->old_name;
    if (!$this->prepareDirectory($export_path)) {
      return FALSE;
    };

    $this->export_path = $export_path;

    // @todo move to separate method, so we can preview code before export.
    // @todo Get instances of UnfeaturizeComponent, either by a traditional
    //   Drupal hook or by some registry magic. Somehow we want to map
    //   features component types to UnfeaturizeComponent classes.
    $this->classes = module_invoke_all('unfeaturize_components');

    $this->setComponentDefaults();
    foreach ($this->defaults as $type => $data) {
      if (isset($this->classes[$type]) && $this->classes[$type] instanceof UnfeaturizeComponentInterface) {
        $class = $this->classes[$type];
        $component = new $class($data);
      }
      $debug;
    }
  }

  /**
   * Gets a scheme's file directory path.
   *
   * @param string $scheme
   *   See $scheme param for file_stream_wrapper_get_instance_by_scheme().
   *
   * @return string
   *   The file directory path.
   */
  protected function getDirectoryPath($scheme = 'public') {
    $wrapper = file_stream_wrapper_get_instance_by_scheme($scheme);
    return $wrapper ? $wrapper->getDirectoryPath() : NULL;
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
  protected function prepareDirectory($directory) {
    if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
      watchdog('Unfeaturize', 'The directory @directory can not be created.', array('@directory' => $directory), WATCHDOG_WARNING);
      return FALSE;
    }
    else {
      watchdog('Unfeaturize', 'The directory @directory was created.', array('@directory' => $directory), WATCHDOG_INFO);
      return TRUE;
    }
  }

  protected function featuresInclude($name) {
    module_load_include('inc', 'features', $name);
  }

  public function setComponents() {
    $this->old_object = features_load_feature($this->old_name);
    $this->component_types = features_get_feature_components();
    $this->old_components = array_intersect_key($this->old_object->info['features'], $this->component_types);
  }

  public function setComponentDefaults() {
    $this->setComponents();
    // Get defaults.
    // @todo Look at features_var_export().
    features_include_defaults(array_keys($this->old_components));
    foreach ($this->old_components as $type => $names) {
      if (isset($this->component_types[$type]['default_hook']) && $hook = $this->component_types[$type]['default_hook']) {
        $callback = $this->old_name . '_' . $hook;
        if (function_exists($callback)) {
          $this->defaults[$type] = call_user_func($callback);
        }
      }
    }
  }

  /**
   * Gets unfeaturized definition function, by component.
   */
  public function getComponentDefinition($component) {
    return $this->componentDefinition();
  }

  /**
   * Gets unfeaturized installer function, by component.
   */
  public function getComponentInstaller($component) {
    return $this->componentInstaller();
  }

}

/**
 * Sketch for component class.
 */
interface UnfeaturizeComponentInterface {

  public function componentDefinition();
  public function componentInstaller();

}

abstract class UnfeaturizeComponent implements UnfeaturizeComponentInterface {

  protected $data;
  protected $old_name;
  protected $name;
  public $code = array();

  public function __construct($old_name, $name, $data) {
    $this->old_name = $old_name;
    $this->name = $name;
    $this->data = $data;
  }

  /**
   * @see features_export_render_defaults()
   */
  public function DocBlock() {
    $code = array();

    $code[] = '/**';
    $code[] = ' * Data from deprecated ' . $this->old_name . '_user_default_roles().';
    $code[] = ' *';
    $code[] = ' * @return array';
    $code[] = ' *   An array of role objects.';
    $code[] = ' */';

    return $code;
  }

}

class UnfeaturizeComponentUserRole extends UnfeaturizeComponent {

  public function componentDefinition() {
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
  public function componentInstaller() {};

}

class UnfeaturizeComponentStrongarm extends UnfeaturizeComponent {

  public function componentDefinition() {}
  public function componentInstaller() {};

}

