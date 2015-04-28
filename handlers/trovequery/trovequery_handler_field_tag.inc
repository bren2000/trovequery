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

class TrovequeryHandlerFieldTag extends views_handler_field {

  /**
   * Called to add the field to a query.
   */
  public function query() {
    $this->field_alias = $this->real_field;
    $this->query->add_where('', $this->definition['trovequery']['arg'], $this->definition['trovequery']['value']);
  }

  /**
   * Define the options available.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['include_lastupdate'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Defines the options form.
   */
  public function options_form(&$form, &$form_state) {
    $form['include_lastupdate'] = array(
      '#title' => t('Include last updated date'),
      '#description' => t('Checking this will include the tag last updated date.'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['include_lastupdate'],
      '#weight' => 1,
    );
    parent::options_form($form, $form_state);
  }

    /**
   * Determine if this field can allow advanced rendering.
   *
   * Fields can set this to FALSE if they do not wish to allow
   * token based rewriting or link-making.
   */
  function allow_advanced_render() {
    return TRUE;
  }

  /**
   * Render output based on options settings.
   */
  public function render($values) {
    if (isset($values->{$this->field}) && is_array($values->{$this->field})) {
      $tags = array();
      foreach ($values->{$this->field} as $tag) {
        $tmp = $tag['value'];
        if ($this->options['include_lastupdate']) {
          $tmp .= ' <span class="tag-lastupdated">Last updated: ' . $tag['lastupdated'] . '</span> ';
        }
        $tags[] = $tmp;
      }
      return theme('item_list', array('items' => $tags,
        'type' => 'ol',
        'title' => NULL,
        'attributes' => array()));
    }
  }

}
