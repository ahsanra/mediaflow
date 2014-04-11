<?php

/**
 * @file
 * Mediabox display forms class.
 */

/**
 * Mediabox UI settings form.
 */
function mediabox_ui_settings_form($form, &$form_state) {

  $form['mediabox_ui_filepath'] = array(
    '#title' => t('File path'),
    '#description' => t('Temporary path for storing temporary cropped images'),
    '#type' => 'textfield',
    '#default_value' => variable_get('mediabox_ui_filepath', 'crops'),
    '#field_prefix' => 'public://',
    '#required' => TRUE,
    '#after_build' => array('mediabox_ui_check_directory'),
  );

  return system_settings_form($form);
}

/**
 * Mediabox UI settings form after build callback.
 */
function mediabox_ui_check_directory($form_element) {
  $directory = drupal_realpath('public://' . $form_element['#value']);
  if (strlen($directory) == 0) {
    return $form_element;
  }

  if (!is_dir($directory) && !drupal_mkdir($directory, NULL, TRUE)) {
    // If the directory does not exists and cannot be created.
    form_set_error($form_element['#parents'][0], t('The directory %directory does not exist and could not be created.', array('%directory' => $directory)));
    watchdog('file system', 'The directory %directory does not exist and could not be created.', array('%directory' => $directory), WATCHDOG_ERROR);
  }

  if (is_dir($directory) && !is_writable($directory) && !drupal_chmod($directory)) {
    // If the directory is not writable and cannot be made so.
    form_set_error($form_element['#parents'][0], t('The directory %directory exists but is not writable and could not be made writable.', array('%directory' => $directory)));
    watchdog('file system', 'The directory %directory exists but is not writable and could not be made writable.', array('%directory' => $directory), WATCHDOG_ERROR);
  }
  elseif (is_dir($directory)) {
    // Create public .htaccess file.
    drupal_set_message(t('Directory exists and is writeable.'));
    file_create_htaccess($directory, FALSE);
  }

  return $form_element;
}
