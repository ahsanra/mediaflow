(function ($) {

Drupal.behaviors.mediaboxBrowserBehavior = {
  attach:function (context, settings) {

    $('#mediabox-cancel-images').click(function () {
      window.parent.jQuery.fancybox.close();
    });

    var selected = 0;
    var fieldCardinality = parseInt(Drupal.settings.mediaboxBrowser.field_cardinality);
    var ids = Drupal.settings.mediaboxBrowser.ids;
    // Don't allow to be selected more than one image (take last submitted)
    if (ids) {
      if (fieldCardinality === 1) {
        ids = ids.slice(-1);
      }
      for (var i = 0; i < ids.length; i++) {
        $('#' + ids[i]).toggleClass('mediabox-selected');
        ++selected;
      }
    }
    $('.mediabox-selectable', context).click(function () {

      var $item = $(this);
      if (!$item.hasClass('mediabox-selected')) {

        // Unlimited cardinality or cardinality limit is not reached for now.
        if(fieldCardinality === -1 || selected < fieldCardinality) {
          ++selected;
          $item.toggleClass('mediabox-selected');
        }
        else if (fieldCardinality === 1) {
          // For cardinality 1 we will unselect previous item and then select
          // current one.
          $('.mediabox-selectable.mediabox-selected').toggleClass('mediabox-selected');
          $item.toggleClass('mediabox-selected');
        }
        else {
          // @todo - we are not handling > 1 cardinality in UI. For this to work
          // we would need to pass additional data - how many mediabox items are
          // currently added.
        }
      } else {
        --selected;
        $item.removeClass('mediabox-selected');
      }
    });

    $('#mediabox-add-images', context).bind('click', function () {
      var str = '';
      var field_id = Drupal.settings.mediaboxBrowser.field;

      $(document).find('.mediabox-selected').each(function (key, element) {
        $(element).attr('id');
        str += $(element).attr('id').toString() + ",";
      });

      window.parent.jQuery('#' + field_id + '_mid_elements').attr('value', str);
      window.parent.jQuery('#' + field_id + '_add_more').trigger('mousedown');
      window.parent.jQuery.fancybox.close();
      return false;
    });

  }
};

})(jQuery);
