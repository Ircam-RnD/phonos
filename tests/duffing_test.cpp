#include <duffing.h>

#include <gtest/gtest.h>

TEST(DuffingTest, Instanciates)
{
    phonos::Duffing<float> model(44100);
}