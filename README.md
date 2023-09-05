# FFT Library for Embedded Systems

![CI](https://github.com/Peter-Herrmann/simple-fft/actions/workflows/test.yml/badge.svg)

`fft.h` is a Fast Fourier Transform (FFT) library designed for use in embedded systems.

## Overview

This library provides an FFT function that uses two statically sized buffers for data shuffling during computation. Input data should be added to the `input_buffer`, and the results can be read from the `output_buffer`. Shuffling happens in place for a minimal memory footprint, so buffers will be overwritten during computation.

## Data Types and Definitions

- `FFT_BUFFER_SIZE`: Defines the buffer size as 4096.
- `sample_t`: Typedef for complex numbers of type float by default. Integer types give very poor accuracy, and doubles will double the memory footprint, but both can be used.
- `freq_t`: Typedef for frequency (typically Hz), represented as a float. 
- `dig_t`: Typedef for raw digital input data.
- `count_t`: Typedef for counting, represented as a uint16_t.

## Functions

### `sample_t *fft()`

This function computes the FFT on the `input_buffer`, overwriting both input and output buffers. It returns a pointer to the `output_buffer`.

### `void load_real_inputs(dig_t *raw_inputs)`

This function takes a buffer of scalar digital inputs and loads them into the complex `input_buffer`.

### `freq_t find_freq(sample_t *freq_buff, freq_t sample_frequency)`

This function takes a complex frequency buffer (the output of an FFT on time domain data), and calculates the scalar magnitude of each frequency bucket, discarding the phase information. It also takes the sampling frequency of the time domain data, in order to return the value (typically frequency in Hz) of the highest magnitude bucket.

## Usage

1. Write inputs to `input_buffer` at a regular time interval.
2. Call `fft()` to perform the FFT on the input data.
3. Utilize functions like `load_real_inputs()` and `find_freq()` to work with raw inputs and frequency information.
4. Read the results from the `output_buffer`.

## Dependencies

- `<complex.h>`: For complex number support.
- `<stdint.h`: For explicit integer types.
