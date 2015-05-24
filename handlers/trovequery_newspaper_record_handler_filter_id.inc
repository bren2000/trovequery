<?php
/**
 * @file
 * Definition of trovequery_handler_filter_facet.
 */

class TrovequeryNewspaperRecordHandlerFilterString extends views_handler_filter_string {

  var $always_required = TRUE;

  /**
   * Called to add the field to a query.
   */
  public function query() {
    if (isset($this->value)) {
      $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], $this->value);
    }
  }


  /**
   * Overrides views_handler_filter_string::operators.
   */
  public function operators() {
    $operators = array(
      '=' => array(
        'title' => t('Is equal to'),
        'short' => t('='),
        'method' => 'op_equal',
        'values' => 1,
      ),
    );
    return $operators;
  }

  /**
   * Handle the 'left' side of the exposed options form.
   *
   * We remove the operator option since we only have one operators available.
   */
  public function expose_form_left(&$form, &$form_state) {
    parent::expose_form_left($form, $form_state);
    $form['expose']['use_operator']['#access'] = FALSE;
    $form['expose']['operator']['#access'] = FALSE;
  }

  /**
   * Remove the optional checkbox since it makes no sense in our context.
   */
  public function expose_form_right(&$form, &$form_state) {
    parent::expose_form_right($form, $form_state);
    $form['expose']['optional']['#access'] = FALSE;
  }

  /**
   * Remove the case sensitive option, since it is irrelevant.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['case']['#access'] = FALSE;
  }

}
