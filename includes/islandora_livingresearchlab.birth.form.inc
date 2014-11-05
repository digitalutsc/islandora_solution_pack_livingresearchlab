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
  module_load_include('inc', 'islandora_livingresearchlab', 'includes/utilities');
  drupal_add_library('system', 'ui.datepicker');
  drupal_add_js("(function ($) { $('.datepicker').datepicker(); })(jQuery);", array('type' => 'inline', 'scope' => 'footer', 'weight' => 5));

  $form = array();

  // Because we have many fields with the same values, we have to set
  // #tree to be able to access them.
  $form['#tree'] = TRUE;

  // buttons
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $form['lrl_birth_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Birth Identifier'),
    '#description' => t('An unique ID for a birth event.'),
    '#required' => TRUE,
    '#size' => 25,
  );
  $form['lrl_birth_cage_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Cage born ID'),
    '#description' => t('An unique ID for Breeding cage.'),
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
  // Children information fieldset.
  $form['lrl_birth_children'] = array(
    '#type' => 'fieldset',
    '#title' => t('Children Information'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="children-fieldset-wrapper">',
    '#suffix' => '</div>',
  );
  $form['lrl_birth_children']['id_schema_x'] = array(
    '#type' => 'select',
    '#title' => t('X segment in S-X-IIII-NN ID schema'),
    '#required' => TRUE,
    '#options' => array(0 => 'wild-type', 1 => 'transgenic'),
    '#default_value' => 0,
  );
  $form['lrl_birth_children']['nums'] = array(
    '#type' => 'select',
    '#title' => t('Total number of children born'),
    '#options' => array(1 => 1, 2 => 2, 3 => 3, 4 => 4, 5 => 5, 6 => 6, 7 => 7, 8 => 8, 9 => 9, 10 => 10,
      11 => 11, 12 => 12, 13 => 13, 14 => 14, 15 => 15, 16 => 16, 17=> 17, 18 => 18, 19 => 19, 20 => 20),
    '#required' => TRUE,
    '#default_value' => 1,
    '#ajax' => array(
      'callback' => 'ajax_update_birth_nums_callback',
      'wrapper' => 'children-fieldset-wrapper',
      'effect' => 'slide',
      'progress' => array('type' => 'none'),
    ),
  );

  $nums_children = !empty($form_state['values']['lrl_birth_children']['nums']) ? $form_state['values']['lrl_birth_children']['nums'] : 1;
  $mice_id_x = !empty($form_state['values']['lrl_birth_children']['id_schema_x']) ? $form_state['values']['lrl_birth_children']['id_schema_x'] : 0;
  for ($i = 1; $i <= $nums_children; $i++) {
    $child_id = islandora_livingresearchlab_generate_mouse_id('0', $mice_id_x, TRUE, $i);
    $form['lrl_birth_children']['child' . $i] = array(
      '#type' => 'textfield',
      '#title' => t('Child #') . $i . ' ' . t('ID'),
      '#size' => 25,
      '#default_value' => $child_id,
      '#value' => $child_id,
    );
  }



  //dpm($form, 'form array');
  //dpm($form_state, 'form_state');

  return $form;
}


/**
 * Callback for both ajax-enabled select list.
 *
 * Selects and returns the fieldset with the children information in it.
 */
function ajax_update_birth_nums_callback($form, $form_state) {
  return $form['lrl_birth_children'];
}

/**
 *
 * Implements hook_form_submit().
 *
 * @todo: complete islandora_livingresearchlab_add_birth_form_submit().
 */
function islandora_livingresearchlab_add_birth_form_submit(array $form, array &$form_state) {
  //dpm($form_state, 'form_state_posted');
  drupal_set_message(t('Mouse 1234 created with Birth Record 111.'));
}