<?php

/**
 * Class MigrateDestinationBean
 *
 * See: https://drupal.org/node/2141211
 */
class MigrateDestinationBean extends MigrateDestinationEntityAPI {
  /**
   * Basic initialization.
   *
   * @param string $entity_type
   * @param string $bundle
   * @param array $options
   *  Options (language, text_format) used for creating fields.
   */
  public function __construct($entity_type, $bundle, array $options = array()) {
    parent::__construct($entity_type, $bundle, $options);
    // Override this for beans, otherwise it would be name key (delta) taken as id
    // For beans to be updated we need to set id as bid instead of name
    $this->id = $this->info['entity keys']['id'];
  }

  public function fields() {
    $fields = parent::fields();

    $fields['view_mode'] = 'View Mode';

    return $fields;
  }
}

/**
 * Class MigrateDestinationMenuLinksWithAttributes
 *
 * Extends MigrateDestinationMenuLinks to add custom attribute fields.
 */
class MigrateDestinationMenuLinksWithAttributes extends MigrateDestinationMenuLinks {
  public function __construct() {
    parent::__construct();
  }

  public function fields() {
    $fields = parent::fields();

    $fields['attribute_id'] = 'Attribute: id';
    $fields['attribute_name'] = 'Attribute: name';
    $fields['attribute_target'] = 'Attribute: target';
    $fields['attribute_rel'] = 'Attribute: rel';
    $fields['attribute_class'] = 'Attribute: class';
    $fields['attribute_style'] = 'Attribute: style';
    $fields['attribute_accesskey'] = 'Attribute: accesskey';

    return $fields;
  }
}
