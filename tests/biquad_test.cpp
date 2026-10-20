#include <utility/biquad.h>

#include <gtest/gtest.h>

TEST(BiquadTest, Instanciates)
{
    phonos::Biquad filter(44100.0f);
}

TEST(BiquadTest, Processes)
{
    phonos::Biquad filter(44100.0f);
    double out = filter.Process(1.0f);
}