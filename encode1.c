#include <stdio.h>
#include<string.h>
#include<stdarg.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */


/*Read and validate Encode args from argv*/
Status read_and_validate_encode_args(char *argv[],EncodeInfo * encInfo)
{

	//validate the encode arguments from argvector//
	encInfo->src_image_fname = argv[2];

	encInfo->secret_fname = argv[3];

	encInfo->stego_image_fname = argv[4];

	return e_success; 

}
//function to open files//
Status open_files(EncodeInfo *encInfo)
{
	// Src Image file
	encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
	//Do Error handling
	if (encInfo->fptr_src_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
		return e_failure;
	}

	// Secret file
	encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
	//Do Error handling
	if (encInfo->fptr_secret == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
		return e_failure;
	}

	// Stego Image file
	encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
	//Do Error handling
	if (encInfo->fptr_stego_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
		return e_failure;
	}

	// No failure return e_success
	return e_success;

}
/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
	uint width, height;
	// Seek to 18th byte
	fseek(fptr_image, 18, SEEK_SET);

	// Read the width (an int)
	fread(&width, sizeof(int), 1, fptr_image);
	printf("width = %u\n", width);

	// Read the height (an int)
	fread(&height, sizeof(int), 1, fptr_image);
	printf("height = %u\n", height);

	fseek(fptr_image, 0L, SEEK_SET);
	
	// Return image capacity
	return width * height * 3;
}

uint get_file_size(FILE * fptr_src)
{

	long size_file;

	//fseek initialises starting of file//
	fseek(fptr_src , 0L , SEEK_END);

	//get file size//
	size_file = ftell(fptr_src);

	printf("size of secret file is %ld\n", size_file);

	fseek(fptr_src, 0L, SEEK_SET);

	return size_file;

}
/* check capacity */
Status check_capacity(EncodeInfo *encInfo)
{
	encInfo-> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);

	encInfo -> size_secret_file = get_file_size( encInfo ->fptr_secret);

	//compare image capacity and text file capacity//
	if(encInfo -> image_capacity > encInfo -> size_secret_file)
	{

		return e_success;
	}
	else
	{
		printf("Error! image file size not exceeds text file\n");

		return e_failure;
	}
}
/* Copy bmp image header */
Status copy_bmp_header(FILE * fptr_src_image, FILE *fptr_stego_image)
{
	int byte_read;

	char buff[200];
	
	//read the 54 byte heading from source image//
	byte_read = fread(buff , 1 , 54 , fptr_src_image);
	
	buff[byte_read] = '\0';

	//copy that read content into destination image//
	fwrite(buff , 1 , byte_read , fptr_stego_image);

	return e_success;
}


/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
 
	int idx , byte_read;

	char image_buffer[50];

	for (idx = 0; magic_string[idx] != '\0'; idx++)
	{

		//read the magic string and store into image buffer//
		byte_read = fread (image_buffer , 1 ,8 ,encInfo -> fptr_src_image);

		image_buffer[byte_read] ='\0';

		//encode that magic string stored into image buffer//
		encode_byte_tolsb(magic_string[idx],image_buffer);

		//write that encoded content into  destination file//
		fwrite (image_buffer , 1 , byte_read , encInfo ->fptr_stego_image);
	}
	return e_success;
}

/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
	int idx ,byte_read;

	char image_buffer[50];

	//locate the postion of . operator//
	char *cptr = strrchr(file_extn ,'.');

	//copy the file extention after . operator//
	strcpy(encInfo ->extn_secret_file , ++cptr);

	encInfo -> secret_extn_size = strlen (encInfo -> extn_secret_file);

	printf("secret file extention size---------------------%d\n",encInfo->secret_extn_size);

	//get byte from src file and store into buffer//
	byte_read = fread(image_buffer , 1 ,8 ,encInfo -> fptr_src_image);

	image_buffer[byte_read] = '\0';

	encode_byte_tolsb(encInfo-> secret_extn_size , image_buffer);

	fwrite(image_buffer , 1 , byte_read , encInfo -> fptr_stego_image);

	//loop should run upto extention of secret file//
	for (idx = 0; encInfo->extn_secret_file[idx] != '\0'; idx++)
	{

		//read the bytes from src image and store into buffer//
	byte_read = fread (image_buffer, 1, 8 , encInfo -> fptr_src_image);

	image_buffer[byte_read] = '\0';

	//encrypt that secret file extention using bytes stored in image buffer//
	encode_byte_tolsb(encInfo -> extn_secret_file[idx], image_buffer);

//	printf("%c\n",encInfo->extn_secret_file[idx]);

	//write thatbuffer content into destination file//
	fwrite(image_buffer , 1 ,byte_read , encInfo -> fptr_stego_image);

	}
return e_success;

}
/* Encode a byte into LSB of image data array */
Status encode_byte_tolsb(char data, char *image_buffer)
{
	int idx ;

	for (idx = 0; idx < 8; idx++ )
	{
		//set and reset the lsb bit//
		if ( data & (1<< idx))
		{
			image_buffer[idx] = image_buffer[idx] | 1;
		}
		
		else
		{
			image_buffer[idx] = image_buffer[idx] &(~1);
		}
	}
	 return e_success;
}
/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
	char  ch ;

	char image_buffer[200];

	int size , byte_read ;

	unsigned int num = 0;

	int idx, jdx ,bit;

	//get the total size from secret file size function//
	long total_size = encInfo -> size_secret_file ;

	size = sizeof(long)*8;

	//runthe llop upto the size of long//
	for (idx = 0; idx < sizeof(long); idx++)
	{
		ch = '\0';
		num = 0;

		for (jdx = 0; jdx <8 ; jdx++)
		{
			size--;


			//mask the bit//
			bit = total_size & (1 << size);

			num = num|bit;
		}
		num >>= size;

		ch = ch | num;

		//read bytes from src image file and store into buffer//
	byte_read = fread (image_buffer, 1, 8 , encInfo -> fptr_src_image);

	image_buffer[byte_read] = '\0';

	//encrypt the char with byte from image buffer //
	encode_byte_tolsb(ch,image_buffer);

	//write buffer read content int destfile//
	fwrite(image_buffer , 1 ,byte_read , encInfo -> fptr_stego_image);
	
	}
	printf("encoded secret file size-------%d", ch);
printf("\n");
	return e_success;
}
//function to encode password size//
Status encode_password_size( EncodeInfo *encInfo)
{
	int byte_read;

	char image_buff[20];

	//read the byte from src image file//
	byte_read = fread(image_buff , 1 , 8 , encInfo -> fptr_src_image );

	image_buff[byte_read] = '\0';

	//encrypt that password size into byte stored in image buffer//
	encode_byte_tolsb(encInfo -> password_size , image_buff);

	//write that encrypted data into destination file//
	fwrite(image_buff , 1 , byte_read , encInfo -> fptr_stego_image);

	return e_success;

}
//function to encode password//
Status encode_password (EncodeInfo *encInfo)
{
	int byte_read,idx;

	char image_buff[20];

	//loop to continue with all the bytes//
	for (idx = 0; encInfo->password[idx] != '\0' ; idx++ )
	{

		//read the byte from src image file//
	byte_read = fread(image_buff , 1 , 8 , encInfo -> fptr_src_image );

	image_buff[byte_read] = '\0';

	//encrypt the password into byte stored in image buffer//
	encode_byte_tolsb(encInfo -> password[idx] , image_buff);

	//write that encrypted data into destination file//
	fwrite(image_buff , 1 , byte_read , encInfo -> fptr_stego_image);
	}

	return e_success;

}
/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)

{
	int byte_read;

	char image_buff[1000001];

	while (feof(fptr_src) == 0) 
	{
		//read all remaining bytes from source file//
	byte_read = fread(image_buff , 1, 1000000 ,fptr_src );

	image_buff[byte_read] = '\0';

	//write that read content into destination file//
	fwrite (image_buff , 1 , byte_read , fptr_dest);
	}
	return e_success;

}
/* Encode function, which does the real encoding */
Status encode_data_to_image(FILE*fptr_secret, FILE *fptr_src_image, FILE *fptr_stego_image ,EncodeInfo * encInfo)
{
	int byte_read_secret , byte_read_image ;

	int idx;

	char image_buffer[10];


	while (feof(fptr_secret)== 0)
	{
		//read the secret file data and stoe into 1 variable//
		byte_read_secret = fread(encInfo ->secret_data , 1, 8 ,fptr_secret);
		encInfo ->secret_data[byte_read_secret] = '\0';

		//loop to continue with remaining bytes//
		for (idx = 0; encInfo->secret_data[idx] != '\0'; idx++)
		{
			//read the byte from the image file//
			byte_read_image = fread(image_buffer , 1, 8, fptr_src_image );
			image_buffer[byte_read_image] = '\0';

			//encrypt secret file data into byte from image file//
	         encode_byte_tolsb(encInfo -> secret_data[idx] ,image_buffer);

			 //write encrypted file into destination file//
			 fwrite(image_buffer, 1 ,byte_read_image , fptr_stego_image);
		}
	}
	return e_success;
}
/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo, char* argv[], int argc)
{
	if (argc == 5)
	{
	   	if (read_and_validate_encode_args( argv , encInfo) != e_success)
		{
	   		printf("Error to read and validate encode arguments\n");
   		}
	}

	//function call to open all 3 files//
	if(open_files(encInfo) != e_success)
	{
		printf("Error to open the file\n");
	}

	//function call to check capacity//
	if (check_capacity(encInfo) != e_success)
	{
		printf("Error to check capacity\n");
	}
	
	//function call to copy bmp header//
   if(copy_bmp_header( encInfo -> fptr_src_image ,encInfo-> fptr_stego_image) != e_success)
   {
	   printf("Error to copy header of bmp file\n");
   }

   //function call to encode magic string//
	if(encode_magic_string(MAGIC_STRING , encInfo) !=e_success )
	{
		printf("Error to encode magic string\n");
	}
	//function call to do encode password//
      encode_password_size( encInfo);

	  //function call to encode password//
      encode_password (encInfo);

	//function call to encode the secret file extention//
   if(encode_secret_file_extn(encInfo -> secret_fname , encInfo) != e_success)
   {
	   printf("Error to encode secret file extention\n");
   }

   //function call to encode secret file size//
	if(encode_secret_file_size(encInfo -> size_secret_file , encInfo)!= e_success)
	{
		printf("Error to encode secret file size\n");
	}

	//function call to encode data to image//
	encode_data_to_image(encInfo->fptr_secret,encInfo->fptr_src_image,encInfo-> fptr_stego_image , encInfo);

	//function call to copy remaining image data//
	copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image);

	//close all 3 files//
	fclose(encInfo->fptr_src_image);
	fclose(encInfo->fptr_secret);
	fclose(encInfo->fptr_stego_image);
}
