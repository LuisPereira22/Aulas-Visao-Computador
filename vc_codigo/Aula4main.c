
#include <stdio.h>
#include "vc.h"

//Abrir imagem, alterar e gravar em novo ficheiro
int main/*vc0004*/(void)
{
	IVC* image;
	int i;

	image = vc_read_image("Images/FLIR/flir-01.pgm");
	if (image == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}

	vc_gray_negative(image);

	vc_write_image("vc-0001.pgm", image);

	vc_image_free(image); 

	system("cmd /c start FilterGear Images/FLIR/flir-01.pgm");
	system("FilterGear vc-0001.pgm");

	printf("Press any key to exit...\n");
	getchar();

	return 0;
}
