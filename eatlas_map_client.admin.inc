<?php
include_once('eatlas_map_client.constants.inc');

/**
 * Show a list (table) of all the map clients
 * @throws Exception
 */
function _eatlas_map_client_overview() {
	$output = '<h3>' . t('eAtlas Map Clients overview') . '</h3>';

	// Load all Map Clients
	$map_client_array = eatlas_map_client_load_all();

	$header = array(t('Title'), array('data' => t('Operations'), 'colspan' => 2));
	$rows = array();
	foreach ($map_client_array as $map_client) {
		$block_id = EATLAS_MAP_CLIENTS_BLOCK_PREFIX . $map_client->mcid;
		$block = block_load('eatlas_map_client', $block_id);
		$rows[] = array(
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
				"admin/config/eatlas/eatlas_map_client/" . $map_client->mcid . "/delete",
				array(
					'query' => array('destination' => 'admin/config/eatlas/eatlas_map_client')
				)
			)
		);
	}

	$output .= theme('table', array(
			'header' => $header,
			'rows' => $rows,
			'empty' => t('No Map Client available.')
	));

	return $output;
}

/**
 * https://api.drupal.org/api/drupal/developer%21topics%21forms_api_reference.html/7
 *
 * @param $form
 * @param $form_state
 * @param null $mcid
 * @return array
 */
function _eatlas_map_client_add_form($form, &$form_state, $mcid = NULL) {
	$form = array();

	if ($mcid !== NULL) {
		$map_client = eatlas_map_client_load($mcid);
	} else {
		$map_client = new stdClass();
	}

	$form['title'] = array(
		'#type' => 'textfield',
		'#title' => t('Block title'),
		'#description' => t('Map Client name, used in the block name.'),
		'#default_value' => property_exists($map_client, 'title') ? $map_client->title : '',
		'#required' => TRUE
	);

	// load map client
	_eatlas_map_client_add_form_fields($form, $map_client);
	$form['mcid'] = array(
		'#type' => 'hidden',
		'#value' => $mcid
	);

	// define actions
	$form['actions'] = array('#type' => 'actions');

	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save')
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
 */
function _eatlas_map_client_add_form_validate($form, &$form_state) {

}

/**
 * Drupal magic: <form ID>_submit
 * Submit add / edit / delete Map Client form
 * @throws Exception
 */
function _eatlas_map_client_add_form_submit($form, &$form_state) {
	$form_state['redirect'] = 'admin/config/eatlas/eatlas_map_client';

	$mcid = isset($form_state['values']['mcid']) ? $form_state['values']['mcid'] : NULL;
	$map_client = _get_edited_map_client($mcid, $form_state['values']);
	$mcid = eatlas_map_client_save($map_client);

	// Save the block title
	// NOTE: There is no API to do this...
	db_insert('block')->fields(array(
		'title' => $form_state['values']['title'],
		'module' => 'eatlas_map_client',
		'delta' => EATLAS_MAP_CLIENTS_BLOCK_PREFIX . $mcid,
		'pages' => ''
	))->execute();
}

/**
 * Display a personalised title for "Delete" Map Client block
 */
function _eatlas_map_client_delete_title($mcid) {
	$block_id = EATLAS_MAP_CLIENTS_BLOCK_PREFIX . $mcid;
	$block = block_load('eatlas_map_client', $block_id);

	return t('Delete @map_client Map Client block', array(
		'@map_client' => ($block && property_exists($block, 'title')) ? $block->title : 'Untitled'
	));
}

/**
 * Called from "admin/config/eatlas/eatlas_map_client/%/delete"
 * Confirm deletion of a Map Client
 */
function _eatlas_map_client_form_confirm_delete($form, &$form_state, $mcid) {
	$block_id = EATLAS_MAP_CLIENTS_BLOCK_PREFIX . $mcid;
	$block = block_load('eatlas_map_client', $block_id);

	// Always provide entity id in the same form key as in the entity edit form.
	$form['mcid'] = array('#type' => 'value', '#value' => $mcid);
	$form['delete'] = array('#type' => 'value', '#value' => TRUE);

	return confirm_form(
		$form,
		t('Are you sure you want to delete the Map Client %title?',
			array('%title' => ($block && property_exists($block, 'title')) ? $block->title : 'Untitled')
		),
		'admin/config/eatlas/eatlas_map_client',
		t('Deleting a Map Client will delete its block. ' .
			'This action cannot be undone.'),
		t('Delete'),
		t('Cancel')
	);
}

/**
 * Drupal magic: <form ID>_submit
 */
function _eatlas_map_client_form_confirm_delete_submit($form, &$form_state) {
	if (isset($form_state['values']['mcid'])) {
		eatlas_map_client_delete($form_state['values']['mcid']);
	}

	$form_state['redirect'] = 'admin/config/eatlas/eatlas_map_client';
	return;
}
