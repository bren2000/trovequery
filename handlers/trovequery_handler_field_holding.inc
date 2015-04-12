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
class TrovequeryHandlerFieldHolding extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);

    $this->limit_values = TRUE;
  }

  public function query() {
    $this->field_alias = $this->real_field;
    $this->query->add_where('', 'include', 'holdings');
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['settings'] = array(
      'default' => array(),
    );

    $options['multi_type'] = array(
      'default' => 'ul'
    );

    $options['list_class'] = array(
      'default' => ''
    );

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $this->multiple_options_form($form, $form_state);

    $settings = $this->options['settings'];

    // Get the settings form.
    $settings_form = array('#value' => array());

    $form['settings'] = $settings_form;
  }

  /**
   * Provide options for multiple value fields.
   */
  function multiple_options_form(&$form, &$form_state) {
    $field = $this->field_info;

    $form['multiple_field_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Multiple field settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 5,
    );
    $form['multi_type'] = array(
      '#type' => 'radios',
      '#title' => t('Display type'),
      '#options' => array(
        'ul' => t('Unordered list'),
        'ol' => t('Ordered list'),
      ),
      '#dependency' => array('edit-options-group-rows' => array(TRUE)),
      '#default_value' => $this->options['multi_type'],
      '#fieldset' => 'multiple_field_settings',
    );
    $form['list_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Class to apply to list'),
      '#default_value' => $this->options['list_class'],
      '#fieldset' => 'multiple_field_settings',
      '#weight' => 5,
    );
  }

  /**
   * Render output based on options settings.
   */
  public function render($values) {
    if (isset($values->{$this->field})) {
      $holding = '';
      foreach ($values->{$this->field} as $h) {
        $holding .= $this->prepareHolding($h);
      }
      //dpm($holding);
      return $holding;
      //return $this->themeHolding($tempHolding);
    }
  }

  function themeHolding($arr) {
    return theme_item_list(array(
            'items' => $arr,
            'title' => NULL,
            'type' => $this->options['multi_type'],
            'attributes' => array('class' => $this->options['list_class']),
          ));
  }

  function prepareHolding($raw) {
    $holding = array();
    if(is_array($raw)) {
      foreach ($raw as $key => $value) {
        if($key == 'nuc') {
          $holding[] = 'NUC: ' . $value;
        }
        if($key == 'url') {
          $holding[] = 'URL: ' . l($value['type'], $value['value']);
        }
        if($key == 'callNumber') {
          $holding['children']['data'] = 'Call number:';
          if (is_array($value)) {
            foreach ($value as $cn) {
              if (is_array($cn)) {
                $holding['children']['children'][] = 'localidentifier: ' . $cn['localIdentifier'] . '<br/>' .
                                'value: ' . $cn['value'];
              }
              else {
                $holding['children']['children'][] = $cn;
              }
            }
          }
        }
      }
    }
    return $this->themeHolding($holding);
  }

}
