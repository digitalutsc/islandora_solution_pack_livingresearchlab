<div class="islandora-livingresearchlab-object islandora">
  <button id="livingresearchlab_image">Living Research!</button>
  <div class ="livingresearchlab_thumb">
    <?php print $variables['tn'] ?>
  </div>
  <div class="islandora-basic-image-content-wrapper clearfix">
    <?php if (isset($islandora_content)): ?>
      <div class="islandora-livingresearchlab-content">
        <?php print $islandora_content; ?>
      </div>
    <?php endif; ?>
  </div>
