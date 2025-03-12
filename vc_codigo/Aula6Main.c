
#include <stdio.h>
#include "vc.h"

//Abrir imagem, alterar e gravar em novo ficheiro
int main6/*vc0006_1*/(void)
{
	IVC* image[4];
	int i;

	image[0] = vc_read_image("Exercicio/Exercicio/PET-Alzheimer.ppm");
	if (image[0] == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}

	image[1] = vc_image_new(image[0]->width, image[0]->height, 3, 255);
	if (image[1] == NULL) {
		printf("ERROR -> vc_image_new():\n\tFile not created!\n");
		(void)getchar();
		return 0;
	}

	image[2] = vc_image_new(image[0]->width, image[0]->height, 1, 255);
	if (image[2] == NULL) {
		printf("ERROR -> vc_image_new():\n\tFile not created!\n");
		(void)getchar();
		return 0;
	}

	image[3] = vc_image_new(image[0]->width, image[0]->height, 1, 255);
	if (image[3] == NULL) {
		printf("ERROR -> vc_image_new():\n\tFile not created!\n");
		(void)getchar();
		return 0;
	}

	vc_rgb_to_hsv(image[0], image[1]);
	vc_write_image("vc-0006_1.pgm", image[1]);
	int azul = vc_hsv_segmentation(image[1], image[2], 161,290,50,100,40,100);
	int verde = vc_hsv_segmentation(image[1], image[3], 71, 160, 50, 100, 14, 100);
	vc_write_image("vc-0006_1b.pgm", image[2]);
	vc_write_image("vc-0006_1g.pgm", image[3]);
    printf("segmentacao do azul:\n");
	vc_pixel_counter(image[2]);
    printf("segmentacao do verde:\n");
	vc_pixel_counter(image[3]);
	

	vc_image_free(image[0]);
	vc_image_free(image[1]);
	vc_image_free(image[2]);
	vc_image_free(image[3]);

	system("cmd /c start FilterGear Exercicio/Exercicio/PET-Alzheimer.ppm");
	system("FilterGear vc-0006_1b.pgm");
	system("FilterGear vc-0006_1g.pgm");

	printf("Press any key to exit...\n");
	getchar();

	return 0;
}