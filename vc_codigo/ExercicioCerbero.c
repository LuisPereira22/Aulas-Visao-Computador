
#include <stdio.h>
#include "vc.h"

int main/*_vc00013_1*/(void) {
	IVC* image, * imageERO, * imageGRAY, * imageDIL, * imageDIL2, * imagefinal;

	image = vc_read_image("Exercicio3/Images/brain.pgm");
	if (image == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}

	imageGRAY = vc_image_new(image->width, image->height, image->channels, image->levels);

	if (imageGRAY == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}

	imageERO = vc_image_new(imageGRAY->width, imageGRAY->height, imageGRAY->channels, imageGRAY->levels);

	if (imageERO == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}

	imageDIL = vc_image_new(imageGRAY->width, imageGRAY->height, imageGRAY->channels, imageGRAY->levels);

	if (imageDIL == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}

	imagefinal = vc_image_new(imageDIL->width, imageDIL->height, imageDIL->channels, imageDIL->levels);

	if (imageDIL == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}

	vc_gray_to_binary2(image, imageGRAY, 60, 205);
	vc_write_image("vc-00013_1_gray.pgm", imageGRAY);

	vc_binary_open(imageGRAY, imageERO, 9);
	vc_write_image("vc-00013_1_open.pgm", imageERO);

	vc_binary_close(imageERO, imageDIL, 3);
    vc_write_image("vc-00013_1_dilate.pgm", imageDIL);

	pintar_cerbero(image, imagefinal, imageDIL);
	vc_write_image("vc-00013_1_pintar.pgm", imagefinal);



	system("cmd /c start FilterGear Exercicio3/Images/brain.pgm");
	system("FilterGear vc-00013_1_gray.pgm");
	system("FilterGear vc-00013_1_open.pgm");
	system("FilterGear vc-00013_1_dilate.pgm");
	system("FilterGear vc-00013_1_pintar.pgm");

	vc_image_free(image);
	vc_image_free(imageGRAY);
	vc_image_free(imageERO);
	vc_image_free(imageDIL);
	vc_image_free(imagefinal);


	printf("Press any key to exit...\n");
	(void)getchar();
	return 0;
}