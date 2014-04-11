<?php

/**
 * @file
 * Field API integration for the file_entity module.
 */

/**
 * Implements hook_field_formatter_info().
 */
function file_entity_field_formatter_info() {
  $info['file_rendered'] = array(
    'label' => t('Rendered file'),
    'description' => t('Display the file in a specific view mode'),
    'field types' => array('file', 'image'),
    'settings' => array('file_view_mode' => 'default'),
  );

  $info['file_audio'] = array(
   'label' => t('Audio'),
    'description' => t('Render the file using an HTML5 audio tag'),
    'field types' => array('file'),
    'settings' => array(
      'controls' => TRUE,
      'autoplay' => FALSE,
      'loop' => FALSE,
      'multiple_file_behavior' => 'sources',
    ),
  );
  
  $info['file_video'] = array(
    'label' => t('Video'),
    'description' => t('Render the file using an HTML5 video tag'),
    'field types' => array('file'),
    'settings' => array(
     'controls' => TRUE,
      'autoplay' => FALSE,
      'loop' => FALSE,
     'muted' => FALSE,
      'size' => array(
        'width' => NULL,
        'height' => NULL,
      ),
      'multiple_file_behavior' => 'sources',
    ),
  );



  return $info;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function file_entity_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = array();

  if ($display['type'] == 'file_rendered') {
    $element['file_view_mode'] = array(
      '#title'   => t('View mode'),
      '#type'    => 'select',
      '#options' => file_entity_view_mode_labels(),
      '#default_value' => $settings['file_view_mode'],
      // Never empty, so no #empty_option
    );
  }

  elseif ($display['type'] == 'file_audio') {
    $element['controls'] = array(
      '#title' => t('Show audio controls'),
      '#type' => 'checkbox',
      '#default_value' => isset($settings['controls']) ? $settings['controls'] : TRUE,
    );
    $element['autoplay'] = array(
      '#title' => t('Autoplay'),
      '#type' => 'checkbox',
      '#default_value' => isset($settings['autoplay']) ? $settings['autoplay'] : FALSE,
    );
    $element['loop'] = array(
      '#title' => t('Loop'),
      '#type' => 'checkbox',
      '#default_value' => isset($settings['loop']) ? $settings['loop'] : FALSE,
    );
    $element['multiple_file_behavior'] = array(
      '#title' => t('Display of multiple files'),
      '#type' => 'radios',
      '#options' => array(
        'sources' => t('Use multiple sources within a single <audio> tag'),
        'tags' => t('Use multiple <audio> tags, each with a single source'),
      ),
      '#default_value' => isset($settings['multiple_file_behavior']) ? $settings['multiple_file_behavior'] : 'sources',
    );
 
  }
  elseif ($display['type'] == 'file_video') {
    $element['controls'] = array(
      '#title' => t('Show video controls'),
      '#type' => 'checkbox',
      '#default_value' => isset($settings['controls']) ? $settings['controls'] : TRUE,
    );
    $element['autoplay'] = array(
      '#title' => t('Autoplay'),
      '#type' => 'checkbox',
      '#default_value' => isset($settings['autoplay']) ? $settings['autoplay'] : FALSE,
    );
    $element['loop'] = array(
      '#title' => t('Loop'),
      '#type' => 'checkbox',
      '#default_value' => isset($settings['loop']) ? $settings['loop'] : FALSE,
    );
    $element['size'] = array(
      '#type' => 'item',
      '#title' => t('Size'),
      '#field_prefix' => '<div class="container-inline">',
      '#field_suffix' => '</div>',
      '#description' => t('The size expressed as WIDTHxHEIGHT (e.g. 640x480). Leave blank to use the video file to determine the size of the player.'),
    );
    $element['size']['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#title_display' => 'invisible',
      '#default_value' => isset($settings['size']['width']) ? $settings['size']['width'] : NULL,
      '#size' => 5,
      '#maxlength' => 5,
      '#field_suffix' => ' x ',
    );
    $element['size']['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#title_display' => 'invisible',
      '#default_value' => isset($settings['size']['height']) ? $settings['size']['height'] : NULL,
      '#size' => 5,
      '#maxlength' => 5,
      '#field_suffix' => ' ' . t('pixels'),
    );
    $element['multiple_file_behavior'] = array(
      '#title' => t('Display of multiple files'),
      '#type' => 'radios',
      '#options' => array(
        'sources' => t('Use multiple sources within a single <video> tag'),
        'tags' => t('Use multiple <video> tags, each with a single source'),
      ),
      '#default_value' => isset($settings['multiple_file_behavior']) ? $settings['multiple_file_behavior'] : 'sources',
    );

  }
  //Hide 'multiple_file_behaviour' options when configuring as a file formatter.
  if (($display['type'] == 'file_audio' || $display['type'] == 'file_video') && empty($field)) {
    $element['multiple_file_behavior']['#access'] = FALSE;
  }


  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function file_entity_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $summary = NULL;

  if ($display['type'] === 'file_rendered') {
    $view_mode_label = file_entity_view_mode_label($settings['file_view_mode'], t('Unknown'));
    $summary = t('View mode: %mode', array('%mode' => $view_mode_label));
  }
  elseif ($display['type'] === 'file_audio') {
    $summary_items = array();
    if (isset($settings['multiple_file_behavior'])) {
      $summary_items[] = t('Multiple files: %multiple', array('%multiple' => $settings['multiple_file_behavior']));
    }
    if (isset($settings['controls'])) {
      $summary_items[] = t('Controls: %controls', array('%controls' => $settings['controls'] ? 'visible' : 'hidden'));
    }
    if (isset($settings['autoplay'])) {
      $summary_items[] = t('Autoplay: %autoplay', array('%autoplay' => $settings['autoplay'] ? t('yes') : t('no')));
    }
    if (isset($settings['loop'])) {
      $summary_items[] = t('Loop: %loop', array('%loop' => $settings['loop'] ? t('yes') : t('no')));
    }
    $summary = implode('<br />', $summary_items);

  }
  elseif ($display['type'] === 'file_video') {
    $summary_items = array();
    if (isset($settings['multiple_file_behavior'])) {
      $summary_items[] = t('Multiple files: %multiple', array('%multiple' => $settings['multiple_file_behavior']));
    }
    if (isset($settings['controls'])) {
      $summary_items[] = t('Controls: %controls', array('%controls' => $settings['controls'] ? 'visible' : 'hidden'));
    }
    if (isset($settings['autoplay'])) {
      $summary_items[] = t('Autoplay: %autoplay', array('%autoplay' => $settings['autoplay'] ? t('yes') : t('no')));
    }
    if (isset($settings['loop'])) {
      $summary_items[] = t('Loop: %loop', array('%loop' => $settings['loop'] ? t('yes') : t('no')));
    }
    if (isset($settings['muted'])) {
      $summary_items[] = t('Muted: %muted', array('%muted' => $settings['muted'] ? t('yes') : t('no')));
    }
    if ($settings['size']['width'] && $settings['size']['height']) {
      $summary_items[] = t('Size: %width x %height', array('%width' => $settings['size']['width'], '%height' => $settings['size']['height']));
    }
    $summary = implode('<br />', $summary_items);
  }



  return $summary;
}

/**
 * Implements hook_field_formatter_view().
 */
function file_entity_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $settings = $display['settings'];
  $element = array();

  if ($display['type'] == 'file_rendered') {
    $view_mode = $settings['file_view_mode'];

    // To prevent infinite recursion caused by reference cycles, we store
    // diplayed nodes in a recursion queue.
    $recursion_queue = &drupal_static(__FUNCTION__, array());

    // If no 'referencing entity' is set, we are starting a new 'reference
    // thread' and need to reset the queue.
    // @todo Bug: $entity->referencing_entity on files referenced in a different
    // thread on the page. E.g: 1 references 12 / 2 references 12 / visit homepage.
    // We'd need a more accurate way...
    if (!isset($entity->referencing_entity)) {
      $recursion_queue = array();
    }

    // The recursion queue only needs to track files.
    if ($entity_type == 'file') {
      list($id) = entity_extract_ids($entity_type, $entity);
      $recursion_queue[$id] = $id;
    }

    // Prevent 'empty' fields from causing a WSOD.
    $items = array_filter($items);

    // Check the recursion queue to determine which nodes should be fully
    // displayed, and which nodes will only be displayed as a title.
    $files_display = array();
    foreach ($items as $delta => $item) {
      if (!isset($recursion_queue[$item['fid']])) {
        $files_display[$item['fid']] = file_load($item['fid']);
      }
    }

    // Load and build the fully displayed nodes.
    if ($files_display) {
      foreach ($files_display as $fid => $file) {
        $files_display[$fid]->referencing_entity = $entity;
        $files_display[$fid]->referencing_field = $field['field_name'];
      }
      $files_built = file_view_multiple($files_display, $view_mode);
    }

    // Assemble the render array.
    foreach ($items as $delta => $item) {
      if (isset($files_built[$item['fid']])) {
        $element[$delta] = $files_built[$item['fid']];
      }
    }
  }

elseif ($display['type'] == 'file_audio') {
    $multiple_file_behavior = isset($settings['multiple_file_behavior']) ? $settings['multiple_file_behavior'] : 'sources';
    $controls = isset($settings['controls']) ? $settings['controls'] : TRUE;

    // Prevent 'empty' fields from causing a WSOD.
    $items = array_filter($items);

    // Build an array of sources for each <audio> element.
    $source_lists = array();
    if ($multiple_file_behavior == 'tags') {
      foreach ($items as $delta => $item) {
        if ($item['type'] == 'audio') {
          $source_lists[] = array($item);
        }
      }
    }
    else {
      foreach ($items as $delta => $item) {
        if ($item['type'] == 'audio') {
          $source_lists[0][] = $item;
        }
      }
    }

    // render each source list as an <audio> element
    foreach ($source_lists as $delta => $sources) {
      $element[$delta] = array(
        '#theme' => 'file_entity_file_audio',
        '#files' => $sources,
        '#controls' => $controls,
        '#autoplay' => isset($settings['autoplay']) ? $settings['autoplay'] : FALSE,
        '#loop' => isset($settings['loop']) ? $settings['loop'] : FALSE,
      );
    }
  }

  elseif ($display['type'] == 'file_video') {
    $multiple_file_behavior = isset($settings['multiple_file_behavior']) ? $settings['multiple_file_behavior'] : 'sources';

    // Prevent 'empty' fields from causing a WSOD.
    $items = array_filter($items);

    // Build an array of sources for each <video> element.
    $source_lists = array();
    if ($multiple_file_behavior == 'tags') {
      foreach ($items as $delta => $item) {
        if ($item['type'] == 'video') {
          $source_lists[] = array($item);
        }
      }
    }
    else {
      foreach ($items as $delta => $item) {
        if ($item['type'] == 'video') {
          $source_lists[0][] = $item;
        }
      }
    }

    // render each source list as an <video> element
    foreach ($source_lists as $delta => $sources) {
      $width = isset($settings['size']['width']) ? $settings['size']['width'] : NULL;
      $height = isset($settings['size']['height']) ? $settings['size']['height'] : NULL;
      $element[$delta] = array(
        '#theme' => 'file_entity_file_video',
        '#files' => $sources,
        '#controls' => isset($settings['controls']) ? $settings['controls'] : TRUE,
        '#autoplay' => isset($settings['autoplay']) ? $settings['autoplay'] : FALSE,
        '#loop' => isset($settings['loop']) ? $settings['loop'] : FALSE,
        '#muted' => isset($settings['muted']) ? $settings['muted'] : FALSE,
        '#width' => ($width && $height) ? $width : NULL,
        '#height' => ($width && $height) ? $height : NULL,
      );
    }

  }











  

  return $element;
}
