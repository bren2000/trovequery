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
 *
 *  Some trove api response fields  can contain HTML, which is typically run
 * through check_plain() and thus not rendered. Here we give the user the
 * option to override this behavior and render the HTML.
 */

class TrovequeryNewspaperRecordHandlerFieldLastCorrection extends views_handler_field {

  /**
   * Called to add the field to a query.
   */
  public function query() {
    $this->field_alias = $this->real_field;
    $this->query->add_where('', $this->definition['trovequery']['arg'], $this->definition['trovequery']['value']);
  }

  /**
   * Render output based on options settings.
   */
  public function render($values) {
    if (isset($values->{$this->field}) && is_array($values->{$this->field})) {
      $last_updated = array();
      $tmp = 'By: ' . $values->{$this->field}['by'];
      $tmp .= ' Date: ' . $values->{$this->field}['lastupdated'];
      $last_updated[] = $tmp;

      return theme('item_list', array('items' => $last_updated,
        'type' => 'ol',
        'title' => NULL,
        'attributes' => array()));
    }
  }

}
