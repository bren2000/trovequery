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
class TrovequeryHandlerFieldTitle extends views_handler_field {

  /**
   * Called to add the field to a query.
   */
  public function query() {
    $this->field_alias = $this->real_field;
  }

  /**
   * Render output based on options settings.
   */
  public function render($values) {
    //$value = $this->get_value($values);
    $value = $values->{$this->field};
    if (is_array($value)) {
      $value = $value['value'];
    }
    return $this->sanitize_value($value);
  }
}
