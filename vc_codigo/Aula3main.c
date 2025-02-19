#include <stdio.h>
#include "vc.h"

int main1(void) 
{
	IVC* image;
	int x, y;
	long int pos;

	image = vc_image_new(255, 255, 1, 255);
	if (image == NULL) 
	{
		printf("Error -> vc_image_new():\n\tOut of memory!\n");
		getchar();
		return 0;
	}

	for (x = 0; x < image->width; x++)
	{
		for (y = 0; y < image->height; y++)
		{
			pos = y * image->bytesperline + x * image->channels;

			//image->data[pos] = (unsigned char)y; gradiente cima-baixo
			//image->data[pos] = (unsigned char)x; gradiente direita-esquerda
			//image->data[pos] = (unsigned char)(((float)x / (float)y) * 255.0f); qualquer tamanho de imagem - incompleto
			//image->data[pos] = (unsigned char) ((x+y) / 2); gradiente diagonal
		}
	}

	vc_write_image("gradienteobliq.pbm", image);

	vc_image_free(image);

	printf("Press any key to exit...\n");
	getchar();

	return 0;
}