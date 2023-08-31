#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "../fft.h"

#define TEST_1_SAMPLE_RATE  (500)
#define TEST_2_SAMPLE_RATE  (2000)

#define OFFSET              (1000)
#define FREQ_1_MAG          (750)
#define FREQ_2_MAG          (375)

#define TEST_1_FREQ         (100)
#define TEST_2_FREQ_1       (50)
#define TEST_2_FREQ_2       (500)

#define PASS                (0)
#define FAIL                (1)

// Magnitude tolerance (unitless) 
#define TOLERANCE           (5.0)



void generate_1_sine_wave(dig_t *samples, freq_t freq, int sample_rate, int num_samples) 
{
    for (int i = 0; i < num_samples; i++) 
    {
        samples[i] = (dig_t)(FREQ_1_MAG * sin( 2 * M_PI * freq * i / sample_rate ) + OFFSET);
    }
}


void generate_2_sine_wave(dig_t *samples, freq_t freq1, freq_t freq2, 
                          int sample_rate, int num_samples) 
{
    for (int i = 0; i < num_samples; i++) 
    {
        samples[i] = (dig_t)(FREQ_1_MAG * sin( 2 * M_PI * freq1 * i / sample_rate )
                           + FREQ_2_MAG * sin( 2 * M_PI * freq2 * i / sample_rate )
                           + OFFSET);
    }
}


void test_fft() 
{
    dig_t samples[FFT_BUFFER_SIZE];
    sample_t output_buffer[FFT_BUFFER_SIZE];

    // Test for single sine wave
    generate_1_sine_wave(samples, TEST_1_FREQ, TEST_1_SAMPLE_RATE, FFT_BUFFER_SIZE);
    load_real_inputs(samples);
    fft();
    freq_t detected_freq = find_freq(output_buffer, TEST_1_SAMPLE_RATE);
    printf("Expected freq for single sine wave: %d, Detected: %f\n", TEST_1_FREQ, detected_freq);
    assert(detected_freq == TEST_1_FREQ);

    // Test for multiple sine waves, make sure both are detected at correct magnitudes
    generate_2_sine_wave(samples, TEST_2_FREQ_1, TEST_2_FREQ_2, TEST_2_SAMPLE_RATE, FFT_BUFFER_SIZE);
    load_real_inputs(samples);
    fft();
    freq_t detected_freq2 = find_freq(output_buffer, TEST_2_SAMPLE_RATE);
    printf("Expected primary freq for two sine waves: %d, Detected: %f\n", TEST_2_FREQ_1, detected_freq2);
    assert(detected_freq2 == TEST_2_FREQ_1);
    int f2_bucket = (FFT_BUFFER_SIZE * TEST_2_FREQ_2) / TEST_2_SAMPLE_RATE;
    double magnitude_diff = fabs((double)output_buffer[f2_bucket] - (double)FREQ_2_MAG);
    printf("Expected magnitude for secondary freq: %d, Detected magnitude difference: %f\n", FREQ_2_MAG, magnitude_diff);
    assert(magnitude_diff <= TOLERANCE);
}



int main() 
{
    test_fft();
    return 0;
}
