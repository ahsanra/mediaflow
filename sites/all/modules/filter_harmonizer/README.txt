
VIEWS FILTER HARMONIZER
=======================
This tiny module solves an operational foible with the Views module regarding
filtering. It allows you to have a contextual filter argument apply only when
there is no regular (exposed) filter value present. For intuitive visual
feedback of the underlying process, the module will also fill out the exposed 
filter form with the contextual filter value(s) used. Once the visitor has
changed the value for the exposed filter, the module makes sure that the
corresponding contextual filter does not interfere.

INSTALLATION AND USE
--------------------
Install and enable like any module.
The only addition you'll see in the Views UI is an extra check box for each
contextual filter field. It is called "Ignore this contextual filter when the
same page has an exposed filter value" and lives under the heading "WHEN THE
FILTER VALUE IS IN THE URL OR A DEFAULT IS PROVIDED" on the contextual filter
config popup. You get to this popup via the "Advanced" fieldset, located in the
upper right corner of the main Views UI page.
For convenience this check box is also available on the Views regular filter
configuration popup.
You can globally enable the above behaviour for ALL fields that have both a
contextual and a regular (exposed) filter, via the check box on the
Administration >> Configuration >> Content Authoring >> Views Filter Harmonizer
page.
Below it is a check box for pushing the active contextual filter value into the 
corresponding exposed filter form, if that isn't active (i.e. set to --All--).

This module works for all Views displays (page, attachment etc.) with AJAX on
as well as off.

Note that Views that are displayed in blocks will not receive any contextual
arguments appended to the URL, unless you provide a default value through a PHP
code snippet like:

  return arg(1);

This extracts the first contextual argument, assuming the View path has only one
component.
With this in place you can then use the "ignore" check box just as you do for
page and attachment displays.
Remember: for exposed filters to appear with Views block displays, AJAX must be
ON.

Using Geofield?
---------------
For Views Filter Harmonizer to work with the Geofield module, the Geofield
Exposed Proximity Filter needs to have its Source of Origin Point set to
"Geocoded Location", not "Contextual Geofield Proximity Filter".
At the same time to display distances the Geofield Proximity Field needs to
have its Source or Origin Point set to "Exposed Geofield Proximity Filter", so
that the distances displayed reflect the filter origin entered.

                                    * * *
