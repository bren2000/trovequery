<?php

use Drupal\trove\troveApi;
use Drupal\trove\TroveApiWorkRecord;

class TrovequeryWork extends TroveApiWorkRecord {

  public function parse($joins = NULL) {
    $results = array();
    dpm($this->response);
    $results[] = $this->buildRow($this->response['work']);
    dpm($results);
    return $results;
  }

  function buildRow($work) {
    $row = new stdClass;
    foreach ($work as $key => $value) {
      $row->{$key} = $value;
    }
    return $row;
  }

}
