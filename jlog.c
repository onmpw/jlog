
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_jlog.h"
#include "storage/jlog_storage.h"
#include <pthread.h>
/* If you declare any globals in php_jlog.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(jlog)
*/

/* True global resources - no need for thread safety here */
static int le_jlog;
static int server_start;
/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("jlog.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_jlog_globals, jlog_globals)
    STD_PHP_INI_ENTRY("jlog.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_jlog_globals, jlog_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */

/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/
void *say_hello()
{
    int n = 0;
    while(1){
        n++;
        printf("Hello Thread! %d\n",n);
        if(n == 5) {
            break;
        }
        sleep(1);
    }
}


PHP_FUNCTION(jlog_start)
{
    if(server_start != 0) {
        return ;
    }
    server_start = 1;

    pthread_t tid;
    int ret = pthread_create(&tid,NULL,say_hello,NULL);

    php_printf("Hello World\n");
    php_printf("Hello World2\n");
    php_printf("Hello World3\n");
    php_printf("Hello World4\n");
    php_printf("Hello World5\n");
    php_printf("Hello World6\n");

    pthread_join(tid,NULL);
}

PHP_FUNCTION(jlog_info)
{

}

PHP_FUNCTION(jlog_stop)
{
    if(server_start == 0) {
        return ;
    }
    server_start = 0;
}


/* {{{ php_jlog_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_jlog_init_globals(zend_jlog_globals *jlog_globals)
{
	jlog_globals->global_value = 0;
	jlog_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(jlog)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	server_start = 0;
    if(!queue_init()) {
        php_error(E_ERROR,"队列初始化失败\n");
    }
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(jlog)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	server_start = 0;
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(jlog)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(jlog)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(jlog)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "jlog support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ jlog_functions[]
 *
 * Every user visible function must have an entry in jlog_functions[].
 */
const zend_function_entry jlog_functions[] = {
	PHP_FE(jlog_start,NULL)
	PHP_FE(jlog_stop,NULL)
	PHP_FE_END	/* Must be the last line in jlog_functions[] */
};
/* }}} */

/* {{{ jlog_module_entry
 */
zend_module_entry jlog_module_entry = {
	STANDARD_MODULE_HEADER,
	"jlog",
	jlog_functions,
	PHP_MINIT(jlog),
	PHP_MSHUTDOWN(jlog),
	PHP_RINIT(jlog),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(jlog),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(jlog),
	PHP_JLOG_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_JLOG
ZEND_GET_MODULE(jlog)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */