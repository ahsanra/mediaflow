<?php print $content; ?>

<?php if(sizeof($actions)): ?>
  <div class="mediabox-button-wrappers">
    <?php foreach($actions as $action): ?>
      <?php print $action; ?>
    <?php endforeach; ?>
  </div>
<?php endif; ?>