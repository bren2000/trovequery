<?php
/**
 * @file
 * Views query plugin for Trove Query.
 */

use Drupal\trove\TroveApi;
use Drupal\trove\TroveApiContributor;
use Drupal\trove\TroveApiResult;
use Drupal\trove\TroveApiNewspaperTitle;
use Drupal\trove\TroveApiNewspaperRecord;
use Drupal\trove\TroveApiList;
use Drupal\trove\TroveApiWorkRecord;


/**
 * Views query plugin for the Trove Query results.
 */
class TrovePluginQuery extends views_plugin_query {

  private $table;
  /*
   * An array of sections of the WHERE query. Each section is in itself
   * an array of pieces. All AND by default.
   */
  public $where = array();

  /*
   * Array of all encountered errors.
   *
   * Each of these is fatal.
   *
   * A non-empty $errors property will
   * result in an empty result being returned.
   *
   * @var array
   */
  protected $errors = array();

  /*
   * Whether to abort the search instead of executing it.
   *
   * @var bool
   */
  protected $abort = FALSE;

  protected $joins = array();

  /**
   * Constructor; Create the basic query object and fill with default values.
   */
  public function init($base_table, $base_field, $options) {
    // parent::init($base_table, $base_field, $options);
    // Required fields are fields that may not be empty in rows of the
    // resultset. Any rows with empty required fields are dropped *prior* to
    // paging.
    $this->required_fields = array();
    $this->ensure_table($base_table);
  }
  /**
   * Ensure table.
   */
  public function ensure_table($table, $relationship = NULL, $join = NULL) {
    if (!isset($this->request)) {
      $this->table = $table;
      switch ($table) {

        case TROVE_NEWSPAPER_TITLES:
          $this->request = new TrovequeryNewspaperTitlesResult($table);
          break;

        case TROVE_RESULT:
          $this->request = new TrovequeryResult($table);
          break;

        case TROVE_CONTRIBUTOR:
          $this->request = new TrovequeryContributors($table);
          break;

        case TROVE_RECORD_NEWSPAPER:
          $this->request = new TrovequeryNewspaper($table);
          break;

        case TROVE_RECORD_WORK:
          $this->request = new TrovequeryWork($table);
          break;

        case TROVE_RECORD_LIST:
          $this->request = new TrovequeryList($table);
          break;

        default:
          break;
      }

    }
    //dpm($this);
  }

  /**
   * Get the arguments attached to the WHERE clauses of this query.
   */
  public function get_where_args() {

    $args = array();
    foreach ($this->where as $group => $where) {
      $args = array_merge($args, $where['args']);
    }

    return $args;
  }

  /**
   * Construct the "WHERE" or "HAVING" part of the query.
   *
   * As views has to wrap the conditions from arguments with AND, a special
   * group is wrapped around all conditions. This special group has the ID 0.
   * There is other code in filters which makes sure that the group IDs are
   * higher than zero.
   */
  public function build_condition($where = 'where') {
    $has_condition = FALSE;
    $has_arguments = FALSE;
    $has_filter = FALSE;

    $main_group = db_and();
    $filter_group = $this->group_operator == 'OR' ? db_or() : db_and();

    foreach ($this->$where as $group => $info) {

      if (!empty($info['conditions'])) {
        $sub_group = $info['type'] == 'OR' ? db_or() : db_and();
        foreach ($info['conditions'] as $key => $clause) {
          // DBTNG doesn't support to add the same subquery twice to the main
          // query and the count query, so clone the subquery to have two
          // instances of the same object. - http://drupal.org/node/1112854
          if (is_object($clause['value']) && $clause['value'] instanceof SelectQuery) {
            $clause['value'] = clone $clause['value'];
          }
          if ($clause['operator'] == 'formula') {
            $has_condition = TRUE;
            $sub_group->where($clause['field'], $clause['value']);
          }
          else {
            $has_condition = TRUE;
            $sub_group->condition($clause['field'], $clause['value'], $clause['operator']);
          }
        }

        // Add the item to the filter group.
        if ($group != 0) {
          $has_filter = TRUE;
          $filter_group->condition($sub_group);
        }
        else {
          $has_arguments = TRUE;
          $main_group->condition($sub_group);
        }
      }
    }

    if ($has_filter) {
      $main_group->condition($filter_group);
    }

    if (!$has_arguments && $has_condition) {
      return $filter_group;
    }
    if ($has_arguments && $has_condition) {
      return $main_group;
    }
  }

  /**
   * Add a simple WHERE clause to the query.
   */
  public function add_where($group, $field, $value = NULL, $operator = NULL) {
    // Ensure all variants of 0 are actually 0. Thus '', 0 and NULL are all
    // the default group.
    if (empty($group)) {
      $group = 0;
    }

    // Check for a group.
    if (!isset($this->where[$group])) {
      $this->set_where_group('AND', $group);
    }

    $this->where[$group]['conditions'][] = array(
      'field' => $field,
      'value' => $value,
    );
  }

  function add_relationship($alias, $join, $base, $link_point = NULL) {
    $this->joins[$this->table] = array('method' => $base, 'arg' => $link_point);
  }

  /**
   * Set a filter on the query object.
   */
  public function add_filter($field, $value, $operator = '=') {
    //dpm($this->request);
    // $this->ensure_table($table);
    $this->request->setFilter($field, $value);
  }

  public function add_field($table, $field, $alias = '', $params = array()) {
    //do nothing
  }

  /**
   * Builds the necessary info to execute the query.
   */
  public function build(&$view) {
    $this->view = $view;
    if (!empty($this->where)) {
      foreach ($this->where as $group_id => $group) {
        if (!empty($group['conditions'])) {
          foreach ($group['conditions'] as $condition) {
            $this->add_filter($condition['field'], $condition['value']);
          }
        }
      }
    }

  }


  /**
   * Executes query and fill the associated view object with values.
   *
   * Values to set: $view->result, $view->total_rows, $view->execute_time,
   * $view->pager['current_page'].
   */
  public function execute(&$view) {

    //dpm($this->pager);

    if ($this->errors || $this->abort) {
      if (error_displayable()) {
        foreach ($this->errors as $msg) {
          drupal_set_message(check_plain($msg), 'error');
        }
      }
      return;
    }
    try {
      $start = microtime(TRUE);

      if (empty($this->args['method'])) {
        //return;
      }

      // Setup pager.
      $view->init_pager();
      if ($this->pager->use_pager()) {
        $this->pager->set_current_page($view->current_page);
      }

      //dpm($this->pager);

      // Switch for different paging methods. Only TROVE_RESULT API methods
      // support paged results.
      switch ($this->table) {
        case TROVE_RESULT:
          $this->add_filter('n', $this->pager->options['items_per_page']);
          $this->add_filter('s', $this->pager->current_page * $this->pager->options['items_per_page']);
          break;

        case TROVE_NEWSPAPER_TITLES:
          if($this->pager->use_pager()) {
            $this->request->setPageing(TRUE);
            $this->request->setPageLimit($this->pager->options['items_per_page']);
            $this->request->setOffset($this->pager->current_page * $this->pager->options['items_per_page']);
          }

        default:
          break;
      }

      $this->request->query();

      $joins = array();
      if (isset($this->joins)) {
        $joins = $this->joins;
      }
      $view->result = $this->request->parse($joins);
      //$view->result = $this->request->parse(NULL);

      $this->pager->total_items = $this->request->getTotalResults();
      $this->pager->update_page_info();
    }
    catch (Exception $e) {
      $this->errors[] = $e->getMessage();
      // Recursion to get the same error behaviour as above.
      return $this->execute($view);
    }
    $view->execute_time = microtime(TRUE) - $start;
  }

}
