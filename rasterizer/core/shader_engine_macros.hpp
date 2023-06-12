#pragma once
#include "assert_macros.hpp"

#define ASSERT_UNIFORM_VALIDITY(container, name) ASSERT_EXISTENCE_IN_CONTAINER(container, name)
#define ASSERT_SHADER_PROGRAM_ID_VALIDITY(container, id) ASSERT_EXISTENCE_IN_CONTAINER(container, id)