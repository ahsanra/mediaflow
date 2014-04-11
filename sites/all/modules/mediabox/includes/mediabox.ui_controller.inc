<?php

/**
 * @file
 * Mediabox entity UI controller.
 */

class MediaboxTypeUIController extends EntityDefaultUIController {

  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage Mediabox types, including fields.';

    return $items;
  }
  
}

/**
 *
 * @param $form
 * @param $form_state
 * @param MediaboxType $bundle
 * @return array
 */
function mediabox_type_form($form, &$form_state, MediaboxType $bundle = NULL) {
  $form = array();

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => isset($bundle->id) ? $bundle->id : '',
  );

  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#description' => t('The human-readable name of this content type. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'),
    '#default_value' => isset($bundle->title) ? $bundle->title : '',
  );

  $form['type'] = array(
    '#title' => t('Type'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#description' => t('The machine-readable name of this content type. It is recommended that this code contain only letters, numbers, and underscores. This name must be unique.'),
    '#default_value' => isset($bundle->type) ? $bundle->type : '',
  );

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#description' => t('Describe this media type. The text will be displayed on the Add new media.'),
    '#default_value' => isset($bundle->description) ? $bundle->description : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save media type'),
    '#submit' => array('mediabox_type_form_submit'),
  );

  return $form;
}

function mediabox_type_form_submit($form, &$form_state) {
  $model_type = entity_ui_form_submit_build_entity($form, $form_state);
  $model_type->save();

  $form_state['redirect'] = 'admin/structure/mediabox';
}
