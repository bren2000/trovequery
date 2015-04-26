<?php

use Drupal\trove\troveApi;
use Drupal\trove\TroveApiNewspaperTitle;

class TrovequeryNewspaperTitlesResult extends TroveApiNewspaperTitle {

  protected $offset = 0;

  protected $pageing = FALSE;

  protected $pageLimit = NULL;

  public function parse($joins = NULL) {
    $results = array();
    //dpm($this->response);

    if($this->pageing) {
      $this->response['response']['records']['newspaper'] = array_slice(
        $this->response['response']['records']['newspaper'],
        $this->offset,
        $this->pageLimit,
        TRUE);
    }

    foreach ($this->response['response']['records']['newspaper'] as $newspaper) {
      $results[] = $this->buildRow($newspaper);
    }
    //dprint_r($this);
    return $results;
  }

  function buildRow($work) {
    $row = new stdClass;
    foreach ($work as $key => $value) {
      $row->{$key} = $value;
    }
    return $row;

  }

  public function setPageing($pageing) {
    $this->pageing = $pageing;
  }

  public function setOffset($offset) {
    $this->offset = $offset;
  }

  public function setPageLimit($page_limit) {
    $this->pageLimit = $page_limit;
  }

}
