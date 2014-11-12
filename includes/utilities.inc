<?php

/**
 * Callback to generate a unique ID for mouse.
 *
 * @todo: islandora_livingresearchlab_generate_mouse_id().
 *
 * Mice ID schema in S-X-IIII-NN format.
 *
 * @param string $s
 *   S segment in Mice ID schema.
 * @param string $x
 *   X segment in Mice ID schema.
 * @param bool $is_litter
 *   IIII segment in Mice ID schema. TRUE for litter type or FALSE for shipment.
 *
 * @return string
 *   A unique mouse ID.
 */
function lrl_generate_mouse_id($s, $x, $is_litter = TRUE, $seq = '01') {
  if (!isset($s)) {
    drupal_set_message(t('S segment in Mice ID schema is missing'), 'error');
    exit;
  }
  if (!isset($x)) {
    drupal_set_message(t('X segement in Mice ID schema is missing'), 'error');
    exit;
  }
  // Padding leading zero for NN segment.
  if (strlen($seq) == 1) {
    $seq_padded = sprintf('%02s', $seq);
  }
  elseif (strlen($seq) == 1) {
    $seq_padded = $seq;
  }
  else {
    drupal_set_message(t('NN segment in Mice ID schema is missing'), 'error');
    exit;
  }
  // Query Mice table to get the last litter no or shipment no.
  if ($is_litter) {
    $init_litter = variable_get('islandora_livingresearchlab_mice_id_init_litter', '0001');
    $result = db_select('islandora_lrl_mice', 'm')
      ->fields('m', array('litter_no'))
      ->distinct()
      ->execute()
      ->fetchCol();
    if (empty($result)) {
      $litter = $init_litter;
    }
    else {
      $litter = sprintf('%04s', max($result) + 1);
    }

    return $s . '-' . $x . '-' . $litter . '-' . $seq_padded;
  }
  else {
    $init_shipment = variable_get('islandora_livingresearchlab_mice_id_init_shipment', '0001');
    $result = db_select('islandora_lrl_mice', 'm')
      ->fields('m', array('shipment_no'))
      ->distinct()
      ->execute()
      ->fetchCol();
    if (empty($result)) {
      $shipment = $init_shipment;
    }
    else {
      $shipment = sprintf('%04s', max($result) + 1);
    }
    return $s . '-' . $x . '-' . $shipment . '-' . $seq_padded;
  }
}
