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
class TrovequeryHandlerFieldIdentifier extends views_handler_field {

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
    $options['display_as_link'] = array(
      'default' => FALSE
      );
    return $options;
  }

  /**
   * Defines the options form.
   */
  public function options_form(&$form, &$form_state) {
    $form['display_as_link'] = array(
      '#title' => t('Display as link'),
      '#description' => t('Display this identifier as a link.'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['display_as_link'],
      '#weight' => 1,
    );
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
    parent::options_form($form, $form_state);
  }

  /**
   * Called to add the field to a query.
   */
  public function query() {
    $this->field_alias = $this->real_field;
    if (isset($this->definition['trovequery'])) {
      $this->query->add_where('', $this->definition['trovequery']['arg'], $this->definition['trovequery']['value']);
    }
  }

  /**
   * Render output based on options settings.
   */
  public function render($values) {
    if (isset($values->{$this->field_alias})) {
      $rendered_identifiers = array();
      $identifiers = $values->{$this->field_alias};
      foreach ($identifiers as $identifier) {
        switch ($this->field) {
          case 'identifier-1':
            if (in_array($identifier['linktype'], array('fulltext', 'restricted', 'unknown', 'notonline'))) {
              $rendered_identifiers[] = $this->build_identifier($identifier);
            }
            break;

          case 'identifier-2':
            if (in_array('subscription', $identifier)) {
              $rendered_identifiers[] = $this->build_identifier($identifier);
            }
            break;

          case 'identifier-3':
           if (in_array('thumbnail', $identifier)) {
              $rendered_identifiers[] = $this->build_identifier($identifier);
            }
            break;

          default:
            break;
        }
      }
      return theme('item_list', array(
        'items' => $rendered_identifiers,
        'type' => $this->options['multi_type'],
        'title' => NULL,
        'attributes' => array('class' => $this->options['list_class'])
        )
      );
    }
  }

  public function build_identifier($identifier) {
    if (!empty($this->options['display_as_link'])) {
      $text = !empty($identifier['linktext']) ? $this->sanitize_value($identifier['linktext']) : $this->sanitize_value($identifier['value'], 'url');
      $link = l($text, $identifier['value']);
      return $link;
    }
    else {
      return $this->sanitize_value($identifier['value'], 'url');
    }
  }

}
