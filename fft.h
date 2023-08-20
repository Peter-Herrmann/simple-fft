/**************************************************************************************************
 * fft.h : A simple Fast Fourier Transform (FFT) function for use in embedded
 *         systems.
 *  
 *         The function uses two statically sized buffers to shuffle
 *         data during computation; time domain (input) data should be added
 *         to the buffer "input_buffer" and results should be read from
 *         "output_buffer". The buffers will be overwritten during computation.
 *
 * Created on: Nov 16, 2022
 *     Author: Peter Herrmann
 *************************************************************************************************/

#ifndef INC_FFT_H_
#define INC_FFT_H_

#include <complex.h>
#include <stdint.h>

#define FFT_BUFFER_SIZE 4096

typedef float complex sample_t;
typedef float freq_t; // Typically Hz
typedef uint16_t dig_t;

/* Write inputs to input buffer, read outputs from output_buffer */
extern sample_t *input_buffer, *output_buffer;


/* Computes FFT on input_buffer, returns a pointer to the output_buffer */
sample_t *fft();


/* Given a buffer of scalar digital inputs, loads the complex input_buffer */
void load_real_inputs(dig_t *raw_inputs);


/* Overwrites frequency buffers with scalar magnitudes.
 * Uses sample_frequency to return frequency with highest magnitude */
freq_t find_freq(sample_t *freq_buff, freq_t sample_frequency);


#endif /* INC_FFT_H_ */