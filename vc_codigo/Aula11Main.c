
#include <stdio.h>
#include "vc.h"

int main12/*_vc00012_1*/(void) {
	IVC* image, * image1, * imageOpen;

	image = vc_read_image("Images/FLIR/flir-01.pgm");
	if (image == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}

	image1 = vc_read_image("Images/FLIR/flir-01.pgm");
	if (image1 == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}


	imageOpen = vc_image_new(image->width, image->height, image->channels, image->levels);

	if (imageOpen == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}


	vc_gray_to_binary(image, image1, 127);
	vc_binary_close(image1, imageOpen, 3);
	vc_write_image("vc-00012_1.pgm", imageOpen);



	system("cmd /c start FilterGear Images/FLIR/flir-01.pgm");
	system("FilterGear vc-00012_1.pgm");

	vc_image_free(image);
	vc_image_free(imageOpen);



	printf("Press any key to exit...\n");
	(void)getchar();
	return 0;
}