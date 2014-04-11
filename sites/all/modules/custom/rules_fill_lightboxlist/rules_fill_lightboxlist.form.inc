
<?php

function rules_fill_lightboxlist_form_alter(&$form, &$form_state, $form_id)
{
}

function rules_fill_lightboxlist_form_views_form_asset_selection_block_alter(&$form, &$form_state, $form_id)
{
  if ($form_state['step'] == 'views_bulk_operations_config_form')
  {
    global $user;
    $sql = "SELECT node.title AS node_title, node.nid AS nid, node.created AS node_created FROM node
      INNER JOIN users users_node ON node.uid = users_node.uid
      WHERE (( (node.status = '1') AND (node.type IN  ('lightbox')) AND( (users_node.uid = '" . $user->uid . "') )))
      ORDER BY node_created DESC";
      $results = db_query($sql);
      foreach($results as $result)
      {
        $formax[$result->nid] = $result->node_title;
      }

    $form['lightboxselector'] = array(
      '#weight' => '1',
      '#key_type' => 'associative',
      '#multiple_toggle' => '1',
      '#type' => 'select',
      '#options' => $formax,
      '#title' => 'Select Lightbox',
    );
  }
}

function rules_fill_lightboxlist_views_bulk_operations_form_alter(&$form, &$form_state, $vbo)
{
}
  
 
