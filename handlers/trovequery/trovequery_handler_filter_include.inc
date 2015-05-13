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

  function value_form(&$form, &$form_state) {
    if (empty($this->value_options)) {
      // Initialize the array of possible values for this filter.
      $this->get_value_options();
    }
    // Allow filter definitions to set form type.
    if (!isset($this->definition['form type'])) {
      if (!empty($form_state['exposed'])) {
        // Exposed filter: use a select box to save space.
        $filter_form_type = 'select';
      }
      else {
        // Configuring a filter: use radios for clarity.
        $filter_form_type = 'radios';
      }
    }
    else {
      $filter_form_type = $this->definition['form type'];
    }
    $form['value'] = array(
      '#type' => $filter_form_type,
      '#title' => $this->value_value,
      '#options' => $this->value_options,
      '#default_value' => $this->value,
    );
    if (!empty($this->options['exposed'])) {
      $identifier = $this->options['expose']['identifier'];
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier])) {
        $form_state['input'][$identifier] = $this->value;
      }
      // If we're configuring an exposed filter, add an <Any> option.
      if (empty($form_state['exposed']) || empty($this->options['expose']['required'])) {
        $any_label = variable_get('views_exposed_filter_any_label', 'new_any') == 'old_any' ? '<Any>' : t('- Any -');
        if ($form['value']['#type'] != 'select') {
          $any_label = check_plain($any_label);
        }
        $form['value']['#options'] = array('All' => $any_label) + $form['value']['#options'];
      }
    }
  }

  function expose_options() {
    parent::expose_options();
    $this->options['expose']['operator_id'] = '';
    $this->options['expose']['label'] = $this->value_value;
    $this->options['expose']['required'] = FALSE;
  }

}
