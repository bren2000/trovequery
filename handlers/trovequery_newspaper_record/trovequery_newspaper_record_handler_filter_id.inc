<?php
/**
 * @file
 * Definition of trovequery_handler_filter_facet.
 */

class TrovequeryNewspaperRecordHandlerFilterString extends views_handler_filter_string {

  /**
   * Called to add the field to a query.
   */
  public function query() {
    if (isset($this->value)) {
      $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], $this->value);
    }
  }

}
