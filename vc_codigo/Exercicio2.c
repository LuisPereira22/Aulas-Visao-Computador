
#include <stdio.h>
#include "vc.h"

int mainex/*_vc00011_1*/(void) {
	IVC* image, * imageERO, * imageGRAY, * imageDIL, * imageDIL2;

	image = vc_read_image("Exercício2/flir-01.pgm");
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
	imageDIL2 = vc_image_new(imageGRAY->width, imageGRAY->height, imageGRAY->channels, imageGRAY->levels);

	if (imageDIL == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}

	vc_gray_to_binary(image, imageGRAY, 127);
	vc_write_image("vc-00011_1_gray.pgm", imageGRAY);

	vc_binary_erode(imageGRAY, imageERO, 3);
	vc_write_image("vc-00011_1_erode.pgm", imageERO);

	vc_binary_dilate(imageERO, imageDIL, 3);
	vc_write_image("vc-00011_1_dilate.pgm", imageERO);

	vc_binary_dilate(imageDIL, imageDIL2, 3);
	vc_write_image("vc-00011_1_dilate2.pgm", imageDIL2);


	system("cmd /c start FilterGear Exercício2/flir-01.pgm");
	system("FilterGear vc-00011_1_gray.pgm");
	system("FilterGear vc-00011_1_erode.pgm");
	system("FilterGear vc-00011_1_dilate.pgm");
	system("FilterGear vc-00011_1_dilate2.pgm");

	vc_image_free(image);
	vc_image_free(imageGRAY);
	vc_image_free(imageDIL);
	vc_image_free(imageERO);
	vc_image_free(imageDIL2);


	printf("Press any key to exit...\n");
	(void)getchar();
	return 0;
}