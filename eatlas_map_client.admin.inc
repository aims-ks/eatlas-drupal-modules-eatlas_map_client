<?php
include_once('eatlas_map_client.constants.inc');

/**
 * Show a list (table) of all the map clients
 * @return string
 */
function _eatlas_map_client_overview() {
  $output = '<h3>' . t('eAtlas Map Clients Configurations overview') . '</h3>';

  // Load all Map Client configurations
  $map_client_configurations_array = eatlas_map_client_configuration_load_all();

  $defTableHeader = array(t('Host'), array('data' => t('Operations'), 'colspan' => 2));
  $defTableRows = array();
  foreach ($map_client_configurations_array as $map_client_configuration) {
    $defTableRows[] = array(
      $map_client_configuration->map_config_host,
      l(
        t('edit'),
        "admin/config/eatlas/eatlas_map_client/configuration/" . $map_client_configuration->mccid . "/edit",
        array(
          'query' => array('destination' => 'admin/config/eatlas/eatlas_map_client')
        )
      ),
      l(
        t('delete'),
        "admin/config/eatlas/eatlas_map_client/configuration/" . $map_client_configuration->mccid . "/delete",
        array(
          'query' => array('destination' => 'admin/config/eatlas/eatlas_map_client')
        )
      )
    );
  }

  $output .= theme('table', array(
    'header' => $defTableHeader,
    'rows' => $defTableRows,
    'empty' => t('No Map Client configuration available.')
  ));

	$output .= '<h3>' . t('eAtlas Map Clients Blocks overview') . '</h3>';

	// Load all Map Client blocks and configurations
	$map_client_blocks_array = eatlas_map_client_block_config_load_all();

	$blockTableHeader = array(t('Title'), array('data' => t('Operations'), 'colspan' => 2));
	$blockTableRows = array();
	foreach ($map_client_blocks_array as $map_client_block) {
		$block_id = EATLAS_MAP_CLIENT_BLOCK_PREFIX . $map_client_block->mcbid;
		$block = block_load('eatlas_map_client', $block_id);
    $blockTableRows[] = array(
			($block && property_exists($block, 'title')) ? $block->title : 'Untitled',
			l(
				t('edit'),
				"admin/structure/block/manage/eatlas_map_client/$block_id/configure",
				array(
					'query' => array('destination' => 'admin/config/eatlas/eatlas_map_client')
				)
			),
			l(
				t('delete'),
				"admin/config/eatlas/eatlas_map_client/block/" . $map_client_block->mcbid . "/delete",
				array(
					'query' => array('destination' => 'admin/config/eatlas/eatlas_map_client')
				)
			)
		);
	}

	$output .= theme('table', array(
			'header' => $blockTableHeader,
			'rows' => $blockTableRows,
			'empty' => t('No Map Client available.')
	));

	return $output;
}

/**
 * @param $form
 * @param $form_state
 * @param null $mccid
 * @return array
 */
function _eatlas_map_client_edit_configuration_form($form, &$form_state, $mccid = NULL) {
  $form = array();

  if ($mccid !== NULL) {
    $map_client_configuration = eatlas_map_client_configuration_load($mccid);
  } else {
    $map_client_configuration = new stdClass();
  }

  // load map client configuration fields
  _eatlas_map_client_edit_configuration_form_fields($form, $map_client_configuration);
  // add hidden fields
  $form['mccid'] = array(
    '#type' => 'hidden',
    '#value' => $mccid
  );

  // define actions
  $form['actions'] = array('#type' => 'actions');

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#access' => _eatlas_map_client_admin_access()
  );

  if ($mccid !== NULL) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#access' => _eatlas_map_client_admin_access(),
      '#weight' => 10,
    );
  }

  return $form;
}

/**
 * Drupal magic: <form ID>_validate
 * Validate add / edit / delete Map Client form
 * @param $form
 * @param $form_state
 */
function _eatlas_map_client_edit_configuration_form_validate($form, &$form_state) {

}

/**
 * Drupal magic: <form ID>_submit
 * Submit add / edit / delete Map Client form
 * @param $form
 * @param $form_state
 */
function _eatlas_map_client_edit_configuration_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/eatlas/eatlas_map_client';

  $mccid = isset($form_state['values']['mccid']) ? $form_state['values']['mccid'] : NULL;
  $map_client_configuration = _get_edited_map_client_configuration($mccid, $form_state['values']);

  eatlas_map_client_configuration_save($map_client_configuration);
}

/**
 * Called from "admin/config/eatlas/eatlas_map_client/configuration/%/delete"
 * Confirm deletion of a Map Client configuration
 * @param $form
 * @param $form_state
 * @param $mccid
 * @return mixed
 */
function _eatlas_map_client_form_confirm_delete_configuration($form, &$form_state, $mccid) {
    // Always provide entity id in the same form key as in the entity edit form.
  $form['mccid'] = array('#type' => 'value', '#value' => $mccid);
  $form['delete'] = array('#type' => 'value', '#value' => TRUE);

  $map_client_configuration = eatlas_map_client_configuration_load($mccid);
  if (property_exists($map_client_configuration, 'map_config_host')) {
    return confirm_form(
      $form,
      t('Are you sure you want to delete the Map Client configuration for %host?',
        array('%host' => $map_client_configuration->map_config_host)
      ),
      'admin/config/eatlas/eatlas_map_client',
      t('This will delete the Map Client configuration. This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
    );
  } else {
    return null;
  }
}

/**
 * Drupal magic: <form ID>_submit
 * @param $form
 * @param $form_state
 */
function _eatlas_map_client_form_confirm_delete_configuration_submit($form, &$form_state) {
  if (isset($form_state['values']['mccid'])) {
    eatlas_map_client_configuration_delete($form_state['values']['mccid']);
  }

  $form_state['redirect'] = 'admin/config/eatlas/eatlas_map_client';
  return;
}

/**
 * @param $form
 * @param $form_state
 * @param null $mcbid
 * @return array
 */
function _eatlas_map_client_add_block_form($form, &$form_state, $mcbid = NULL) {
	$form = array();

	if ($mcbid !== NULL) {
		$map_client_block = eatlas_map_client_block_config_load($mcbid);
	} else {
		$map_client_block = new stdClass();
	}

	$form['title'] = array(
		'#type' => 'textfield',
		'#title' => t('Block title'),
		'#description' => t('Map Client block name.'),
		'#default_value' => property_exists($map_client_block, 'title') ? $map_client_block->title : '',
		'#required' => TRUE
	);

	// load map client
	_eatlas_map_client_add_block_form_fields($form, $map_client_block);
	$form['mcbid'] = array(
		'#type' => 'hidden',
		'#value' => $mcbid
	);

	// define actions
	$form['actions'] = array('#type' => 'actions');

	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save'),
    '#access' => _eatlas_map_client_admin_access()
	);

	$form['actions']['delete'] = array(
		'#type' => 'submit',
		'#value' => t('Delete'),
		'#access' => _eatlas_map_client_admin_access(),
		'#weight' => 10,
	);

	return $form;
}

/**
 * Drupal magic: <form ID>_validate
 * Validate add / edit / delete Map Client form
 * @param $form
 * @param $form_state
 */
function _eatlas_map_client_add_block_form_validate($form, &$form_state) {

}

/**
 * Drupal magic: <form ID>_submit
 * Submit add / edit / delete Map Client form
 * @param $form
 * @param $form_state
 */
function _eatlas_map_client_add_block_form_submit($form, &$form_state) {
	$form_state['redirect'] = 'admin/config/eatlas/eatlas_map_client';

	$mcbid = isset($form_state['values']['mcbid']) ? $form_state['values']['mcbid'] : NULL;
	$map_client = _get_edited_map_client_block($mcbid, $form_state['values']);
	$mcbid = eatlas_map_client_block_config_save($map_client);

	// Save the block title
	// NOTE: There is no API to do this...
	db_insert('block')->fields(array(
		'title' => $form_state['values']['title'],
		'module' => 'eatlas_map_client',
		'delta' => EATLAS_MAP_CLIENT_BLOCK_PREFIX . $mcbid,
		'pages' => ''
	))->execute();
}

/**
 * Called from "admin/config/eatlas/eatlas_map_client/%/delete"
 * Confirm deletion of a Map Client
 * @param $form
 * @param $form_state
 * @param $mcbid
 * @return mixed
 */
function _eatlas_map_client_form_confirm_delete_block($form, &$form_state, $mcbid) {
	$block_id = EATLAS_MAP_CLIENT_BLOCK_PREFIX . $mcbid;
	$block = block_load('eatlas_map_client', $block_id);

	// Always provide entity id in the same form key as in the entity edit form.
	$form['mcbid'] = array('#type' => 'value', '#value' => $mcbid);
	$form['delete'] = array('#type' => 'value', '#value' => TRUE);

	return confirm_form(
		$form,
		t('Are you sure you want to delete the Map Client block %title?',
			array('%title' => ($block && property_exists($block, 'title')) ? $block->title : 'Untitled')
		),
		'admin/config/eatlas/eatlas_map_client',
		t('This will delete the Map Client block. ' .
			'This action cannot be undone.'),
		t('Delete'),
		t('Cancel')
	);
}

/**
 * Drupal magic: <form ID>_submit
 * @param $form
 * @param $form_state
 */
function _eatlas_map_client_form_confirm_delete_block_submit($form, &$form_state) {
	if (isset($form_state['values']['mcbid'])) {
		eatlas_map_client_block_config_delete($form_state['values']['mcbid']);
	}

	$form_state['redirect'] = 'admin/config/eatlas/eatlas_map_client';
	return;
}
