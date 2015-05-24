<?php
/**
 * @file
 * Views field handler for basic Trove Query fields.
 */

/**
 * Views field handler for basic Trove Query fields.
 *
 * The only thing we're doing here is making sure the field_alias
 * gets set properly, and that none of the sql-specific query functionality
 * gets called.
 *
 *  Some trove api response fields  can contain HTML, which is typically run
 * through check_plain() and thus not rendered. Here we give the user the
 * option to override this behavior and render the HTML.
 */

class TrovequeryHandlerFieldList extends views_handler_field {

  /**
   * Define the options available.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['render_html'] = array('default' => FALSE);
    $options['include_lastupdate'] = array('default' => FALSE);
    $options['include_author'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Defines the options form.
   */
  public function options_form(&$form, &$form_state) {
    $form['include_lastupdate'] = array(
      '#title' => t('Include last updated date'),
      '#description' => t('Checking this will include the list last updated date.'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['include_lastupdate'],
      '#weight' => 1,
    );
    $form['include_author'] = array(
      '#title' => t('Include list author'),
      '#description' => t('Checking this will include the list author username.'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['include_author'],
      '#weight' => 1,
    );
    parent::options_form($form, $form_state);
  }

  /**
   * Called to add the field to a query.
   */
  public function query() {
    $this->field_alias = $this->real_field;
    $this->query->add_where('', $this->definition['trovequery']['arg'], $this->definition['trovequery']['value']);
  }

  /**
   * Render output based on options settings.
   */
  public function render($values) {
    if (isset($values->{$this->field}) && is_array($values->{$this->field})) {
      $lists = array();
      foreach ($values->{$this->field} as $list) {
        $tmp = check_plain($list['value']);
        if ($this->options['include_author']) {
          $tmp .= ' <span class="list-author">By: ' . $list['by'] . '</span>';
        }
        if ($this->options['include_lastupdate']) {
          $tmp .= ' <span class="list-lastupdated">Last updated: ' . $list['lastupdated'] . '</span> ';
        }
        $lists[] = $tmp;
      }
      return theme('item_list', array('items' => $lists,
        'type' => 'ol',
        'title' => NULL,
        'attributes' => array()));
    }
  }

}
