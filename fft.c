#include <math.h>
#include "fft.h"

void
fft(float data_re[], float data_im[], const unsigned int N)
{
  rearrange(data_re, data_im, N);
  compute(data_re, data_im, N);
}

void
rearrange(float data_re[], float data_im[], const unsigned int N)
{
  unsigned int target = 0;
  for(unsigned int position=0; position<N;position++)
    {
      if(target>position) {
	const float temp_re = data_re[target];
	const float temp_im = data_im[target];
	data_re[target] = data_re[position];
	data_im[target] = data_im[position];
	data_re[position] = temp_re;
	data_im[position] = temp_im;
      }
      unsigned int mask = N;
      while(target & (mask >>=1))
	target &= ~mask;
      target |= mask;
    }
}

void compute(float data_re[], float data_im[], const unsigned int N)
{
  const float pi = -3.14159265358979323846;
  const int unroll = 2;

  for(unsigned int step=1; step<N; step <<=1) {
    const unsigned int jump = step << 1;
    const unsigned int jump_unroll = jump * unroll;
    const float step_d = (float) step;
    float twiddle_re = 1.0;
    float twiddle_im = 0.0;
    for(unsigned int group=0; group<step; group++)
    {
		//set up remainder for loop unroll
        int remainder = (group*jump) % unroll; //Need to make sure this is correct!
		int loop_remainder = N - remainder;

		if(group+jump_unroll >= N) //use naive if can't unroll at all (e.g. just 2 dft's)
		{
			for(unsigned int pair_N=group; pair_N<N; pair_N+=jump)
			{
				const unsigned int match = pair_N + step;
				const float product_re = twiddle_re*data_re[match]-twiddle_im*data_im[match];
				const float product_im = twiddle_im*data_re[match]+twiddle_re*data_im[match];
				data_re[match] = data_re[pair_N]-product_re;
				data_im[match] = data_im[pair_N]-product_im;
				data_re[pair_N] += product_re;
				data_im[pair_N] += product_im;
			}
		}
		

		else //unroll if can
		{
			for(unsigned int pair=group; pair<loop_remainder; pair+=jump_unroll)
			{
				const unsigned int match1 = pair + step;
				const unsigned int pair2 = pair + jump;
				const unsigned int match2 = pair2 + step;

				const float product_re = twiddle_re*data_re[match1]-twiddle_im*data_im[match1];
				const float product_im = twiddle_im*data_re[match1]+twiddle_re*data_im[match1];
				const float product_re2 = twiddle_re*data_re[match2]-twiddle_im*data_im[match2];
				const float product_im2 = twiddle_im*data_re[match2]+twiddle_re*data_im[match2];

				data_re[match1] = data_re[pair]-product_re;
				data_im[match1] = data_im[pair]-product_im;
				data_re[match2] = data_re[pair2]-product_re2;
				data_im[match2] = data_im[pair2]-product_im2;

				data_re[pair] += product_re;
				data_im[pair] += product_im;
				data_re[pair2] += product_re2;
				data_im[pair2] += product_im2;
			}

			if (remainder != 0) //use naive for remainder if exists
			{
				for(unsigned int pair_rem=loop_remainder; pair_rem<N; pair_rem+=jump)
				{
					const unsigned int match = pair_rem + step;
					const float product_re = twiddle_re*data_re[match]-twiddle_im*data_im[match];
					const float product_im = twiddle_im*data_re[match]+twiddle_re*data_im[match];
					data_re[match] = data_re[pair_rem]-product_re;
					data_im[match] = data_im[pair_rem]-product_im;
					data_re[pair_rem] += product_re;
					data_im[pair_rem] += product_im;
				}
			}
		}
	
	    // we need the factors below for the next iteration
	    // if we don't iterate then don't compute
	    if(group+1 == step)
	    {
	        continue;
	    }

	    float angle = pi*((float) group+1)/step_d;
	    twiddle_re = cos(angle);
	    twiddle_im = sin(angle);
    }
  }
}
