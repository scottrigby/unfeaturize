<?php

/**
 * @file
 * taxonomy.module components hook implementations for Unfeaturize module.
 */

/**
 * Implements hook_unfeaturize_restore() for taxonomy component.
 *
 * @see taxonomy_features_rebuild()
 */
function taxonomy_unfeaturize_restore($module, $defaults) {
  if ($vocabularies = $defaults) {
    $existing = taxonomy_get_vocabularies();
    foreach ($vocabularies as $vocabulary) {
      $vocabulary = (object) $vocabulary;
      foreach ($existing as $existing_vocab) {
        if ($existing_vocab->machine_name === $vocabulary->machine_name) {
          $vocabulary->vid = $existing_vocab->vid;
        }
      }
      taxonomy_vocabulary_save($vocabulary);
    }
  }
}
