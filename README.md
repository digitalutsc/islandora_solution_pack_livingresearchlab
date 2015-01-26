islandora_livingresearchlab_solution_pack
=========================================


## Introduction

This is a beta solution pack for the University of Toronto Scarborough Digital Scholarship Unit.  Objects use the Islandora collection content model to display relationships.
Note: This repo is based on an older repo islandora_livingresearchlab-2.x-dev for better naming convention. 

## Dependencies

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora PDF Solution Pack](https://github.com/Islandora/islandora_solution_pack_pdf)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Configure Mice ID Schema information in Administration » Islandora » Living Research Lab Solution Pack (admin/islandora/solution_pack_config/livingresearchlab).

## Index dwc.* fields in Solr

A dwc_to_solr.xslt file (xsl/dwc_to_solr.xslt) is included in the module in order to index dwc fields in Solr.

* Please drop this file to 'islandora_transforms' folder and update the foxmlToSolr.xslt file to include new xslt file.
* Modify solr schema.xml file to add dwc.* and dcterms.* fields

```xml
<dynamicField name="dwc.*"  type="text_general" indexed="true" stored="true" multiValued="true"/>
```
    

## Troubleshooting/Issues
This is a beta version.

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers

* [Lingling Jiang](https://github.com/sprklinginfo)
* Kim Pham


=======
