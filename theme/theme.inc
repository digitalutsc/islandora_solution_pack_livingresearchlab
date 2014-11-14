<?php

/**
 * @file
 * Theme hooks.
 */

/**
 * Implements hook_preprocess().
 */
function islandora_livingresearchlab_preprocess_islandora_livingresearchlab(array &$variables) {
  module_load_include('inc', 'islandora', 'includes/datastream');
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  module_load_include('inc', 'islandora', 'includes/authtokens');
  module_load_include('inc', 'islandora', 'includes/metadata');
  module_load_include('inc', 'islandora_livingresearchlab', 'includes/utilities');
  drupal_add_js('misc/form.js');
  drupal_add_js('misc/collapse.js');
  $object = $variables['object'];
  $repository = $object->repository;
  try {
    if (isset($object['DC']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['DC'])) {
      $dc = $object['DC']->content;
      $dc_object = DublinCore::importFromXMLString($dc);
    }
  }
  catch (Exception $e) {
    drupal_set_message(t('Error retrieving object %s %t', array('%s' => $object->id, '%t' => $e->getMessage())), 'error', FALSE);
  }
  $variables['islandora_object_label'] = $object->label;
  $variables['theme_hook_suggestions'][] = 'islandora_livingresearchlabs__' . str_replace(':', '_', $object->id);
  $variables['parent_collections'] = islandora_get_parents_from_rels_ext($object);
  $variables['metadata'] = islandora_retrieve_metadata_markup($object);
  $variables['description'] = islandora_retrieve_description_markup($object);

  // Thumbnail.
  if (isset($object['TN']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['TN'])) {
    $thumbnail_size_url = url("islandora/object/{$object->id}/datastream/TN/view", array('absolute' => TRUE));
    $params = array(
      'title' => $object->label,
      'path' => $thumbnail_size_url,
      'width' => '20%',
    );
    $variables['islandora_thumbnail_img'] = theme('image', $params);
  }
}

