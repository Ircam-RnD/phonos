#include <iostream>
#include <chrono>
#include <vector>
#include <utility/biquad.h>

int main() {
    const int sample_rate = 44100;
    const double duration = 1.0; // 1 second
    const int num_samples = static_cast<int>(sample_rate * duration);
    
    std::cout << "Simple Biquad Process benchmark\n";
    std::cout << "Duration: " << duration << " seconds\n";
    std::cout << "Sample rate: " << sample_rate << " Hz\n";
    std::cout << "Number of samples: " << num_samples << "\n\n";
    
    // Create test signal (sine wave)
    std::vector<double> input_signal(num_samples);
    for (int i = 0; i < num_samples; ++i) {
        input_signal[i] = std::sin(2.0 * M_PI * 440.0 * i / sample_rate); // 440 Hz sine wave
    }
    
    // Create biquad filter in lowpass mode
    phonos::Biquad filter(sample_rate, phonos::kLowPass, 1000.0, 0.0, 0.7);
    
    // Warm up the function
    for (int i = 0; i < 1000; ++i) {
        filter.Process(input_signal[i % num_samples]);
    }
    
    // Benchmark the Process function
    auto start_time = std::chrono::high_resolution_clock::now();
    
    double result = 0.0;
    for (int i = 0; i < num_samples; ++i) {
        result += filter.Process(input_signal[i]);
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    
    double time_per_sample = static_cast<double>(duration_ns.count()) / num_samples;
    
    std::cout << "Total processing time: " << duration_ns.count() << " nanoseconds\n";
    std::cout << "Average time per sample: " << time_per_sample << " nanoseconds\n";
    std::cout << "Result sum (for verification): " << result << "\n";
    
    return 0;
}