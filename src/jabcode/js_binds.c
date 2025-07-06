#include <emscripten.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./include/jabcode.h"

EMSCRIPTEN_KEEPALIVE
int get_default_symbol_number() {
  return DEFAULT_SYMBOL_NUMBER;
}

/**
 * @brief JABCode reader main function
 * @return 0: success | 255: not detectable | other non-zero: decoding failed
*/
EMSCRIPTEN_KEEPALIVE
int read_image(char *image_file_name, char *decoded)
{

	jab_bitmap* bitmap;
	bitmap = readImage(image_file_name);
	if(bitmap == NULL)
		return 1;

	//find and decode JABCode in the image
	jab_int32 decode_status;
	jab_decoded_symbol symbols[MAX_SYMBOL_NUMBER];
	jab_data* decoded_data = decodeJABCodeEx(bitmap, NORMAL_DECODE, &decode_status, symbols, MAX_SYMBOL_NUMBER);
	if(decoded_data == NULL)
	{
		free(bitmap);
		reportError("Decoding JABCode failed");
		if(decode_status > 0)
			return (jab_int32)(symbols[0].module_size + 0.5f);
		else
			return 1;
	}

	//output warning if the code is only partly decoded with COMPATIBLE_DECODE mode
	if(decode_status == 2)
	{
		JAB_REPORT_INFO(("The code is only partly decoded. Some slave symbols have not been decoded and are ignored."))
	}

	//output result
	/*if(output_as_file)
	{
		FILE* output_file = fopen(argv[3], "wb");
		if(output_file == NULL)
		{
			reportError("Can not open the output file");
			return 255;
		}
		fwrite(decoded_data->data, decoded_data->length, 1, output_file);
		fclose(output_file);
	}
	else
	{*/
		for(jab_int32 i=0; i<decoded_data->length; i++)
			printf("%c", decoded_data->data[i]);
		printf("\n");
	/*}*/

	free(bitmap);
	free(decoded_data);
    return 0;
}
