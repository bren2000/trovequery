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

  function expose_options() {
    parent::expose_options();
    $this->options['expose']['operator_id'] = '';
    $this->options['expose']['label'] = $this->value_value;
    $this->options['expose']['required'] = FALSE;
  }

}
