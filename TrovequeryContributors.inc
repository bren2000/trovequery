<?php

use Drupal\trove\troveApi;
use Drupal\trove\TroveApiContributor;

class TrovequeryContributors extends TroveApiContributor {

  public function parse($joins = NULL) {
    $results = array();
    //dpm($this->response);
    foreach ($this->response['response']['contributor'] as $contributor) {
      $results[] = $this->buildRow($contributor);
    }
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
