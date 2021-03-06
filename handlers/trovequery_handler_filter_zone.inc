<?php
/**
 * @file
 * Views field handler for basic Trove Query fields.
 */

class TrovequeryHandlerFilterZone extends views_handler_filter_in_operator {

  /**
   * Sets the 'value options'.
   */
  public function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Zone type');
      $zones = call_user_func($this->definition['trovequery']['zones_method']);
      foreach ($zones as $type => $info) {
        $options[$type] = $info;
      }
      $this->value_options = $options;
    }
  }

  /**
   * Called to add the field to a query.
   */
  public function query() {
    if (isset($this->value, $this->definition['trovequery'])) {
      $this->query->args['method'] = $this->definition['trovequery']['method'];
      if (is_array($this->value)) {
        if (in_array('all', $this->value)) {
          $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], 'all');
        }
        else {
          $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], implode($this->value, ','));
        }
      }
      else {
        $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], $this->value);
      }
    }
  }

  /**
   * Overrides views_handler_filter_in_operator::operators.
   */
  public function operators() {
    $operators = array(
      'in' => array(
        'title' => t('Is one of'),
        'short' => t('in'),
        'short_single' => t('='),
        'method' => 'op_simple',
        'values' => 1,
      ),
    );
    return $operators;
  }

  /**
   * Validate the selected options.
   */
  public function validate() {
    $this->get_value_options();
    $errors = array();

    if ($this->options['exposed'] && !$this->options['expose']['required'] && empty($this->value)) {
      // Don't validate if field is exposed and no default value is provided.
      return $errors;
    }

    if (empty($this->value)) {
      $errors[] = t('You must select at least one zone');
    }

    if (isset($this->value['all'])) {
      unset($this->value);
      $this->value['all'] = 'all';
      return $errors;
    }

    return $errors;
  }

}
