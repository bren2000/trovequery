<?php
/**
 * @file
 * Trovequery Rlationship.
 */

class TrovequeryNewspaperRelationship extends views_handler_relationship {
  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    dpm($this->definition);
    $right_table = $this->definition['base'];
    $method = $this->definition['trovequery']['method'];
    $arg = $this->definition['trovequery']['arg'];

    $this->query->add_relationship($right_table, $method, $arg);
  }



}
