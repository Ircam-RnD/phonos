#define MINIAUDIO_IMPLEMENTATION
#include "utility/audiowrite.h"
#include "utility/maths.h"

#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <websterFDTD.h>
#include <websterFDTD_optimized.h>

int main(int, char*[])
{
    std::string path = "websterFDTD.wav";
    float samplerate = 44100;
    float duration = 5; // Reduced for faster testing
    std::size_t num_sample = static_cast<int>(samplerate * duration);

    // Create both instances - one with original and one with optimized logic
    phonos::WebsterFDTD<float, 300> resonator1(samplerate, 1.0f);
    phonos::WebsterFDTD_optimized<float, 300> resonator2(samplerate, 1.0f);

    resonator1.set_yielding_walls(true);
    resonator1.SetConstantSection(1e-4);
    resonator1.set_time_varying_geometry(true);
    resonator1.set_l0(1.0);

    resonator2.set_yielding_walls(true);
    resonator2.SetConstantSection(1e-4);
    resonator2.set_time_varying_geometry(true);
    resonator2.set_l0(1.0);

    std::cout << "Number of segment is " << resonator1.get_N() << std::endl;

    // Test with same random inputs
    std::vector<float> samples1, samples2;
    samples1.resize(num_sample);
    samples2.resize(num_sample);

    // Run original version
    float input = 0;
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < samples1.size(); i++) {
        if (i == 0) {
            input = 1.f;
        } else {
            input = 0.f;
        }
        resonator1.Process(input);
        samples1[i] = resonator1.ReadRadiatedPressure();
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    double time1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() / duration;

    // Run comparison version
    t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < samples2.size(); i++) {
        if (i == 0) {
            input = 1.f;
        } else {
            input = 0.f;
        }
        resonator2.Process(input);
        samples2[i] = resonator2.ReadRadiatedPressure();
    }
    t2 = std::chrono::high_resolution_clock::now();
    double time2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() / duration;

    // Check if outputs are equal
    bool identical = true;
    for (size_t i = 0; i < samples1.size(); ++i) {
        if (std::abs(samples1[i] - samples2[i]) > 1e-6f) {
            identical = false;
            std::cout << "Difference at sample " << i << ": " << samples1[i] << " vs " << samples2[i] << std::endl;
            break;
        }
    }

    if (identical) {
        std::cout << "Results are identical within machine precision!" << std::endl;
    } else {
        std::cout << "ERROR: Results differ!" << std::endl;
    }

    std::cout << "Original version: " << time1 << " milliseconds of computation per second of output" << std::endl;
    std::cout << "Comparison version: " << time2 << " milliseconds of computation per second of output" << std::endl;
    return 0;
}