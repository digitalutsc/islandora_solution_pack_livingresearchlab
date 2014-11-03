<?php
/**
 * @file
 * Autocomplete functionality for islandora_livingresearchlab.
 */


/**
 * Autocomplete callback for lrl object search.
 * @todo: islandora_livingresearchlab_autocomplete().
 *
 * @string string $string
 *   The user supplied string that is being searched for.
 */
function islandora_livingresearchlab_autocomplete($string, $parent = FALSE) {
  $matches = array();
  $islandora_tuque = islandora_get_tuque_connection();

  $result = db_query("SELECT title FROM {node} WHERE title LIKE '%$string%' LIMIT 10");
  $matches = array();
  foreach($result as $item) {
    $matches[$item->title] = check_plain($item->title);
  }

  drupal_json_output($matches);
}