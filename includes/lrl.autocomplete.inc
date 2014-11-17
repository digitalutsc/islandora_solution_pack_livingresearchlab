<?php
/**
 * @file
 * Autocomplete functionality for islandora_livingresearchlab.
 */


/**
 * Autocomplete callback for lrl object search.
 * @todo: islandora_livingresearchlab_autocomplete().
 *
 * @param string $type
 *   Selects eithe Scholar or Department
 *
 * @return array
 *   array of all candidates
 */
function islandora_livingresearchlab_autocomplete($type, $string = '') {
  $mappings = array(
    'mouse' => array(
      'cmodel' => 'islandora:livingresearchlab_mouseCModel',
      'title' => 'dc.title',
    ),
    'experiment' => array(
      'cmodel' => 'islandora:livingresearchlab_experimentCModel',
      'title' => 'dc.title',
    ),
  );

  $qualifier = $mappings[$type]['title'] . ':*';
  $path = request_path();
  if ($path) {
    $path_array = explode('/', $path);
    if (!empty($string)) {
      $qualifier = $mappings[$type]['title'] . ':*' . $string . '*';
    }
  }

  $params = array(
    'fl' => array($mappings[$type]['title'], 'PID'),
  );
  $query = "$qualifier AND " . 'RELS_EXT_hasModel_uri_mt:"' . $mappings[$type]['cmodel'] . '"';
  $url = parse_url(variable_get('islandora_solr_url', 'localhost:8080/solr'));
  $solr = new Apache_Solr_Service($url['host'], $url['port'], $url['path'] . '/');
  $solr->setCreateDocuments(FALSE);
  try {
    $results = $solr->search($query, 0, 1000, $params);
    $json = json_decode($results->getRawResponse(), TRUE);
  }
  catch (Exception $e) {
    watchdog_exception('Islandora Entities', $e, 'Got an exception while searching entities for callback.', array(), WATCHDOG_ERROR);
  }
  $list = array();
  foreach ($json['response']['docs'] as $choice) {
    if (isset($choice[$mappings[$type]['title']])) {
      $list[$choice[$mappings[$type]['title']][0]] = $choice[$mappings[$type]['title']][0] . ' (' . $choice['PID'] . ')';
    }
  }
  drupal_json_output($list);
}