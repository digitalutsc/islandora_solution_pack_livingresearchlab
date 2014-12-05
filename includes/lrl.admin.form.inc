<?php
/**
 * @file
 * lrl.admin.form.inc
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_livingresearchlab_admin() {
  $form = array();

  $form['description'] = array(
    '#type' => 'markup',
    '#markup' => '<div><h2>Mice ID Schema</h2><p><strong>S-X-IIII-NN</strong> where:</p><ul><li>S = 0 in-house bred, 1 for purchased</li>
<li>X = 0 for wild-type, 1 for transgenic</li><li>IIII = 4-digit litter/shipment ID number</li><li>NN = 2-digit mouse number (numbers are assigned
to each mouse in a litter/shipment)</li></ul></div>',
  );

  $form['islandora_livingresearchlab_mice_id_init_litter'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial litter #'),
    '#description' => t('Initial litter number used to auto-generate Mice ID IIII segment.'),
    '#default_value' => variable_get('islandora_livingresearchlab_mice_id_init_litter', '0001'),
    '#size' => 4,
  );
  $form['islandora_livingresearchlab_mice_id_init_shipment'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial shipment #'),
    '#description' => t('Initial shipment number used to auto-generate Mice ID IIII segment.'),
    '#default_value' => variable_get('islandora_livingresearchlab_mice_id_init_shipment', '0001'),
    '#size' => 4,
  );

  return system_settings_form($form);
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Add custom submit handler.
 */
function islandora_livingresearchlab_form_islandora_livingresearchlab_admin_alter(&$form, &$form_state, $form_id) {
  $form['#submit'][] = 'lrl_mice_id_table_set';
}

/**
 * Custom admin form submit handler.
 *
 * Update mice id table for init shipment or litter number.
 */
function lrl_mice_id_table_set($form, &$form_state) {
  $init_litter = $form_state['values']['islandora_livingresearchlab_mice_id_init_litter'];
  $init_shipment = $form_state['values']['islandora_livingresearchlab_mice_id_init_shipment'];
  db_update('islandora_lrl_mice')
    ->fields(array('highest_no' => sprintf('%04s', $init_shipment - 1)))
    ->condition('type', 'shipment')
    ->execute();
  db_update('islandora_lrl_mice')
    ->fields(array('highest_no' => sprintf('%04s', $init_litter - 1)))
    ->condition('type', 'litter')
    ->execute();
}
