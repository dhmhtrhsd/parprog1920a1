#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <emmintrin.h>

void getWallTime(double *wct){
	struct timeval tp;
	gettimeofday(&tp, NULL);
	*wct=(double)(tp.tv_sec + tp.tv_usec/1000000.0);
}


int main(){

	double ts, te, mflops;
  float *img, *final_img; //Declaration of two float pointers
	__m128 *vimg, *vfinal_img;
	float k0 = 0.5, k1 = 0.5, k2 = 0.5, k3 = 0.5, k5 = 0.5, k6 = 0.5, k7 = 0.5, k8 = 0.5, k4 = 5.0;

    int i;
    i = posix_memalign((void **)&img,16,N*M*sizeof(float));
    if(i!=0){
      exit(1);
    }

    i = posix_memalign((void **)&final_img,16,N*M*sizeof(float));
    if(i!=0){
      exit(1);
    }

	//Array initialization with random values
    for(int i=0; i < N*M; i++){
        img[i] = i * 0.475;
				final_img[i] = 0.0;
    }

	getWallTime(&ts); //Start time


	for(int i = 1; i < N-1; i++){
		int px=0;
		vimg = (__m128 *)img;
		vfinal_img = (__m128 *)final_img;
		for(int j = 1; j < M-1; j+=4){
			px++;
			final_img[i * M + px] = img[(i * M + px)-M-1] * k0 + img[(i * M + px)-M] * k1 + img[(i * M + px)-M+1] * k2 +
			img[(i * M + px)-1] * k3 + img[(i * M + px)] * k4 + img[(i * M + px)+1] * k5 + img[(i * M + px)+M-1] * k6 +
			img[(i * M + px)+M] * k7 + img[(i * M + px)+M+1] * k8;
		}
	}

	getWallTime(&te); // End time

	printf("Time: %f\n\n", te-ts);

	// compute mflops/sec (2 operations per M*N passes)
  mflops = (M*N*2.0)/((te-ts)*1e6);
  printf("MFLOPS/sec = %f\n\n",mflops);

	//DEBUGGING
	printf("*** Debugging ***\n\n");
	printf("Array size: %d\n\n", N*M);

	int debug_pixel = M+1;
	printf("img[%d]: %f(first pixel that can be calculated)\n", debug_pixel, img[debug_pixel]); //first original pixel
	printf("final_img[%d]: %f\n\n", debug_pixel, final_img[debug_pixel]); //first new pixel

	debug_pixel = 2*M+1;
	printf("img[%d]: %f(random pixel)\n", debug_pixel, img[debug_pixel]);	//random original pixel
	printf("final_img[%d]: %f\n\n", debug_pixel, final_img[debug_pixel]); //random new pixel

	debug_pixel = N*M-M-2;
	printf("img[%d]: %f(last pixel that can be calculated)\n", debug_pixel, img[debug_pixel]);	//last original pixel
	printf("final_img[%d]: %f\n\n", debug_pixel, final_img[debug_pixel]); //last new pixel

	free(img);
	free(final_img);

	return 0;

}
