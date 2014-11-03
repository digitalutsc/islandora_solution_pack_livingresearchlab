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
  drupal_add_library('system', 'ui.datepicker');
  drupal_add_js("(function ($) { $('.datepicker').datepicker(); })(jQuery);", array('type' => 'inline', 'scope' => 'footer', 'weight' => 5));

  $form = array();

  // buttons
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $form['lrl_birth_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Birth Identifier'),
    '#description' => t('An unique ID for a birth event'),
    '#required' => TRUE,
    '#size' => 25,
  );
  $form['lrl_birth_cage_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Cage born ID'),
    '#description' => t('An unique ID for Breeding cage'),
    '#size' => 25,
  );
  $form['lrl_birth_mother'] = array(
    '#type' => 'textfield',
    '#title' => t('Mother'),
    '#autocomplete_path' => 'islandora/lrl_object/autocomplete/mother',
    '#size' => 25,
  );
  $form['lrl_birth_father'] = array(
    '#type' => 'textfield',
    '#title' => t('Father'),
    '#autocomplete_path' => 'islandora/lrl_object/autocomplete/father',
    '#size' => 25,
  );
  $form['lrl_birth_weandate'] = array(
    '#type' => 'textfield',
    '#title' => t('Wean Date'),
    '#attributes' => array(
      'class' => array('datepicker'),
    ),
    '#size' => 25,
  );
  $form['lrl_birth_dob'] = array(
    '#type' => 'textfield',
    '#title' => t('Date of Birth'),
    '#attributes' => array(
      'class' => array('datepicker'),
    ),
    '#size' => 25,
  );
  $form['lrl_birth_setupdate'] = array(
    '#type' => 'textfield',
    '#title' => t('Set Up Date'),
    '#attributes' => array(
      'class' => array('datepicker'),
    ),
    '#size' => 25,
  );
  // Children information fieldset
  $form['lrl_birth_children'] = array(
    '#type' => 'fieldset',
    '#title' => t('Children Information'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['lrl_birth_children']['first'] = array(
    '#type' => 'textfield',
    '#title' => t('Child #1'),
    '#required' => TRUE,
    '#size' => 25,
  );


  //dpm($form['actions']);

  return $form;
}

/**
 *
 * Implements hook_form_submit().
 *
 * @todo: complete islandora_livingresearchlab_add_birth_form_submit().
 */
function islandora_livingresearchlab_add_birth_form_submit(array $form, array &$form_state) {
  drupal_set_message(t('Mouse 1234 created with Birth Record 111.'));
}