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
  $timestamp = time();

  $form = array();

  // Because we have many fields with the same values, we have to set
  // #tree to be able to access them.
  $form['#tree'] = TRUE;

  // buttons.
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
    '#default_value' => $timestamp,
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
    '#autocomplete_path' => 'islandora/lrl_object/autocomplete/mouse',
    '#required' => TRUE,
    '#size' => 25,
  );
  $form['lrl_birth_father'] = array(
    '#type' => 'textfield',
    '#title' => t('Father'),
    '#autocomplete_path' => 'islandora/lrl_object/autocomplete/mouse',
    '#required' => TRUE,
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
    '#options' => array(0 => 'Unknown', 1 => 1, 2 => 2, 3 => 3, 4 => 4, 5 => 5, 6 => 6, 7 => 7, 8 => 8, 9 => 9, 10 => 10,
      11 => 11, 12 => 12, 13 => 13, 14 => 14, 15 => 15, 16 => 16, 17=> 17, 18 => 18, 19 => 19, 20 => 20),
    '#required' => TRUE,
    '#default_value' => 0,
    '#ajax' => array(
      'callback' => 'ajax_update_birth_nums_callback',
      'wrapper' => 'children-id-fieldset-wrapper',
    ),
  );

  // Children ID fieldset.
  $form['lrl_birth_children']['id_fieldset'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="children-id-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  $nums_children = !empty($form_state['values']['lrl_birth_children']['nums']) ? $form_state['values']['lrl_birth_children']['nums'] : 0;
  $mice_id_x = !empty($form_state['values']['lrl_birth_children']['id_schema_x']) ? $form_state['values']['lrl_birth_children']['id_schema_x'] : 0;

  for ($i = 1; $i <= $nums_children; $i++) {
    $child_id = lrl_generate_mouse_id('0', $mice_id_x, TRUE, $i);
    $form['lrl_birth_children']['id_fieldset']['child' . $i] = array(
      '#type' => 'textfield',
      '#title' => t('Child #') . $i . ' ' . t('ID'),
      '#size' => 25,
      '#default_value' => $child_id,
    );
  }
  return $form;
}


/**
 * Callback for both ajax-enabled select list.
 *
 * Selects and returns the fieldset with the children information in it.
 */
function ajax_update_birth_nums_callback($form, $form_state) {
  return $form['lrl_birth_children']['id_fieldset'];
}

/**
 * Implements hook_form_submit().
 */
function islandora_livingresearchlab_add_birth_form_submit(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/tuque');
  module_load_include('inc', 'islandora', 'includes/tuque_wrapper');

  $birth_id = $form_state['values']['lrl_birth_id'];
  $birth_cage_id = $form_state['values']['lrl_birth_cage_id'];
  $mother_id = $form_state['values']['lrl_birth_mother'];
  $father_id = $form_state['values']['lrl_birth_father'];
  $weandate = $form_state['values']['lrl_birth_weandate'];
  $dob = $form_state['values']['lrl_birth_dob'];
  $setupdate = $form_state['values']['lrl_birth_setupdate'];
  $children = $form_state['values']['lrl_birth_children']['id_fieldset'];
  $litter = explode('-', $children['child1']);

  foreach ($children as $child) {
    // Update Mice table to track Mice IDs.
    try {
      $query = db_update('islandora_lrl_mice')
        ->fields(array('highest_no' => $litter[2]))
        ->condition('type', 'litter')
        ->execute();
      drupal_set_message(t('Mouse ID !mid created with Birth Record ID !bid.', array('!mid' => $child, '!bid' => $birth_id)));
    }
    catch (PDOException $e) {
      form_set_error("Failed to update Mice table.", $e->getMessage());
    }
    try {

      $dwr_url = 'http://rs.tdwg.org/dwc/dwcrecord/';
      $dwc_url = 'http://rs.tdwg.org/dwc/terms/';
      $dcterms_url = 'http://purl.org/dc/terms/';

      $dom = new DOMDocument('1.0', 'utf-8');

      $dwr = $dom->appendChild($dom->createElementNS($dwr_url, 'dwr:DarwinRecordSet'));
      $dwr->setAttributeNS(
        'http://www.w3.org/2001/XMLSchema-instance',
        'xsi:schemaLocation',
        'http://rs.tdwg.org/dwc/dwcrecord/  http://rs.tdwg.org/dwc/xsd/tdwg_dwc_classes.xsd'
      );
      $dwr->setAttributeNS('http://www.w3.org/2000/xmlns/', 'xmlns:dwr', $dwr_url);
      $dwr->setAttributeNS('http://www.w3.org/2000/xmlns/', 'xmlns:dwc', $dwc_url);
      $dwr->setAttributeNS('http://www.w3.org/2000/xmlns/', 'xmlns:dcterms', $dcterms_url);

      $occurrence = $dwr->appendChild($dom->createElementNS($dwc_url, 'dwc:Occurrence'));
      $occurrence_id = $occurrence->appendChild($dom->createElementNS($dwc_url, 'dwc:occurrenceID', $child));
      $basis_record = $occurrence->appendChild($dom->createElementNS($dwc_url, 'dwc:basisOfRecord', 'LivingSpecimen'));
      $sex = $occurrence->appendChild($dom->createElementNS($dwc_url, 'dwc:sex', 'NotYetDefined'));

      $taxon = $dwr->appendChild($dom->createElementNS($dwc_url, 'dwc:Taxon'));
      $taxon_id = $taxon->appendChild($dom->createElementNS($dwc_url, 'dwc:taxonID', 'NotYetDefined'));
      $taxon_remarks = $taxon->appendChild($dom->createElementNS($dwc_url, 'dwc:taxonRemarks', 'mouseType'));

      $location = $dwr->appendChild($dom->createElementNS($dcterms_url, 'dcterms:Location'));
      $loc_id = $location->appendChild($dom->createElementNS($dwc_url, 'dwc:locationID', $birth_cage_id));
      $locality = $location->appendChild($dom->createElementNS($dwc_url, 'dwc:locality', 'Cage born'));
      $location_remarks = $location->appendChild($dom->createElementNS($dwc_url, 'dwc:locationRemarks', date('Y-m-d', strtotime($dob))));

      $event = $dwr->appendChild($dom->createElementNS($dwc_url, 'dwc:Event'));
      $event_id = $event->appendChild($dom->createElementNS($dwc_url, 'dwc:eventID', $birth_id));
      $event_date = $event->appendChild($dom->createElementNS($dwc_url, 'dwc:eventDate', date('Y-m-d', strtotime($dob))));
      $event_remarks = $event->appendChild($dom->createElementNS($dwc_url, 'dwc:eventRemarks', 'dateOfBirth'));

      $rels = $dwr->appendChild($dom->createElementNS($dwc_url, 'dwc:ResourceRelationship'));
      $rels_id = $rels->appendChild($dom->createElementNS($dwc_url, 'dwc:relatedResourceID', $mother_id));
      $rels_resource = $rels->appendChild($dom->createElementNS($dwc_url, 'dwc:relationshipOfResource', 'hasMother'));

      $rels = $dwr->appendChild($dom->createElementNS($dwc_url, 'dwc:ResourceRelationship'));
      $rels_id = $rels->appendChild($dom->createElementNS($dwc_url, 'dwc:relatedResourceID', $father_id));
      $rels_resource = $rels->appendChild($dom->createElementNS($dwc_url, 'dwc:relationshipOfResource', 'hasFather'));

      $event = $dwr->appendChild($dom->createElementNS($dwc_url, 'dwc:Event'));
      $event_date = $event->appendChild($dom->createElementNS($dwc_url, 'dwc:eventDate', date('Y-m-d', strtotime($weandate))));
      $event_remarks = $event->appendChild($dom->createElementNS($dwc_url, 'dwc:eventRemarks', 'weanDate'));

      $event = $dwr->appendChild($dom->createElementNS($dwc_url, 'dwc:Event'));
      $event_date = $event->appendChild($dom->createElementNS($dwc_url, 'dwc:eventDate', date('Y-m-d', strtotime($setupdate))));
      $event_remarks = $event->appendChild($dom->createElementNS($dwc_url, 'dwc:eventRemarks', 'setupDate'));

      $dom->preserveWhiteSpace = FALSE;
      $dom->formatOutput = TRUE;

      $motherid_part = explode('(', $mother_id);
      $mother_pid = substr($motherid_part[1], 0, strlen($motherid_part[1]) - 1);
      $fatherid_part = explode('(', $father_id);
      $father_pid = substr($fatherid_part[1], 0, strlen($fatherid_part[1]) - 1);

      $connection = islandora_get_tuque_connection();
      $repository = $connection->repository;

      // Get the namespace for mouseCModel via CollectionPolicy class.
      $collection_object = islandora_object_load('islandora:mice_collection');
      $collection_policy = new CollectionPolicy($collection_object['COLLECTION_POLICY']->content);
      $collection_models = $collection_policy->getContentModels();
      if (isset($collection_models['islandora:livingresearchlab_mouseCModel'])) {
        $namespace = check_plain($collection_models['islandora:livingresearchlab_mouseCModel']['namespace']);
      }
      else {
        $namespace = 'islandora';
      }
      $object = $repository->constructObject($namespace);
      $object->label = $child;
      $object->models = array('islandora:livingresearchlab_mouseCModel');
      $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', 'islandora:mice_collection');
      $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasMother', $mother_pid);
      $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasFather', $father_pid);
      $dsid = 'DWC';
      $ds = $object->constructDatastream($dsid, 'M');
      $ds->label = $dsid;
      $ds->mimetype = 'text/xml';
      $ds->setContentFromString($dom->saveXML());
      $object->ingestDatastream($ds);
      $repository->ingestObject($object);
    }
    catch (PDOException $e) {
      form_set_error("Failed to create Mouse Object(s) in Fedora.", $e->getMessage());
    }

  }
}

