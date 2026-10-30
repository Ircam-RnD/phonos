#pragma once

#include <algorithm>
#include <cmath>

namespace phonos {

enum BiquadMode { kLowPass, kHighPass, kBandPass, kNotch, kPeak, kLowShelf, kHighShelf };

class Biquad {
private:
    // Filter coefficients
    double b0_{1}, b1_{0}, b2_{0}, a0_{1}, a1_{0}, a2_{0};

    // Filter state variables
    double x0_{0}, x1_{0}, x2_{0}, y0_{0}, y1_{0}, y2_{0};

    // Filter parameters
    float sr_{44100};
    float freq_{100};
    float dB_gain_{0};
    float Q_{0.7};
    BiquadMode mode{kLowPass};

    // Precomputed values for coefficient calculation
    double A_, omega_, sin_omega_, cos_omega_, alpha_;

    void ComputeCoeffs()
    {
        // Precompute common terms once per parameter change
        omega_ = 2 * M_PI * freq_ / sr_;
        cos_omega_ = std::cos(omega_);
        sin_omega_ = std::sin(omega_);

        // Calculate alpha for all modes
        if (Q_ > 0.0f) {
            alpha_ = sin_omega_ / (2 * Q_);
        } else {
            alpha_ = 0;
        }

        switch (mode) {
        case kLowPass:
            {
                double one_minus_cos = 1 - cos_omega_;
                b0_ = one_minus_cos / 2;
                b1_ = one_minus_cos;
                b2_ = b0_;
                a0_ = 1 + alpha_;
                a1_ = -2 * cos_omega_;
                a2_ = 1 - alpha_;
            }
            break;
        case kHighPass:
            {
                double one_plus_cos = 1 + cos_omega_;
                b0_ = one_plus_cos / 2;
                b1_ = -one_plus_cos;
                b2_ = b0_;
                a0_ = 1 + alpha_;
                a1_ = -2 * cos_omega_;
                a2_ = 1 - alpha_;
            }
            break;
        case kBandPass:
            {
                b0_ = Q_ * alpha_;
                b1_ = 0;
                b2_ = -Q_ * alpha_;
                a0_ = 1 + alpha_;
                a1_ = -2 * cos_omega_;
                a2_ = 1 - alpha_;
            }
            break;
        case kNotch:
            {
                b0_ = 1;
                b1_ = -2 * cos_omega_;
                b2_ = 1;
                a0_ = 1 + alpha_;
                a1_ = -2 * cos_omega_;
                a2_ = 1 - alpha_;
            }
            break;
        case kPeak:
            {
                // Precompute A for peak filter
                double sqrt_A = std::sqrt(std::pow(10, dB_gain_ / 20));
                double alpha_A = alpha_ * sqrt_A;

                b0_ = 1 + alpha_A;
                b1_ = -2 * cos_omega_;
                b2_ = 1 - alpha_A;
                a0_ = 1 + alpha_ / sqrt_A;
                a1_ = -2 * cos_omega_;
                a2_ = 1 - alpha_ / sqrt_A;
            }
            break;
        case kLowShelf:
            {
                // Precompute A for shelf filters
                double sqrt_A = std::sqrt(std::pow(10, dB_gain_ / 20));
                A_ = sqrt_A;

                double beta = sqrt_A / Q_;
                double a_plus_1 = A_ + 1;
                double a_minus_1 = A_ - 1;
                double a_minus_1_cos = a_minus_1 * cos_omega_;
                double a_plus_1_cos = a_plus_1 * cos_omega_;

                b0_ = A_ * (a_plus_1 - a_minus_1_cos + beta * sin_omega_);
                b1_ = 2 * A_ * (a_minus_1 - a_plus_1_cos);
                b2_ = A_ * (a_plus_1 - a_minus_1_cos - beta * sin_omega_);
                a0_ = a_plus_1 + a_minus_1_cos + beta * sin_omega_;
                a1_ = -2 * (a_minus_1 + a_plus_1_cos);
                a2_ = a_plus_1 + a_minus_1_cos - beta * sin_omega_;
            }
            break;
        case kHighShelf:
            {
                // Precompute A for shelf filters
                double sqrt_A = std::sqrt(std::pow(10, dB_gain_ / 20));
                A_ = sqrt_A;

                double beta = sqrt_A / Q_;
                double a_plus_1 = A_ + 1;
                double a_minus_1 = A_ - 1;
                double a_minus_1_cos = a_minus_1 * cos_omega_;
                double a_plus_1_cos = a_plus_1 * cos_omega_;

                b0_ = A_ * (a_plus_1 - a_minus_1_cos + beta * sin_omega_);
                b1_ = -2 * A_ * (a_minus_1 + a_plus_1_cos);
                b2_ = A_ * (a_plus_1 - a_minus_1_cos - beta * sin_omega_);
                a0_ = a_plus_1 - a_minus_1_cos + beta * sin_omega_;
                a1_ = 2 * (a_minus_1 - a_plus_1_cos);
                a2_ = a_plus_1 - a_minus_1_cos - beta * sin_omega_;
            }
            break;
        }
    }

public:
    Biquad(float sample_rate = 44100,
           BiquadMode mode = kLowPass,
           float frequency = 100,
           float dB_gain = 0,
           float Q = 0.7)
    {
        sr_ = sample_rate;
        set_mode(mode);
        set_freq(frequency);
        set_gain(dB_gain);
        set_Q(Q);
    }

    void set_freq(float freq_)
    {
        this->freq_ = std::clamp(freq_, 0.0f, sr_);
        ComputeCoeffs();
    }

    void set_gain(float dB_gain_)
    {
        this->dB_gain_ = std::clamp(dB_gain_, -60.0f, 20.0f);
        ComputeCoeffs();
    }

    void set_Q(float Q_)
    {
        this->Q_ = std::clamp(Q_, 0.001f, 100.0f);
        ComputeCoeffs();
    }

    void set_mode(BiquadMode mode)
    {
        this->mode = mode;
        ComputeCoeffs();
    }

    void InitializeState(double initial_value)
    {
        // Initialize filter states to steady-state value
        // For a step input, steady-state output equals DC gain * input
        double dc_gain = (b0_ + b1_ + b2_) / (a0_ + a1_ + a2_);
        double steady_state_output = dc_gain * initial_value;

        x0_ = x1_ = x2_ = initial_value;
        y0_ = y1_ = y2_ = steady_state_output;
    }

    double Process(double input)
    {
        // Direct form II transposed structure for better numerical stability
        double y0 = (b0_ * input + b1_ * x1_ + b2_ * x2_ - a1_ * y1_ - a2_ * y2_) / a0_;

        // Update delay line values
        x2_ = x1_;
        x1_ = input;
        y2_ = y1_;
        y1_ = y0;

        return y0;
    }
};

} // namespace phonos