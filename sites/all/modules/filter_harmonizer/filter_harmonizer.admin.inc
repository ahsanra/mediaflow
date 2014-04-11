<?php
/**
 * @file
 * filter_harmonizer.admin.module
 *
 * Configuration form for global and advanced filter harmonizer settings.
 */

/**
 * Menu callback for admin settings.
 */
function filter_harmonizer_admin_config() {
  $form['filter_harmonizer_settings_global'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter Harmonizer global settings'),
  );
  $form['filter_harmonizer_settings_global']['filter_harmonizer_always'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ignore <strong>any</strong> <em>contextual</em> filter that also has an <em>exposed</em> filter value set for the same field'),
    '#default_value' => variable_get('filter_harmonizer_always'),
    '#description' => t("If checked, the equivalent per-field check box on the contextual and exposed filter configuration popups <em>won't</em> appear for any field."),
  );
  $form['filter_harmonizer_settings_global']['filter_harmonizer_fill_exposed'] = array(
    '#type' => 'checkbox',
    '#title' => t('When the exposed filter form is empty, auto-fill it with the contextual filter value applied to the page'),
    '#default_value' => variable_get('filter_harmonizer_fill_exposed', TRUE),
    '#description' => t('May not work on some multi-valued exposed filter forms.'),
  );

  $form['filter_harmonizer_settings_advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter Harmonizer advanced settings'),
  );
  $form['filter_harmonizer_settings_advanced']['filter_harmonizer_debug_users'] = array(
    '#type' => 'textfield',
    '#title' => t('Show informational messages during execution'),
    '#default_value' => variable_get('filter_harmonizer_debug_users', ''),
    '#description' => t('Enter a comma-separated list of names of users that should see info messages coming from this module, e.g., for debugging purposes. Use <strong>anon</strong> for the anonymous user.<br/>Debug messages are likely to show incorrect values on View displays with AJAX set to "Yes".'),
  );
  return system_settings_form($form);
}
