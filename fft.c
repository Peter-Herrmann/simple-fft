/**************************************************************************************************
 * fft.c : A simple Fast Fourier Transform (FFT) function for use in embedded systems.
 *
 * Created on: Nov 16, 2022
 *     Author: Peter Herrmann
 *************************************************************************************************/

#include "fft.h"

#define N FFT_BUFFER_SIZE
#define N_BIT_WIDTH 12

typedef uint16_t count_t;

const float tau = 6.28318530718;

// Buffers used for IO. Used during decimation and butterfly
sample_t buffers[2][N];

// Interface variables for storing IO (big stuff, don't want to make copies)
sample_t *input_buffer  = buffers[0];
sample_t *output_buffer = buffers[1];


//// Butterfly Operations /////////////////////////////////////////////////////////////////////////

/* Adds the "straight" (no multiplication) sections of the butterfly */
void butterfly_straight(const sample_t *in, sample_t *out, count_t num_elem)
{
	for(count_t i = 0; i < num_elem / 2; i++)
	{
		out[i]                = in[2 * i];
		out[num_elem / 2 + i] = in[2 * i + 1];
	}
}

/* Adds the butterfly sections that must be multiplied by a twiddle factor */
void butterfly_twiddle(const sample_t *in, sample_t *out,
					   sample_t twiddle_N, count_t num_elem)
{
	for(count_t i = 0; i < num_elem / 2; i++)
	{
		out[i]                = in[2 * i] + in[2 * i + 1] * cexp(-tau * I * twiddle_N);
		out[num_elem / 2 + i] = in[2 * i] - in[2 * i + 1] * cexp(-tau * I * twiddle_N);
	}
}


//// Reverse_Bits /////////////////////////////////////////////////////////////////////////////////

/* Returns the num_bits least significant bits of input in reverse order */
sample_t reverse_bits(count_t input, count_t num_bits)
{
	count_t output = 0;

	for(count_t i = 0; i < num_bits; i++)
		output |= ( (input >> i) & 1 ) << (num_bits - 1 - i);

	return (sample_t)output;
}


//// Decimation and Butterflies ///////////////////////////////////////////////////////////////////
// Butterfly and decimation are not done in normal sequence; since they are all multiply 
// accumulate operations, they can be done in any order.


/* Performs decimation BIT_WIDTH - 1 times, each time completing the straight
 * sections of the butterfly. The buffers are shuffled back and forth. */
int decimate_straight(int b_sel)
{
	for(int bit_pos = 0; bit_pos < N_BIT_WIDTH - 1; bit_pos++, b_sel++)
	{
		count_t DFT_points = N >> bit_pos;

		for(count_t DFT_block = 0; DFT_block < N; DFT_block += DFT_points)
			butterfly_straight(buffers[b_sel & 1] + DFT_block,
                                           buffers[~b_sel & 1] + DFT_block,
                                           DFT_points);
	}

	return b_sel;
}

/* Performs decimation BIT_WIDTH times, each time performing the twiddle factor sections of the 
 * butterfly. */
int decimate_twiddle(int b_sel)
{
	for(int bit_pos = 0; bit_pos < N_BIT_WIDTH; bit_pos++, b_sel++)
	{
		count_t DFT_points = N >> bit_pos;

		for(count_t DFT_block = 0; DFT_block < N; DFT_block += DFT_points)
		{
			sample_t twiddle_N = reverse_bits(DFT_block / DFT_points,
                                                          bit_pos) / (sample_t)(2 << bit_pos);
			butterfly_twiddle(buffers[ b_sel & 1] + DFT_block,
                                          buffers[~b_sel & 1] + DFT_block,
                                          twiddle_N, DFT_points);
		}
	}

	return b_sel;
}


//// FFT //////////////////////////////////////////////////////////////////////////////////////////

sample_t *fft()
{
	// The first bit of buffer-select is used to determine which buffer is which.
	uint8_t buffer_select = (input_buffer == buffers[0]) ? 0 : 1;

	buffer_select = decimate_straight(buffer_select);
	buffer_select = decimate_twiddle (buffer_select);
	buffer_select = decimate_straight(buffer_select);

	if (buffer_select & 1)
	{
		input_buffer  = buffers[0];
		output_buffer = buffers[1];
	}
	else
	{
		input_buffer  = buffers[1];
		output_buffer = buffers[0];
	}

	return output_buffer;
}


//// Load Real Inputs /////////////////////////////////////////////////////////////////////////////

void load_real_inputs(dig_t *raw_inputs)
{
	for (uint16_t i = 0; i < N; i++)
		input_buffer[i] = raw_inputs[i];
}


//// Find Frequency ///////////////////////////////////////////////////////////////////////////////

freq_t find_freq(sample_t *freq_buff, freq_t sample_frequency)
{
	freq_t max = 0;
	count_t max_index = 0;

	for (int i = 1; i < N / 2; i++)
	{
		freq_t mag = (freq_buff[i] = (freq_t)cabs(freq_buff[i]));
		if (mag >= max)
		{
			max = mag;
			max_index = i;
		}
	}

	return (sample_frequency * (max_index)) / N;
}
