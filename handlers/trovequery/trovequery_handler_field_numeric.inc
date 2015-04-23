<?php

class TrovequeryHandlerFieldNumeric extends views_handler_field_numeric {

  /**
   * Called to add the field to a query.
   */
  public function query() {
    $this->field_alias = $this->real_field;
  }

}
