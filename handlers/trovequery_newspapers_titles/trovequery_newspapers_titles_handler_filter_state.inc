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
class TrovequeryNewspapersTitlesHandlerFilterState extends views_handler_filter_in_operator {

  var $value_form_type = 'select';

  var $value_form_multiple = FALSE;

  /**
   * Sets the 'value options'.
   */
  public function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Select state');
      $zones = call_user_func($this->definition['trovequery']['optionsMethod']);
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
        $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], implode($this->value, ','));
      }
      else {
        $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], $this->value);
      }
    }
  }

  /**
   * Overrides views_handler_filter_in_operator::exposed_form.
   */
  function expose_form(&$form, &$form_state) {
    parent::expose_form($form, $form_state);
    $form['expose']['reduce'] = array(
      '#type' => 'select',
      '#title' => t('Limit list to selected items'),
      '#multiple' => FALSE,
      '#description' => t('If checked, the only items presented to the user will be the ones selected here.'),
      '#default_value' => !empty($this->options['expose']['reduce']), // safety
    );
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

  function value_form(&$form, &$form_state) {
    $form['value'] = array();
    $options = array();

    if (empty($form_state['exposed'])) {
      // Add a select all option to the value form.
      $options = array('all' => t('Select all'));
    }

    $this->get_value_options();
    $options += $this->value_options;
    $default_value = (array) $this->value;

    $which = 'all';
    if (!empty($form['operator'])) {
      $source = ($form['operator']['#type'] == 'radios') ? 'radio:options[operator]' : 'edit-options-operator';
    }
    if (!empty($form_state['exposed'])) {
      $identifier = $this->options['expose']['identifier'];

      if (empty($this->options['expose']['use_operator']) || empty($this->options['expose']['operator_id'])) {
        // exposed and locked.
        $which = in_array($this->operator, $this->operator_values(1)) ? 'value' : 'none';
      }
      else {
        $source = 'edit-' . drupal_html_id($this->options['expose']['operator_id']);
      }

      if (!empty($this->options['expose']['reduce'])) {
        $options = $this->reduce_value_options();

        if (!empty($this->options['expose']['multiple']) && empty($this->options['expose']['required'])) {
          $default_value = array();
        }
      }

      if (empty($this->options['expose']['multiple'])) {
        if (empty($this->options['expose']['required']) && (empty($default_value) || !empty($this->options['expose']['reduce']))) {
          $default_value = 'All';
        }
        elseif (empty($default_value)) {
          $keys = array_keys($options);
          $default_value = array_shift($keys);
        }
        else {
          $copy = $default_value;
          $default_value = array_shift($copy);
        }
      }
    }

    if ($which == 'all' || $which == 'value') {
      $form['value'] = array(
        '#type' => $this->value_form_type,
        '#title' => $this->value_title,
        '#options' => $options,
        '#default_value' => $default_value,
        // These are only valid for 'select' type, but do no harm to checkboxes.
        '#multiple' => $this->value_form_multiple,
        //'#size' => count($options) > 8 ? 8 : count($options),
      );
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier])) {
        $form_state['input'][$identifier] = $default_value;
      }

      if ($which == 'all') {
        if (empty($form_state['exposed']) && (in_array($this->value_form_type, array('checkbox', 'checkboxes', 'radios', 'select')))) {
          $form['value']['#prefix'] = '<div id="edit-options-value-wrapper">';
          $form['value']['#suffix'] = '</div>';
        }
        $form['value']['#dependency'] = array($source => $this->operator_values(1));
      }
    }
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
      $errors[] = t('You must select at least one state');
    }

    return $errors;
  }

}
