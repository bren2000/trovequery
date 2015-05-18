<?php

use Drupal\trove\troveApi;
use Drupal\trove\TroveApiList;

class TrovequeryList extends TroveApiList {

  public function parse($joins = NULL) {
    $results = array();
    //dpm($this->response);
    $results[] = $this->buildRow($this->response['list']);
    //dpm($results);
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
