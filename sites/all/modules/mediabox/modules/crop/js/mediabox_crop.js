(function ($) {

"use strict";

/**
 * Used when original image style, image style or crop ratio settings are
 * changed from UI which will trigger mediabox_ui_crop() callback.
 */
Drupal.behaviors.crop = {
  attach:function (context, settings) {
    if (!settings.crop) {
      return;
    }

    // Apply the MyBehaviour effect to the elements only once.
    var pane = $('#' + settings.crop.wrapper_id, context);
    pane.once('crop', function () {

      var init_coords = settings.crop.coords;
      var cropImage = $('img.jcrop-image', pane);
      var formsContext = pane.parent();

      var aspectRatio = false;
      if(settings.crop.ratio === 1) {
        aspectRatio = (init_coords.x1 - init_coords.x) / (init_coords.y1 - init_coords.y);
      }

      $(cropImage).Jcrop({
        'onChange':function (coords) {
          $('input.jcrop-x', formsContext).attr('value', coords.x);
          $('input.jcrop-y', formsContext).attr('value', coords.y);
          $('input.jcrop-x1', formsContext).attr('value', coords.x2);
          $('input.jcrop-y1', formsContext).attr('value', coords.y2);
          $('input.jcrop-height', formsContext).attr('value', coords.h);
          $('input.jcrop-width', formsContext).attr('value', coords.w);
        },
        'onSelect':function (coords) {
          $('input.jcrop-x', formsContext).attr('value', coords.x);
          $('input.jcrop-y', formsContext).attr('value', coords.y);
          $('input.jcrop-x1', formsContext).attr('value', coords.x2);
          $('input.jcrop-y1', formsContext).attr('value', coords.y2);
          $('input.jcrop-height', formsContext).attr('value', coords.h);
          $('input.jcrop-width', formsContext).attr('value', coords.w);
        },
        'aspectRatio': aspectRatio,
        'setSelect':[init_coords.x, init_coords.y, init_coords.x1, init_coords.y1]
      });
    });

    pane.removeOnce('crop', function () {
    });

  }
}

/**
 * We need to initialize default crop states.
 */
Drupal.behaviors.cropDefault = {
  attach:function (context, settings) {

    // Apply the MyBehaviour effect to the elements only once.
    $('.crop-image-set').each(function (index, values) {
      var $pane = $(this);
      $pane.once('cropDefault', function () {

        var cropImage = $('img.jcrop-image', $pane);

        var x = $('input.jcrop-x', $pane).attr('value');
        var y = $('input.jcrop-y', $pane).attr('value');
        var x1 = $('input.jcrop-x1', $pane).attr('value');
        var y1 = $('input.jcrop-y1', $pane).attr('value');
        //var height = $('input.jcrop-height', $pane).attr('value');
        //var width = $('input.jcrop-width', $pane).attr('value');

        var aspectRatio = false;
        if($($pane).parent().parent().find('input.ratio').is(':checked')) {
          aspectRatio = (x1 - x) / (y1 - y);
        }

        $(cropImage).Jcrop({
          'onChange':function (coords) {
            $('input.jcrop-x', $pane).attr('value', coords.x);
            $('input.jcrop-y', $pane).attr('value', coords.y);
            $('input.jcrop-x1', $pane).attr('value', coords.x2);
            $('input.jcrop-y1', $pane).attr('value', coords.y2);
            $('input.jcrop-height', $pane).attr('value', coords.h);
            $('input.jcrop-width', $pane).attr('value', coords.w);
          },
          'onSelect':function (coords) {
            $('input.jcrop-x', $pane).attr('value', coords.x);
            $('input.jcrop-y', $pane).attr('value', coords.y);
            $('input.jcrop-x1', $pane).attr('value', coords.x2);
            $('input.jcrop-y1', $pane).attr('value', coords.y2);
            $('input.jcrop-height', $pane).attr('value', coords.h);
            $('input.jcrop-width', $pane).attr('value', coords.w);
          },
          'aspectRatio': aspectRatio,
          'setSelect':[x, y, x1, y1]
        });
      });
    });
  }
}

})(jQuery);
