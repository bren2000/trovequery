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
class TrovequeryHandlerFieldImage extends views_handler_field {

  /**
   * Called to add the field to a query.
   */
  public function query() {
    $this->field_alias = $this->real_field;
  }

  /**
   * Render the field.
   */
  public function render($values) {
    if (isset($values->{$this->real_field}) && is_array($values->{$this->real_field})) {
      foreach ($values->{$this->real_field} as $identifier) {
        if (isset($identifier['linktype']) && $identifier['linktype'] == 'thumbnail') {
          $path = $identifier['value'];
        }
      }
      if(isset($path)) {
        $variables = array(
          'path' => $path,
        );
        return theme('image', $variables);
      }
    }
  }

}
