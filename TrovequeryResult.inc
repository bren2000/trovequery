<?php

use Drupal\trove\troveApi;
use Drupal\trove\TroveApiResult;

class TrovequeryResult extends TroveApiResult {

  public function parse($joins = NULL) {
    $results = array();
    foreach ($this->response['response']['zone'] as $zone) {
      $zoneName = $zone['name'];
      if (isset($zone['records']['work'])) {
        foreach ($zone['records']['work'] as $work) {
          $results[] = $this->buildRow($work, $zoneName);
        }
      } elseif (isset($zone['records']['article'])) {
        foreach ($zone['records']['article'] as $work) {
          $results[] = $this->buildRow($work, $zoneName);
        }
      // } elseif (isset($zone['records']['people'])) {
      //   foreach ($zone['records']['people'] as $work) {
      //     $results[] = $this->buildRow($work, $zoneName);
      //   }
      } elseif (isset($zone['records']['list'])) {
        foreach ($zone['records']['list'] as $work) {
          $results[] = $this->buildRow($work, $zoneName);
        }
      }
    }
    return $results;
  }

  function buildRow($work, $zone) {
    $row = new stdClass;
    $row->zone = $zone;
    foreach ($work as $key => $value) {
      $row->{$key} = $value;
    }
    //dpm(get_class_methods('TrovequeryResult'));
    return $row;
  }

}
