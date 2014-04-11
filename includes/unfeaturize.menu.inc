<?php

/**
 * @file
 * menu.module components hook implementations for Unfeaturize module.
 */

/**
 * Implements hook_unfeaturize_restore() for menu_custom component.
 *
 * @see menu_custom_features_rebuild()
 */
function menu_custom_unfeaturize_restore($module, $defaults) {
  foreach ($defaults as $menu) {
    menu_save($menu);
  }
}

/**
 * Implements hook_unfeaturize_restore() for menu_links component.
 *
 * @see menu_links_features_rebuild()
 */
function menu_links_unfeaturize_restore($module, $defaults) {
  menu_links_features_rebuild_ordered($defaults);
}
