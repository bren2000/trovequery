<?php

use Drupal\trove\troveApi;
//use Drupal\trove\troveApiResult;
use Drupal\trove\TroveApiNewspaperTitle;

class TrovequeryNewspaperTitlesResult extends TroveApiNewspaperTitle {

  public function parse() {
    $results = array();
    dpm($this->response);
    foreach ($this->response['response']['records']['newspaper'] as $newspaper) {
      $results[] = $this->buildRow($newspaper);
    }
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
