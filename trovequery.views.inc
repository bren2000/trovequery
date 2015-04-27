<?php
/**
 * @file
 * Definition of trovequery view.
 */

/**
 * Implements hook_views_plugins().
 */

use Drupal\trove\TroveApi;
use Drupal\trove\TroveApiContributor;
use Drupal\trove\TroveApiResult;
use Drupal\trove\TroveApiRecord;
use Drupal\trove\TroveApiNewspaperTitle;

function trovequery_views_plugins() {
  $plugin = array();
  $plugin['query']['TrovePluginQuery'] = array(
    'title' => t('Trove API Query'),
    'help' => t('Trove API query object.'),
    'handler' => 'TrovePluginQuery',
    'use pager' => TRUE,
  );
  return $plugin;
}

/**
 * Implements hook_views_data().
 */
function trovequery_views_data() {
  $data = array();

  $data[TROVE_RESULT]['table']['group']  = t('Trove Results');
  $data[TROVE_RESULT]['table']['base'] = array(
    'title' => t('Trove Results'),
    'help' => t('Query Trove.'),
    'field' => 'title',
    'query class' => 'TrovePluginQuery',
  );

  $data[TROVE_RESULT]['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of this result.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldTitle',
    ),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterInclude',
      'help' => t('Keyword filter on the title field.'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => TROVE_RESULT,
        'index' => 'title',
      ),
    ),
    'argument' => array(
      'handler' => 'TrovequeryHandlerArgumentTitle',
      'trovequery' => array(
        'arg' => 'q',
        'method' => TROVE_RESULT,
        'index' => 'title',
      ),
    ),
  );

  $data[TROVE_RESULT]['isbn'] = array(
    'title' => t('ISBN'),
    'help' => t('The ISBN for this result.'),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterIncludeString',
      'help' => t('Filter on ISBNs.'),
      'label' => t('The ISBN of the item.'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => TROVE_RESULT,
        'index' => 'isbn',
      ),
    ),
  );

  $data[TROVE_RESULT]['issn'] = array(
    'title' => t('ISSN'),
    'help' => t('The ISSN for this result.'),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterIncludeString',
      'help' => t('Filter on ISSNs.'),
      'label' => t('The ISSN of the item.'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => TROVE_RESULT,
        'index' => 'issn',
      ),
    ),
  );

   $data[TROVE_RESULT]['relevance'] = array(
    'title' => t('Relevance'),
    'help' => t('The relevance of this result in the search.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldRelevance',
    ),
  );


  $data[TROVE_RESULT]['id'] = array(
    'title' => t('ID'),
    'help' => t('The work ID.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldId',
    ),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterIncludeString',
      'help' => t('Filter on work number (ID).'),
      'label' => t('The work ID.'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => TROVE_RESULT,
        'index' => 'id',
      ),
    ),
    'relationship' => array(
      'handler' => 'TrovequeryRelationship',
      'base' => TROVE_RECORD_NEWSPAPER,
      'trovequery' => array(
        'arg' => 'q',
        'method' => TROVE_RECORD_NEWSPAPER,
       ),
      'label' => t('id'),
    ),
  );

  $data[TROVE_RESULT]['tag'] = array(
    'title' => t('Tag'),
    'help' => t('Tags associated with this item.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldTag',
      'trovequery' => array(
        'arg' => 'include',
        'value' => 'tags',
      ),
    ),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterInclude',
      'help' => t('Limit results to tagged items.'),
      'label' => t('The item has tags?'),
      'use equal' => TRUE,
      'trovequery' => array(
        'arg' => 'q',
        'method' => TROVE_RESULT,
        'index' => 'has',
        'value' => 'tags',
      ),
    ),
  );

  $data[TROVE_RESULT]['date'] = array(
    'title' => t('Date'),
    'help' => t("Date of publication"),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldDate',
    ),
  );

  $data[TROVE_RESULT]['isPartOf'] = array(
    'title' => t('Is part of'),
    'help' => t("For articles, the journal or periodical this article belongs to (if known)."),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldPartOf',
    ),
  );

  $data[TROVE_RESULT]['subject'] = array(
    'title' => t('Subject'),
    'help' => t("Subjects describe what a work is about. A variety of vocabularies are used, including LCSH."),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldSubject',
      'trovequery' => array(
        'arg' => 'reclevel',
        'value' => 'full',
      ),
    ),
  );

  $data[TROVE_RESULT]['abstract'] = array(
    'title' => t('Abstract'),
    'help' => t("A description, summary, abstract, or first few words of this work, if available."),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldType',
      'trovequery' => array(
        'arg' => 'reclevel',
        'value' => 'full',
      ),
    ),
  );

  $data[TROVE_RESULT]['language'] = array(
    'title' => t('Language'),
    'help' => t("The language/s this work is available in. More than one language element may be present."),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldType',
      'trovequery' => array(
        'arg' => 'reclevel',
        'value' => 'full',
      ),
    ),
  );

  $data[TROVE_RESULT]['wikipedia'] = array(
    'title' => t('Wikipedia articles'),
    'help' => t("Links to any known associated wikipedia articles. Includes markup."),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldSnippet',
      'trovequery' => array(
        'arg' => 'reclevel',
        'value' => 'full',
      ),
    ),
  );

  $data[TROVE_RESULT]['versionCount'] = array(
    'title' => t('Version count'),
    'help' => t("A work is made up of one or more versions. This is the number of versions that are grouped together to make up this work."),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldString',
    ),
  );

  $data[TROVE_RESULT]['tagCount'] = array(
    'title' => t('Tag count'),
    'help' => t("Tags can be placed on the work as a whole, or on a particular version that is part of the work. This element shows the number of public tags on this work or any of its versions. There is no way to access private tags through the API."),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldString',
    ),
  );

  $data[TROVE_RESULT]['tagCount'] = array(
    'title' => t('Tag count'),
    'help' => t("Tags can be placed on the work as a whole, or on a particular version that is part of the work. This element shows the number of public tags on this work or any of its versions. There is no way to access private tags through the API."),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldString',
    ),
  );

  $data[TROVE_RESULT]['comments_date'] = array(
    'title' => t('Comments date'),
    'help' => t('Comments posted on this date.'),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterDate',
      'help' => t('Limit results to items with comments posted in this date range.'),
      'label' => t('Comments date range'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => TROVE_RESULT,
        'index' => 'commentlastupdated',
      ),
    ),
  );

  $data[TROVE_RESULT]['lastupdated'] = array(
    'title' => t('Last updated'),
    'help' => t('The date the item was last updated.'),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterDate',
      'help' => t('Limit results to items updated before, after or between certain dates'),
      'label' => t('Last updated date'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => TROVE_RESULT,
        'index' => 'lastupdated',
      ),
    ),
  );

  $data[TROVE_RESULT]['relevance_sort'] = array(
    'title' => t('Relevance'),
    'sort' => array(
      'handler' => 'TrovequeryHandlerSortRelevance',
      'help' => t('Sort results by relevance.'),
      'sort method' => 'sortby',
      'sort order' => 'relevance',
    ),
  );
  $data[TROVE_RESULT]['date_sort'] = array(
    'title' => t('Date sort'),
    'sort' => array(
      'handler' => 'TrovequeryHandlerSortDate',
      'help' => t('Sort results by relevance.'),
      'sort method' => 'sortby',
      'sort order' => 'date',
    ),
  );

  $data[TROVE_RESULT]['comment'] = array(
    'title' => t('Comment'),
    'help' => t('Comments associated with this item.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldComment',
      'trovequery' => array(
        'arg' => 'include',
        'value' => 'comments',
      ),
    ),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterInclude',
      'help' => t('Limit results to items with comments.'),
      'label' => t('The item has comments?'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => TROVE_RESULT,
        'index' => 'has',
        'value' => 'comments',
      ),
    ),
  );

  $data[TROVE_RESULT]['troveUrl'] = array(
    'title' => t('Trove URL'),
    'help' => t('The Trove URL of this result.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldUrl',
    ),
  );

  $data[TROVE_RESULT]['issued'] = array(
    'title' => t('Date issued'),
    'help' => t('The year/s this work was issued, created or published. May also include month and day. May be a range.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldString',
    ),
  );

  $data[TROVE_RESULT]['type'] = array(
    'title' => t('Type'),
    'help' => t('The format of this work. There may be more than one type element.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldType',
      'trovequery' => array(
        'arg' => 'include',
        'value' => 'holdings',
      ),
    ),
  );

  $data[TROVE_RESULT]['query'] = array(
    'title' => t('Query'),
    'help' => t('The query keywords'),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterQuery',
      'trovequery' => array(
        'arg' => 'q',
        'method' => TROVE_RESULT,
      ),
    ),
  );

  $data[TROVE_RESULT]['facet_article'] = array(
    'title' => t('Article format'),
    'help' => t('The article format facet to filter on'),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterFacet',
      'trovequery' => array(
        'arg' => 'l-format',
        'method' => TROVE_RESULT,
        'facets_method' => 'trove_get_facets_format',
      ),
    ),
  );

  $data[TROVE_RESULT]['facet_availaility'] = array(
    'title' => t('Availability'),
    'help' => t('Filter on whether the item is online or not.'),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterFacet',
      'trovequery' => array(
        'arg' => 'l-availability',
        'method' => TROVE_RESULT,
        'facets_method' => 'trove_get_facets_availability',
      ),
    ),
  );

  $data[TROVE_RESULT]['zone'] = array(
    'title' => t('Zone'),
    'help' => t('The trove zone this work belongs to'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldTitle',
    ),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterZone',
      'help' => t('Filter results based on trove zones'),
      'trovequery' => array(
        'arg' => 'zone',
        'method' => TROVE_RESULT,
        'zones_method' => 'trove_get_zones',
      ),
    ),
  );

  $data[TROVE_RESULT]['contributor'] = array(
    'title' => t('Contributor'),
    'help' => t('The contibutor of this work.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldContributor',
    ),
  );

  $data[TROVE_RESULT]['snippet'] = array(
    'title' => t('Snippet'),
    'help' => t('A snippet of this work.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldSnippet',
    ),
  );

  $data[TROVE_RESULT]['category'] = array(
    'title' => t('Category'),
    'help' => t('The category of this work, only applies to Nespapers.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldString',
    ),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterFacet',
      'trovequery' => array(
        'arg' => 'l-category',
        'method' => TROVE_RESULT,
        'facets_method' => 'trove_get_facets_category',
      ),
    ),
  );

  $data[TROVE_RESULT]['list'] = array(
    'title' => t('Lists'),
    'help' => t('Lists associated with this item.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldList',
      'trovequery' => array(
        'arg' => 'include',
        'value' => 'lists',
      ),
    ),
  );

  $data[TROVE_RESULT]['identifier'] = array(
    'title' => t('Image'),
    'help' => t('A thumbnail image for the work - if available.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldImage',
    ),
  );

  $data[TROVE_RESULT]['holdingsCount'] = array(
    'title' => t('Holdings count'),
    'help' => t('The number of organisations (libraries, archives, etc) who have a copy of this work.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldNumeric',
      'click sortable' => FALSE,
    ),
  );

  $data[TROVE_RESULT]['holding'] = array(
    'title' => t('Holdings'),
    'help' => t('The NUC symbol of an organisation with a copy of this work.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldHolding',
    ),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterHolding',
      'help' => t('Filter by NUC (ID of organisation who has a copy of this work).'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => TROVE_RESULT,
        'index' => 'nuc',
        'index_method' => 'trove_get_index_contributors',
      ),
    ),
  );

  $data[TROVE_NEWSPAPER_TITLES]['table']['group']  = t('Trove newspaper titles');
  $data[TROVE_NEWSPAPER_TITLES]['table']['base'] = array(
    'title' => t('Trove newspaper titles'),
    'help' => t('Query Trove newspaper titles.'),
    'field' => 'title',
    'query class' => 'TrovePluginQuery',
  );

  $data[TROVE_NEWSPAPER_TITLES]['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of this newspaper.'),
    'field' => array(
      'handler' => 'TrovequeryNewspaperTitlesHandlerFieldState',
      ),
    );

  $data[TROVE_NEWSPAPER_TITLES]['id'] = array(
    'title' => t('Id'),
    'help' => t('ID of the newspaper title.'),
    'field' => array(
      'handler' => 'TrovequeryNewspaperTitlesHandlerFieldState',
      ),
    );

  $data[TROVE_NEWSPAPER_TITLES]['issn'] = array(
    'title' => t('issn'),
    'help' => t('International Standard Serial Number.'),
    'field' => array(
      'handler' => 'TrovequeryNewspaperTitlesHandlerFieldState',
      ),
    );

  $data[TROVE_NEWSPAPER_TITLES]['troveUrl'] = array(
    'title' => t('troveUrl'),
    'help' => t('A link to view this newspaper title in Trove.'),
    'field' => array(
      'handler' => 'TrovequeryNewspaperTitlesHandlerFieldUrl',
      ),
    );

  $data[TROVE_NEWSPAPER_TITLES]['startDate'] = array(
    'title' => t('Date: start'),
    'help' => t('The publication date of the easliest issue of this newspaper available in Trove.'),
    'field' => array(
      'handler' => 'TrovequeryNewspaperTitlesHandlerFieldDate',
      ),
    );

  $data[TROVE_NEWSPAPER_TITLES]['endDate'] = array(
    'title' => t('Date: end'),
    'help' => t('The publication date of the  most recently issue of this newspaper available in Trove.'),
    'field' => array(
      'handler' => 'TrovequeryNewspaperTitlesHandlerFieldDate',
      ),
    );

  $data[TROVE_NEWSPAPER_TITLES]['state'] = array(
    'title' => t('State'),
    'help' => t('The state the newspaper was primarily published in.'),
    'field' => array(
      'handler' => 'TrovequeryNewspaperTitlesHandlerFieldState',
      ),
    'filter' => array(
      'handler' => 'TrovequeryNewspapersTitlesHandlerFilterState',
      'help' => t('Filter results based on state.'),
      'trovequery' => array(
        'arg' => 'state',
        'method' => TROVE_NEWSPAPER_TITLES,
        'optionsMethod' => 'trove_get_states',
        ),
      ),
    );

  $data[TROVE_CONTRIBUTOR]['table']['group']  = t('Trove contributos');
  $data[TROVE_CONTRIBUTOR]['table']['base'] = array(
    'title' => t('Trove contributors'),
    'help' => t('Query Trove\'s contributors.'),
    'field' => 'name',
    'query class' => 'TrovePluginQuery',
  );

  $data[TROVE_CONTRIBUTOR]['name'] = array(
    'title' => t('Name'),
    'help' => t('The name of this contributor.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerField',
      ),
    );

  $data[TROVE_CONTRIBUTOR]['id'] = array(
    'title' => t('ID'),
    'help' => t('The id of this contributor.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerField',
      ),
    );

  $data[TROVE_CONTRIBUTOR]['nuc'] = array(
    'title' => t('NUC'),
    'help' => t('The NUC of this contributor.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldType',
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['table']['group']  = t('Trove newspaper record');
  $data[TROVE_RECORD_NEWSPAPER]['table']['base'] = array(
    'title' => t('Trove newspaper record'),
    'help' => t('Query Trove newspaper records.'),
    'field' => 'title',
    'query class' => 'TrovePluginQuery',
  );

  $data[TROVE_RECORD_NEWSPAPER]['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of this newspaper.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldTitle',
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['id'] = array(
    'title' => t('ID'),
    'help' => t('The id of this newspaper.'),
    'field' => array(
      'handler' => 'TrovequeryNewspaperTitlesHandlerFieldState',
      ),
    'filter' => array(
      'handler' => 'TrovequeryNewspaperRecordHandlerFilterString',
      'trovequery' => array(
        'arg' => 'id',
        ),
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['lastCorrection'] = array(
    'title' => t('Last correction'),
    'help' => t('Further information about the most recent correction to this article. The correction itself is not included.'),
    'field' => array(
      'handler' => 'TrovequeryNewspaperRecordHandlerFieldLastCorrection',
      'trovequery' => array(
        'arg' => 'reclevel',
        'value' => 'full',
        ),
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['heading'] = array(
    'title' => t('Heading'),
    'help' => t('The article heading.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerField',
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['category'] = array(
    'title' => t('Category'),
    'help' => t('The type of article.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerField',
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['edition'] = array(
    'title' => t('Edition'),
    'help' => t('If this article appeared in a special newspaper/periodical edition, the name of that edition is included here.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerField',
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['date'] = array(
    'title' => t('Date'),
    'help' => t('When the issue this article belongs to was published. YYYY-MM-DD.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerField',
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['page'] = array(
    'title' => t('Page'),
    'help' => t('Determine the sequence of delivery of the pages in an issue.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerField',
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['pageSequence'] = array(
    'title' => t('Page Sequence'),
    'help' => t('In addition to the page sequence number, indicates if the article is part of a supplement/section.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerField',
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['pageLabel'] = array(
    'title' => t('Page Label'),
    'help' => t('Sometimes used to indicate what is printed on the page, or perhaps the page number within a section/supplement. Sometimes numeric and sometimes not.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerField',
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['articleText'] = array(
    'title' => t('Article text'),
    'help' => t('The full text of the article, including all corrections. Article paragraphs are enclosed in a <p> element.  Lines are enclosed by <span>. Article text is not available for some articles, such as those from the Australian Women’s Weekly, and articles with a status of “coming soon”. For these articles, there will be no articleText element present'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldInclude',
      'trovequery' => array(
        'arg' => 'include',
        'value' => 'articletext',
        ),
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['comment'] = array(
    'title' => t('Comment'),
    'help' => t('A public comment added to the article. There may be more than one comment element.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldComment',
      'trovequery' => array(
        'arg' => 'include',
        'value' => 'comments',
        ),
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['pdf'] = array(
    'title' => t('PDF'),
    'help' => t('A link leading to the pdf for the page this article appears on. There may be more than one pdf element if the article appears over multiple pages.
PDFs are not available for some articles, such as the Australian Women’s Weekly, and articles with a status of “coming soon”.
.'),
    'field' => array(
      'handler' => 'TrovequeryNewspaperRecordHandlerFieldPdf',
      'trovequery' => array(
        'arg' => 'reclevel',
        'value' => 'full',
        ),
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['troveUrl'] = array(
    'title' => t('Trove URL'),
    'help' => t('A link to view this article in the Trove web interface. If this record was retrieved in response to a search, the search terms will be included in the URL which allows them to be highlighted when viewing in the article. '),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldUrl',
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['trovePageUrl'] = array(
    'title' => t('Trove page URL'),
    'help' => t('A link to view the first page of this article, in the Trove web interface. This is not available for articles with a status of “coming soon”.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldUrl',
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['supplement'] = array(
    'title' => t('Supplement'),
    'help' => t('The newspaper/periodical section this article appeared in. In the Trove web interface, this is shown on the page view. A supplement is part of an issue.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldInclude',
      'trovequery' => array(
        'arg' => 'reclevel',
        'value' => 'full',
        ),
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['illustrated'] = array(
    'title' => t('Illustrated'),
    'help' => t('Flags whether the article is illustrated or not.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerField',
      ),
    );


  $data[TROVE_RECORD_NEWSPAPER]['section'] = array(
    'title' => t('Section'),
    'help' => t('The newspaper/periodical section this article appeared in. In the Trove web interface, this is shown on the page view. A section is part of an issue.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldInclude',
      'trovequery' => array(
        'arg' => 'reclevel',
        'value' => 'full',
        ),
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['wordCount'] = array(
    'title' => t('Word count'),
    'help' => t('The number of words in the full text of the article.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldInclude',
      'trovequery' => array(
        'arg' => 'reclevel',
        'value' => 'full',
        ),
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['correctionCount'] = array(
    'title' => t('Correction count'),
    'help' => t('The number of corrections made to the article. Each correction may involve many lines.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldInclude',
      'trovequery' => array(
        'arg' => 'reclevel',
        'value' => 'full',
        ),
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['listCount'] = array(
    'title' => t('List count'),
    'help' => t('The number of public Trove user created lists this article has been added to.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldInclude',
      'trovequery' => array(
        'arg' => 'reclevel',
        'value' => 'full',
        ),
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['tagCount'] = array(
    'title' => t('Tag count'),
    'help' => t('The number of public tags added to this article by Trove users.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldInclude',
      'trovequery' => array(
        'arg' => 'reclevel',
        'value' => 'full',
        ),
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['commentCount'] = array(
    'title' => t('Comment count'),
    'help' => t('The number of public comments added to this article by Trove users.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldInclude',
      'trovequery' => array(
        'arg' => 'reclevel',
        'value' => 'full',
        ),
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['tag'] = array(
    'title' => t('Tags'),
    'help' => t('A public tag added to the article. There may be more than one tag element.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldTag',
      'trovequery' => array(
        'arg' => 'include',
        'value' => 'tags',
        ),
      ),
    );

  $data[TROVE_RECORD_NEWSPAPER]['list'] = array(
    'title' => t('Lists'),
    'help' => t('The name of a public, user created list this article belongs to. There may be more than one list element.'),
    'field' => array(
      'handler' => 'TrovequeryNewspaperRecordHandlerFieldLastCorrection',
      'trovequery' => array(
        'arg' => 'include',
        'value' => 'lists',
        ),
      ),
    );
  return $data;

}

