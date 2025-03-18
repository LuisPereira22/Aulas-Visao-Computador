
#include <stdio.h>
#include "vc.h"

//Abrir imagem, alterar e gravar em novo ficheiro
int main7/*vc0007_1*/(void)
{
	IVC* image;
	int i;

	image = vc_read_image("ExercicioSegm/Images/cells.pgm");
	if (image == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}

	vc_gray_to_binary_global_mean(image);
	vc_gray_negative(image);

	vc_write_image("vc-0007_1.pgm", image);

	vc_image_free(image);

	system("cmd /c start FilterGear ExercicioSegm/Images/cells.pgm");
	system("FilterGear vc-0007_1.pgm");

	printf("Press any key to exit...\n");
	getchar();

	return 0;
}

//Abrir imagem, alterar e gravar em novo ficheiro
int main8/*vc0008_1*/(void)
{
	IVC* image[2];
	int i;

	image[0] = vc_read_image("ExercicioSegm/Images/cells.pgm");
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

	vc_gray_to_binary_midpoint(image[0], image[1]);
	vc_gray_negative(image[1]);

	vc_write_image("vc-0008_1.pgm", image[1]);

	vc_image_free(image[0]);
	vc_image_free(image[1]);

	system("cmd /c start FilterGear ExercicioSegm/Images/cells.pgm");
	system("FilterGear vc-0008_1.pgm");

	printf("Press any key to exit...\n");
	getchar();

	return 0;
}