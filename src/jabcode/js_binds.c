#include <emscripten.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./include/jabcode.h"

#define MAX_CHARS 500

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
	if(decoded_data == NULL) {
		free(bitmap);
		reportError("Decoding JABCode failed");
    return NULL;
	}

  char *result_str = (char*) malloc(sizeof(char) * (decoded_data->length + 1));
  strcpy(result_str, decoded_data->data);
  result_str[decoded_data->length] = '\0';

	free(bitmap);
	free(decoded_data);

  return result_str;
}

EMSCRIPTEN_KEEPALIVE
int encode_image(jab_char *data_string) {
  int result = 0;
  int color_number = 8;
  int symbol_number = 0;

  jab_encode* enc = createEncode(color_number, symbol_number);

  jab_int32 		symbol_versions_number = 30;
  jab_int32* 		symbol_ecc_levels = 0;
  jab_int32 		symbol_ecc_levels_number = 0;
  jab_vector2d* 	symbol_versions = 0;
  jab_int32* 		symbol_positions = 0;
  jab_int32 		symbol_positions_number = 0;

  enc->master_symbol_width = 300;
  enc->master_symbol_height = 300;

	for(jab_int32 loop=0; loop<symbol_number; loop++)
	{
		if(symbol_ecc_levels)
			enc->symbol_ecc_levels[loop] = symbol_ecc_levels[loop];
		if(symbol_versions)
			enc->symbol_versions[loop] = symbol_versions[loop];
		if(symbol_positions)
			enc->symbol_positions[loop] = symbol_positions[loop];
	}

  jab_data* data = (jab_data *)malloc(sizeof(jab_data) + strlen(data_string) * sizeof(jab_char));
  if(!data) {
      reportError("Memory allocation for input data failed");
      destroyEncode(enc);
      free(data);
      return 1;
  }
  data->length = strlen(data_string);
  memcpy(data->data, data_string, strlen(data_string));

	if(generateJABCode(enc, data) != 0) {
		reportError("Creating jab code failed");
    destroyEncode(enc);
    free(data);
		return 1;
	}
  if(!saveImage(enc->bitmap, "out.png"))
  {
    reportError("Saving image failed");
    destroyEncode(enc);
    free(data);
    return 1;
  }

  destroyEncode(enc);
  free(data);
  return 0;
}
