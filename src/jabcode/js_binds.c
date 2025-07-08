#include <emscripten.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./include/jabcode.h"

#define MAX_CHARS 500

EMSCRIPTEN_KEEPALIVE
int get_default_symbol_number() {
  return DEFAULT_SYMBOL_NUMBER;
}

EMSCRIPTEN_KEEPALIVE
char *read_image(char *image_file_name)
{

	jab_bitmap* bitmap;
	bitmap = readImage(image_file_name);
	if(bitmap == NULL)
		return NULL;

	//find and decode JABCode in the image
	jab_int32 decode_status;
	jab_decoded_symbol symbols[MAX_SYMBOL_NUMBER];
	jab_data* decoded_data = decodeJABCodeEx(bitmap, NORMAL_DECODE, &decode_status, symbols, MAX_SYMBOL_NUMBER);
	if(decoded_data == NULL)
	{
		free(bitmap);
		reportError("Decoding JABCode failed");
		if(decode_status > 0)
			return NULL;
		else
			return NULL;
	}

	//output warning if the code is only partly decoded with COMPATIBLE_DECODE mode
	if(decode_status == 2)
	{
		JAB_REPORT_INFO(("The code is only partly decoded. Some slave symbols have not been decoded and are ignored."))
	}

  char *result_str = (char*) malloc(sizeof(char) * MAX_CHARS);
  strcpy(result_str, decoded_data->data);
  result_str[decoded_data->length * sizeof(char)] = '\0';
  return result_str;

	free(bitmap);
	free(decoded_data);
    return 0;
}
