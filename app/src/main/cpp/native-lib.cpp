#include <jni.h>
#include <string>
#include <include/jerryscript.h>
#include <jerryscript-ext/handler.h>

#include <android/log.h>

/**
 * Standalone Jerry exit codes
 */
#define JERRY_STANDALONE_EXIT_CODE_OK   (0)
#define JERRY_STANDALONE_EXIT_CODE_FAIL (1)

/**
 * Register a JavaScript function in the global object.
 */
static void
register_js_function (const char *name_p, /**< name of the function */
                      jerry_external_handler_t handler_p) /**< function callback */
{
  jerry_value_t result_val = jerryx_handler_register_global ((const jerry_char_t *) name_p, handler_p);

  if (jerry_value_is_error (result_val))
  {
    printf ("Warning: failed to register '%s' method.", name_p);
  }

  jerry_release_value (result_val);
} /* register_js_function */

/**
 * Jerryscript simple test
 */
int test_jerry(){
  const jerry_char_t script[] = "print ('Hello, World!');var a = 7; var b = 5; print(a+b);";

  /* Initialize engine */
  jerry_init (JERRY_INIT_EMPTY);

  /* Register the print function in the global object. */
  register_js_function ("print", jerryx_handler_print);


      /* Setup Global scope code */
  jerry_value_t ret_value = jerry_parse (NULL, 0, script, sizeof (script) - 1, JERRY_PARSE_NO_OPTS);

  if (!jerry_value_is_error (ret_value))
  {
    /* Execute the parsed source code in the Global scope */
    ret_value = jerry_run (ret_value);
  }

  int ret_code = JERRY_STANDALONE_EXIT_CODE_OK;

  if (jerry_value_is_error (ret_value))
  {
    __android_log_print (ANDROID_LOG_ERROR, "shiwentao", "Script Error!");

    ret_code = JERRY_STANDALONE_EXIT_CODE_FAIL;
  }

  jerry_release_value (ret_value);

  /* Cleanup engine */
  // jerry_cleanup ();

  return ret_code;

} /* test_jerry */

extern "C" JNIEXPORT jstring JNICALL
Java_lynccom_so_1test_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

  const jerry_char_t script[] = "var str = 'Hello, World!';";

  bool ret_value = true; //jerry_run_simple (script, sizeof (script) - 1, JERRY_INIT_EMPTY);
  std::string hello;
  if (ret_value) {
    hello = "Hello from C++ run: true";
  } else {
    hello = "Hello from C++ run: false";
  }

  __android_log_print (ANDROID_LOG_ERROR, "shiwentao", "test_jerry");
  test_jerry();

    return env->NewStringUTF(hello.c_str());
}
