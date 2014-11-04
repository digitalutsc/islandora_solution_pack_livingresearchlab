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
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="children-fieldset-wrapper">',
    '#suffix' => '</div>',
  );
  $form['lrl_birth_children']['nums'] = array(
    '#type' => 'textfield',
    '#title' => t('Total number of children born'),
    '#required' => TRUE,
    '#default_value' => 1,
    '#size' => 25,
  );
  $form['lrl_birth_children']['update_num'] = array(
    '#type' => 'submit',
    '#value' => t('Update the number'),
    '#submit' => array('ajax_update_birth_nums'),
    // See the examples in ajax_example.module for more details on the
    // properties of #ajax.
    '#ajax' => array(
      'callback' => 'ajax_update_birth_nums_callback',
      'wrapper' => 'children-fieldset-wrapper',
    ),
    '#attributes' => array(
      'class' => array('update_num_btn'),
    ),
  );

  // Use $form_state['num_children'] to track the number of children in the birth.
  if (empty($form_state['num_children'])) {
    $form_state['num_children'] = $form['lrl_birth_children']['nums']['#default_value'];
  }
  for ($i = 1; $i <= $form_state['num_children']; $i++) {
    $form['lrl_birth_children']['child' . $i] = array(
      '#type' => 'textfield',
      '#title' => t('Child #' . $i . ' ID'),
      '#size' => 25,
    );
  }


  //dpm($form, 'form array');
  //dpm($form_state, 'form_state');

  return $form;
}

/**
 * Submit handler for the "Update the number" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function ajax_update_birth_nums($form, &$form_state) {
  $form_state['num_children'] = $form['lrl_birth_children']['nums']['#value'];
  $form_state['rebuild'] = TRUE;
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
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
  drupal_set_message(t('Mouse 1234 created with Birth Record 111.'));
}