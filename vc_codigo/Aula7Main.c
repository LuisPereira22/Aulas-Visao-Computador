
#include <stdio.h>
#include "vc.h"

//Abrir imagem, alterar e gravar em novo ficheiro
int main/*vc0007_1*/(void)
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