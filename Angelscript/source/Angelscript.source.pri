HEADERS += \
    Angelscript/source/as_array.h \
    Angelscript/source/as_atomic.h \
    Angelscript/source/as_builder.h \
    Angelscript/source/as_bytecode.h \
    Angelscript/source/as_callfunc.h \
    Angelscript/source/as_callfunc_arm_gcc.S \
    Angelscript/source/as_callfunc_arm_xcode.S \
    Angelscript/source/as_compiler.h \
    Angelscript/source/as_config.h \
    Angelscript/source/as_configgroup.h \
    Angelscript/source/as_context.h \
    Angelscript/source/as_criticalsection.h \
    Angelscript/source/as_datatype.h \
    Angelscript/source/as_debug.h \
    Angelscript/source/as_gc.h \
    Angelscript/source/as_generic.h \
    Angelscript/source/as_map.h \
    Angelscript/source/as_memory.h \
    Angelscript/source/as_module.h \
    Angelscript/source/as_namespace.h \
    Angelscript/source/as_objecttype.h \
    Angelscript/source/as_outputbuffer.h \
    Angelscript/source/as_parser.h \
    Angelscript/source/as_property.h \
    Angelscript/source/as_restore.h \
    Angelscript/source/as_scriptcode.h \
    Angelscript/source/as_scriptengine.h \
    Angelscript/source/as_scriptfunction.h \
    Angelscript/source/as_scriptnode.h \
    Angelscript/source/as_scriptobject.h \
    Angelscript/source/as_string.h \
    Angelscript/source/as_string_util.h \
    Angelscript/source/as_symboltable.h \
    Angelscript/source/as_texts.h \
    Angelscript/source/as_thread.h \
    Angelscript/source/as_tokendef.h \
    Angelscript/source/as_tokenizer.h \
    Angelscript/source/as_typeinfo.h \
    Angelscript/source/as_variablescope.h

SOURCES += \
    Angelscript/source/as_atomic.cpp \
    Angelscript/source/as_builder.cpp \
    Angelscript/source/as_bytecode.cpp \
    Angelscript/source/as_callfunc.cpp \
    Angelscript/source/as_callfunc_arm.cpp \
    Angelscript/source/as_callfunc_mips.cpp \
    Angelscript/source/as_callfunc_ppc.cpp \
    Angelscript/source/as_callfunc_ppc_64.cpp \
    Angelscript/source/as_callfunc_sh4.cpp \
    Angelscript/source/as_callfunc_x64_gcc.cpp \
    Angelscript/source/as_callfunc_x64_mingw.cpp \
    Angelscript/source/as_callfunc_x64_msvc.cpp \
    Angelscript/source/as_callfunc_x86.cpp \
    Angelscript/source/as_callfunc_xenon.cpp \
    Angelscript/source/as_compiler.cpp \
    Angelscript/source/as_configgroup.cpp \
    Angelscript/source/as_context.cpp \
    Angelscript/source/as_datatype.cpp \
    Angelscript/source/as_gc.cpp \
    Angelscript/source/as_generic.cpp \
    Angelscript/source/as_globalproperty.cpp \
    Angelscript/source/as_memory.cpp \
    Angelscript/source/as_module.cpp \
    Angelscript/source/as_objecttype.cpp \
    Angelscript/source/as_outputbuffer.cpp \
    Angelscript/source/as_parser.cpp \
    Angelscript/source/as_restore.cpp \
    Angelscript/source/as_scriptcode.cpp \
    Angelscript/source/as_scriptengine.cpp \
    Angelscript/source/as_scriptfunction.cpp \
    Angelscript/source/as_scriptnode.cpp \
    Angelscript/source/as_scriptobject.cpp \
    Angelscript/source/as_string.cpp \
    Angelscript/source/as_string_util.cpp \
    Angelscript/source/as_thread.cpp \
    Angelscript/source/as_tokenizer.cpp \
    Angelscript/source/as_typeinfo.cpp \
    Angelscript/source/as_variablescope.cpp

OTHER_FILES += \
    Angelscript/source/as_callfunc_arm_msvc.asm \
    Angelscript/source/as_callfunc_x64_msvc_asm.asm
