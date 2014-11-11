<?php

/**
 * Callback to generate a unique ID for mouse.
 *
 * @todo: islandora_livingresearchlab_generate_mouse_id().
 *
 * Mice ID schema in S-X-IIII-NN format.
 *
 * @param string $s
 *   S segement in Mice ID schema.
 * @param string $x
 *   X segement in Mice ID schema.
 * @param bool $is_litter
 *   IIII segment in Mice ID schema. TRUE for litter type or FALSE for shipment.
 *
 * @return string
 *   A unique mouse ID.
 */
function lrl_generate_mouse_id($s, $x, $is_litter = TRUE, $seq = '01') {
  if (!isset($s)) {
    drupal_set_message(t('S segment in Mice ID schema is missing'));
  }
  if (!isset($x)) {
    drupal_set_message(t('X segement in Mice ID schema is missing'));
  }
  // Padding leading zero for NN segment.
  if (strlen($seq) == 1) {
    $seq_padded = sprintf('%02s', $seq);
  }
  elseif (strlen($seq) == 1) {
    $seq_padded = $seq;
  }
  else {
    drupal_set_message(t('NN segment in Mice ID schema is missing'));
    exit;
  }
  // @todo: should query table to get the last litter id or shipment id.
  if ($is_litter) {
    $litter = variable_get('islandora_livingresearchlab_mice_id_init_litter', '0001');
    return $s . '-' . $x . '-' . $litter . '-' . $seq_padded;
  }
  else {
    $shipment = variable_get('islandora_livingresearchlab_mice_id_init_shipment', '0001');
    return $s . '-' . $x . '-' . $shipment . '-' . $seq_padded;
  }
}
