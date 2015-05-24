<?php
/**
 * @file
 * Views field handler for basic Trove Query fields.
 */

/**
 * Views field handler for basic Trove Query fields.
 *
 * The only thing we're doing here is making sure the field_alias
 * gets set properly, and that none of the sql-specific query functionality
 * gets called.
 */
class TrovequeryNewspaperRecordHandlerFieldPdf extends views_handler_field_url {

  /**
   * Called to add the field to a query.
   */
  public function query() {
    $this->field_alias = $this->real_field;
    $this->query->add_where('', $this->definition['trovequery']['arg'], $this->definition['trovequery']['value']);
  }

  protected function render_link($link) {
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = $link;
    return !empty($this->options['text']) ? $this->sanitize_value($this->options['text']) : $this->sanitize_value($link, 'url');
  }

  /**
   * Render output based on options settings.
   */
  public function render($values) {
    if (isset($values->{$this->field}) && is_array($values->{$this->field})) {
      $pdfs = array();
      foreach ($values->{$this->field} as $pdf) {
        if (!empty($this->options['display_as_link'])) {
          $pdfs[] = $this->render_link($pdf);
        }
        else {
          $pdfs[] = $this->sanitize_value($pdf, 'url');
        }
      }
      return theme('item_list', array('items' => $pdfs,
        'type' => 'ol',
        'title' => NULL,
        'attributes' => array()));
    }
    else {
      if (!empty($this->options['display_as_link'])) {
        return  $this->render_link($values->{$this->field});
      }
      else {
        return $this->sanitize_value($values->{$this->field}, 'url');
      }
    }
  }

}
