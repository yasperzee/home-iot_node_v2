#ifndef BUILD_JSON_DOCS_H
#define BUILD_JSON_DOCS_H

/*************************build_json_docs.h*************************************

    Description:

*******************************************************************************/
/* -----------------------------------------------------------------------------
    Version 0.1     Yasperzee   12'22    Cleaning and refactoring

#TODO:
------------------------------------------------------------------------------*/
//#include <Arduino.h>

class buildJsonDocs {
  public:
    String build_json_getdata_html(void);
    String build_json_getinfo_html(void);
    String build_json_getDebug_html(void);
    String build_json_getSettings_html(void);
    String build_json_putSettings_html(void);
  private:
  };

#endif // BUILD_JSON_DOCS_H