
#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types


#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;
	unsigned char secret_extn_size;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

	char password[10];
	unsigned char password_size;

} DecodeInfo;

/* Decoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo, char * argv[], int argc);

/* Get File pointers for i/p and o/p files */
Status open_files_decode(DecodeInfo *decInfo);


/* Get file size */
uint get_file_size(FILE *fptr);

/* Store Magic String */
Status decode_magic_string( DecodeInfo *decInfo);

/*decode the password size*/
Status decode_password_size( DecodeInfo *decInfo);

/*deode the password*/
Status decode_password (DecodeInfo *decInfo);

/*Decode secret file extenstion */
Status decode_secret_file_extn( char *secret_fname, DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size( DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);


/* Decode function, which does the real decoding */
Status decode_data_to_image(FILE *fptr_secret, FILE *fptr_stego_image , DecodeInfo * decInfo);


/* Decode a byte into LSB of image data array */
Status decode_lsb_tobyte(char *data, char *image_buffer);


/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_stego_image);

/* decode function, which does the real decoding */
Status decode_image_to_data(FILE*fptr_secret, FILE *fptr_stego_image ,DecodeInfo * decInfo);

#endif
