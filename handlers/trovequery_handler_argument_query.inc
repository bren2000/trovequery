<?php

class TrovequeryHandlerArgumentQuery   extends views_handler_argument {
  // /**
  //  * The following functions express SQLisms in the base handler class. They
  //  * are all blanked out in overrides.
  //  */
  // function default_summary($order, $by = NULL) { }
  // function summary_query() { }
  // function summary_name_field() { }
  // function summary_basics($count_field = TRUE) { }
  // function summary_sort($order, $by = NULL) { }

  function query($group_by = false) {
    $this->field_alias = $this->real_field;
    //dpm($this->argument . ' : ' . $this->definition['trovequery']);
    if (isset($this->argument, $this->definition['trovequery'])) {
      if (isset($this->definition['trovequery'])) {
        $this->query->add_where('', $this->definition['trovequery']['arg'], $this->argument);
      }
    }
  }

    /**
   * Override default_actions() to remove summary actions.
   */
  function default_actions($which = NULL) {
    $defaults = array(
      'ignore' => array(
        'title' => t('Display all values'),
        'method' => 'default_ignore',
        'breadcrumb' => TRUE, // generate a breadcrumb to here
      ),
      'not found' => array(
        'title' => t('Hide view / Page not found (404)'),
        'method' => 'default_not_found',
        'hard fail' => TRUE, // This is a hard fail condition
      ),
      'empty' => array(
        'title' => t('Display empty text'),
        'method' => 'default_empty',
        'breadcrumb' => TRUE, // generate a breadcrumb to here
      ),
      'default' => array(
        'title' => t('Provide default argument'),
        'method' => 'default_default',
        'form method' => 'default_argument_form',
        'has default argument' => TRUE,
        'default only' => TRUE, // this can only be used for missing argument, not validation failure
      ),
    );
    if ($which) {
      if (in_array($which, array('ignore', 'not found', 'empty', 'default'))) {
        return parent::default_actions($which);
      }
      return;
    }
    $actions = parent::default_actions();
    unset($actions['summary asc']);
    unset($actions['summary desc']);
    unset($actions['summary asc by count']);
    unset($actions['summary desc by count']);
    //dpm($actions);
    return $actions;
  }


}
