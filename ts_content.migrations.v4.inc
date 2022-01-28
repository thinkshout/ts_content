<?php
/**
 * @file
 * ThinkShout Content migration classes for Sheets API v4.
 *
 * To migrate from the XML-based migration classes that were used with Sheets
 * API v3, find subclasses of TSXMLContentMigration in your project and extend
 * TSContentV4Migration instead. (Note: any custom XML or xpath code will need
 * to be refactored to work with the new plain JSON objects.)
 */

/**
 * TS Content migration base class
 */
abstract class TSContentV4Migration extends Migration {
  /**
   * A Migration constructor takes an array of arguments as its first parameter.
   *
   * The arguments must be passed through to the parent constructor, unless
   * you're using a version of the Migrate module < 2.6.
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    // With migrate_ui enabled, migration pages will indicate people involved in
    // the particular migration, with their role and contact info. We default
    // the list in the shared class; it can be overridden for specific
    // migrations.
    // $this->team = array(
    //   new MigrateTeamMember('First Last', 'first.last@example.com', t('Developer')),
    // );

    $this->dependencies = $arguments['dependencies'];

    $this->addUnmigratedSources($arguments['unmigrated_sources']);
    $this->addUnmigratedDestinations($arguments['unmigrated_destinations']);
  }

  /**
   * Construct the machine name (for "drush ms" and other places).
   */
  protected function generateMachineName($class_name = NULL) {
    return $this->arguments['machine_name'];
  }

  /**
   * Custom method to create a source fields array.
   */
  protected function parseSourceFields(array $fields) {
    $source_fields = array();
    foreach ($fields as $field_settings) {
      if (isset($field_settings['source'])) {
        $source_fields[$field_settings['source']] = $field_settings['label'];
      }
    }
    return $source_fields;
  }

  /**
   * Creates custom field mapping for a ts_content migration document.
   *
   * @param array $fields
   *   An array of field data arrays.
   *
   * @return mixed
   *   The custom field mapping object.
   */
  protected function createCustomFieldMapping(array $fields) {
    foreach ($fields as $destination => $field_settings) {
      if (isset($field_settings['source'])) {
        $mapping = $this->addFieldMapping($destination, $field_settings['source']);
      }
      else {
        $mapping = $this->addFieldMapping($destination);
      }
      if (isset($field_settings['default_value'])) {
        $mapping->defaultValue($field_settings['default_value']);
      }
      if (isset($field_settings['source_migration'])) {
        $mapping->sourceMigration($field_settings['source_migration']);
      }
      if (isset($field_settings['arguments'])) {
        $mapping->arguments($field_settings['arguments']);
      }
      if (isset($field_settings['delimiter'])) {
        $mapping->separator($field_settings['delimiter']);
      }
      if (isset($field_settings['callbacks'])) {
        $mapping->callbacks($field_settings['callbacks']);
      }
    }

    if (isset($mapping)) {
      return $mapping;
    }
    else {
      return NULL;
    }
  }

  protected function postImport() {
    if (isset($this->arguments['post_migration'])) {
      call_user_func($this->arguments['post_migration']);
    }
  }

  /**
   * Creates a ts_content migration source.
   *
   * @param string $data_url
   *   The URL the migration retrieves data from.
   * @param array $fields
   *   Migration source fields.
   * @param array $extra_source_fields
   *   Migration source fields that don't belong in source data, usually because
   *   they're not user-editable.
   */
  protected function createMigrationSource($data_url, $fields, $extra_source_fields = array()) {
    $source_fields = $this->parseSourceFields($fields);

    if (!empty($extra_source_fields)) {
      $source_fields = array_merge($source_fields, $extra_source_fields);
    }

    $this->source = new TSContentV4MigrateSource($data_url, $source_fields);
  }
}

/**
 * TS Content migrate source class
 */
class TSContentV4MigrateSource extends MigrateSource {
  protected $currentId;
  protected $data;
  protected $dataUrl;
  protected $fields;

  /**
   * {@inheritDoc}
   */
  public function __construct($data_url, $fields, array $options = array()) {
    parent::__construct($options);
    $this->dataUrl = $data_url;
    $this->fields = $fields;
  }

  /**
   * Convert google sheet data into a json object.
   *
   * @return array
   */
  protected function data() {
    if (!isset($this->data)) {
      $this->data = [];

      $request = drupal_http_request($this->dataUrl);
      $json = drupal_json_decode($request->data);

      $field_names = [];

      foreach ($json['values'] as $index => $values) {
        if ($index == 0) {
          foreach ($values as $value) {
            $field_names[] = preg_replace('/\W+/', '', strtolower($value));
          }
        }
        else {
          $this->data[$index] = (object) array_combine($field_names, $values);
        }
      }
    }

    return $this->data;
  }

  /**
   * Return a string representing the source, for display in the UI.
   */
  public function __toString() {
    return 'ts_content url = ' . $this->dataUrl;
  }

  /**
   * Returns a list of fields available to be mapped from the source,
   * keyed by field name.
   */
  public function fields() {
    return $this->fields;
  }

  /**
   * Return the number of available source records.
   */
  public function computeCount() {
    return count($this->data());
  }

  /**
   * Do whatever needs to be done to start a fresh traversal of the source data.
   */
  public function performRewind() {
    $this->currentId = 0;
  }

  /**
   * Fetch the next row of data, returning it as an object. Return FALSE
   * when there is no more data available.
   */
  public function getNextRow() {
    $data = $this->data();
    $next_id = $this->currentId + 1;
    if (!empty($data[$next_id])) {
      $this->currentId = $next_id;
      return $data[$next_id];
    }
    return NULL;
  }
}
