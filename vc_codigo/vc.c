//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLIT�CNICO DO C�VADO E DO AVE
//                          2022/2023
//             ENGENHARIA DE SISTEMAS INFORM�TICOS
//                    VIS�O POR COMPUTADOR
//
//             [  DUARTE DUQUE - dduque@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Desabilita (no MSVC++) warnings de fun��es n�o seguras (fopen, sscanf, etc...)
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include "vc.h"


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//            FUN��ES: ALOCAR E LIBERTAR UMA IMAGEM
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// Alocar mem�ria para uma imagem
IVC *vc_image_new(int width, int height, int channels, int levels)
{
	IVC *image = (IVC *) malloc(sizeof(IVC));

	if(image == NULL) return NULL;
	if((levels <= 0) || (levels > 255)) return NULL;

	image->width = width;
	image->height = height;
	image->channels = channels;
	image->levels = levels;
	image->bytesperline = image->width * image->channels;
	image->data = (unsigned char *) malloc(image->width * image->height * image->channels * sizeof(char));

	if(image->data == NULL)
	{
		return vc_image_free(image);
	}

	return image;
}


// Libertar mem�ria de uma imagem
IVC *vc_image_free(IVC *image)
{
	if(image != NULL)
	{
		if(image->data != NULL)
		{
			free(image->data);
			image->data = NULL;
		}

		free(image);
		image = NULL;
	}

	return image;
}

int vc_gray_negative(IVC* srcdst)
{
	unsigned char* data = (unsigned char*)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->bytesperline;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	//verificacao de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) return 0;
	if (channels != 1) return 0;

	//Inverte a imagem gray
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			//data[pos] = 255 - data[pos];
			data[pos] = srcdst->levels - data[pos];
		}
	}
	return 1;
}

int vc_rgb_negative(IVC* srcdst)
{
	unsigned char* data = (unsigned char*)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	//verificacao de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) return 0;
	if (channels != 3) return 0;

	//Inverte a imagem gray
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			data[pos] = 255 - data[pos];
			data[pos + 1] = 255 - data[pos + 1];
			data[pos + 2] = 255 - data[pos + 2];
		}
	}
	return 1;
}

int vc_rgb_to_gray(IVC* src, IVC* dst)
{
	unsigned char* datasrc = (unsigned char*)src->data;
	int bytesperline_src = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char* datadst = (unsigned char*)dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x, y;
	long int pos_src, pos_dst;
	float rf, gf, bf;

	//Verificaocao de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height)) return 0;
	if ((src->channels != 3) || (dst->channels != 1)) return 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos_src = y * bytesperline_src + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;

			rf = (float)datasrc[pos_src];
			gf = (float)datasrc[pos_src + 1];
			bf = (float)datasrc[pos_src + 2];

			datadst[pos_dst] = (unsigned char)((rf * 0.299) + (gf * 0.587) + (bf * 0.114));
		}
	}
}



int vc_scale_gray_to_color_palette(IVC* src, IVC* dst)
{
	unsigned char* datasrc = (unsigned char*)src->data;
	int bytesperline_src = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char* datadst = (unsigned char*)dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x, y;
	long int pos_src, pos_dst;
	float rf, gf, bf, hue;
	int temp;

	//Verificaocao de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height)) return 0;
	if ((src->channels != 1) || (dst->channels != 3)) return 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos_src = y * bytesperline_src + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;

			temp = (int)datasrc[pos_src];

			if (temp < 64)
			{
				datadst[pos_dst] = 0;
				datadst[pos_dst + 1] = temp * 4;
				datadst[pos_dst + 2] = 255;
			}
			else if (temp >= 64 && temp < 128)
			{
				datadst[pos_dst] = 0;
				datadst[pos_dst + 1] = 255;
				datadst[pos_dst + 2] = 255-(temp-64)*4;
			}
			else if (temp >= 128 && temp < 192)
			{
				datadst[pos_dst] = (temp-128) * 4;
				datadst[pos_dst +1] = 255;
				datadst[pos_dst+2] = 0;
			}
			else
			{
				datadst[pos_dst] = 255;
				datadst[pos_dst+1] = 255 - (temp - 192) * 4;
				datadst[pos_dst+2] = 0;
			}
		}
	}
	return 0;
}

int vc_hsv_segmentation(IVC* src, IVC* dst, int hmin, int hmax, int smin, int smax, int vmin, int vmax)
{
	unsigned char* dataSrc = (unsigned char*)src->data;
	int bytesPerLineSrc = src->width * src->channels;
	int channelsSrc = src->channels;
	unsigned char* dataDst = (unsigned char*)dst->data;
	int bytesPerLineDst = dst->width * dst->channels;
	int channelsDst = dst->channels;
	int width = src->width;
	int height = src->height;
	long int posSrc, posDst;
	float h, s, v;

	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height)) return 0;
	if ((src->channels != 3) || (dst->channels != 1)) return 0;
	if (dst->levels != 255) return 0;
	if (!(hmin >= 0 && hmax <= 360)) return 0;
	if (!(smin >= 0 && smax <= 255)) return 0;
	if (!(vmin >= 0 && vmax <= 255)) return 0;

	hmin = ((float)hmin * 255) / 360;
	hmax = ((float)hmax * 255) / 360;
	smin = ((float)smin * 255) / 100;
	smax = ((float)smax * 255) / 100;
	vmin = ((float)vmin * 255) / 100;
	vmax = ((float)vmax * 255) / 100;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			posSrc = y * bytesPerLineSrc + x * channelsSrc;
			posDst = y * bytesPerLineDst + x * channelsDst;

			h = (float)dataSrc[posSrc];
			s = (float)dataSrc[posSrc + 1];
			v = (float)dataSrc[posSrc + 2];

			if (h >= hmin && h <= hmax && s >= smin && s <= smax && v >= vmin && v <= vmax)
			{
				dataDst[posDst] = 255;
			}
			else
			{
				dataDst[posDst] = 0;
			}
		}
	}
	return 1;
}

int vc_rgb_to_hsv(IVC* src, IVC* dst)
{
	unsigned char* datasrc = (unsigned char*)src->data;
	unsigned char* datadst = (unsigned char*)dst->data;
	int bytesperline_src = src->width * src->channels;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_src = src->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x, y;
	long int pos_src, pos_dst;
	float rf, gf, bf, max, min, hue, sat;

	// Verifica��o de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height)) return 0;
	if ((src->channels != 3) || (dst->channels != 3)) return 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos_src = y * bytesperline_src + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;

			rf = (float)datasrc[pos_src];
			gf = (float)datasrc[pos_src + 1];
			bf = (float)datasrc[pos_src + 2];

			max = MAX3(rf, gf, bf);
			min = MIN3(rf, gf, bf);

			if ((max == min) || (max == 0))
			{
				hue = 0;
				sat = 0;

			}
			else {

				sat = (max - min) / max * 255.0f;

				if (max == rf)
				{
					if (gf >= bf)
					{
						hue = 60.0f * ((gf - bf) / (max - min));
					}
					else
					{
						hue = 360.0f + 60.0f * ((gf - bf) / (max - min));
					}
				}
				else if (max == gf)
				{
					hue = 120.0f + 60.0f * ((bf - rf) / (max - min));
				}
				else if (max == bf)
				{
					hue = 240.0f + 60.0f * ((rf - gf) / (max - min));
				}
			}
			datadst[pos_dst] = (unsigned char)((hue / 360.0f) * 255.0f);
			datadst[pos_dst + 1] = (unsigned char)sat;
			datadst[pos_dst + 2] = (unsigned char)max;
		}
	}
	return 1;
}

int vc_pixel_counter(IVC* src)
{
	unsigned char* data = (unsigned char*)src->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y, count = 0, percentagem, totalpixeis = 0;
	long int pos;

	//verificacao de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if (channels != 1) return 0;
	//Inverte a imagem gray
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			totalpixeis++;
			pos = y * bytesperline + x * channels;

			if (data[pos] == 255)
			{
				count++;
			}
		}
	}
	printf("Numero de pixeis brancos: %d\n", count);
	percentagem = (count * 100) / totalpixeis;
	printf("Percentagem de pixeis: %d\n", percentagem);
	return 1;

}

int vc_gray_to_binary(IVC* src, IVC* dst, int threshold) {
	unsigned char* datasrc = (unsigned char*)src->data;
	int bytesperline = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char* datadst = (unsigned char*)dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x, y;
	long int pos_src, pos_dst;

	//Verifica��o de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height)) return 0;
	if ((src->channels != 1) || (dst->channels != 1)) return 0;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			pos_src = y * bytesperline + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;

			if (datasrc[pos_src] > threshold)
				datadst[pos_dst] = 255;
			else
				datadst[pos_dst] = 0;
		}
	}
	return 1;
}

int vc_gray_to_binary2(IVC* src, IVC* dst, int threshold1, int threshold2) {
	unsigned char* datasrc = (unsigned char*)src->data;
	int bytesperline = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char* datadst = (unsigned char*)dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x, y;
	long int pos_src, pos_dst;

	//Verifica��o de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height)) return 0;
	if ((src->channels != 1) || (dst->channels != 1)) return 0;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			pos_src = y * bytesperline + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;

			if (datasrc[pos_src] > threshold1 && datasrc[pos_src] < threshold2)
				datadst[pos_dst] = 255;
			else
				datadst[pos_dst] = 0;
		}
	}
	return 1;
}

int vc_gray_to_binary_global_mean(IVC* srcdst)
{
	unsigned char* datasrc = (unsigned char*)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels_src = srcdst->channels;
	int x, y, count = 0;
	long int pos_src;
	float threshold = 0, brilho = 0;

	//Verifica��o de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) return 0;
	if (channels_src != 1) return 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos_src = y * bytesperline + x * channels_src;
			count++;
			brilho += datasrc[pos_src];
		}
	}

	float media = brilho / count;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			pos_src = y * bytesperline + x * channels_src;

			if (datasrc[pos_src] > media)
				datasrc[pos_src] = 255;	
			else {
				datasrc[pos_src] = 0;
			}
		}
	}
	return 1;
}

int vc_gray_to_binary_midpoint(IVC* src, IVC* dst){
	unsigned char* datasrc = (unsigned char*)src->data;
	int bytesperline = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char* datadst = (unsigned char*)dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x, y, nx, ny, vmin, vmax;
	long int pos_src, pos_src_for, pos_dst;
	float threshold = 0;
	int neighbor = 25 * 25;

	//Verifica��o de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height)) return 0;
	if ((src->channels != 1) || (dst->channels != 1)) return 0;
	int offset = (int)(neighbor) / 2;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos_src = y * bytesperline + x * channels_src;
			pos_dst = y * bytesperline + x * channels_dst;
			vmax = 0;
			vmin = 500;

			for (ny = y - offset; ny <= y + offset; ny++)
			{
				for (nx = x-offset; nx <= x+offset; nx++) 
				{
				

					if (nx >= 0 && nx < width && ny >= 0 && ny < height)
					{

						pos_src_for = (ny)*bytesperline + (nx)*channels_src;

						if (datasrc[pos_src_for] < vmin)
							vmin = datasrc[pos_src_for];
						if (datasrc[pos_src_for] > vmax)
							vmax = datasrc[pos_src_for];
						
					}
				}
			}
			threshold = (int)(vmin + vmax)/2;
			if (datasrc[pos_src] > threshold) {
				datadst[pos_dst] = 255;
			}
			else {
				datadst
					[pos_dst] = 0;
			}
		}
	}
}

int vc_gray_to_binary_niblack(IVC* src, IVC* dst, int kernel, float k)
{
	unsigned char* datasrc = (unsigned char*)src->data;
	int bytesperline = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char* datadst = (unsigned char*)dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x, y, nx, ny;
	long int pos_src, pos_src_for, pos_dst;
	float threshold = 0, brilho = 0, sum = 0;
	int media, desvio;


	//Verifica��o de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height)) return 0; 
	if ((src->channels != 1) || (dst->channels != 1)) return 0;
	int offset = (int)(kernel) / 2;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos_src = y * bytesperline + x * channels_src;
			pos_dst = y * bytesperline + x * channels_dst;
			brilho = 0;
			sum = 0;
			for (ny = y - offset; ny <= y + offset; ny++)
			{
				for (nx = x - offset; nx <= x + offset; nx++)
				{

					if (nx >= 0 && nx < width && ny >= 0 && ny < height)
					{

						pos_src_for = (ny)*bytesperline + (nx)*channels_src;
						brilho += datasrc[pos_src_for];

					}
				}
			}
			media = brilho / (kernel*kernel);

			for (ny = y - offset; ny <= y + offset; ny++)
			{
				for (nx = x - offset; nx <= x + offset; nx++)
				{

                  if (nx >= 0 && nx < width && ny >= 0 && ny < height)
					{
                        pos_src_for = (ny)*bytesperline + (nx)*channels_src;
						int cont = datasrc[pos_src_for] - media;
						sum += cont * cont;
					}
				}
			}
			desvio = sqrt(sum / (kernel * kernel));
			threshold = media + k * desvio;
			if (datasrc[pos_src] > threshold) {
				datadst[pos_dst] = 255;
			}
			else {
				datadst[pos_dst] = 0;
			}
		}

	}
}

int vc_binary_dilate(IVC* src, IVC* dst, int kernel)
{
	unsigned char* datasrc = (unsigned char*)src->data;
	int bytesperline = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char* datadst = (unsigned char*)dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x, y, nx, ny, flag;
	long int pos_src, pos_src_for, pos_dst;
	float threshold = 0;

	//Verifica��o de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height)) return 0;
	if ((src->channels != 1) || (dst->channels != 1)) return 0;
	int offset = (int)(kernel) / 2;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos_src = y * bytesperline + x * channels_src;
			pos_dst = y * bytesperline + x * channels_dst;
			flag = 0;
			for (ny = y - offset; ny <= y + offset; ny++)
			{
				for (nx = x - offset; nx <= x + offset; nx++)
				{

					if (nx >= 0 && nx < width && ny >= 0 && ny < height)
					{

						pos_src_for = (ny)*bytesperline + (nx)*channels_src;
						if (datasrc[pos_src_for] == 255)
						{
							flag = 1;
						}

					}
				}
			}
			if (flag == 1) {
				datadst[pos_dst] = 255;
			}
			else {
				datadst[pos_dst] = 0;
			}
		}
	}
}

int vc_binary_erode(IVC* src, IVC* dst, int kernel)
{
	unsigned char* datasrc = (unsigned char*)src->data;
	int bytesperline = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char* datadst = (unsigned char*)dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x, y, nx, ny, flag;
	long int pos_src, pos_src_for, pos_dst;
	float threshold = 0;

	//Verifica��o de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height)) return 0;
	if ((src->channels != 1) || (dst->channels != 1)) return 0;
	int offset = (int)(kernel) / 2;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos_src = y * bytesperline + x * channels_src;
			pos_dst = y * bytesperline + x * channels_dst;
			flag = 0;
			for (ny = y - offset; ny <= y + offset; ny++)
			{
				for (nx = x - offset; nx <= x + offset; nx++)
				{

					if (nx >= 0 && nx < width && ny >= 0 && ny < height)
					{

						pos_src_for = (ny)*bytesperline + (nx)*channels_src;
						if (datasrc[pos_src_for] == 0)
						{
							flag = 1;
						}

					}
				}
			}
			if (flag == 1) {
				datadst[pos_dst] = 0;
			}
			else {
				datadst[pos_dst] = 255;
			}
		}
	}
}

int vc_binary_open(IVC* src, IVC* dst, int kernel)
{
	IVC* tempimage;

	tempimage = vc_image_new(src->width, src->height, 1, 255);

	if (tempimage == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}

	vc_binary_erode(src, tempimage, kernel);
	vc_binary_dilate(tempimage, dst, kernel);
	vc_image_free(tempimage);
}

int vc_binary_close(IVC* src, IVC* dst, int kernel)
{
	IVC* tempimage;

	tempimage = vc_image_new(src->width, src->height, 1, 255);

	if (tempimage == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}

	vc_binary_dilate(src, tempimage, kernel);
	vc_binary_erode(tempimage, dst, kernel);
	vc_image_free(tempimage);
}

int pintar_cerbero(IVC* src, IVC* dst, IVC* bin)
{
	unsigned char* datasrc = (unsigned char*)src->data;
	unsigned char* databin = (unsigned char*)bin->data;
	int bytesperline = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char* datadst = (unsigned char*)dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x, y;
	long int pos_src, pos_dst;

	//Verifica��o de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height)) return 0;
	if ((src->channels != 1) || (dst->channels != 1)) return 0;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			pos_src = y * bytesperline + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;

			if (databin[pos_src] == 255)
				datadst[pos_dst] = datasrc[pos_src];
			else
				datadst[pos_dst] = 0;
		}
	}
	return 1;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    FUN��ES: LEITURA E ESCRITA DE IMAGENS (PBM, PGM E PPM)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


char *netpbm_get_token(FILE *file, char *tok, int len)
{
	char *t;
	int c;
	
	for(;;)
	{
		while(isspace(c = getc(file)));
		if(c != '#') break;
		do c = getc(file);
		while((c != '\n') && (c != EOF));
		if(c == EOF) break;
	}
	
	t = tok;
	
	if(c != EOF)
	{
		do
		{
			*t++ = c;
			c = getc(file);
		} while((!isspace(c)) && (c != '#') && (c != EOF) && (t - tok < len - 1));
		
		if(c == '#') ungetc(c, file);
	}
	
	*t = 0;
	
	return tok;
}


long int unsigned_char_to_bit(unsigned char *datauchar, unsigned char *databit, int width, int height)
{
	int x, y;
	int countbits;
	long int pos, counttotalbytes;
	unsigned char *p = databit;

	*p = 0;
	countbits = 1;
	counttotalbytes = 0;

	for(y=0; y<height; y++)
	{
		for(x=0; x<width; x++)
		{
			pos = width * y + x;

			if(countbits <= 8)
			{
				// Numa imagem PBM:
				// 1 = Preto
				// 0 = Branco
				//*p |= (datauchar[pos] != 0) << (8 - countbits);
				
				// Na nossa imagem:
				// 1 = Branco
				// 0 = Preto
				*p |= (datauchar[pos] == 0) << (8 - countbits);

				countbits++;
			}
			if((countbits > 8) || (x == width - 1))
			{
				p++;
				*p = 0;
				countbits = 1;
				counttotalbytes++;
			}
		}
	}

	return counttotalbytes;
}


void bit_to_unsigned_char(unsigned char *databit, unsigned char *datauchar, int width, int height)
{
	int x, y;
	int countbits;
	long int pos;
	unsigned char *p = databit;

	countbits = 1;

	for(y=0; y<height; y++)
	{
		for(x=0; x<width; x++)
		{
			pos = width * y + x;

			if(countbits <= 8)
			{
				// Numa imagem PBM:
				// 1 = Preto
				// 0 = Branco
				//datauchar[pos] = (*p & (1 << (8 - countbits))) ? 1 : 0;

				// Na nossa imagem:
				// 1 = Branco
				// 0 = Preto
				datauchar[pos] = (*p & (1 << (8 - countbits))) ? 0 : 1;
				
				countbits++;
			}
			if((countbits > 8) || (x == width - 1))
			{
				p++;
				countbits = 1;
			}
		}
	}
}


IVC *vc_read_image(char *filename)
{
	FILE *file = NULL;
	IVC *image = NULL;
	unsigned char *tmp;
	char tok[20];
	long int size, sizeofbinarydata;
	int width, height, channels;
	int levels = 255;
	int v;
	
	// Abre o ficheiro
	if((file = fopen(filename, "rb")) != NULL)
	{
		// Efectua a leitura do header
		netpbm_get_token(file, tok, sizeof(tok));

		if(strcmp(tok, "P4") == 0) { channels = 1; levels = 1; }	// Se PBM (Binary [0,1])
		else if(strcmp(tok, "P5") == 0) channels = 1;				// Se PGM (Gray [0,MAX(level,255)])
		else if(strcmp(tok, "P6") == 0) channels = 3;				// Se PPM (RGB [0,MAX(level,255)])
		else
		{
			#ifdef VC_DEBUG
			printf("ERROR -> vc_read_image():\n\tFile is not a valid PBM, PGM or PPM file.\n\tBad magic number!\n");
			#endif

			fclose(file);
			return NULL;
		}
		
		if(levels == 1) // PBM
		{
			if(sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &width) != 1 || 
			   sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &height) != 1)
			{
				#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tFile is not a valid PBM file.\n\tBad size!\n");
				#endif

				fclose(file);
				return NULL;
			}

			// Aloca mem�ria para imagem
			image = vc_image_new(width, height, channels, levels);
			if(image == NULL) return NULL;

			sizeofbinarydata = (image->width / 8 + ((image->width % 8) ? 1 : 0)) * image->height;
			tmp = (unsigned char *) malloc(sizeofbinarydata);
			if(tmp == NULL) return 0;

			#ifdef VC_DEBUG
			printf("\nchannels=%d w=%d h=%d levels=%d\n", image->channels, image->width, image->height, levels);
			#endif

			if((v = fread(tmp, sizeof(unsigned char), sizeofbinarydata, file)) != sizeofbinarydata)
			{
				#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tPremature EOF on file.\n");
				#endif

				vc_image_free(image);
				fclose(file);
				free(tmp);
				return NULL;
			}

			bit_to_unsigned_char(tmp, image->data, image->width, image->height);

			free(tmp);
		}
		else // PGM ou PPM
		{
			if(sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &width) != 1 || 
			   sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &height) != 1 || 
			   sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &levels) != 1 || levels <= 0 || levels > 255)
			{
				#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tFile is not a valid PGM or PPM file.\n\tBad size!\n");
				#endif

				fclose(file);
				return NULL;
			}

			// Aloca mem�ria para imagem
			image = vc_image_new(width, height, channels, levels);
			if(image == NULL) return NULL;

			#ifdef VC_DEBUG
			printf("\nchannels=%d w=%d h=%d levels=%d\n", image->channels, image->width, image->height, levels);
			#endif

			size = image->width * image->height * image->channels;

			if((v = fread(image->data, sizeof(unsigned char), size, file)) != size)
			{
				#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tPremature EOF on file.\n");
				#endif

				vc_image_free(image);
				fclose(file);
				return NULL;
			}
		}
		
		fclose(file);
	}
	else
	{
		#ifdef VC_DEBUG
		printf("ERROR -> vc_read_image():\n\tFile not found.\n");
		#endif
	}
	
	return image;
}


int vc_write_image(char *filename, IVC *image)
{
	FILE *file = NULL;
	unsigned char *tmp;
	long int totalbytes, sizeofbinarydata;
	
	if(image == NULL) return 0;

	if((file = fopen(filename, "wb")) != NULL)
	{
		if(image->levels == 1)
		{
			sizeofbinarydata = (image->width / 8 + ((image->width % 8) ? 1 : 0)) * image->height + 1;
			tmp = (unsigned char *) malloc(sizeofbinarydata);
			if(tmp == NULL) return 0;
			
			fprintf(file, "%s %d %d\n", "P4", image->width, image->height);
			
			totalbytes = unsigned_char_to_bit(image->data, tmp, image->width, image->height);
			printf("Total = %ld\n", totalbytes);
			if(fwrite(tmp, sizeof(unsigned char), totalbytes, file) != totalbytes)
			{
				#ifdef VC_DEBUG
				fprintf(stderr, "ERROR -> vc_read_image():\n\tError writing PBM, PGM or PPM file.\n");
				#endif

				fclose(file);
				free(tmp);
				return 0;
			}

			free(tmp);
		}
		else
		{
			fprintf(file, "%s %d %d 255\n", (image->channels == 1) ? "P5" : "P6", image->width, image->height);
		
			if(fwrite(image->data, image->bytesperline, image->height, file) != image->height)
			{
				#ifdef VC_DEBUG
				fprintf(stderr, "ERROR -> vc_read_image():\n\tError writing PBM, PGM or PPM file.\n");
				#endif

				fclose(file);
				return 0;
			}
		}
		
		fclose(file);

		return 1;
	}
	
	return 0;
}
