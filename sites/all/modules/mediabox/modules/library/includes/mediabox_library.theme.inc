<?php

/**
 * @file
 * Mediabox library theme.
 */

/**
 * Preprocess function for: mediabox-library-detail.tpl.php
 */
function template_preprocess_mediabox_library_detail(&$variables) {
  $mediabox = $variables['mediabox'];
  $file = file_load($mediabox->fid);

  $image_properties = array(
    'path' => $file->uri,
    'style_name' => 'mediabox_detail',
  );

  $image = theme('image_style', $image_properties);

  $variables['mediabox'] = $mediabox;
  $variables['image'] = $image;
  $variables['file'] = $file;
}

function theme_mediabox_library_list_form($variables) {
  $form = $variables['form'];
  $header = mediabox_library_list_header();

  // Data
  if (array_key_exists('items', $form) && is_array($form['items'])) {
    $items = array();

    foreach (element_children($form['items']) as $item) {
      if (is_array($form['items'][$item])) {
        $row = array(
          'data' => array(
            render($form['items'][$item]),
            $form['items'][$item]['#params']['image'],
            l($form['items'][$item]['#params']['title'], 'admin/library/update/' . $item),
            $form['items'][$item]['#params']['author'],
            $form['items'][$item]['#params']['type'],
            $form['items'][$item]['#params']['filetype'],
            $form['items'][$item]['#params']['filesize'],
            $form['items'][$item]['#params']['date'],
            $form['items'][$item]['#params']['actions'],
          )
        );

        $items[] = $row;
      }
    }
  }

  // Table
  $variables = array(
    'header' => $header,
    'rows' => isset($items) ? $items : array(),
    'empty' => t('No media files found.'),
    'attributes' => array(
      'class' => array('library'),
    ),
  );

  // Render
  $output = drupal_render_children($form);
  $output .= theme('table', $variables) . theme('pager', array('parameters' => array('limit' => MEDIABOX_PAGER_SIZE)));

  return $output;
}
