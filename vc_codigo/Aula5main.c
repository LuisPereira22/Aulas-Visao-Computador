
#include <stdio.h>
#include "vc.h"

//Abrir imagem, alterar e gravar em novo ficheiro
int main3/*vc0005_1*/(void)
{
	IVC* image[2];
	int i;

	image[0] = vc_read_image("Images/FLIR/flir-02.pgm");
	if (image[0] == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}

	image[1] = vc_image_new(image[0]->width, image[0]->height, 3, image[0]->levels);
	if (image[1] == NULL) {
		printf("ERROR -> vc_image_new():\n\tFile not created!\n");
		(void)getchar();
		return 0;
	}

	vc_scale_gray_to_color_palette(image[0], image[1]);

	vc_write_image("vc-0005_1.pgm", image[1]);

	vc_image_free(image[0]);
	vc_image_free(image[1]);

	system("cmd /c start FilterGear Images/FLIR/flir-02.pgm");
	system("FilterGear vc-0005_1.pgm");

	printf("Press any key to exit...\n");
	getchar();

	return 0;
}