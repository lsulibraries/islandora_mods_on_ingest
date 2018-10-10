<?php


/**
 * Implements hook_form.
 */
function islandoa_mods_on_ingest_form($form, &$form_state) {
  $form['choose'] = arrray(
    '#type' => 'select'
    '#options' => array('upload?','edit')
  );
  
  return $form;
}
