<?php
/**
 * @file
 * Views field handler for basic Trove Query fields.
 */

class TrovecontribHandlerFilterQuery extends views_handler_filter_string {

  public $always_required = TRUE;

  /**
   * Called to add the field to a query.
   */
  public function query() {
    if (isset($this->value, $this->definition['trovecontrib'])) {
      $this->query->args['method'] = $this->definition['trovecontrib']['method'];
      if (is_array($this->value)) {
        $this->query->args[$this->definition['trovecontrib']['arg']] = implode($this->value, ',');
      }
      else {
        $this->query->args[$this->definition['trovecontrib']['arg']] = $this->value;
      }
    }
  }

  /**
   * Set the operator.
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
