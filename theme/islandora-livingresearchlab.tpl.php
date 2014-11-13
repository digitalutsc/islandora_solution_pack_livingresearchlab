<div class="islandora-livingresearchlabs-object islandora">
  <div class="islandora-livingresearchlabs-content-wrapper clearfix">
    <?php if (isset($islandora_thumbnail_img)): ?>
      <div class="islandora-livingresearchlabs-object-content">
        <?php print $islandora_thumbnail_img; ?>
      </div>
    <?php endif; ?>
  </div>
  <div class="islandora-livingresearchlabs-object-metadata">
    <?php print $description; ?>
    <?php if ($parent_collections): ?>
      <div>
        <h2><?php print t('In collections'); ?></h2>
        <ul>
          <?php foreach ($parent_collections as $collection): ?>
            <li><?php print l($collection->label, "islandora/object/{$collection->id}"); ?></li>
          <?php endforeach; ?>
        </ul>
      </div>
    <?php endif; ?>
    <?php print $metadata; ?>
  </div>
</div>