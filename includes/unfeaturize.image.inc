<?php

/**
 * @file
 * image.module components hook implementations for Unfeaturize module.
 */

/**
 * Implements hook_unfeaturize_restore() for image component.
 *
 * We can not pass our unfeaturized defaults to image_features_revert()
 * directly, because it assumes they should be from features_get_default().
 * The code is exactly the same otherwise.
 *
 * @see image_features_revert()
 */
function image_unfeaturize_restore($module, $defaults) {
  if ($default_styles = $defaults) {
    foreach (array_keys($default_styles) as $default_style) {
      if ($style = image_style_load($default_style)) {
        if ($style['storage'] != IMAGE_STORAGE_DEFAULT) {
          image_default_style_revert($style);
        }
      }
    }
  }
}
