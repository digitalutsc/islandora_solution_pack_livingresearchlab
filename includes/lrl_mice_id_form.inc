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

  $form['#tree'] = TRUE;

  $form['mice_id_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Define Mice ID Schema'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['mice_id_fields']['description'] = array(
    '#type' => 'markup',
    '#markup' => '<div><p><strong>S-X-IIII-NN</strong> where:</p><ul><li>S = 0 in-house bred, 1 for purchased</li>
<li>X = 0 for wild-type, 1 for transgenic</li><li>IIII = 4-digit litter/shipment ID number</li><li>NN = 2-digit mouse number (numbers are assigned
to each mouse in a litter/shipment)</li></ul></div>',
  );


  $form['mice_id_fields']['mice_id_s'] = array(
    '#type' => 'select',
    '#title' => t('S segment'),
    '#description' => t('0 in-house bred, 1 for purchased.'),
    '#options' => array(0 => 'in-house bred', 1 => 'purchased'),
    '#required' => TRUE,
    '#default_value' => 0,
  );

  $form['mice_id_fields']['mice_id_x'] = array(
    '#type' => 'select',
    '#title' => t('X segment'),
    '#description' => t('0 for wild-type, 1 for transgenic.'),
    '#options' => array(0 => 'wild-type', 1 => 'transgenic'),
    '#required' => TRUE,
    '#default_value' => 0,
  );

  $form['mice_id_fields']['mice_id_litter_or_shipment'] = array(
    '#type' => 'select',
    '#title' => t('Litter or Shipment?'),
    '#required' => TRUE,
    '#default_value' => 0,
    '#options' => array(0 => 'litter', 1 => 'shipment'),
  );

  $form['mice_id_fields']['mice_id_litter'] = array(
    '#type' => 'textfield',
    '#title' => t('Litter ID'),
    '#description' => t('4-digit litter ID number.'),
    '#size' => 25,
    '#default_value' => sprintf('%04s', $last_litter + 1),
    '#element_validate' => array('_form_element_validate'),
    '#states' => array(
      'visible' => array(
        ':input[name="mice_id_fields[mice_id_litter_or_shipment]"]' => array('value' => 0),
      ),
    ),
  );
  $form['mice_id_fields']['mice_id_shipment'] = array(
    '#type' => 'textfield',
    '#title' => t('Shipment ID'),
    '#description' => t('4-digit shipment ID number.'),
    '#size' => 25,
    '#default_value' => sprintf('%04s', $last_shipment + 1),
    '#element_validate' => array('_form_element_validate'),
    '#states' => array(
      'visible' => array(
        ':input[name="mice_id_fields[mice_id_litter_or_shipment]"]' => array('value' => 1),
      ),
    ),
  );
  return $form;
}

/**
 * The submit handler for lrl_mice_id_from.
 */
function islandora_lrl_mice_id_form_submit(array $form, array &$form_state) {
  $step_storage = &islandora_ingest_form_get_step_storage($form_state, 'islandora_lrl_mice_id');
  $step_storage['values'] = $form_state['values'];
}

/**
 * Custom form element validate.
 *
 * @param array $element
 *   A form element.
 * @param array $form_state
 *   The drupal form state.
 * @param array $form
 *   The drupal form.
 */
function _form_element_validate($element, &$form_state, $form) {
  if (empty($element['#value'])) {
    form_error($element, t('@field field can not be empty.', array('@field' => $element['#title'])));
  }
}

/**
 * Mouse CModel ingest callback.
 *
 * Update the mice ID table if a mouse object is ingested successfully.
 */
function islandora_lrl_mice_table_update(array &$form_state) {
  $metadata_step_storage = &islandora_ingest_form_get_step_storage($form_state, 'xml_form_builder_metadata_step');
  if ($metadata_step_storage['created']['DWC']) {
    $mic_id_step_storage = &islandora_ingest_form_get_step_storage($form_state, 'islandora_lrl_mice_id');
    if ($mic_id_step_storage['values']['mice_id_fields']['mice_id_litter_or_shipment']) {
      $shipment_no = $mic_id_step_storage['values']['mice_id_fields']['mice_id_shipment'];
      $highest_recorded = db_select('islandora_lrl_mice', 'm')
        ->fields('m', array('highest_no'))
        ->condition('type', 'shipment')
        ->execute()
        ->fetchField();
      if ($shipment_no > $highest_recorded) {
        db_update('islandora_lrl_mice')
          ->fields(array('highest_no' => $shipment_no))
          ->condition('type', 'shipment')
          ->execute();
      }
      else {
        return;
      }

    }
    else {
      $litter_no = $mic_id_step_storage['values']['mice_id_fields']['mice_id_litter'];
      $highest_recorded = db_select('islandora_lrl_mice', 'm')
        ->fields('m', array('highest_no'))
        ->condition('type', 'litter')
        ->execute()
        ->fetchField();
      if ($litter_no > $highest_recorded) {
        db_update('islandora_lrl_mice')
          ->fields(array('highest_no' => $litter_no))
          ->condition('type', 'litter')
          ->execute();
      }
      else {
        return;
      }
    }
  }
}

