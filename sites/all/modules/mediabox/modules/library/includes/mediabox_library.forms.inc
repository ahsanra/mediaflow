<?php

/**
 * @file
 * Mediabox library forms.
 */

/**
 * @todo - seems that this is not used, @see mediabox_library() callback
 * that is not used.
 */
function mediabox_library_list_form($form, &$form_state) {
  $form = array();

  // Hook implementation
  $options = array(
    'mediabox_library_action_remove_selected' => t('Remove selected media')
  );
  $options = array_merge(module_invoke_all('mediabox_actions_options'), $options);

  // Class
  $form['#attributes']['class'] = array('library-form');

  // Search submit
  $form['search_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search Media'),
    '#submit' => array('mediabox_library_list_form_search_submit'),
    '#attributes' => array(
      'class' => array('library-search-submit'),
    ),
  );

  // Search
  $search = !empty($_GET['search']) ? $_GET['search'] : NULL;
  $form['search'] = array(
    '#type' => 'textfield',
    '#default_value' => $search,
    '#attributes' => array(
      'class' => array('library-search-input'),
    ),
  );

  // Actions
  $form['action'] = array(
    '#title' => t('Action:'),
    '#type' => 'select',
    '#options' => $options,
    '#empty_value' => '',
    '#empty_option' => '-------',
  );

  // Actions submit
  $form['action_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Go'),
    '#submit' => array('mediabox_library_list_form_actions_submit'),
  );

  $medias = mediabox_db_get_media_list($search);

  if (is_array($medias)) {
    foreach ($medias as $key => $media) {
      $file = file_load($media->fid);
      $mediabox = new mediabox();
      $mediabox->populateFromObj($media);

      $image_properties = array(
        'path' => $file->uri,
        'style_name' => MEDIABOX_LIBRARY_IMAGE_STYLE,
      );

      $image = theme('image_style', $image_properties);

      $form['items'][$media->id] = array(
        '#type' => 'checkbox',
        '#tree' => TRUE,
        '#params' => array(
          'image' => $image,
          'title' => $mediabox->title,
          'author' => $mediabox->getAuthorName(),
          'type' => $mediabox->type,
          'filetype' => $mediabox->getFiletype(),
          'filesize' => $mediabox->getFilesize(),
          'date' => format_date($mediabox->created),
          'actions' => mediabox_library_actions($mediabox),
        )
      );
    }
  }

  return $form;
}

function mediabox_library_list_form_search_submit($form, &$form_state) {
  $search = $form_state['values']['search'];

  $form_state['redirect'] = array(
    current_path(), array(
      'query' => array(
        'search' => $search,
      ),
    ),
  );
}

function mediabox_library_list_form_actions_submit($form, &$form_state) {
  $data = $form_state['values'];

  // Get list of selected media
  $selected_media = array();
  foreach ($data as $data_key => $data_value) {
    if (is_numeric($data_key) && is_numeric($data_value) && $data_value == 1) {
      $selected_media[] = $data_key;
    }
  }

  // Call user defined function if exists
  if (!empty($data['action'])) {
    if (function_exists($data['action'])) {
      call_user_func($data['action'], $selected_media);
    }
    else {
      drupal_set_message(t('Function with name "@name" does not exist!', array(
        '@name' => $data['action'],
      )), 'warning');
    }

  }
}

/**
 * Bulk upload form.
 *
 * @param $form
 * @param $form_state
 * @param array $options
 *   Can have two suboption arrays:
 *     - 'upload_validators' => array(
 *         'file_validate_extensions' => array('jpg jpeg gif png'),
 *         'my_custom_file_validator => array('some validation criteria'),
 *       )
 *     - 'plupload_settings' => array(
 *         'runtimes' => 'html5',
 *         'chunk_size' => '1mb',
 *       )
 *
 *   plupload_settings is array of settings, that will be passed to Plupload
 *   library. @see: http://www.plupload.com/documentation.php.
 * @return array
 *
 */
function mediabox_library_form_upload($form, &$form_state, $options = array()) {
  $items = array();
  $library = drupal_get_library('plupload', 'plupload');

  /** If callbacks are added to plupload module we can write javascript function
   * which will on click on start upload and then on UploadComplete trigger
   * submit of that page and submit button can be removed from page
   * (just to remain with plupload controls (add & start)
   * @see https://drupal.org/node/2065927
   * UPDATE: Two additional parameters approved, autoupload&autosubmit which will
   * do necessary work for us, waiting to be rolled in new release of plupload.
   */
  $options['plupload_settings'] =  array(
    'runtimes' => 'html5',
    'chunk_size' => '1mb',
    // 'callbacks' => array('UploadComplete' => 'Drupal.mediabox.uploadComplete'),
  );

  if (!file_exists(_plupload_library_path())) {
    drupal_set_message(t('System can not find plupload library at path !path. Please,
      download the library !here and place it to desired location',
      array(
        '!path' => _plupload_library_path(),
        '!here' => l(t('here'), $library['website']))),
      'warning'
    );
  }

  $items['type'] = array(
    '#type' => 'select',
    '#options' => mediabox_db_get_bundles_keyed(),
  );

  $items['files'] = array(
    '#type' => 'plupload',
    '#autoupload' => TRUE,
    '#autosubmit' => TRUE,
    '#title' => t('Files'),
    '#upload_validators' => isset($options['upload_validators']) ? $options['upload_validators'] : NULL,
    '#plupload_settings' => isset($options['plupload_settings']) ? $options['plupload_settings'] : NULL,
  );

  $items['bulk_update'] = array(
    '#type' => 'checkbox',
    '#title' => t('I want to edit medias after upload'),
  );

  $items['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => array('mediabox_library_form_upload_submit'),
  );

  return $items;
}

/**
 * Submit bulk upload.
 *
 * @param $form
 * @param $form_state
 */
function mediabox_library_form_upload_submit($form, &$form_state) {
  $scheme = variable_get('file_default_scheme', 'public') . '://';
  $saved_files = array();
  $ids = array();

  // We can't use file_save_upload() because of http://www.jacobsingh.name/content/tight-coupling-no-not
  foreach ($form_state['values']['files'] as $uploaded_file) {
    if ($uploaded_file['status'] == 'done') {
      $source = $uploaded_file['tmppath'];

      $destination = file_stream_wrapper_uri_normalize($scheme . $uploaded_file['name']);
      $destination = file_unmanaged_move($source, $destination, FILE_EXISTS_RENAME);

      if ($destination) {
        $file = plupload_file_uri_to_object($destination);

        $result = mediabox_db_save_media_from_file($file, $form_state['values']['type']);

        if ($result) {
          $ids[] = $result->id;
        }

        $saved_files[] = $file;
      }
      else {
        drupal_set_message(t('Unable to save file. Check your !settings.'), array(
          '!settings' =>
          l(t('filesystem configuration'), 'admin/config/media/file-system')
        ), FALSE);
      }

    }
    else {
      form_set_error('pud', "Upload of {$uploaded_file['name']} failed");
    }
  }
  // Remembering selected ids so we can mark them as selected in browse page.
  $form_state['ids'] = $ids;

  // Query is not empty so we want to make bulk update.
  if (sizeof($ids) && $form_state['values']['bulk_update']) {
    $id = array_shift($ids);
    $form_state['redirect'] = array('admin/structure/mediabox-library/update/' . $id, array('query' => array('ids' => $ids)));
  }
  else {
    $form_state['redirect'] = 'admin/structure/mediabox-library';
  }

}

/**
 * Bulk update form.
 *
 * @param $form
 * @param $form_state
 * @param $mediabox
 * @return array
 */
function mediabox_library_form_bulk_update($form, &$form_state, $mediabox) {
  drupal_set_title(mediabox_page_edit_title($mediabox));

  form_load_include($form_state, 'inc', 'mediabox', 'includes/mediabox.forms');
  $form = mediabox_form_media($form, $form_state, $mediabox);

  $form['bulk_update_query'] = array(
    '#type' => 'value',
    '#value' => 'x',
  );

  // for all buttons there is same redirect
  $form['actions']['submit']['#submit'][] = 'mediabox_library_form_bulk_update_submit';
  $form['actions']['delete']['#submit'][] = 'mediabox_library_form_bulk_update_submit';
  $form['actions']['cancel']['#submit'][] = 'mediabox_library_form_bulk_update_submit';

  // cancel all button will stop bulk update
  $form['actions']['cancel_all'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel all'),
    '#submit' => array('mediabox_library_form_bulk_update_cancel'),
    '#weight' => 10,
  );

  return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function mediabox_library_form_bulk_update_submit($form, &$form_state) {
  $params = drupal_get_query_parameters();

  $ids = array();
  if (isset($params['ids'])) {
    $ids = $params['ids'];
  }

  if (sizeof($ids)) {
    $id = array_shift($ids);
    $form_state['redirect'] = array(
      'admin/structure/mediabox-library/update/' . $id, array(
        'query' => array(
          'ids' => $ids,
        )
      )
    );
  }
  else {
    $form_state['redirect'] = 'admin/structure/mediabox-library';
  }
}

/**
 * Cancel whole bulk update
 *
 * @param $form
 * @param $form_state
 */
function mediabox_library_form_bulk_update_cancel($form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/mediabox-library';
}
