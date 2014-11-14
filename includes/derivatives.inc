<?php

/**
 * @file
 * Creates additional datatreams for ingested object
 */

/**
 * Creates the TN derivative for the ingested object.
 *
 * @param AbstractObject $object
 *   The object to add the deriviative datastreams to.
 * @param bool $force
 *   Whether the derivative generation is being forced or not.
 *
 * @return array
 *   An array describing the outcome of the thumbnail creation.
 *
 * @see hook_islandora_derivative()
 */
function islandora_livingresearchlab_create_tn_mouse(AbstractObject $object, $force = FALSE) {
  if (!isset($object['TN']) || $force === TRUE) {
    return islandora_livingresearchlab_add_datastream($object, 'TN', drupal_get_path('module', 'islandora_livingresearchlab') . '/images/mouse.png');
  }
}

function islandora_livingresearchlab_create_tn_experiment(AbstractObject $object, $force = FALSE) {
  if (!isset($object['TN']) || $force === TRUE) {
    return islandora_livingresearchlab_add_datastream($object, 'TN', drupal_get_path('module', 'islandora_livingresearchlab') . '/images/experiments.png');
  }
}


/**
 * Adds the given file as a datastream to the given object.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 * @param string $datastream_id
 *   The datastream id of the added datastream.
 * @param string $file_uri
 *   A URI to the file containing the content for the datastream.
 *
 * @return array
 *   An array describing the outcome of the datastream addition.
 */
function islandora_livingresearchlab_add_datastream(AbstractObject $object, $datastream_id, $file_uri) {
  try {
    $ingest = !isset($object[$datastream_id]);
    $mime_detector = new MimeDetect();

    if ($ingest) {
      $ds = $object->constructDatastream($datastream_id, "M");
      $ds->label = $datastream_id;
    }
    else {
      $ds = $object[$datastream_id];
    }
    $ds->mimetype = $mime_detector->getMimetype($file_uri);
    $ds->setContentFromFile(drupal_realpath($file_uri));
    if ($ingest) {
      $object->ingestDatastream($ds);
    }
    return array(
      'success' => TRUE,
      'messages' => array(
        array(
          'message' => t('Created @dsid derivative for OBJ (@pid).'),
          'message_sub' => array(
            '@dsid' => $datastream_id,
            '@pid' => $object->id,
          ),
          'type' => 'dsm',
        ),
      ),
    );
  }
  catch (exception $e) {
    return array(
      'success' => FALSE,
      'messages' => array(
        array(
          'message' => t('Failed to add @dsid datastream for @pid. Error message: @message<br/>Stack: @stack'),
          'message_sub' => array(
            '@dsid' => $datastream_id,
            '@pid' => $object->id,
            '@message' => $e->getMessage(),
            '@stack' => $e->getTraceAsString(),
          ),
          'type' => 'watchdog',
          'severity' => WATCHDOG_ERROR,
        ),
      ),
    );
  }
}