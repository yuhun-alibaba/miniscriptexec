#指定需要CMAKE的最小版本
cmake_minimum_required(VERSION 3.4.1)

#add_compile_options(-std=c++11)
add_compile_options(-fexceptions)
add_compile_options(-Werror)
add_definitions(-DOS_ANDROID=1)
# for rapid json
add_definitions(-DRAPIDJSON_HAS_STDSTRING=1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fno-rtti")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--exclude-libs,libgcc.a -Wl,--exclude-libs,libunwind.a")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--exclude-libs,ALL,--gc-sections")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} j,--exclude-libs,ALL,--gc-sections,ALL")

# remove redundant sections
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections -funwind-tables -fno-short-enums -nostdinc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections -funwind-tables -fno-short-enums -nostdinc++")


set(COMPILE_FLAGS_LIBM "${COMPILE_FLAGS_LIBM} -Wno-error=sign-compare")
set(COMPILE_FLAGS_LIBM "${COMPILE_FLAGS_LIBM} -Wno-error=sign-conversion")
set(COMPILE_FLAGS_LIBM "${COMPILE_FLAGS_LIBM} -Wno-sign-conversion")
set(COMPILE_FLAGS_LIBM "${COMPILE_FLAGS_LIBM} -Wno-sign-compare")
set(COMPILE_FLAGS_LIBM "${COMPILE_FLAGS_LIBM} -Wno-strict-aliasing")

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-libm/include)


set(JERRY_LIBM_SRC_LIST
        src/main/cpp/jerry-libm/acos.c
        src/main/cpp/jerry-libm/asin.c
        src/main/cpp/jerry-libm/atan.c
        src/main/cpp/jerry-libm/atan2.c
        src/main/cpp/jerry-libm/ceil.c
        src/main/cpp/jerry-libm/copysign.c
        src/main/cpp/jerry-libm/exp.c
        src/main/cpp/jerry-libm/fabs.c
        src/main/cpp/jerry-libm/finite.c
        src/main/cpp/jerry-libm/floor.c
        src/main/cpp/jerry-libm/fmod.c
        src/main/cpp/jerry-libm/isnan.c
        src/main/cpp/jerry-libm/log.c
        src/main/cpp/jerry-libm/nextafter.c
        src/main/cpp/jerry-libm/pow.c
        src/main/cpp/jerry-libm/scalbn.c
        src/main/cpp/jerry-libm/sqrt.c
        src/main/cpp/jerry-libm/trig.c
        )

## jerry-core compile

# Optional build settings
set(ENABLE_ALL_IN_ONE          OFF     CACHE BOOL   "Enable all-in-one build?")
# Optional features
set(FEATURE_CPOINTER_32_BIT    OFF     CACHE BOOL   "Enable 32 bit compressed pointers?")
set(FEATURE_DEBUGGER           OFF     CACHE BOOL   "Enable JerryScript debugger?")
set(FEATURE_ERROR_MESSAGES     OFF     CACHE BOOL   "Enable error messages?")
set(FEATURE_EXTERNAL_CONTEXT   OFF     CACHE BOOL   "Enable external context?")
set(FEATURE_JS_PARSER          ON      CACHE BOOL   "Enable js-parser?")
set(FEATURE_LINE_INFO          OFF     CACHE BOOL   "Enable line info?")
set(FEATURE_LOGGING            OFF     CACHE BOOL   "Enable logging?")
set(FEATURE_MEM_STATS          OFF     CACHE BOOL   "Enable memory statistics?")
set(FEATURE_MEM_STRESS_TEST    OFF     CACHE BOOL   "Enable mem-stress test?")
set(FEATURE_PARSER_DUMP        OFF     CACHE BOOL   "Enable parser byte-code dumps?")
set(FEATURE_PROFILE            "es5.1" CACHE STRING "Use default or other profile?")
set(FEATURE_REGEXP_STRICT_MODE OFF     CACHE BOOL   "Enable regexp strict mode?")
set(FEATURE_REGEXP_DUMP        OFF     CACHE BOOL   "Enable regexp byte-code dumps?")
set(FEATURE_SNAPSHOT_EXEC      OFF     CACHE BOOL   "Enable executing snapshot files?")
set(FEATURE_SNAPSHOT_SAVE      OFF     CACHE BOOL   "Enable saving snapshot files?")
set(FEATURE_SYSTEM_ALLOCATOR   OFF     CACHE BOOL   "Enable system allocator?")
set(FEATURE_VALGRIND           OFF     CACHE BOOL   "Enable Valgrind support?")
set(FEATURE_VM_EXEC_STOP       OFF     CACHE BOOL   "Enable VM execution stopping?")
set(MEM_HEAP_SIZE_KB           "512"   CACHE STRING "Size of memory heap, in kilobytes")
set(REGEXP_RECURSION_LIMIT     "0"     CACHE STRING "Limit of regexp recursion depth")
set(VM_RECURSION_LIMIT         "0"     CACHE STRING "Limit of VM recursion depth")

# Option overrides
if(USING_MSVC)
    set(ENABLE_ALL_IN_ONE ON) # FIXME: This should not be needed but right now it is. To be tracked down and followed up.

    set(ENABLE_ALL_IN_ONE_MESSAGE " (FORCED BY COMPILER)")
endif()

if(FEATURE_SYSTEM_ALLOCATOR)
    set(FEATURE_CPOINTER_32_BIT ON)

    set(FEATURE_CPOINTER_32_BIT_MESSAGE " (FORCED BY SYSTEM ALLOCATOR)")
endif()

if (MEM_HEAP_SIZE_KB GREATER 512)
    set(FEATURE_CPOINTER_32_BIT ON)

    set(FEATURE_CPOINTER_32_BIT_MESSAGE " (FORCED BY HEAP SIZE)")
endif()

if(NOT FEATURE_JS_PARSER)
    set(FEATURE_SNAPSHOT_EXEC ON)
    set(FEATURE_PARSER_DUMP   OFF)

    set(FEATURE_SNAPSHOT_EXEC_MESSAGE " (FORCED BY DISABLED JS PARSER)")
    set(FEATURE_PARSER_DUMP_MESSAGE   " (FORCED BY DISABLED JS PARSER)")
endif()

if(JERRY_CMDLINE_SNAPSHOT)
    set(FEATURE_SNAPSHOT_SAVE ON)

    set(FEATURE_SNAPSHOT_SAVE_MESSAGE " (FORCED BY SNAPSHOT TOOL)")
endif()

if(FEATURE_MEM_STATS OR FEATURE_PARSER_DUMP OR FEATURE_REGEXP_DUMP)
    set(FEATURE_LOGGING ON)

    set(FEATURE_LOGGING_MESSAGE " (FORCED BY STATS OR DUMP)")
endif()

# Status messages
message(STATUS "ENABLE_ALL_IN_ONE           " ${ENABLE_ALL_IN_ONE} ${ENABLE_ALL_IN_ONE_MESSAGE})
message(STATUS "FEATURE_CPOINTER_32_BIT     " ${FEATURE_CPOINTER_32_BIT} ${FEATURE_CPOINTER_32_BIT_MESSAGE})
message(STATUS "FEATURE_DEBUGGER            " ${FEATURE_DEBUGGER})
message(STATUS "FEATURE_ERROR_MESSAGES      " ${FEATURE_ERROR_MESSAGES})
message(STATUS "FEATURE_EXTERNAL_CONTEXT    " ${FEATURE_EXTERNAL_CONTEXT})
message(STATUS "FEATURE_JS_PARSER           " ${FEATURE_JS_PARSER})
message(STATUS "FEATURE_LINE_INFO           " ${FEATURE_LINE_INFO})
message(STATUS "FEATURE_LOGGING             " ${FEATURE_LOGGING} ${FEATURE_LOGGING_MESSAGE})
message(STATUS "FEATURE_MEM_STATS           " ${FEATURE_MEM_STATS})
message(STATUS "FEATURE_MEM_STRESS_TEST     " ${FEATURE_MEM_STRESS_TEST})
message(STATUS "FEATURE_PARSER_DUMP         " ${FEATURE_PARSER_DUMP} ${FEATURE_PARSER_DUMP_MESSAGE})
message(STATUS "FEATURE_PROFILE             " ${FEATURE_PROFILE})
message(STATUS "FEATURE_REGEXP_STRICT_MODE  " ${FEATURE_REGEXP_STRICT_MODE})
message(STATUS "FEATURE_REGEXP_DUMP         " ${FEATURE_REGEXP_DUMP})
message(STATUS "FEATURE_SNAPSHOT_EXEC       " ${FEATURE_SNAPSHOT_EXEC} ${FEATURE_SNAPSHOT_EXEC_MESSAGE})
message(STATUS "FEATURE_SNAPSHOT_SAVE       " ${FEATURE_SNAPSHOT_SAVE} ${FEATURE_SNAPSHOT_SAVE_MESSAGE})
message(STATUS "FEATURE_SYSTEM_ALLOCATOR    " ${FEATURE_SYSTEM_ALLOCATOR})
message(STATUS "FEATURE_VALGRIND            " ${FEATURE_VALGRIND})
message(STATUS "FEATURE_VM_EXEC_STOP        " ${FEATURE_VM_EXEC_STOP})
message(STATUS "MEM_HEAP_SIZE_KB            " ${MEM_HEAP_SIZE_KB})
message(STATUS "REGEXP_RECURSION_LIMIT      " ${REGEXP_RECURSION_LIMIT})
message(STATUS "VM_RECURSION_LIMIT          " ${VM_RECURSION_LIMIT})

# Include directories
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-core
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-core/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-core/api
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-core/debugger
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-core/ecma/base
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-core/ecma/builtin-objects
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-core/ecma/builtin-objects/typedarray
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-core/ecma/operations
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-core/jcontext
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-core/jmem
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-core/jrt
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-core/lit
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-core/parser/js
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-core/parser/regexp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-core/vm)

set(JERRY_CORE_SRC_LIST
        src/main/cpp/jerry-core/api/jerry.c
        src/main/cpp/jerry-core/api/jerry-debugger.c
        src/main/cpp/jerry-core/api/jerry-debugger-transport.c
        src/main/cpp/jerry-core/api/jerry-snapshot.c

        src/main/cpp/jerry-core/debugger/debugger.c

        src/main/cpp/jerry-core/ecma/base/ecma-alloc.c
        src/main/cpp/jerry-core/ecma/base/ecma-gc.c
        src/main/cpp/jerry-core/ecma/base/ecma-helpers.c
        src/main/cpp/jerry-core/ecma/base/ecma-helpers-conversion.c
        src/main/cpp/jerry-core/ecma/base/ecma-helpers-errol.c
        src/main/cpp/jerry-core/ecma/base/ecma-helpers-external-pointers.c
        src/main/cpp/jerry-core/ecma/base/ecma-helpers-number.c
        src/main/cpp/jerry-core/ecma/base/ecma-helpers-string.c
        src/main/cpp/jerry-core/ecma/base/ecma-helpers-value.c
        src/main/cpp/jerry-core/ecma/base/ecma-helpers-values-collection.c
        src/main/cpp/jerry-core/ecma/base/ecma-init-finalize.c
        src/main/cpp/jerry-core/ecma/base/ecma-lcache.c
        src/main/cpp/jerry-core/ecma/base/ecma-literal-storage.c
        src/main/cpp/jerry-core/ecma/base/ecma-module.c
        src/main/cpp/jerry-core/ecma/base/ecma-property-hashmap.c

        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-array-iterator-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-array-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-array.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-arraybuffer-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-arraybuffer.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-boolean-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-boolean.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-dataview-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-dataview.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-date-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-date.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-error-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-error.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-evalerror.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-evalerror-prototype.c

        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-function-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-function.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-global.c

        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-helpers.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-helpers-date.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-helpers-error.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-helpers-json.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-helpers-sort.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-iterator-prototype.c

        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-json.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-map.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-map-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-math.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-number.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-number-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-object.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-object-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-promise.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-promise-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-rangeerror.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-rangeerror-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-referenceerror.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-referenceerror-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-regexp.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-regexp-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-set.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-set-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-string.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-string-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-string-iterator-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-symbol.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-symbol-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-syntaxerror.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-syntaxerror-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-type-error-thrower.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-typeerror.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-typeerror-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-urierror.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtin-urierror-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/ecma-builtins.c

        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-float32array.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-float32array-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-float64array.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-float64array-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int8array.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int8array-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int16array.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int16array-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int32array.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int32array-prototype.c

        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-typedarray.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-typedarray-helpers.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-typedarray-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint8array.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint8array-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint8clampedarray.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint8clampedarray-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint16array.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint16array-prototype.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint32array.c
        src/main/cpp/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint32array-prototype.c

        src/main/cpp/jerry-core/ecma/operations/ecma-array-object.c
        src/main/cpp/jerry-core/ecma/operations/ecma-arraybuffer-object.c
        src/main/cpp/jerry-core/ecma/operations/ecma-boolean-object.c
        src/main/cpp/jerry-core/ecma/operations/ecma-comparison.c
        src/main/cpp/jerry-core/ecma/operations/ecma-container-object.c
        src/main/cpp/jerry-core/ecma/operations/ecma-conversion.c
        src/main/cpp/jerry-core/ecma/operations/ecma-dataview-object.c
        src/main/cpp/jerry-core/ecma/operations/ecma-eval.c
        src/main/cpp/jerry-core/ecma/operations/ecma-exceptions.c
        src/main/cpp/jerry-core/ecma/operations/ecma-function-object.c
        src/main/cpp/jerry-core/ecma/operations/ecma-get-put-value.c
        src/main/cpp/jerry-core/ecma/operations/ecma-iterator-object.c
        src/main/cpp/jerry-core/ecma/operations/ecma-jobqueue.c
        src/main/cpp/jerry-core/ecma/operations/ecma-lex-env.c
        src/main/cpp/jerry-core/ecma/operations/ecma-number-arithmetic.c
        src/main/cpp/jerry-core/ecma/operations/ecma-number-object.c
        src/main/cpp/jerry-core/ecma/operations/ecma-objects.c
        src/main/cpp/jerry-core/ecma/operations/ecma-objects-arguments.c
        src/main/cpp/jerry-core/ecma/operations/ecma-objects-general.c
        src/main/cpp/jerry-core/ecma/operations/ecma-promise-object.c
        src/main/cpp/jerry-core/ecma/operations/ecma-reference.c
        src/main/cpp/jerry-core/ecma/operations/ecma-regexp-object.c
        src/main/cpp/jerry-core/ecma/operations/ecma-string-object.c
        src/main/cpp/jerry-core/ecma/operations/ecma-symbol-object.c
        src/main/cpp/jerry-core/ecma/operations/ecma-typedarray-object.c

#        src/main/cpp/jerry-core/ecma/operations/*.c

#        src/main/cpp/jerry-core/jcontext/*.c
        src/main/cpp/jerry-core/jcontext/jcontext.c

#        src/main/cpp/jerry-core/jmem/*.c

        src/main/cpp/jerry-core/jmem/jmem-allocator.c
        src/main/cpp/jerry-core/jmem/jmem-heap.c
        src/main/cpp/jerry-core/jmem/jmem-poolman.c


#        src/main/cpp/jerry-core/jrt/*.c
        src/main/cpp/jerry-core/jrt/jrt-fatals.c

#        src/main/cpp/jerry-core/lit/*.c
        src/main/cpp/jerry-core/lit/lit-char-helpers.c
        src/main/cpp/jerry-core/lit/lit-magic-strings.c
        src/main/cpp/jerry-core/lit/lit-strings.c

#        src/main/cpp/jerry-core/parser/js/*.c
        src/main/cpp/jerry-core/parser/js/byte-code.c
        src/main/cpp/jerry-core/parser/js/common.c
        src/main/cpp/jerry-core/parser/js/js-lexer.c
        src/main/cpp/jerry-core/parser/js/js-parser.c
        src/main/cpp/jerry-core/parser/js/js-parser-expr.c
        src/main/cpp/jerry-core/parser/js/js-parser-mem.c
        src/main/cpp/jerry-core/parser/js/js-parser-module.c
        src/main/cpp/jerry-core/parser/js/js-parser-scanner.c
        src/main/cpp/jerry-core/parser/js/js-parser-statm.c
        src/main/cpp/jerry-core/parser/js/js-parser-util.c

#        src/main/cpp/jerry-core/parser/regexp/*.c
        src/main/cpp/jerry-core/parser/regexp/re-bytecode.c
        src/main/cpp/jerry-core/parser/regexp/re-compiler.c
        src/main/cpp/jerry-core/parser/regexp/re-parser.c

#        src/main/cpp/jerry-core/vm/*.c
        src/main/cpp/jerry-core/vm/opcodes.c
        src/main/cpp/jerry-core/vm/opcodes-ecma-arithmetics.c
        src/main/cpp/jerry-core/vm/opcodes-ecma-bitwise.c
        src/main/cpp/jerry-core/vm/opcodes-ecma-relational-equality.c
        src/main/cpp/jerry-core/vm/vm.c
        src/main/cpp/jerry-core/vm/vm-stack.c
        src/main/cpp/jerry-core/vm/vm-utils.c
        )


# add jerry-port
include_directories(
            ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-port/default/include
        )
set(JERRY_PORT_SRC_LIST
        src/main/cpp/jerry-port/default/default-date.c
        src/main/cpp/jerry-port/default/default-debugger.c
        src/main/cpp/jerry-port/default/default-external-context.c
        src/main/cpp/jerry-port/default/default-fatal.c
        src/main/cpp/jerry-port/default/default-io.c
        )


# add jerry-ext
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-ext/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-ext/include/jerryscript-ext
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jerry-ext/common
)

set(JERRY_EXT_SRC_LIST
        src/main/cpp/jerry-ext/arg/arg.c
        src/main/cpp/jerry-ext/arg/arg-js-iterator-helper.c
        src/main/cpp/jerry-ext/arg/arg-transform-functions.c
        src/main/cpp/jerry-ext/debugger/debugger-common.c
        src/main/cpp/jerry-ext/debugger/debugger-rp.c
        src/main/cpp/jerry-ext/debugger/debugger-serial.c
        src/main/cpp/jerry-ext/debugger/debugger-sha1.c
        src/main/cpp/jerry-ext/debugger/debugger-tcp.c
        src/main/cpp/jerry-ext/debugger/debugger-ws.c

        src/main/cpp/jerry-ext/handle-scope/handle-scope.c
        src/main/cpp/jerry-ext/handle-scope/handle-scope-allocator.c
        src/main/cpp/jerry-ext/handler/handler-assert.c
        src/main/cpp/jerry-ext/handler/handler-gc.c
        src/main/cpp/jerry-ext/handler/handler-print.c
        src/main/cpp/jerry-ext/handler/handler-register.c
        src/main/cpp/jerry-ext/module/module.c
        )

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             jerryscript

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp
             ${JERRY_LIBM_SRC_LIST}
             ${JERRY_CORE_SRC_LIST}
             ${JERRY_PORT_SRC_LIST}
             ${JERRY_EXT_SRC_LIST}
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                        jerryscript

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )