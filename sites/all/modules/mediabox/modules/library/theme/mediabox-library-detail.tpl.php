<div class="library-detail clearfix">
  <div class="library-image"><?php print $image; ?></div>
  <!-- /.library-image -->

  <div class="library-info">
    <ul>
      <li><strong><?php print t('Filename'); ?></strong>: <?php print $file->filename; ?></li>
      <li><strong><?php print t('Type'); ?></strong>: <?php print $file->filemime; ?></li>
      <li><strong><?php print t('Resolution');?></strong>: <?php print $mediabox->meta_dimensions; ?></li>
    </ul>
  </div>
  <!-- /.info -->
</div><!-- /.library-detail -->