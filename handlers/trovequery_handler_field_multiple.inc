<?php

class TrovequeryHandlerFieldMultiple extends  views_handler_field_prerender_list {
/**
   * An array to store field renderable arrays for use by render_items.
   *
   * @var array
   */
  public $items = array();

  /**
   * Does the field supports multiple field values.
   *
   * @var bool
   */
  public $multiple;


  function init(&$view, &$options) {
    parent::init($view, $options);
  }

  /**
   * Called to add the field to a query.
   **/
  function query($use_groupby = FALSE) {
    $this->field_alias = $this->real_field;
  }

  function pre_render(&$values) {
    $this->items = array();
    $holdings = array();

    foreach ($values as $result) {
      $items[] = $this->get_value($result, NULL, TRUE);
    }
    dpm($holdings);

    if (is_array($holdings)) {
     foreach ($holdings as $key => $value) {
       switch ($key) {
         case 'url':
           //dpm($value);
           $this->items['url'] = l($value['type'], $value['value']);
           break;

         default:
           break;
       }
     }
    }

  }

  function render_item($count, $item) {
    dpm($item);
    return $items['url'];
  }

}

