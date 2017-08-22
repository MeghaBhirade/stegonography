#include<stdio.h>
#include<string.h>
#include<stdarg.h>
#include "decode.h"
#include "types.h"
#include "common.h"


/* Function Definitions */


Status open_files_decode(DecodeInfo *decInfo)
{
	// Stego Image file
	decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
	//Do Error handling
	if (decInfo->fptr_stego_image == NULL)
	{
		perror("fopen");

		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
		return e_failure;
	}

	return e_success;
}

/*Read and validate decode args from argv*/
Status read_and_validate_decode_args(char *argv[],DecodeInfo * decInfo)
{

	//validate the encode arguments from argvector//

	decInfo->secret_fname = argv[2];

	decInfo->stego_image_fname = argv[3];

	return e_success; 
}

/* Store Magic String */
Status decode_magic_string( DecodeInfo *decInfo)
{

	int idx , byte_read;
	char image_buffer[50];
	char magic_string[5];

	fseek( decInfo->fptr_stego_image , 54L ,SEEK_SET );

	for (idx = 0; idx < 2; idx++)
	{
		//read the magic string and store into image buffer//
		byte_read = fread (image_buffer , 1 ,8 ,decInfo -> fptr_stego_image);

		image_buffer[byte_read] ='\0';

		//decode that magic string stored into image buffer//
		decode_lsb_tobyte(&magic_string[idx],image_buffer);
	}

	magic_string[idx]='\0';

	printf("magic string---------%s\n",magic_string);

	//compare decrypted magic string with orginal magic string//
	if (strcmp(magic_string ,MAGIC_STRING) == 0)

		return e_success;
	else
		return e_failure;
	
}

//function to decode password size//
Status decode_password_size( DecodeInfo *decInfo)
{
	int byte_read , idx;

	char image_buff[20];

	//get byte from destination file and store into buffer//
	byte_read = fread(image_buff , 1 , 8 , decInfo -> fptr_stego_image );

	image_buff[byte_read] = '\0';

	//decrypt password size from byte stored in buffer//
	decode_lsb_tobyte(&decInfo -> password_size , image_buff);

	printf("password size ------------- %d\n",decInfo->password_size);
	return e_success;

}

//function to decode password//
Status decode_password (DecodeInfo *decInfo)
{

	int byte_read,idx;

	char image_buff[20] , password[10];

	//loop should run upto password size//
	for (idx = 0; idx < decInfo-> password_size ; idx++ )
	{

		//get byte from destination image and store into buffer
	byte_read = fread(image_buff , 1 , 8 , decInfo -> fptr_stego_image );
	
	image_buff[byte_read] = '\0';

	//decode password//
	decode_lsb_tobyte(& password[idx] , image_buff);

	}
	password[idx] ='\0';

	printf("password -------------%s\n",password);
	if (strcmp(password , decInfo-> password) == 0)

		return e_success;
	else
	{
		printf("Invalid password\n");
		return e_failure;
	}
}

/* Decode secret file extenstion */
Status decode_secret_file_extn( char *secret_fname, DecodeInfo *decInfo)
{

	int idx = 0 ,jdx,byte_read;

	char image_buffer[50] , ch;

	//get byte from destination file and store into buffer//
	byte_read = fread(image_buffer , 1 ,8 ,decInfo->fptr_stego_image);

	image_buffer[byte_read] = '\0';

	//decrypt secret file extention using bytes from image buffer//
	decode_lsb_tobyte(&decInfo -> secret_extn_size , image_buffer);


	while (decInfo ->secret_fname[idx] != '\0')
	{
		idx++;
	}
		//pass dot after file name//
		decInfo->secret_fname[idx] = '.';

		idx++;

		//loop should run upto secret file size extention//
		for (jdx = 0; jdx < decInfo->secret_extn_size; jdx++)
		{
		byte_read = fread (image_buffer , 1 , 8, decInfo -> fptr_stego_image);

		image_buffer[byte_read] = '\0';

		//decrypt the file extention char by char//
		decode_lsb_tobyte(&ch , image_buffer);

		//store that extention into secret file name//
		decInfo -> secret_fname[idx] =ch;
		idx++;
		}

decInfo->secret_fname[idx] = '\0';

		printf("file name is------------------ %s\n",decInfo->secret_fname);
return e_success;

}

/* decode a lsb into byte of image data array */
Status decode_lsb_tobyte(char *data, char *image_buffer)
{
	int idx , bits;

	*data = *data & 0;

	for (idx =0; idx < 8; idx++)
	{
		//image buffer anded with 1//
		bits =image_buffer[idx] &1;

		//bits shifted with index types and or with data//
		*data = *data | (bits << idx);
	}

	return e_success;
}

/* Encode secret file size */
Status decode_secret_file_size( DecodeInfo *decInfo)
{

	long byte_read;

	int idx;

	char image_buffer[200];

	decInfo -> size_secret_file =0;

	unsigned char size = 0;

	//loop should run upto size of long//
	for (idx = 0; idx < sizeof(long); idx++ )
	{
	
	//copy byte from destination file and store into buffer//	
	byte_read = fread(image_buffer , 1 , 8 , decInfo -> fptr_stego_image);


	image_buffer[byte_read] = '\0';

	//decrypt secret file size using byte stored in image buffer//
	decode_lsb_tobyte(&size ,image_buffer);

	decInfo -> size_secret_file |= size;

		if(sizeof(long)-1 != idx)

		//copy all the bytes and pass through pointer//	
		decInfo -> size_secret_file <<= (sizeof(char)*8);

	}

	printf("size of secret file %ld\n", decInfo->size_secret_file);

return e_success;
}

/* decode function, which does the real decoding */
Status decode_image_to_data(FILE*fptr_secret, FILE *fptr_stego_image ,DecodeInfo * decInfo)
{

	int byte_read,idx;
	char ch;
	char image_buffer[10];
	long total_size = decInfo->size_secret_file;

	//loop should run upto size of secret file//
	for(idx = 0; idx < total_size; idx++)
	{
		//get byte from destination file and store into buffer//
		byte_read = fread(image_buffer , 1, 8 ,decInfo->fptr_stego_image);

			image_buffer[byte_read] = '\0';

			//decrypt charecter from byte stored in image buffer//
			decode_lsb_tobyte(&ch , image_buffer);

			//write that character to secret file//
		    fwrite(&ch , 1 , 1 , decInfo-> fptr_secret);

	}
	return e_success;

}

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo, char* argv[], int argc)
{

	if (argc == 4)
	{
		//function call to read and validate decode arguments//
	   	if (read_and_validate_decode_args( argv , decInfo) != e_success)
		{
	   		printf("Error to read and validate decode arguments\n");

			return e_failure;
   		}
	}

	//function call to open all 2 files//
	if(open_files_decode(decInfo) != e_success)
	{
		printf("Error to open the file\n");

		return e_failure;
	}
   //function call to decode magic string//
	if(decode_magic_string(decInfo) !=e_success )
	{
		printf("Error to decode magic string\n");

		return e_failure;
	}
	//function call to decode password size//
     if (decode_password_size( decInfo)!= e_success )
	 {
		 printf("Error to decode password size\n");

		 return e_failure;
	 }

	  //function call to decode password//

     if (decode_password (decInfo)!= e_success )
	 {
		 printf("Error to decode password\n");

		 return e_failure;
	 }

	//function call to decode the secret file extention//
   if(decode_secret_file_extn(decInfo -> secret_fname , decInfo) != e_success)

   {
	   printf("Error to decode secret file extention\n");
   }
   else
   {
	decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");

	//Do Error handling
	if (decInfo->fptr_secret == NULL)
	{
		perror("fopen");

		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);
	//	return e_failure;
	}
   }

   //function call to decode secret file size//
	if(decode_secret_file_size( decInfo)!= e_success)
	{
		printf("Error to decode secret file size\n");

		return e_failure;
	}
	//function call to decode image to data//
	if(decode_image_to_data(decInfo->fptr_secret, decInfo->fptr_stego_image , decInfo)!= e_success)

	{
		printf("Error to decode image to data\n");

		return e_failure;
	}

	//close all 2 files//
	fclose(decInfo->fptr_secret);
	fclose(decInfo->fptr_stego_image);


return e_success;
}














