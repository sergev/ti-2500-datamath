add_library(ModelCxx STATIC
    ModelCxx.cpp
)

#set_target_properties(ModelCxx PROPERTIES Swift_MODULE_NAME "ModelCxx")

target_compile_options(ModelCxx PUBLIC
    "$<$<COMPILE_LANGUAGE:Swift>:-cxx-interoperability-mode=default>"
)
