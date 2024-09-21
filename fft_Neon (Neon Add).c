#include <math.h>
#include "fft.h"
#include <arm_neon.h>

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

void
compute(float data_re[], float data_im[], const unsigned int N)
{
  const float pi = -3.14159265358979323846;
  
  for(unsigned int step=1; step<N; step <<=1) {
    const unsigned int jump = step << 1;
    const float step_d = (float) step;
    float twiddle_re = 1.0;
    float twiddle_im = 0.0;
    for(unsigned int group=0; group<step; group++)
      {
	for(unsigned int pair=group; pair<N; pair+=jump)
	  {
	    const unsigned int match = pair + step;
      
      float temp_vector0[2];
      float temp_vector1[2];
      float temp_vector2[2];
      float temp_vector3[2];
      temp_vector0[0] = twiddle_re;
      temp_vector1[0] = data_re[match];
      temp_vector2[0] = -twiddle_im;
      temp_vector3[0] = data_im[match];
      temp_vector0[1] = twiddle_im;
      temp_vector1[1] = data_re[match];
      temp_vector2[1] = twiddle_re;
      temp_vector3[1] = data_im[match];
      
      //load
      float32x2_t vector0 = vld1_f32(temp_vector0);
      float32x2_t vector1 = vld1_f32(temp_vector1);
      float32x2_t vector2 = vld1_f32(temp_vector2);
      float32x2_t vector3 = vld1_f32(temp_vector3);
      //mult & add
      float32x2_t data0 = vmul_f32(vector0, vector1); //mult
      float32x2_t data1 = vmul_f32(vector2, vector3); //mult
      float32x2_t ans = vadd_f32(data0, data1); //add
      //product_re = ans[0];
      //product_im = ans[1];

	    data_re[match] = data_re[pair]-ans[0];
	    data_im[match] = data_im[pair]-ans[1];
	    data_re[pair] += ans[0];
	    data_im[pair] += ans[1];
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