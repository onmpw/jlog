
#ifndef PHP_JLOG_H
#define PHP_JLOG_H

extern zend_module_entry jlog_module_entry;
#define phpext_jlog_ptr &jlog_module_entry

#define PHP_JLOG_VERSION "0.3.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_JLOG_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_JLOG_API __attribute__ ((visibility("default")))
#else
#	define PHP_JLOG_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_BEGIN_MODULE_GLOBALS(jlog)
	zend_bool  enable_thread;
ZEND_END_MODULE_GLOBALS(jlog)

PHP_MINIT_FUNCTION(jlog);
PHP_MSHUTDOWN_FUNCTION(jlog);
PHP_RINIT_FUNCTION(jlog);
PHP_RSHUTDOWN_FUNCTION(jlog);
PHP_MINFO_FUNCTION(jlog);


/* In every utility function you add that needs to use variables 
   in php_jlog_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as JLOG_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define JLOG_G(v) TSRMG(jlog_globals_id, zend_jlog_globals *, v)
#else
#define JLOG_G(v) (jlog_globals.v)
#endif

#define J_ERROR     1
#define J_WARNING   2
#define J_INFO      3
#define J_DEBUG     4
#define J_NOTICE    5
#endif	/* PHP_JLOG_H */

