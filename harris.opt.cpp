#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <omp.h>
#include <algorithm>

#ifndef PAD
#define PAD 4096
#endif

void  harris_opt(int  C, int  R, float * img, float *& harris)
{
  
  float * Ixx;
  Ixx = (float *) (malloc((sizeof(float ) * ((2 + R) * (2 + C)))));
  
  float *dummy3;
  dummy3 = (float *) (malloc(PAD));
  
  float * Ixy;
  Ixy = (float *) (malloc((sizeof(float ) * ((2 + R) * (2 + C)))));
  
  float *dummy4;
  dummy4 = (float *) (malloc(PAD));
  
  float * Iyy;
  Iyy = (float *) (malloc((sizeof(float ) * ((2 + R) * (2 + C)))));
  
  float *dummy5;
  dummy5 = (float *) (malloc(PAD));
  
  float * Sxx;
  Sxx = (float *) (malloc((sizeof(float ) * ((2 + R) * (2 + C)))));
  
  float *dummy6;
  dummy6 = (float *) (malloc(PAD));
  
  float * Sxy;
  Sxy = (float *) (malloc((sizeof(float ) * ((2 + R) * (2 + C)))));
  
  float *dummy7;
  dummy7 = (float *) (malloc(PAD));
  
  float * Syy;
  Syy = (float *) (malloc((sizeof(float ) * ((2 + R) * (2 + C)))));

  harris = (float *) (malloc((sizeof(float ) * ((2 + R) * (2 + C)))));


  /*#ifndef B
  #define B 64
  #endif
  
  #pragma ivdep
  #pragma omp parallel for
  for (int  i = 1; (i <= R); i+=B)
  {
	  
    for (int  j = 1; (j <= C); j+=B)
    {
		for (int ii  = i; (ii<=R) && (ii<=i+B); ii++) {
			
		  for (int jj = j; (jj<=C) && (jj<=j+B); jj++) {
			  
			int index1 = (((-1 + ii) * (C + 2)) + (-1 + jj));		// [i-1] [j-1]
			int index2 = (((1 + ii) * (C + 2)) + (-1 + jj)); 		// [i+1] [j-1]
			int index3 = (((-1 + ii) * (C + 2)) + jj);			// [i-1] [j]
			int index4 = (((1 + ii) * (C + 2)) + jj);				// [i+1] [j]
			int index5 = (((-1 + ii) * (C + 2)) + (1 + jj)); 		// [i-1] [j+1]
			int index6 = (((1 + ii) * (C + 2)) + (1 + jj));		// [i+1] [j+1]
			int index7 = ((ii * (C + 2)) + (-1 + jj));			// [i] [j-1]
			int index8 = ((ii * (C + 2)) + (1 + jj));				// [i] [j+1]
			// X derivative
			Ix[((ii * (2 + C)) + jj)] = (img[index1] * -0.0833333333333f) + 
										(img[index2] * 0.0833333333333f) + 
										(img[index3] * -0.166666666667f) + 
										(img[index4] * 0.166666666667f) + 
										(img[index5] * -0.0833333333333f) + 
										(img[index6] * 0.0833333333333f);
                                
			// Y derivative
			Iy[((ii * (2 + C)) + jj)] = (img[index1] * -0.0833333333333f) + 
										(img[index5] * 0.0833333333333f) + 
										(img[index7] * -0.166666666667f) + 
										(img[index8] * 0.166666666667f) + 
										(img[index2] * -0.0833333333333f) + 
										(img[index6] * 0.0833333333333f);
                                
          }
		}
		
	}
  }*/
 
  #pragma ivdep
  #pragma omp parallel for
  for (int  i = 1; (i <= R); i = (i + 1))
  {
    for (int  j = 1; (j <= C); j = (j + 1))
    {
	  float resx,resy;
      // X derivative
      resx = (img[(((-1 + i) * (C + 2)) + (-1 + j))] * -0.0833333333333f) + 
                                (img[(((1 + i) * (C + 2)) + (-1 + j))] * 0.0833333333333f) + 
                                (img[(((-1 + i) * (C + 2)) + j)] * -0.166666666667f) + 
                                (img[(((1 + i) * (C + 2)) + j)] * 0.166666666667f) + 
                                (img[(((-1 + i) * (C + 2)) + (1 + j))] * -0.0833333333333f) + 
                                (img[(((1 + i) * (C + 2)) + (1 + j))] * 0.0833333333333f);
      
      
      // Y derivative
      resy = (img[(((-1 + i) * (C + 2)) + (-1 + j))] * -0.0833333333333f) + 
                                (img[(((-1 + i) * (C + 2)) + (1 + j))] * 0.0833333333333f) + 
                                (img[((i * (C + 2)) + (-1 + j))] * -0.166666666667f) + 
                                (img[((i * (C + 2)) + (1 + j))] * 0.166666666667f) + 
                                (img[(((1 + i) * (C + 2)) + (-1 + j))] * -0.0833333333333f) + 
                                (img[(((1 + i) * (C + 2)) + (1 + j))] * 0.0833333333333f);
                                
      Ixx[((i * (2 + C)) + j)] = resx * resx;
      Iyy[((i * (2 + C)) + j)] = resy * resy;
      Ixy[((i * (2 + C)) + j)] = resx * resy;
      
    }
  }
  

  
  #pragma ivdep
  #pragma omp parallel for
  for (int  i = 2; (i < R); i=i+1) {
    for (int  j = 2; (j < C); j=j+1) {
	  int index1, index2, index3;

	  index1 = (((-1 + i) * (2 + C)) + j);			// [i-1] [j]
	  index2 = ((i * (2 + C)) + j);					// [i] [j]
	  index3 = (((1 + i) * (2 + C)) + j);			// [i+1] [j]
	  
      Syy[index2] = Iyy[ index1-1 ] + 
                                 Iyy[ index1 ] + 
                                 Iyy[ index1+1 ] + 
                                 Iyy[ index2-1 ] + 
                                 Iyy[ index2 ] + 
                                 Iyy[ index2+1 ] + 
                                 Iyy[ index3-1 ] + 
                                 Iyy[ index3 ] + 
                                 Iyy[ index3+1 ];

 } }
 
  #pragma omp parallel for
  for (int  i = 2; (i < R); i=i+1) {
    for (int  j = 2; (j < C); j=j+1) {
	  int index1, index2, index3;

	  index1 = (((-1 + i) * (2 + C)) + j);			// [i-1] [j]
	  index2 = ((i * (2 + C)) + j);					// [i] [j]
	  index3 = (((1 + i) * (2 + C)) + j);			// [i+1] [j]
	  
      Sxy[index2] = Ixy[ index1-1 ] + 
                                 Ixy[ index1 ] +
                                 Ixy[ index1+1 ] + 
                                 Ixy[ index2-1 ] + 
                                 Ixy[ index2] + 
                                 Ixy[ index2+1 ] + 
                                 Ixy[ index3-1 ] + 
                                 Ixy[ index3 ] + 
                                 Ixy[ index3+1 ];
} }
 #pragma omp parallel for
  for (int  i = 2; (i < R); i=i+1) {
    for (int  j = 2; (j < C); j=j+1) {
	  int index1, index2, index3;

	  index1 = (((-1 + i) * (2 + C)) + j);			// [i-1] [j]
	  index2 = ((i * (2 + C)) + j);					// [i] [j]
	  index3 = (((1 + i) * (2 + C)) + j);			// [i+1] [j]
	  
      Sxx[index2] = Ixx[index1-1 ] + 
                                 Ixx[index1 ] + 
                                 Ixx[index1+1 ] + 
                                 Ixx[index2-1 ] + 
                                 Ixx[index2 ] + 
                                 Ixx[index2+1 ] + 
                                 Ixx[index3-1 ] + 
                                 Ixx[index3 ] + 
                                 Ixx[index3+1 ];
                                 
     
    } 
  }
  
  #pragma ivdep
  #pragma omp parallel for
  for (int  i = 2; (i < R); i++) {  
    for (int  j = 2; (j < C); j++) {
	  int index = (i * (2 + C)) + j;
	  float trace = 
          Sxx[index] + Syy[index];

      float det = 
          Sxx[index] * Syy[index] - Sxy[index] * Sxy[index];

      harris[index] = det - (0.04f * trace * trace);
    }
  }
  
  

  free(Ixx);
  free(Ixy);
  free(Iyy);
  free(Sxx);
  free(Sxy);
  free(Syy);
  free(dummy3); free(dummy4); free(dummy5);
  free(dummy6); free(dummy7);
}
