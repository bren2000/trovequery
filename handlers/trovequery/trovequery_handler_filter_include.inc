<?php
/**
 * @file
 * Views field handler for basic Trove Query fields.
 */

class TrovequeryHandlerFilterInclude extends views_handler_filter_boolean_operator {

  /**
   * Called to add the field to a query.
   */
  public function query() {
    $this->field_alias = $this->real_field;
    if (isset($this->value, $this->definition['trovequery'])) {
      $this->query->add_where('',
        $this->definition['trovequery']['arg'],
        $this->definition['trovequery']['index'] . ':' . $this->definition['trovequery']['value']);
    }
  }

  /**
   * Sets the'value options'.
   */
  public function get_value_options() {
    $this->value_options = array(
      1 => t('Yes'),
      0 => t('No'),
    );
  }

}