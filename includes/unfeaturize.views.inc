<?php

/**
 * @file
 * views.module components hook implementations for Unfeaturize module.
 */

/**
 * Implements hook_unfeaturize_restore() for views_view component.
 *
 * Recreate relevant pieces of views_ui_import_validate(), otherwise we would
 * need to add views_ui.module as a dependency, and deal with form errors and
 * system messages (not ideal for drush).
 *
 * Bypass the validation of view pluigns/handlers, just as Views reading a
 * default views_view from code would.
 *
 * After initializing the handler, views_ui_import_submit() stores the view in
 * cache with @code views_ui_cache_set($view) @endcode. The user is then
 * redirected to edit before manual save. We don't want to force UI interaction.
 * Instead we draw from views_ui_edit_view_form_validate() and
 * views_ui_edit_view_form_submit() as the user would do manually. Note since we
 * don't set the cache, we also save an additional ctools_object_cache_clear().
 */
function views_view_unfeaturize_restore($module, $defaults) {
  foreach($defaults as $view) {
    // Check API version.
    if (empty($view->api_version) || $view->api_version < 2) {
      features_log(t('That view is not compatible with this version of Views.
        If you have a view from views1 you have to go to a drupal6 installation and import it there.'), 'error');
      return FALSE;
    }
    elseif (version_compare($view->api_version, views_api_version(), '>')) {
      features_log(t('That view is created for the version @import_version of views, but you only have @api_version', array(
        '@import_version' => $view->api_version,
        '@api_version' => views_api_version())), 'error');
      return FALSE;
    }

    // Copy the view, and set the base table properly if this is overwriting an
    // existing view.
    $test = views_get_view($view->name);
    if ($test->vid) {
      $view->vid = $test->vid;
    }
    $view->update();

    // Mimic the views_ui edit form submission. Note view::validate() calls
    // view::init_display() for us.
    $errors = $view->validate();
    if ($errors !== TRUE) {
      foreach ($errors as $error) {
        features_log($error, 'error');
      }
      return FALSE;
    }

    // If we get this far, restore the view.
    $view->save();
    features_log(t('The view %name has been restored.', array('%name' => $view->get_human_name())), 'status');
  }
}
