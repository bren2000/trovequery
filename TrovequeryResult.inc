<?php

use Drupal\trove\troveApi;
use Drupal\trove\TroveApiResult;

class TrovequeryResult extends TroveApiResult {

  public function parse() {
    $results = array();
    foreach ($this->response['response']['zone'] as $zone) {
      $zoneName = $zone['name'];
      foreach ($zone['records']['work'] as $work) {
        $row = new stdClass;
        $row->zone = $zoneName;
        foreach ($work as $key => $value) {
          $row->{$key} = $value;
        }
        $results[] = $row;
      }
    }
    dpm($results);
    return $results;
  }

}
