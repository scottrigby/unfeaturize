<?php

/**
 * @file
 * page_manager.module components hook implementations for Unfeaturize module.
 */

/**
 * Implements hook_unfeaturize_restore() for page_manager_pages component.
 *
 * We can not use page_manager_pages_features_revert() because CTools components
 * are deleted from the database on revert.
 *
 * @see page_manager_page_import_subtask_submit()
 */
function page_manager_pages_unfeaturize_restore($module, $defaults) {
  foreach ($defaults as $page) {
    $task_name = page_manager_make_task_name('page', $page->name);
    $cache = page_manager_get_page_cache($task_name);
    if (!$cache) {
      $cache = new stdClass();
    }
    page_manager_page_new_page_cache($page, $cache);
    page_manager_set_page_cache($cache);
    page_manager_save_page_cache($cache);
  }
}
