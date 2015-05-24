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
class TrovequeryHandlerFieldComment extends TrovequeryHandlerFieldString {


  /**
   * Define the options available.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['render_html'] = array(
      'default' => FALSE
      );
    $options['multi_type'] = array(
      'default' => 'ol'
      );
    $options['list_class'] = array(
      'default' => ''
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
    $form['render_html'] = array(
      '#title' => t('Render HTML'),
      '#description' => t('By default, HTML returned from the Trove API is escaped and will not render. Checking this option will disable this behavior and allow the HTML to render. Note that this could be a security problem if Trove filtering of dangerous HTML has problems.'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['render_html'],
      '#weight' => 1,
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
    $this->query->add_where('', $this->definition['trovequery']['arg'], $this->definition['trovequery']['value']);
  }

  /**
   * Render the field.
   *
   * Comments are contained in a nested array with a valua, an author, and a
   * last updated timestanp.
   */
  public function render($values) {
    if(isset($values->{$this->field})) {
      $comments = '';
      $class = isset($this->options['list_class'])? " class=\"" . $this->options['list_class'] . "\"" : '';
      $value = $values->{$this->field};
      if (is_array($value)) {
        $comments .= '<' . $this->options['multi_type'] . $class . '>';
        foreach ($value as $key => $comment) {
          $comments .= '<li>';
          if ($this->options['render_html']) {
            $comments .= $comment['value'];
          }
          else {
            $comments .= check_plain($comment['value']);
          }
          $comments .= '<p><i>by: ' . $comment['by'];
          $comments .= ', last updated: ' . $comment['lastupdated'] . '</i></p>';
          $comments .= '</li>';
        }
        $comments .= '</' . $this->options['multi_type'] . '>';
        $value = $comments;
      } else {
        return NULL;
      }
      return $value;
    }
    return NULL;
  }

  function prepareComments() {

  }

  function themeComment() {

  }

}
