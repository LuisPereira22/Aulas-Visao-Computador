
#include <stdio.h>
#include "vc.h"

//Abrir imagem, alterar e gravar em novo ficheiro
int main10/*vc00010_1*/(void)
{
	IVC* image[4];
	int i;

	image[0] = vc_read_image("Images/FLIR/flir-01.pgm");
	if (image[0] == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}

	image[1] = vc_image_new(image[0]->width, image[0]->height, 1, 255);
	if (image[1] == NULL) {
		printf("ERROR -> vc_image_new():\n\tFile not created!\n");
		(void)getchar();
		return 0;
	}

	image[2] = vc_image_new(image[0]->width, image[0]->height, 1, 255);
	if (image[2]== NULL) {
		printf("ERROR -> vc_image_new():\n\tFile not created!\n");
		(void)getchar();
		return 0;
	}

	image[3]= vc_image_new(image[0]->width, image[0]->height, 1, 255);
	if (image[3]== NULL) {
		printf("ERROR -> vc_image_new():\n\tFile not created!\n");
		(void)getchar();
		return 0;
	}



	vc_gray_to_binary(image[0], image[1], 127);
	vc_binary_dilate(image[1], image[2], 3);
	vc_binary_erode(image[1], image[3], 3);
	vc_gray_negative(image[3]);

	vc_write_image("vc-00010_1.pgm", image[2]);
	vc_write_image("vc-00010_2.pgm", image[3]);

	vc_image_free(image[0]);
	vc_image_free(image[1]);
	vc_image_free(image[2]);
	vc_image_free(image[3]);

	system("cmd /c start FilterGear Images/FLIR/flir-01.pgm");
	system("FilterGear vc-00010_1.pgm");
	system("FilterGear vc-00010_2.pgm");

	printf("Press any key to exit...\n");
	getchar();

	return 0;
}