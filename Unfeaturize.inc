<?php
/**
 * @file
 * Contains Unfeaturize.
 */

/**
 * An object to help unfeaturize a module.
 */
abstract class Unfeaturize {

  /**
   * The name of the module to unfeaturize.
   *
   * @var string
   */
  protected $module;

  /**
   * An optional array of Features components to exclude.
   *
   * @var array
   */
  protected $exclude = array();

  /**
   * Constructs the Unfeaturize object.
   *
   * @param string $module
   *   The name of the module to unfeaturize.
   * @param string $new_name
   *   An optional new name for the unfeaturized module. If not provided, the
   *   new module will be exported with the same name as $module.
   * @param array $exclude
   *   An optional array of Features components to exclude.
   */
  public function __construct($module, $new_name = NULL, $exclude = array()) {
    $this->module = $module;
    $this->exclude = $exclude;
  }

  /**
   * Exports a new unfeaturized module from an existing Features-tracked module.
   *
   * The new module will contain unfeaturized variants of (supported) components
   * in the original.
   */
  public function export() {
    $files_directory = $this->getDirectoryPath();
    if ($this->prepareDirectory($files_directory . '/unfeaturized/' . $this->module)) {
      // @todo This can come later.
    };
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

}
