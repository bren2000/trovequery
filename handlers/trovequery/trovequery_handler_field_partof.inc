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
 */
class TrovequeryHandlerFieldPartOf extends views_handler_field {

  /**
   * Called to add the field to a query.
   */
  public function query() {
    $this->field_alias = $this->real_field;
  }

  /**
   * Define the options available.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['render_link'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Defines the options form.
   */
  public function options_form(&$form, &$form_state) {
    $form['render_link'] = array(
      '#title' => t('Render as link'),
      '#description' => t('Render \'part of\' as link to access point for more info (if available)'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['render_link'],
      '#weight' => 1,
    );
    parent::options_form($form, $form_state);
  }

  /**
   * Render the field.
   */
  public function render($values) {
    if (isset($values->{$this->field})) {
      if (isset($values->{$this->field}['value'])){
        $value = $values->{$this->field}['value'];
      }
      if (isset($values->{$this->field}['url']) && $this->options['render_link'] == TRUE) {
        $path = $values->{$this->field}['url'];
        return l($value, TROVE_URL . $path);
      }
      else {
        return $value;
      }
    }
  }

}
