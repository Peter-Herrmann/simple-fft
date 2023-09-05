#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "fft.h"

#define TEST_1_SAMPLE_RATE      (500)
#define TEST_2_SAMPLE_RATE      (2000)

#define OFFSET                  (1000)
#define FREQ_1_MAG              (750)
#define FREQ_2_MAG              (375)

#define TEST_1_FREQ             (20)
#define TEST_2_FREQ_1           (50)
#define TEST_2_FREQ_2           (100)

#define PASS                    (0)
#define FAIL                    (1)

// Magnitude tolerance (unitless) 
#define MAG_TOLERANCE           (5.0)
#define FREQ_TOLERANCE(rate)    (((double)rate)/((double)FFT_BUFFER_SIZE))



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


void print_samples(dig_t *samples, int num_samples) 
{
    printf("Printing samples:\n");
    for (int i = 0; i < num_samples; i++) 
    {
        printf("%d, %u\n", i, samples[i]);
    }
    printf("\n");
}

void print_fft_output(sample_t *output, int num_samples) 
{
    printf("Printing FFT output:\n");
    for (int i = 0; i < num_samples/2; i++) 
    {
        printf("%d,%f, %f\n", i, creal(output[i]), cimag(output[i]));
    }
    printf("\n");
}


void test_fft() 
{
    dig_t samples[FFT_BUFFER_SIZE];

    // Test for single sine wave
    generate_1_sine_wave(samples, TEST_1_FREQ, TEST_1_SAMPLE_RATE, FFT_BUFFER_SIZE);
    load_real_inputs(samples);
    fft();
    freq_t detected_freq = find_freq(output_buffer, TEST_1_SAMPLE_RATE);
    printf("Expected freq for single sine wave: %d, Detected: %f\n", TEST_1_FREQ, detected_freq);
    assert(fabs(detected_freq - TEST_1_FREQ) <= FREQ_TOLERANCE(TEST_1_SAMPLE_RATE));

    // Test for multiple sine waves, make sure both are detected at correct magnitudes
    generate_2_sine_wave(samples, TEST_2_FREQ_1, TEST_2_FREQ_2, TEST_2_SAMPLE_RATE, FFT_BUFFER_SIZE);
    load_real_inputs(samples);
    fft();
    freq_t detected_freq2 = find_freq(output_buffer, TEST_2_SAMPLE_RATE);
    printf("Expected primary freq for two sine waves: %d, Detected: %f\n", TEST_2_FREQ_1, detected_freq2);
    assert(fabs(detected_freq2 - TEST_2_FREQ_1) <= FREQ_TOLERANCE(TEST_2_SAMPLE_RATE));
    int f2_bucket = (int)round(((double)FFT_BUFFER_SIZE * (double)TEST_2_FREQ_2) / (double)TEST_2_SAMPLE_RATE);
    printf("Magnitude for secondary freq %d (bucket %d): %f\n", TEST_2_FREQ_2, f2_bucket, (float)output_buffer[f2_bucket]);
    assert((double)output_buffer[f2_bucket] >= FREQ_2_MAG * 1000); // FFT output not to scale
}



int main() 
{
    test_fft();
    return 0;
}
