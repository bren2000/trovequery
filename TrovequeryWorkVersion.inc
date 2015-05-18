<?php

use Drupal\trove\troveApi;
use Drupal\trove\TroveApiWorkRecord;

class TrovequeryWorkVersion extends TroveApiWorkRecord {

  public function parse($joins = NULL) {
    $results = array();
    //dpm($this->response);
    foreach ($this->response['work']['version'] as $version) {
      $results[] = $this->buildRow($version);
    }
    //dpm($results);
    return $results;
  }

  function buildRow($version) {
    $row = new stdClass;
    foreach ($version as $key => $value) {
      $row->{$key} = $value;
    }
    return $row;
  }

}
