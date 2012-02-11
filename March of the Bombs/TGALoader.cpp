/**
 * An uncompressed TGA image loader.
 * 
 * @author Tom Arnold
 */
#include "TGALoader.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
// Eliminate extraneous calls to sizeof().
const size_t size_uchar = sizeof(uchar);
const size_t size_sint = sizeof(sint);
/**
 * Load a TGA image into an info structure.
 *
 * @param fn The TGA image to load.
 * @return An info structure.
 * @author Tom Arnold
 */
tga_data_t* tga_data_load(const char* fn) {
	tga_data_t* tga = NULL;
	FILE* fh = NULL;
	int md, t;

	fopen_s(&fh, fn, "rb");
		// fh = fopen(fn, "rb"); /* Open the file in binary mode. */
	if (fh == NULL) { /* Problem opening file? */
		fprintf(stderr, "Error: problem opening TGA file (%s).\n", fn);
	} else {
		tga = new tga_data_t;

		uchar idLength = 0;
		sint colorMapLength = 0;
		uchar colorEntrySize = 0;
		{ // Load information about the tga, aka the header.
			fseek(fh, 1, SEEK_SET);	// Seek to the id length
			fread(&idLength, size_uchar, 1, fh);
			fseek(fh, 6, SEEK_SET);	// Seek to the color map length
			fread(&colorMapLength, size_sint, 1, fh);
			fseek(fh, 8, SEEK_SET);	// Seek to the color entry size
			fread(&colorEntrySize, size_uchar, 1, fh);

			fseek(fh, 12, SEEK_SET); // Seek to the width.
			fread(&tga->w, size_sint, 1, fh);
			fseek(fh, 14, SEEK_SET); // Seek to the height.
			fread(&tga->h, size_sint, 1, fh);
			fseek(fh, 16, SEEK_SET); // Seek to the depth.
			fread(&tga->depth, size_sint, 1, fh);
		}



		{ // Load the actual image data.

			int totalColorMapSize = colorMapLength * colorEntrySize / 8;
			int imageDataIndex = 18 + idLength + totalColorMapSize;

			md = tga->depth / 8; // Mode = components per pixel.
			tga->channels = md;
			t = tga->h * tga->w * md; // Total bytes = h * w * md.
			printf("Reading %d bytes.\n", t);
			tga->data = new uchar[t]; //(uchar*)malloc(size_uchar * t); // Allocate memory for the image data.
			fseek(fh, imageDataIndex, SEEK_SET); // Seek to the image data.
			fread(tga->data, size_uchar, t, fh);
			fclose(fh); // We're done reading.
			if (md >= 3) { // Mode 3 = RGB, Mode 4 = RGBA
				uchar aux;  // TGA stores RGB(A) as BGR(A) so	!!!!!No, as R, GR, BGR or BGRA
				for (int i = 0; i < t; i+= md) {
					aux = tga->data[i]; // we need to swap red and blue.
					tga->data[i] = tga->data[i + 2];
					tga->data[i + 2] = aux;
				}
			}

			//// OpenGL wants the rows in reverse ordering
			//for (int v = 0; v < tga->h / 2; v++)
			//{
			//	int rowBase1 = v * tga->w * md;
			//	int rowBase2 = (tga->h - v - 1) * tga->w * md;
			//	for (int u = 0; u < tga->w * md; u++)
			//	{
			//		swap(tga->data[rowBase1 + u], tga->data[rowBase2 + u]);
			//	}
			//}
		}
		printf("Loaded texture -> (%s)\nWidth: %d\nHeight: %d\nDepth: %d\n", fn, tga->w, tga->h, tga->depth);
	}
	return tga;
}

void swap(uchar& val1, uchar& val2)
{
	uchar tmp = val1;
	val1 = val2;
	val2 = tmp;
}

/*
int main(int argc, char** argv)
{
	tga_data_t* tex = NULL;

	if (argc != 2)
	{
		printf("Usage: ./tga image.tga\n");
		exit(1);	
	}

	// Load an uncompressed TGA file.
	tex = tga_data_load(argv[1]);

	return EXIT_SUCCESS;
}
*/
