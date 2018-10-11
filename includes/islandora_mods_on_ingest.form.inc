<?php


/**
 * Implements hook_form.
 */
function islandora_mods_on_ingest_form($form, &$form_state) {
  $form['choose'] = array(
    '#type' => 'select',
    '#options' => array('upload?','edit')
  );
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $form['upload_file'] = array(
    '#title' => t('Upload MODS.xml'),
    '#type' => 'managed_file',
    '#required' => false,
    '#description' => t('Upload a valid MODS.xml file'),
    //'#default_value' => '',
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('xml'),
      'file_validate_size' => $upload_size,
    )
  );
  dpm($form_state);
  dpm($form);
  return $form;
}

function islandora_mods_on_ingest_form_submit() {

}
