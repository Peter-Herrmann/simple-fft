/******************************************************************************
 * fft.h : A simple Fast Fourier Transform (FFT) function for use in embedded
 * 		   systems.
 *
 * 		   The function uses two statically sized buffers to shuffle
 * 		   data during computation; time domain (input) data should be added
 * 		   to the buffer "input_buffer" and results should be read from
 * 		   "output_buffer". The buffers will be overwritten during computation.
 *
 *  Created on: Nov 16, 2022
 *      Author: Peter Herrmann
 *****************************************************************************/

#ifndef INC_FFT_H_
#define INC_FFT_H_

#include <complex.h>
#include "stm32l4xx_hal.h"

#define FFT_BUFFER_SIZE 4096

// "Complex" macros do not work with explicit int types (like uint16_t)
typedef float complex sample_t;
typedef float freq_t;
typedef uint16_t dig_t;
typedef uint16_t count_t;

/* Write inputs to input buffer, read outputs from output_buffer */
extern sample_t *input_buffer, *output_buffer;


/* Computes FFT on input_buffer, overwriting input and output buffers.
 *     returns a pointer to the output_buffer */
sample_t *fft();


/* Given a buffer of scalar digital inputs, loads the complex input_buffer */
void load_real_inputs(dig_t *raw_inputs);


/* Overwrites frequency buffers with scalar magnitudes.
 *     returns the frequency of the largest magnitude bucket */
freq_t find_freq(sample_t *freq_buff, freq_t sample_frequency);


#endif /* INC_FFT_H_ */