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
class TrovequeryHandlerFieldContributor extends views_handler_field {

  /**
   * Define the options available.
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['multi_type'] = array(
      'default' => 'ol'
      );
    $options['list_class'] = array(
      'default' => ''
      );
    $options['separator'] = array(
      'default' => ', '
    );
    return $options;
  }

  /**
   * Defines the options form.
   */
  public function options_form(&$form, &$form_state) {
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
        'separator' => t('Simple separator'),
      ),
      '#dependency' => array('edit-options-group-rows' => array(TRUE)),
      '#default_value' => $this->options['multi_type'],
      '#fieldset' => 'multiple_field_settings',
    );
     $form['separator'] = array(
      '#type' => 'textfield',
      '#title' => t('Separator'),
      '#default_value' => $this->options['separator'],
      '#dependency' => array(
        'radio:options[multi_type]' => array('separator'),
      ),
      '#dependency_count' => 1,
      '#fieldset' => 'multiple_field_settings',
    );
    $form['list_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Class to apply to list'),
      '#default_value' => $this->options['list_class'],
      '#fieldset' => 'multiple_field_settings',
      '#weight' => 5,
    );
    parent::options_form($form, $form_state);
  }

  function themeList($arr) {
    return theme_item_list(array(
            'items' => $arr,
            'title' => NULL,
            'type' => $this->options['multi_type'],
            'attributes' => array('class' => $this->options['list_class']),
          ));
  }

  /**
   * Called to add the field to a query.
   */
  public function query() {
    $this->field_alias = $this->real_field;
  }

  public function render($values) {
    if (isset($values->{$this->field}) && is_array($values->{$this->field})) {
      $contributor = array();
      foreach ($values->{$this->field} as $key => $value) {
        $contributor[] = $value;
      }
      if ($this->options['multi_type'] == 'separator') {
        return implode(filter_xss_admin($this->options['separator']), $contributor);
      }
      else {
        return $this->themeList($contributor);
      }
    }
  }

}
