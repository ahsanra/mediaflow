<?php

/**
 * @file
 * Mediabox library pages.
 */

/**
 * @todo - seems that this is not used?
 */
function mediabox_library() {
  $path = drupal_get_path('module', 'mediabox_library');
  drupal_add_css($path . '/css/mediabox_library.css');
  drupal_add_js($path . '/js/mediabox_library.js');
  
  module_load_include('inc', 'mediabox_library', 'includes/mediabox_library.forms');
  return drupal_render(drupal_get_form('mediabox_library_list_form'));
}

/**
 * Display library links
 *
 * @param mediabox $mediabox
 * @return string
 */
function mediabox_library_actions($mediabox) {
  $links = array(
    array(
      'title' => t('Update'),
      'href' => 'admin/structure/mediabox-library/update/' . $mediabox->id,
    ),
    array(
      'title' => t('Remove'),
      'href' => 'admin/library/remove/' . $mediabox->id,
    ),
  );

  // invoke hook to collect actions from other modules
  $links = array_merge(module_invoke_all('mediabox_actions', $mediabox), $links);

  return theme('links', array(
    'links' => $links,
  ));
}

/**
 * Uploads new media objects.
 *
 * @return array|mixed
 */
function mediabox_library_upload() {
  module_load_include('inc', 'mediabox_library', 'includes/mediabox_library.forms');
  return drupal_get_form('mediabox_library_form_upload');
}

/**
 * Page callback
 * uri: admin/library/remove/%media
 *
 * @param array $mediaboxes
 */
function mediabox_library_remove($mediaboxes = array()) {

  if ($mediaboxes == NULL) {
    drupal_set_message(t('No mediabox file found'));
    drupal_goto('admin/library');
  }

  // Multiple media files to remove
  if (!is_array($mediaboxes)) {
    $mediaboxes = array($mediaboxes->id => $mediaboxes);
  }

  foreach ($mediaboxes as $mediabox) {
    if (mediabox_delete($mediabox) === FALSE) {

      drupal_set_message(t('Can not remove file "%title".', array(
        '%title' => $mediabox->title,
      )), 'error');
    }
    else {

      drupal_set_message(t('Media file "%title" successfully removed.', array(
        '%title' => $mediabox->title,
      )));
    }
  }

  drupal_goto('admin/library');
}

/**
 * Page callback
 * uri: admin/library/update/%mediabox
 *
 * @param mediabox $mediabox
 * @return string
 */
function mediabox_library_update($mediabox) {
  $path = drupal_get_path('module', 'mediabox_library');
  drupal_add_css($path . '/css/mediabox_library.css');
  drupal_add_js($path . '/js/mediabox_library.js');

  module_load_include('inc', 'mediabox', 'includes/mediabox.forms');
  return theme('mediabox_library_detail', array('mediabox' => $mediabox)) .
    drupal_render(drupal_get_form('mediabox_form_media', $mediabox));
}
