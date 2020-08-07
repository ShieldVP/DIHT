#include <gtest/gtest.h>
#include "main.h"

TEST ( TestConst1, A ) {
  ASSERT_EQ(A, 5);
}

TEST ( TestConst2, B ) {
  ASSERT_EQ(B, 10);
}
