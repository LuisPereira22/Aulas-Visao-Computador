
#include <stdio.h>
#include "vc.h"

//Abrir imagem, alterar e gravar em novo ficheiro
int main/*vc0004_1*/(void)
{
	IVC* image[2];
	int i;

	image[0] = vc_read_image("Images/Additional/fruits.ppm");
	if (image[0] == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}

	image[1] = vc_image_new(image[0]->width, image[0]->height, 1, image[0]->levels);
	if (image[1] == NULL) {
		printf("ERROR -> vc_image_new():\n\tFile not created!\n");
		(void)getchar();
		return 0;
	}

	vc_rgb_to_gray(image[0], image[1]);

	vc_write_image("vc-0004_1.pgm", image[1]);

	vc_image_free(image[0]);
	vc_image_free(image[1]);

	system("cmd /c start FilterGear Images/Additional/fruits.ppm");
	system("FilterGear vc-0004_1.pgm");

	printf("Press any key to exit...\n");
	getchar();

	return 0;
}
