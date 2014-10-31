<?php
/**
 * @file
 * Add birth to the Solution Pack.
 */


/**
 * Birth Form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_livingresearchlab_add_birth_form(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  $form = array();
  $form['lrl_birth_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Birth Identifier'),
    '#description' => t('An unique ID for a birth event'),
    '#required' => TRUE,
    '#size' => 20,
  );
  $form['lrl_birth_cage_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Cage born ID'),
    '#description' => t('An unique ID for Breeding cage'),
    '#size' => 20,
  );
  $form['lrl_birth_mother'] = array(
    '#type' => 'textfield',
    '#title' => t('Cage born ID'),
    '#description' => t('An unique ID for Breeding cage'),
    '#size' => 20,
  );


  //drupal_set_message(t('print birth'));


  return system_settings_form($form);
}
