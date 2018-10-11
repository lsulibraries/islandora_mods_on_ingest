<?php


/**
 * Implements hook_form.
 */
function islandora_mods_on_ingest_form($form, &$form_state) {
  $form['choose'] = array(
    '#type' => 'select',
    '#title' => t('Choose Upload or Edit Form.'),
    '#default' => 0,
    '#options' => array('Upload','Edit Form')
  );
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $form['upload_file'] = array(
    'dsid_fieldset' => array(
      //add visibility condition.
      '#states' => array(
        'visible' => array(
          ':input[name="choose"]' => array('value' => '0'),
        ),
      ),
      //borrow from datastream.inc
      '#type' => 'fieldset',
      '#title' => t("Upload MODS.xml"),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      'file' => array(
        '#type' => 'managed_file',
        '#required' => TRUE,
        '#title' => t('Upload Document'),
        '#size' => 64,
        '#description' => t('Select a file to upload.<br/>Files must be less than <strong>@size MB.</strong>', array('@size' => $upload_size)),
        '#default_value' => isset($form_state['values']['files']) ? $form_state['values']['files'] : NULL,
        '#upload_location' => file_default_scheme() . '://',
        '#upload_validators' => array(
          'file_validate_extensions' => array('xml'),
          // Assume its specified in MB.
          'file_validate_size' => array($upload_size * 1024 * 1024),
        ),
      ),
    ),
  );
  return $form;
}

function islandora_mods_on_ingest_form_submit($form, &$form_state) {
  module_load_include('inc', 'islandora', 'includes/ingest.form');
  //load shared storage to attempt and replace next step if we upload mods (not working).
  $shared_storage = &islandora_ingest_form_get_step_storage($form_state, 'islandora_mods_on_ingest');
  $shared_storage['upload_or_edit'] = $form_state['values']['choose'];
  if ($form_state['values']['choose'] == 0) {
    //do something whith the file... mimic what is done with the image in the next penultimate step.
    //borrowed from islandora_sp_large_image_cmodel and adapted for mods.
    //works great, and ever fills in the edit form at the next step, but we still have that edit step
    $object = islandora_ingest_form_get_object($form_state);
    if ($form_state['values']['file']) {
      if (empty($object['MODS'])) {
        $ds = $object->constructDatastream('MODS', 'M');
        $object->ingestDatastream($ds);
      }
      else {
        $ds = $object['MODS'];
      }
      $file = file_load($form_state['values']['file']);
      $path = drupal_realpath($file->uri);
      $ds->setContentFromFile($path, FALSE);
      $ds->label = $file->filename;
      $ds->mimetype = $file->filemime;
    }
    unset($form_state['islandora']['step_storage']['xml_form_builder_metadata_step']);
    unset($form_state['islandora']['step_storage']['xml_form_builder_association_step']);
    unset($form_state['islandora']['step_storage']['islandora_mods_on_ingest']);
    dpm($form_state);
  }
}
