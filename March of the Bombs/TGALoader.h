#ifndef _TGALOADER_H_
#define _TGALOADER_H_

// Some macros to shorten things up.
#define uchar unsigned char
#define sint short int

#ifndef NULL
#define NULL 0
#endif

/**
 * This structure holds info about the TGA image before we load it into OpenGL.
 *
 * @author Tom Arnold
 */
struct tga_data_t {
	uchar depth;
	sint w, h;
	uchar* data;
	uchar channels;

	~tga_data_t()
	{
		if (data != NULL)
		{
			delete[] data;
			data = NULL;
		}
	}
};

tga_data_t* tga_data_load(const char* fn);
void swap(uchar& val1, uchar& val2);

#endif
