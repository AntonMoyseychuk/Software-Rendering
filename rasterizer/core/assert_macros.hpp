#pragma once
#include <cassert>

#define ASSERT_EXISTENCE_IN_CONTAINER(container, id) assert(container.count((id)) == 1)