<?php

/**
 * @file
 * Mediabox display fields class.
 */

/**
 * Implements hook_field_info().
 */
function mediabox_ui_field_info() {
  return array(
    'mediabox' => array(
      'label' => t('Mediabox'),
      'description' => t('This field stores a reference to a mediabox files.'),
      'settings' => array(
        'mediabox_allowed_extensions' => 'gif jpg jpeg png',
        'mediabox_bundle',
        'mediabox_display_bundle' => 'display',
        'mediabox_image_styles_to_crop',
        'mediabox_default_style' => '',
        'mediabox_default_crop_style' => '',
        'mediabox_lock_aspect_ratio' => 0,
        'mediabox_show_inline_placeholder_help' => 1,
        'mediabox_show_image_info' => 1,
        'mediabox_show_additional_fields' => 1,
        'mediabox_better_ui' => 0,
        'mediabox_better_ui_settings' => array(
          'item_in_popup' => FALSE,
          'popup_title' => t('Edit image'),
        ),
        'mediabox_texts' => array(
          'add_button' => t('Add new items'),
          'delete_button' => t('Delete selected item'),
          'restore_button' => t('Restore selected item'),
          'browse_button' => t('Browse all items'),
          'upload_button' => t('Upload new items'),
          'add_image_text' => t('Add image'),
          'add_images_text' => t('Add images'),
        ),
      ),
      'default_widget' => 'mediabox_default_widget',
      'default_formatter' => 'mediabox_default_formatter',
    ),
  );
}

/**
 * Implements hook_field_settings_form().
 */
function mediabox_ui_field_settings_form($field, $instance, $has_data) {
  $settings = $field['settings'];
  $form = array();

  switch ($field['type']) {
    case 'mediabox':
      $bundle_options = mediabox_db_get_bundle_options();
      $display_bundle_options = mediabox_ui_db_get_bundle_options();

      $options = array_keys($bundle_options);
      $options = reset($options);

      $form['mediabox_allowed_extensions'] = array(
        '#type' => 'textfield',
        '#title' => t('Allowed extensions'),
        '#default_value' => $settings['mediabox_allowed_extensions'],
        '#required' => TRUE,
      );

      // @todo - note that it seems we are not using this info anywhere, and
      // we should use it for 'image info elements' in
      // mediabox_element_process() function, right?
      // Also default value is hardcoded here.
      $form['mediabox_bundle'] = array(
        '#type' => 'select',
        '#description' => t('Select mediabox bundle for this field.'),
        '#title' => t('Mediabox bundle'),
        '#default_value' => $options,
        '#options' => $bundle_options,
        '#required' => TRUE,
      );

      $form['mediabox_display_bundle'] = array(
        '#type' => 'select',
        '#title' => t('Mediabox display bundle'),
        '#description' => t('Select mediabox display bundle for this field.'),
        '#default_value' => $settings['mediabox_display_bundle'],
        '#options' => $display_bundle_options,
        '#required' => TRUE,
      );

      $image_styles = mediabox_get_image_styles_options();

      $form['mediabox_image_styles_to_crop'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Select image styles'),
        '#default_value' => !empty($settings['mediabox_image_styles_to_crop']) ? $settings['mediabox_image_styles_to_crop'] : array(),
        '#options' => $image_styles,
        '#required' => TRUE,
      );

      $form['mediabox_default_crop_style'] = array(
        '#type' => 'select',
        '#description' => t('Select default image style for "Crop from image style".'),
        '#title' => t('Crop from image style'),
        '#default_value' => !empty($settings['mediabox_default_crop_style']) ? $settings['mediabox_default_crop_style'] : $options,
        '#options' => $image_styles,
        '#required' => TRUE,
        '#element_validate' => array('mediabox_default_style_element_validate'),
      );

      $form['mediabox_default_style'] = array(
        '#type' => 'select',
        '#description' => t('Select default image style.'),
        '#title' => t('Default image style'),
        '#default_value' => !empty($settings['mediabox_default_style']) ? $settings['mediabox_default_style'] : $options,
        '#options' => $image_styles,
        '#required' => TRUE,
        '#element_validate' => array('mediabox_default_style_element_validate'),
      );

      $form['mediabox_lock_aspect_ratio'] = array(
        '#type' => 'checkbox',
        '#title' => t('Lock aspect ratio'),
        '#default_value' => $settings['mediabox_lock_aspect_ratio'],
      );

      $form['mediabox_show_inline_placeholder_help'] = array(
        '#type' => 'checkbox',
        '#title' => t('Show inline placeholder help text'),
        '#default_value' => $settings['mediabox_show_inline_placeholder_help'],
      );

      $form['mediabox_show_image_info'] = array(
        '#type' => 'checkbox',
        '#title' => t('Show image info form elements'),
        '#default_value' => $settings['mediabox_show_image_info'],
      );

      $form['mediabox_show_additional_fields'] = array(
        '#type' => 'checkbox',
        '#title' => t('Show additional fields'),
        '#default_value' => $settings['mediabox_show_additional_fields'],
      );

      $form['mediabox_better_ui'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use better UI'),
        '#description' => t('Mediabox field UI improvements for much better end user experience.'),
        '#default_value' => $settings['mediabox_better_ui'],
      );

      $form['mediabox_better_ui_settings'] = array(
        '#type' => 'fieldset',
        '#title' => t('Better UI Settings'),
        '#states' => array(
          'invisible' => array(
           ':input[name="field[settings][mediabox_better_ui]"]' => array('checked' => FALSE),
          ),
        ),
      );
      $form['mediabox_better_ui_settings']['item_in_popup'] = array(
        '#type' => 'checkbox',
        '#title' => t('Popup item on thumb double click'),
        '#description' => t('Check if you want to enable double click on thumbnails which will show item edit in popup.'),
        '#default_value' => $settings['mediabox_better_ui_settings']['item_in_popup'],
      );
      $form['mediabox_better_ui_settings']['popup_title'] = array(
        '#type' => 'textfield',
        '#title' => t('Popup title'),
        '#default_value' => $settings['mediabox_better_ui_settings']['popup_title'],
        '#states' => array(
          'invisible' => array(
           ':input[name="field[settings][mediabox_better_ui_settings][item_in_popup]"]' => array('checked' => FALSE),
          ),
        ),
      );

      $form['mediabox_texts'] = array(
        '#type' => 'fieldset',
        '#title' => t('Labels and help texts'),
      );
      $form['mediabox_texts']['add_button'] = array(
        '#type' => 'textfield',
        '#title' => t('Add items button text'),
        '#description' => t('Open dialog'),
        '#default_value' => $settings['mediabox_texts']['add_button'],
      );
      $form['mediabox_texts']['delete_button'] = array(
        '#type' => 'textfield',
        '#title' => t('Delete selected item button text'),
        '#description' => t('Mark delete'),
        '#default_value' => $settings['mediabox_texts']['delete_button'],
      );
      $form['mediabox_texts']['restore_button'] = array(
        '#type' => 'textfield',
        '#title' => t('Restore selected item button text'),
        '#description' => t('Unmark delete'),
        '#default_value' => $settings['mediabox_texts']['restore_button'],
      );
      $form['mediabox_texts']['browse_button'] = array(
        '#type' => 'textfield',
        '#title' => t('Browse button text'),
        '#description' => t('Dialog menu item'),
        '#default_value' => $settings['mediabox_texts']['browse_button'],
      );
      $form['mediabox_texts']['upload_button'] = array(
        '#type' => 'textfield',
        '#title' => t('Upload button text'),
        '#description' => t('Dialog menu item'),
        '#default_value' => $settings['mediabox_texts']['upload_button'],
      );
      $form['mediabox_texts']['add_image_text'] = array(
        '#type' => 'textfield',
        '#title' => t('Add image button text'),
        '#description' => t('Dialog browse page - single selection'),
        '#default_value' => $settings['mediabox_texts']['add_image_text'],
      );
      $form['mediabox_texts']['add_images_text'] = array(
        '#type' => 'textfield',
        '#title' => t('Add images button text'),
        '#description' => t('Dialog browse page - multiple selection'),
        '#default_value' => $settings['mediabox_texts']['add_images_text'],
      );

      break;
  }

  return $form;
}

/**
 * Default style form element validate callback.
 */
function mediabox_default_style_element_validate($element, &$form_state) {
  $mediabox_image_styles_to_crop = $form_state['values']['field']['settings']['mediabox_image_styles_to_crop'];
  if (!in_array($element['#value'], $mediabox_image_styles_to_crop, TRUE)) {
    form_error($element, t('@title is not in selected image styles.', array('@title' => $element['#title'])));
  }
}

/**
 * Implements hook_field_is_empty().
 */
function mediabox_ui_field_is_empty($item, $field) {
  // We are storing some helper values in items (like mid_elements string) and
  // we want to remove this stuff because it can be problematic later in
  // mediabox_ui_field_presave().
  if (!is_array($item)) {
    return TRUE;
  }

  if (!empty($item['_remove']) || empty($item['mid'])) {
    // If we have mediabox display lets delete it.
    if (!empty($item['display_mid'])) {
      if ($mediabox_display = mediabox_display_load($item['display_mid'])) {
        mediabox_display_delete($mediabox_display);
      }
    }
    return TRUE;
  }

  return FALSE;
}

/**
 * Implements hook_field_widget_info().
 */
function mediabox_ui_field_widget_info() {
  return array(
    'mediabox_default_widget' => array(
      'label' => t('Mediabox selector'),
      'field types' => array('mediabox'),
      'behaviors' => array(
        // We need to override default behaviour for adding new items - FIELD_BEHAVIOUR_CUSTOM
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_CUSTOM,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function mediabox_ui_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  switch ($instance['widget']['type']) {
    case "mediabox_default_widget":

      $field_elements = array();

      // empty settings for cropping -- needed for default initialization ? not sure why
      drupal_add_js(array('crop' => array('key' => 'null')), 'setting');

      // Attach javascript and styles to field and not to form. Why? Because we
      // can have multiple mediabox fields in form and in that situation if some
      // mediabox fields have mediabox_better_ui and some other not then fields
      // will start to overwrite each other attached stuff if we use form.
      $field_elements['#attached'] = array(
        'library' => array(array('mediabox_ui', 'jquery.fancybox'), array('mediabox_crop', 'Jcrop')),
        'js' => array(
          drupal_get_path('module', 'mediabox_ui') . '/js/mediabox_ui.js',
          drupal_get_path('module', 'mediabox_crop') . '/js/mediabox_crop.js'
        ),
        'css' => array(
          drupal_get_path('module', 'mediabox_ui') . '/css/mediabox_ui.css',
        ),
      );
      if ($field['settings']['mediabox_better_ui']) {
        $field_elements['#attached']['library'][] = array('system', 'ui.draggable');
        $field_elements['#attached']['library'][] = array('system', 'ui.sortable');
        $field_elements['#attached']['js'][] = array(
          'data' => array('mediabox_ui' => array('fields' => array(str_replace('_', '-', $field['field_name']) => array(
            'fieldCardinality' => $field['cardinality'],
            'deleteItemText' => check_plain($field['settings']['mediabox_texts']['delete_button']),
            'restoreItemText' => check_plain($field['settings']['mediabox_texts']['restore_button']),
            'itemInPopup' => $field['settings']['mediabox_better_ui_settings']['item_in_popup'],
            'popupTitle' => $field['settings']['mediabox_better_ui_settings']['popup_title'],
          )))),
          'type' => 'setting',
        );
        $field_elements['#attached']['js'][] = drupal_get_path('module', 'mediabox_ui') . '/js/mediabox_ui_better_ui.js';
      }

      $field_name = $field['field_name'];
      $parents = $form['#parents'];

      // Get DELTA count
      $field_state = field_form_get_state($parents, $field_name, $langcode, $form_state);
      $max = $field_state['items_count'];

      $title = check_plain($instance['label']);
      $description = field_filter_xss($instance['description']);
      $multiple = $field['cardinality'] > 1 || $field['cardinality'] == FIELD_CARDINALITY_UNLIMITED;

      $id_prefix = implode('-', array_merge($parents, array($field_name)));
      $wrapper_id = drupal_html_id($id_prefix . '-add-more-wrapper');

      // Cycling deltas.
      for ($delta = 0; $delta <= $max; $delta++) {
        if ($max > 0 && $delta == $max) {
          continue;
        }

        $element_wrapper_id = drupal_html_id($id_prefix . '-element-' . $delta);
        if (isset($form_state['values'][$field_name][$langcode][$delta])) {
          $items[$delta] = $form_state['values'][$field_name][$langcode][$delta];
        }
        $items[$delta]['element_wrapper_id'] = $element_wrapper_id;

        $element = array(
          '#entity_type' => $instance['entity_type'],
          '#bundle' => $instance['bundle'],
          '#field_name' => $field_name,
          '#language' => $langcode,
          '#field_parents' => $parents,
          '#columns' => array_keys($field['columns']),
          // For multiple fields, title and description are handled by the wrapping table.
          '#title' => $multiple ? '' : $title,
          '#description' => $multiple ? '' : $description,
          // Only the first widget should be required.
          '#required' => $delta == 0 && $instance['required'],
          '#delta' => $delta,
          '#weight' => $delta,
          '#prefix' => '<div id="' . $element_wrapper_id . '" class="mediabox-element">',
          '#suffix' => "</div>",
          '#item' => isset($items[$delta]) ? $items[$delta] : array(),
          '#theme' => 'mediabox_ui_element',
          '#type' => 'mediabox',
          '#mediabox_options' => $field['settings'],
        );

        // adding _remove, this is used to mark media files for removal
        // it is not possible(not in a nice way - using form ajax) to remove medias on the run.
        // we are just marking medias for removal - actual removal is done in hook_entity_presave
        // position if this element is done in mediabox_multiple_value_form theme - neat!
        $element['_remove'] = array(
          '#type' => 'checkbox',
          '#attributes' => array('class' => array('mediabox-remove-item-checkbox')),
        );

        // adding _weight, this is used for sorting in 'mediabox_multiple_value_form'
        $element['_weight'] = array(
          '#type' => 'weight',
          '#title' => t('Weight for row @number', array('@number' => $delta + 1)),
          '#title_display' => 'invisible',
          '#delta' => $max,
          '#attributes' => array('style' => 'display:none'),
          '#default_value' => isset($items[$delta]['_weight']) ? $items[$delta]['_weight'] : $delta,
        );

        $field_elements[$delta] = $element;
      }

      $field_elements += array(
        '#theme' => 'mediabox_multiple_value_form',
        '#field_name' => $field['field_name'],
        '#cardinality' => $field['cardinality'],
        '#title' => $title,
        '#required' => $instance['required'],
        '#description' => $description,
        '#prefix' => '<div id="' . $wrapper_id . '" class="mediabox-form-wrapper">',
        '#suffix' => '</div>',
        '#max_delta' => $max,
      );

      // Browse button.
      $field_id = strtr($id_prefix, '-', '_');
      $field_elements['browse'] = array(
        '#type' => 'markup',
        '#markup' => l(
          $field['settings']['mediabox_texts']['add_button'],
          'mediabox-browser/' . $field_id,
          array(
            'attributes' => array(
              // Link (looks like button) for fancyBox browser.
              'class' => array('mediabox-browser-modal', 'button', 'btn', 'overlay-exclude'),
            ),
          )
        ),
      );

      // Add more button.
      $add_more_id = strtr($id_prefix, '-', '_') . '_add_more';
      drupal_add_js(array('mediabox' => array('add_more_id' => $add_more_id)), 'setting');
      $field_elements['add_more'] = array(
        '#type' => 'submit',
        '#name' => $add_more_id,
        '#id' => $add_more_id,
        '#value' => t('Add another item'),
        '#attributes' => array('class' => array('field-add-more-submit')),
        '#limit_validation_errors' => array(array_merge($parents, array($field_name, $langcode))),
        '#submit' => array('mediabox_ui_add_more_submit'),
        '#attributes' => array(
          'class' => array('mediabox-add-more-button'),
        ),
        '#ajax' => array(
          'callback' => 'mediabox_ui_add_more_js',
          'wrapper' => $wrapper_id,
          'effect' => 'fade',
        ),
      );

      // Add field element for storing data string of mediabox ids we want to
      // add (1,2,3,4, ...).
      $mid_elements_id = strtr($id_prefix, '-', '_') . '_mid_elements';
      $field_elements['mid_elements'] = array(
        '#type' => 'textfield',
        '#id' => $mid_elements_id,
        '#prefix' => '<div class="hidden" style="display:none">',
        '#suffix' => '</div>',
      );

      return $field_elements;
      break;
  }
}

/**
 * Submit callback, this is used even if ajax callback is defined.
 * Creating as many elements as needed, mostly important we are setting
 * items_count.
 *
 * @param $form
 * @param $form_state
 */
function mediabox_ui_add_more_submit($form, &$form_state) {

  // Element that triggered this action.
  $button = $form_state['triggering_element'];
  // Go one level up in the form, to the widgets container.
  $element = drupal_array_get_nested_value($form, array_slice($button['#array_parents'], 0, -1));

  $language = $element['#language'];
  $parents = $element['#field_parents'];
  $field_name = $element['#field_name'];

  // New items are stored in mid_elements like a comma separated string of mids.
  $mid_elements = $form_state['values'][$field_name][$language]['mid_elements'];

  // Get field state.
  $field_state = field_form_get_state($parents, $field_name, $language, $form_state);
  if (!isset($field_state['items_count'])) {
    $field_state['items_count'] = 0;
  }

  // If we have new selected items in browser lets process them.
  if (!empty($mid_elements)) {
    $media_ids = explode(',', trim($mid_elements, ','));

    // We need to calculate how much left space we have in this field and if
    // needed remove new mediabox items that can not be added because of
    // cardinality limit.
    $field_info = field_info_field($field_name);
    if ($field_info['cardinality'] == 1) {
      $id = $media_ids[0];
      // Empty $media_ids because of count check below
      // we don't want to add one more to count.
      $media_ids = array();
      // By unsetting $form_state for image with cardinality 1 we simulate
      // creation of new mediabox entity which will follow later:
      // see mediabox_element_process().
      unset($form_state['values'][$field_name][$language][0]);
      $form_state['values'][$field_name][$language][0]['mid'] = $id;
    }
    else {
      if ($field_info['cardinality'] != FIELD_CARDINALITY_UNLIMITED) {
        $remaining = $field_info['cardinality'] - $field_state['items_count'];
        if (count($media_ids) > $remaining) {
          array_splice($media_ids, $remaining);
        }
      }
      // Process cardinality unlimited or all fixed except 1.
      foreach ($media_ids as $delta => $id) {
        // Set mediabox #ID to as many elements as needed.
        // All required fields will be rendered afterwards.
        $next_item = $field_state['items_count'] + $delta;
        $form_state['values'][$field_name][$language][$next_item]['mid'] = $id;
      }
    }
      // Raise the items count.
      $field_state['items_count'] += count($media_ids);

      // Set field state with updated items_count.
      field_form_set_state($parents, $field_name, $language, $form_state, $field_state);    
    $form_state['rebuild'] = TRUE;
  }

}

/**
 * Ajax callback for replacing element.
 *
 * @param $form
 * @param $form_state
 * @return array|null
 */
function mediabox_ui_add_more_js($form, $form_state) {
  // Element that triggered this action.
  $button = $form_state['triggering_element'];
  $element = drupal_array_get_nested_value($form, array_slice($button['#array_parents'], 0, -1));

  // @todo - what is the purpose of this code, commenting it for now.
  //$field_name = $element['#field_name'];
  //$langcode = $element['#language'];
  //
  //$parents = $element['#field_parents'];
  //
  //$field_state = field_form_get_state($parents, $field_name, $langcode, $form_state);
  //$field = $field_state['field'];

  return $element;
}

/**
 * Implements hook_field_presave().
 *
 * We are creating new entity of mediabox display.
 * According to field settings we need to set mediabox display bundle.
 *
 * Copying mediabox entity into new mediabox display entity.
 * This provides us with total independence between backend of mediabox
 * (mediabox entity) and fronted of mediabox ( mediabox display ).
 *
 */
function mediabox_ui_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  // If items do not have entity key defined this means that this presave is
  // not comming from entity form submit or any other save where we need to
  // handle saving of mediabox display field so lets just return.
  if (empty($items[0]) || empty($items[0]['entity'])) {
    return;
  }

  // Get all instances for our display bundle.
  // @todo we are setting only for default bundle yet.
  $bundle_instances = field_info_instances('mediabox_display');

  // @todo Add support for private files.
  $crop_path = 'public://' . variable_get('mediabox_ui_filepath', 'crops') . '/';

  $mediabox_field = $field['field_name'];

  // Loop through each items in our field instance.
  $controller = entity_get_controller('mediabox_display');

  // Loop through all fields which where attached to element and add them to
  // our entity, assigning all fields to our entity.
  foreach ($items as &$item) {

    // Load old and store new crop.
    $mediabox_display = $item['entity']['mediabox_display'];
    $old_crop = mediabox_crop_load($mediabox_display->crop_id);
    $crop = new MediaboxCrop();
    $crop->populateFromArray($item['styles']['set']);
    
    // Compare them for changes, if any we should save it.
    // If old_crop is empty that means image is different than one before.
    if ($old_crop->x != $crop->x || $old_crop->y != $crop->y ||
        $old_crop->x1 != $crop->x1 || $old_crop->y1 != $crop->y1) { 
    
      $crop->ratio = $item['image_info']['ratio'];
      $crop->origin_image_style = $item['image_info']['origin_image_style'];
      $crop->id = NULL;
      mediabox_crop_db_save($crop);

      $mediabox_display->crop_id = $crop->id;
      // Loop through all fields which where attached to element and add them
      // to our entity, assigning all fields to our entity.
      $mediabox_display->title = $item['image_info']['title'];
      $mediabox_display->alternate_text = $item['image_info']['alternate_text'];
      $mediabox_display->style_name = $item['image_info']['image_style'];

      // Load original file.
      $file = file_load($item['fid']);
      // Duplicate our image so we can perform crop later.
      $realpath = drupal_realpath($file->uri);

      // We are adding $crop->id . '-' prefix here so we can have multiple
      // items that comes from original file. Adding crop id as a prefix will
      // guaranty unique file name in this situation.
      $new_file_name = $crop_path . $crop->id . '-' . $file->filename;

      if (!empty($mediabox_display->mediabox_image[$langcode][0]['fid'])) {
        // There was already a file cropped before.
        // We are not creating new file, but replacing the old one,
        // flagging FILE_EXISTS_REPLACE.
        $new_file = file_save_data(file_get_contents($realpath), $new_file_name, FILE_EXISTS_REPLACE);
      }
      else {
        // This is our first crop.
        // If file of same name exists (probably belongs to other entity)
        // find new name.
        $new_file = file_save_data(file_get_contents($realpath), $new_file_name, FILE_EXISTS_RENAME);
      }

      $controller->save($mediabox_display);

      // @todo - erik why we are using here this foreach with bundle_instances?
      // Can't we just filter current field without foreach?
      foreach ($bundle_instances[$mediabox_display->type] as $delta => $bundle_instance) {
        // Field might be empty and thus filtered out by field api.
        $field_value = $item['add_fields'][$delta];
        if (isset($field_value)) {
          unset($field_value[$langcode]['add_more']);

          $field_value[$langcode] = _field_filter_items(field_info_field($delta), $field_value[$langcode]);
          $mediabox_display->$delta = $field_value;
        }
      }

      $image = image_load($new_file->uri);
      $style = image_style_load($crop->origin_image_style);

      if (sizeof($style['effects'])) {
        // Resize the image to 'origin' style value.
        foreach ($style['effects'] as $effect) {
          image_effect_apply($image, $effect);
        }
      }

      // Perform crop on image and save it.
      image_crop($image, $crop->x, $crop->y, $crop->width, $crop->height);
      image_save($image);

      $mediabox_display->crop_id = $crop->id;
      $file_item = array(
        'fid' => $new_file->fid,
        'alt' => $mediabox_display->alternate_text,
        'title' => $mediabox_display->title,
        'width' => $crop->width,
        'height' => $crop->height,
      );

      file_save($new_file);

      // Flush old styles.
      image_path_flush($new_file->uri);

      $mediabox_display->mediabox_image[$langcode][0] = $file_item;
      $controller->save($mediabox_display);

      // Save newly created mediabox display id in item.
      $item['display_mid'] = $mediabox_display->id;
    }
  }
  // We have or mediabox display created/updated, now lets get to the
  // 'text-inline' functionality and replace all temporary mediabox:delta tokens
  // to proper mediabox:id:mid tokens.
  // @todo - we need to improve this and support cardinality > 1 and also other
  // fields (all with proper text filter) and not just body field.
  if ($entity_type == 'node') {
    if (isset($entity->body[$entity->language][0])) {
      $body = $entity->body[$entity->language][0]['value'];

      preg_match_all('/mediabox:[0-9]+/', $body, $matches);

      foreach (array_shift($matches) as $match) {
        $parts = explode(':', $match);
        $delta = $parts[1];

        if (isset($entity->{$mediabox_field}[$entity->language][$delta])) {
          $mid = $entity->{$mediabox_field}[$entity->language][$delta]['display_mid'];
          // Replace delta with correct mid.
          $body = str_replace($match, 'mediabox:id:' . $mid, $body);
        }
      }

      $entity->body[$entity->language][0]['value'] = $body;
    }
  }
}

function mediabox_ui_db_save_coords($data) {
  $result = FALSE;

  $data['entity_id'] = 0;
  $data['created'] = REQUEST_TIME;
  $data['modified'] = REQUEST_TIME;

  if (!empty($data['id'])) {
    $result = drupal_write_record('mediabox_crops', $data, array('id'));
  }
  else {
    $result = drupal_write_record('mediabox_crops', $data);
  }

  return $data['id'];
}

/**
 * Implements hook_field_formatter_info().
 */
function mediabox_ui_field_formatter_info() {

  $formatters = array(
    'mediabox_default_formatter' => array(
      'label' => t('Entity display'),
      'field types' => array('mediabox'),
      'settings' => array(
        'mediabox_display' => 'default',
      ),
    ),
    'mediabox_image' => array(
      'label' => t('Mediabox default formatter'),
      'field types' => array('image'),
    ),
    'mediabox_views' => array(
      'label' => t('Views display'),
      'field types' => array('mediabox'),
      'settings' => array(
        'mediabox_view_name' => NULL,
        'mediabox_view_display' => 'default',
        'mediabox_view_arguments' => 'mediabox_display_ids',
      ),
    )
  );

  return $formatters;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function mediabox_ui_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {

  $display = $instance['display'][$view_mode];

  $settings = $display['settings'];
  $element = array();
  $entity_info = entity_get_info('mediabox_display');

  foreach ($entity_info['view modes'] as $delta => $view_mode) {
    $options[$delta] = $view_mode['label'];
  }

  switch ($display['type']) {

    case 'mediabox_default_formatter':
      $element['mediabox_display'] = array(
        '#title' => t('Display'),
        '#type' => 'select',
        '#default_value' => $settings['type'],
        '#required' => TRUE,
        '#options' => $options,
      );
      break;
    case 'mediabox_views':
      // Get all mediabox_display views - do not use sql here but views api
      // functions so we get also views from code configurations.
      $views = views_get_enabled_views();
      $display_views = array();
      foreach ($views as $name => $view) {
        if ($view->base_table == 'mediabox_display') {
          $display_views[$name] = check_plain($view->human_name);
        }
      }

      $element['mediabox_view_name'] = array(
        '#title' => t('View'),
        '#type' => 'select',
        '#default_value' => $settings['mediabox_view_name'],
        '#required' => TRUE,
        '#options' => $display_views,
        '#ajax' => array(
          'callback' => 'mediabox_ui_ajax_reload_display',
          'wrapper' => 'field-display-overview-wrapper',
          'effect' => 'fade',
        )
      );

      $view_name = $settings['mediabox_view_name'];
      if (isset($form_state['values']['fields'][$field['field_name']]['settings_edit_form']['settings']['mediabox_view_name'])) {
        $view_name = $form_state['values']['fields'][$field['field_name']]['settings_edit_form']['settings']['mediabox_view_name'];
      }

      // Get displays of selected view.
      $display_view = views_get_view($view_name);
      $display_options = array();
      foreach ($display_view->display as $key => $display) {
        $display_options[$key] = $display->display_title;
      }

      if (count($display_options)) {
        $element['mediabox_view_display'] = array(
          '#title' => t('Display'),
          '#type' => 'select',
          '#default_value' => $settings['mediabox_view_display'],
          '#required' => TRUE,
          '#options' => $display_options,
        );
      }

      $element['mediabox_view_arguments'] = array(
        '#title' => t('Arguments'),
        '#type' => 'select',
        '#default_value' => $settings['mediabox_view_arguments'],
        '#options' => array(
          'mediabox_display_ids' => t('Mediabox display IDs'),
          'entity_id' => t('Entity ID'),
        ),
        '#required' => TRUE,
      );

      break;

  }

  return $element;
}

/**
 * @param $form
 * @param $form_state
 */
function mediabox_ui_ajax_reload_display(&$form, &$form_state) {
  $form_state['rebuild'];
  return $form['fields'];
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function mediabox_ui_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  switch ($display['type']) {
    case 'mediabox_default_formatter':
      $mediabox_display_delta = $settings['mediabox_display'];
      $entity_info = entity_get_info('mediabox_display');
      $mediabox_display = $entity_info['view modes'][$mediabox_display_delta]['label'];

      return t('Display mode: @display.', array('@display' => $mediabox_display));
      break;
    case 'mediabox_views':
      if ($settings['mediabox_view_arguments'] == 'mediabox_display_ids') {
        $msg = t('Formatter will pass IDs of mediabox entities. Make sure your view is using proper contextual filter!');
      }
      elseif ($settings['mediabox_view_arguments'] == 'entity_id') {
        $msg = t('Formatter will pass entity ID of mediabox field. Make sure your view is using proper contextual filter!');
      }
      $output = '<div style="color: red; font-weight: bold;">' . $msg . '</div>';
      $output .= t("<strong>View name:</strong> %name", array('%name' => $settings['mediabox_view_name'])) . '<br />';
      $output .= t("<strong>Display name:</strong> %name", array('%name' => $settings['mediabox_view_display']));

      return $output;
      break;
  }
}

/**
 * Implements hook_field_formatter_view().
 */
function mediabox_ui_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, &$items, $display) {
  $formatter = $display['type'];
  $element = array();

  switch ($formatter) {
    case 'mediabox_default_formatter':
      // Default formatter for displaying whole entity as attached field.
      foreach ($items as $delta => $item) {
        $mediabox_display = entity_load('mediabox_display', array($item['display_mid']));
        $view = mediabox_display_view(
          $mediabox_display,
          $display['settings']['mediabox_display']
        );

        $element[$delta]['#markup'] = drupal_render($view);
      }
      break;

    case 'mediabox_image':
      // Formatter for displaying image.
      foreach ($items as $delta => $item) {
        if (!empty($entity->style_name)) {
          $element[$delta]['#markup'] = theme('image_style',
            array(
              'style_name' => $entity->style_name,
              'path' => $item['uri'],
              'attributes' => array(),
            ) + $item
          );
        }
        else {
          $element[$delta]['#markup'] = theme('image',
            array(
              'path' => $item['uri'],
              'attributes' => array(),
            ) + $item
          );
        }
      }
      break;

    case 'mediabox_views':
      $view_name = $display['settings']['mediabox_view_name'];
      $view_display = $display['settings']['mediabox_view_display'];

      switch ($display['settings']['mediabox_view_arguments']) {
        case 'mediabox_display_ids':
          $ids = array();
          foreach ($items as $delta => $item) {
            $ids[$delta] = $item['display_mid'];
          }
          $args[] = implode('+', $ids);
          break;
        case 'entity_id':
          $args[] = entity_id($entity_type, $entity);
          break;
      }

      $formatter_view = views_get_view($view_name);
      $element[]['#markup'] = $formatter_view->preview($view_display, $args);
      break;

  }

  return $element;
}

/**
 * View mediabox_display entity.
 *
 * @param $mediabox_display
 * @param string $view_mode
 * @param string $langcode
 * @return bool|string
 */
function mediabox_display_view($mediabox_displays, $view_mode = "display", $langcode = NULL) {

  if (!is_array($mediabox_displays)) {
    $mediabox_display = $mediabox_displays;
    $mediabox_displays = array($mediabox_display->id => $mediabox_display);
  }

  foreach ($mediabox_displays as $mediabox_display) {
    $mediabox_display->content = array();

    if (!isset($langcode)) {
      $langcode = $GLOBALS['language_content']->language;
    }
    field_attach_prepare_view(
      'mediabox_display',
      array($mediabox_display->id => $mediabox_display),
      $view_mode,
      $langcode
    );
    entity_prepare_view('mediabox_display', array($mediabox_display->id => $mediabox_display));

    $mediabox_display->content += field_attach_view('mediabox_display', $mediabox_display, $view_mode);

    $mediabox_display->content['title'] = array(
      '#markup' => '<div class="mediabox-title">' . $mediabox_display->title . '</div>',
    );

    $mediabox_display->content['alternate_text'] = array(
      '#markup' => '<div class="mediabox-alt-text">' . $mediabox_display->alternate_text . '</div>',
    );

    $build = $mediabox_display->content;
    unset($mediabox_display->content);

    $build += array(
      '#theme' => 'mediabox_display',
      '#mediabox_display' => $mediabox_display,
      '#view_mode' => $view_mode,
      '#language' => $langcode,
    );

    $key = isset($mediabox_display->id) ? $mediabox_display->id : NULL;
    $view['mediabox_display'][$key] = $build;
  }

  return $view;
}

function template_preprocess_mediabox_display(&$variables) {
  $variables['view_mode'] = $variables['elements']['#view_mode'];

  // Provide a distinct $teaser boolean.
  $variables['mediabox_display'] = $variables['elements']['#mediabox_display'];
  $mediabox_display = $variables['mediabox_display'];

  // Flatten the mediabox object's member fields.
  $variables = array_merge((array) $mediabox_display, $variables);

  // Helpful $content variable for templates.
  $variables += array('content' => array());
  foreach (element_children($variables['elements']) as $key) {
    $variables['content'][$key] = $variables['elements'][$key];
  }
  // Make the field variables available with the appropriate language.
  field_attach_preprocess('mediabox_display', $mediabox_display, $variables['content'], $variables);
}
