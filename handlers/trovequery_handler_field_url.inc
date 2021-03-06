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
class TrovequeryHandlerFieldUrl extends views_handler_field_url {

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
    if (isset($values->{$this->field})) {
      $value = $values->{$this->field};
      if (!empty($this->options['display_as_link'])) {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = $value;
        $text = !empty($this->options['text']) ? $this->sanitize_value($this->options['text']) : $this->sanitize_value($value, 'url');
        return $text;
      }
      else {
        return $this->sanitize_value($value, 'url');
      }
    }
  }

}
