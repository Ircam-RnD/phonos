#define MINIAUDIO_IMPLEMENTATION
#include "articulation.h"
#include "utility/audiowrite.h"
#include "utility/maths.h"
#include "vocal_folds/body_cover_pair.h"
#include "voice.h"
#include "vowels.h"

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

int main(int, char*[])
{
    std::string path = "voice_apa.wav";
    float samplerate = 48000;
    float duration = 2.0;
    float subglottal_pressure = 800;
    std::size_t num_sample = static_cast<int>(samplerate * duration);
    phonos::Voice<phonos::BodyCoverPair<double>, double> proc(samplerate, true);
    phonos::Articulation art;
    art.SetFromVowel(phonos::vowels::a);
    proc.get_resonator()->set_l0(17e-2);
    proc.get_resonator()->set_time_varying_geometry(true);
    proc.get_resonator()->SetTargetGeometryFromArticulation(art);
    proc.get_resonator()->set_lp_frequencies(10);
    proc.get_resonator()->initializeFilters();

    proc.set_lambda_sav(1000);

    // Artciulation definition
    auto six_point_art_a = art.get_reduced_set();
    auto six_point_art_p = six_point_art_a;
    six_point_art_p.areas[5] = 0;

    auto current_six_point_art = six_point_art_a;

    float delay{0.8}, closing{0.1}, closed{0.2}, opening{0.1}; // Times in s
    float t1 = delay;
    float t2 = delay + closing;
    float t3 = t2 + closed;
    float t4 = t3 + opening;

    std::vector<float> samples;
    samples.resize(num_sample);

    float current_time = 0;
    for (int i = 0; i < samples.size(); i++) {

        if (current_time < t1) {

        } else if (current_time < t2) {
            current_six_point_art.areas[5] =
                (six_point_art_a.areas[5] * (t2 - current_time) + six_point_art_p.areas[5] * (current_time - t1)) /
                (t2 - t1);
            art.setFromReducedSet(current_six_point_art);
            proc.get_resonator()->SetTargetGeometryFromArticulation(art);
        } else if (current_time < t3) {
            art.setFromReducedSet(six_point_art_p);
            proc.get_resonator()->SetTargetGeometryFromArticulation(art);
        } else if (current_time < t4) {
            current_six_point_art.areas[5] =
                (six_point_art_p.areas[5] * (t4 - current_time) + six_point_art_a.areas[5] * (current_time - t3)) /
                (t4 - t3);
            art.setFromReducedSet(current_six_point_art);
            proc.get_resonator()->SetTargetGeometryFromArticulation(art);
        } else {
            art.setFromReducedSet(six_point_art_a);
            proc.get_resonator()->SetTargetGeometryFromArticulation(art);
        }

        proc.Process(subglottal_pressure);
        samples[i] = proc.ReadRadiatedPressure();
        current_time = i / samplerate;
    }
    phonos::normalize_vector(samples);
    // Write an example wav file
    phonos::WriteWav(path, samples, samplerate);
    return 0;
}
