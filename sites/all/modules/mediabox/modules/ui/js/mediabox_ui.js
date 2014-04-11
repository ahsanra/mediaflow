(function ($) {

"use strict";

function hideMediaboxItem(context) {
  var $checkbox = $('input', context);
  var $item = $checkbox.parent().parent().next().children();
  if ($checkbox.is(':checked')) {
    $item.slideUp('fast');
  } else {
    $item.slideDown('fast');
  }
}

Drupal.behaviors.mediaboxBrowser = {

  attach:function (context, settings) {

    $('.mediabox-remove .form-type-checkbox', context).once('close', function () {
      var $context = $(this);
      $context.click(function () {
        var $checkbox = $('input', this);

        if ($checkbox.is(':checked')) {
          $context.removeClass('mediabox-restore');
          $checkbox.attr('checked', false);
        } else {
          $context.addClass('mediabox-restore');
          $checkbox.attr('checked', true);
        }

        hideMediaboxItem($context);
      });
    });

    $('.mediabox-remove .form-type-checkbox', context).each(function (index) {
      var $context = $(this);
      var $checkbox = $('input', this);

      if ($checkbox.is(':checked')) {
        $context.addClass('mediabox-restore');
      } else {
        $context.removeClass('mediabox-restore');
      }

      hideMediaboxItem($context);
    });

    $('.mediabox-browser-modal').fancybox({
      width: 980,
      height: 940,
      titleShow: false,
      autoscale: false,
      autoDimensions: false,
      scrolling: 'yes',
      type: 'iframe'
    });
  }

};

})(jQuery);
