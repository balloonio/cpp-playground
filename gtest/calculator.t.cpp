#include "gtest/gtest.h"
#include "calculator.h"

namespace Playground
{
TEST (CalculatorTest, PositiveAdd)
{
    Calculator cal = Calculator();
    EXPECT_EQ ( 20, cal.add(4, 16) );
}

int main(int argc, char * argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
}
