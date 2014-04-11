<?php

/**
 * @file
 * Crop data for entites
 * This is not really bind to any entity - can be used more flexible
 */
class MediaboxCrop {

  public $id;
  public $entity_id;
  public $x;
  public $y;
  public $x1;
  public $y1;
  public $ratio;
  public $origin_image_style;
  public $height;
  public $width;
  public $created;
  public $modified;

  public function populateFromClass($class) {
    if (!empty($class)) {
      foreach (get_class_vars('MediaboxCrop') as $key => $value) {
        if (property_exists($class, $key)) {
          $this->$key = $class->$key;
        }
      }
    }
  }

  public function populateFromArray($array) {
    if (!empty($array)) {
      foreach (get_class_vars('MediaboxCrop') as $key => $value) {
        if (key_exists($key, $array) && isset($array[$key])) {
          $this->$key = $array[$key];
        }
      }
    }
  }
}
