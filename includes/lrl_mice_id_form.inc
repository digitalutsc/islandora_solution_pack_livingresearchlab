<?php
/**
 * @file
 * Gather information for Mice ID schema.
 */

/**
 * Gather information to generate mice ID.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_lrl_mice_id_form(array $form, array &$form_state) {

  $form = array();
  $last_litter = db_select('islandora_lrl_mice', 'm')
    ->fields('m', array('highest_no'))
    ->condition('type', 'litter')
    ->execute()
    ->fetchField();
  $last_shipment = db_select('islandora_lrl_mice', 'm')
    ->fields('m', array('highest_no'))
    ->condition('type', 'shipment')
    ->execute()
    ->fetchField();

  $form['description'] = array(
    '#type' => 'markup',
    '#markup' => '<div><h2>Mice ID Schema</h2><p><strong>S-X-IIII-NN</strong> where:</p><ul><li>S = 0 in-house bred, 1 for purchased</li>
<li>X = 0 for wild-type, 1 for transgenic</li><li>IIII = 4-digit litter/shipment ID number</li><li>NN = 2-digit mouse number (numbers are assigned
to each mouse in a litter/shipment)</li></ul></div>',
  );


  $form['mice_id_s'] = array(
    '#type' => 'select',
    '#title' => t('S segment'),
    '#description' => t('0 in-house bred, 1 for purchased.'),
    '#options' => array(0 => 'in-house bred', 1 => 'purchased'),
    '#required' => TRUE,
    '#default_value' => 0,
  );

  $form['mice_id_x'] = array(
    '#type' => 'select',
    '#title' => t('X segment'),
    '#description' => t('0 for wild-type, 1 for transgenic.'),
    '#options' => array(0 => 'wild-type', 1 => 'transgenic'),
    '#required' => TRUE,
    '#default_value' => 0,
  );

  $form['mice_id_litter_or_shipment'] = array(
    '#type' => 'select',
    '#title' => t('Litter or Shipment?'),
    '#required' => TRUE,
    '#default_value' => 0,
    '#options' => array(0 => 'litter', 1 => 'shipment'),
  );

  $form['mice_id_litter'] = array(
    '#type' => 'textfield',
    '#title' => t('Litter ID'),
    '#description' => t('4-digit litter ID number.'),
    '#size' => 25,
    '#default_value' => $last_litter,
    '#states' => array(
      'visible' => array(
        ':input[name="mice_id_litter_or_shipment"]' => array('value' => 0),
      ),
    ),
  );
  $form['mice_id_shipment'] = array(
    '#type' => 'textfield',
    '#title' => t('Shipment ID'),
    '#description' => t('4-digit shipment ID number.'),
    '#size' => 25,
    '#default_value' => $last_shipment,
    '#states' => array(
      'visible' => array(
        ':input[name="mice_id_litter_or_shipment"]' => array('value' => 1),
      ),
    ),
  );

  return $form;
}