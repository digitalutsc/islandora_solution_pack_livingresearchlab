<?php
/**
 * @file
 * Callbacks to update relationships.
 */


/**
 * Callback to update relationships between Experiment and Protocol.
 *
 * Update relationships if an Experiment object is ingested successfully.
 */
function islandora_lrl_experiment_rels_update($form, array &$form_state, $update = FALSE) {
  module_load_include('inc', 'islandora_livingresearchlab', 'includes/utilities');
  if (!$update) {
    $metadata_step_storage = &islandora_ingest_form_get_step_storage($form_state, 'xml_form_builder_metadata_step');
    $object = islandora_ingest_form_get_object($form_state);
    if ($update && $metadata_step_storage['created']['DWC'] && $object) {
      $protocol_id = lrl_extract_pid($metadata_step_storage['values']['dwc_fields_experiment']['ResourceRelationship']) ? lrl_extract_pid($metadata_step_storage['values']['dwc_fields_experiment']['ResourceRelationship']) : NULL;
      if (!empty($protocol_id)) {
        $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isGovernedBy', $protocol_id);
      }
    }
  }
  else {
    $pid = $form_state['pid'];
    $object = islandora_object_load($pid);
    $protocol_id = lrl_extract_pid($form_state['values']['dwc_fields_experiment']['ResourceRelationship']) ? lrl_extract_pid($form_state['values']['dwc_fields_experiment']['ResourceRelationship']) : NULL;
    if ($object && !empty($protocol_id)) {
      $object->relationships->remove(ISLANDORA_RELS_EXT_URI, 'isGovernedBy');
      $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isGovernedBy', $protocol_id);
    }
  }


}


/**
 * Callback to update relationships between Experiment and DataSession.
 *
 * Update relationships if an DataSession object is ingested successfully.
 */
function islandora_lrl_datasession_rels_update($form, array &$form_state, $update = FALSE) {
  module_load_include('inc', 'islandora_livingresearchlab', 'includes/utilities');
  if (!$update) {
    $metadata_step_storage = &islandora_ingest_form_get_step_storage($form_state, 'xml_form_builder_metadata_step');
    $object = islandora_ingest_form_get_object($form_state);
    if ($metadata_step_storage['created']['DWC'] && $object) {
      $exp_id = lrl_extract_pid($metadata_step_storage['values']['dwc_fields_datasession']['ResourceRelationship']);
      $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isSessionOf', $exp_id);

      // Multiple data files associated with the datasession.
      $data_files = $metadata_step_storage['values']['dwc_fields_datasession']['datafilespanel'];
      _datafiles($data_files, $object);
    }
  }
  else {
    $pid = $form_state['pid'];
    $object = islandora_object_load($pid);
    if ($object) {
      $object->relationships->remove(ISLANDORA_RELS_EXT_URI, 'isSessionOf');
      $object->relationships->remove(ISLANDORA_RELS_EXT_URI, 'isDataMeasurementOf');
      $object->relationships->remove(ISLANDORA_RELS_EXT_URI, 'hasDataFile');

      $exp_id = lrl_extract_pid($form_state['values']['dwc_fields_datasession']['ResourceRelationship']);
      if (!empty($exp_id)) {
        $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isSessionOf', $exp_id);
      }

      $data_files = $form_state['values']['dwc_fields_datasession']['datafilespanel'];
      _datafiles($data_files, $object);
    }
  }

}

/**
 * Helper function to update datasession rels.
 *
 * @param array $files
 *   An array contains datafiles.
 *
 * @param AbstractObject $object
 *   The islandora object.
 */
function _datafiles(array $files, $object) {
  $len = count($files);
  if ($len >= 1) {
    foreach ($files as $i => $data_file) {
      $file = $data_file['measurementID'];
      if (preg_match('/\s/', $file)) {
        $file = preg_replace('/\s+/', '_', $file);
      }
      $mice[$i] = explode('(', trim($data_file['measurementUnit']));
      $mice_id = substr($mice[$i][1], 0, -1);
      $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isDataMeasurementOf', $mice_id);
      $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasDataFile', $file);
    }
  }
}

/**
 * Callback to update relationships between Mice.
 *
 * Update relationships if a Mouse object is ingested successfully.
 */
function islandora_lrl_mouse_rels_update($form, array &$form_state, $update = FALSE) {
  module_load_include('inc', 'islandora_livingresearchlab', 'includes/utilities');
  if (!$update) {
    $metadata_step_storage = &islandora_ingest_form_get_step_storage($form_state, 'xml_form_builder_metadata_step');
    $object = islandora_ingest_form_get_object($form_state);
    if ($metadata_step_storage['created']['DWC'] && $object) {
      if (!empty($metadata_step_storage['values']['dwc_fields_mouse']['birthFieldset']['ResourceRelationship2']['motherrelatedResourceid'])) {
        $mother = $metadata_step_storage['values']['dwc_fields_mouse']['birthFieldset']['ResourceRelationship2']['motherrelatedResourceid'];
        $pid = lrl_extract_pid($mother) ? lrl_extract_pid($mother) : NULL;
        if (!empty($pid)) {
          $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasMother', $pid);
        }
      }
      if (!empty($metadata_step_storage['values']['dwc_fields_mouse']['birthFieldset']['ResourceRelationship3']['fatherrelatedResourceid'])) {
        $father = $metadata_step_storage['values']['dwc_fields_mouse']['birthFieldset']['ResourceRelationship3']['fatherrelatedResourceid'];
        $pid = lrl_extract_pid($father) ? lrl_extract_pid($father) : NULL;
        if (!empty($pid)) {
          $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasFather', $pid);
        }
      }
      if (!empty($metadata_step_storage['values']['dwc_fields_mouse']['locationtabcagehistory'][0]['locationid1'])
        || !empty($metadata_step_storage['values']['dwc_fields_mouse']['locationtabcagehistory'][1]['locationid1'])) {
        $cage_history = $metadata_step_storage['values']['dwc_fields_mouse']['locationtabcagehistory'];
        foreach ($cage_history as $cage) {
          if (trim($cage['locality1']) == 'currentCage') {
            $cage_id = $cage['locationid1'];
          }
        }
        if (isset($cage_id) && !empty($cage_id)) {
          $pid = lrl_extract_pid($cage_id) ? lrl_extract_pid($cage_id) : NULL;
          if (!empty($pid)) {
            $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isCurrentResidentOf', $pid);
          }
        }
      }

      if (!empty($metadata_step_storage['values']['dwc_fields_mouse']['ResourceRelationship1']['protocolrelatedResourceid'])) {
        $protocol = $metadata_step_storage['values']['dwc_fields_mouse']['ResourceRelationship1']['protocolrelatedResourceid'];
        $pid = lrl_extract_pid($protocol) ? lrl_extract_pid($protocol) : NULL;
        if (!empty($pid)) {
          $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isTestSubjectUnder', $pid);
        }
      }

    }
  }
  else {
    $pid = $form_state['pid'];
    $object = islandora_object_load($pid);
    if ($object) {
      $object->relationships->remove(ISLANDORA_RELS_EXT_URI, 'hasMother');
      $object->relationships->remove(ISLANDORA_RELS_EXT_URI, 'hasFather');
      $object->relationships->remove(ISLANDORA_RELS_EXT_URI, 'isCurrentResidentOf');
      $object->relationships->remove(ISLANDORA_RELS_EXT_URI, 'isTestSubjectUnder');
      // Update hasMother/hasFather rels.
      $m_pid = lrl_extract_pid($form_state['values']['dwc_fields_mouse']['birthFieldset']['ResourceRelationship2']['motherrelatedResourceid']) ? lrl_extract_pid($form_state['values']['dwc_fields_mouse']['birthFieldset']['ResourceRelationship2']['motherrelatedResourceid']) : NULL;
      if (!empty($m_pid)) {
        $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasMother', $m_pid);
      }
      $f_pid = lrl_extract_pid($form_state['values']['dwc_fields_mouse']['birthFieldset']['ResourceRelationship3']['fatherrelatedResourceid']) ? lrl_extract_pid($form_state['values']['dwc_fields_mouse']['birthFieldset']['ResourceRelationship3']['fatherrelatedResourceid']) : NULL;
      if (!empty($f_pid)) {
        $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasMother', $f_pid);
      }
      // Update currentCage rels.
      $cage_history = $form_state['values']['dwc_fields_mouse']['locationtabcagehistory'];
      foreach ($cage_history as $cage) {
        if (trim($cage['locality1']) == 'currentCage') {
          $cage_id = $cage['locationid1'];
        }
      }
      $cage_pid = lrl_extract_pid($cage_id) ? lrl_extract_pid($cage_id) : NULL;
      if (!empty($cage_pid)) {
        $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isCurrentResidentOf', $cage_pid);
      }
      // Update protocol rels.
      $protocol_pid = lrl_extract_pid($form_state['values']['dwc_fields_mouse']['ResourceRelationship1']['protocolrelatedResourceid']) ? lrl_extract_pid($form_state['values']['dwc_fields_mouse']['ResourceRelationship1']['protocolrelatedResourceid']) : NULL;
      if (!empty($protocol_pid)) {
        $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isTestSubjectUnder', $protocol_pid);
      }
    }
  }

}
