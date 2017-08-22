#include<stdio.h>
#include<stdarg.h>
#include<stdlib.h>
#include<string.h>
#include "types.h"
#include "encode.h"
#include "common.h"
#include "decode.h"

//function to read information for encoding//
Status read_info(EncodeInfo *encInfo)
{
	int size;
	char buff[20];

	//read secret file name and store into buffer//
	printf("Enter the source file : ");
	scanf("%s",buff);

	//find the length of buffer//
	size = strlen(buff);

	//dynamic allocation of memory//
	char *ptr = (char *) malloc (sizeof(char*) * size);
	strcpy(ptr , buff);

	//pass the pointer to function//
	encInfo -> src_image_fname = ptr;
		
	printf("%s\n", encInfo->src_image_fname);

	//read secret file name and store into buffer//
	printf("Enter the secret file : ");
	scanf("%s",buff);

	//find the length of buffer//
	size = strlen(buff);

	//dynamic allocation of memory//
	ptr = (char *) malloc (sizeof(char*) * size);
	strcpy(ptr , buff);

	//pass the pointer to function//
	encInfo ->secret_fname = ptr ;
	
	printf("%s\n", encInfo->secret_fname);

	//read destination file name and store into buffer//
	printf("Enter the destination file :");
	scanf("%s",buff); 

	//find the length of buffer//
	size = strlen(buff);

	//dynamic allocation of memory//
	ptr = (char *) malloc (sizeof(char*) * size);

	//copy buffer content to pointer//
	strcpy(ptr , buff);

	//pass the pointer to function//
	encInfo -> stego_image_fname = ptr ;
	
	printf("%s\n", encInfo->stego_image_fname);
	
	return e_success;
}

//function to read decode information//
Status read_info_decode(DecodeInfo *decInfo)
{

	int size;
	char buff[20];

	//read secret file name and store into buffer//
	printf("Enter the secret file : ");
	scanf("%s",buff);

	//find the length of buffer//
	size = strlen(buff);

	//dynamic allocation of memory//
	char*ptr = (char *) malloc (sizeof(char*) * size);

	//copy buffer content to pointer//
	strcpy(ptr , buff);

	//pass the pointer to function//
	decInfo ->secret_fname = ptr ;
	
	printf("%s\n", decInfo->secret_fname);

	//read destination file name and store into buffer//
	printf("Enter the destination file :");
	scanf("%s",buff); 

	//find the length of buffer//
	size = strlen(buff);

	//dynamic allocation of memory//
	ptr = (char *) malloc (sizeof(char*) * size);

	//copy buffer content to pointer//
	strcpy(ptr , buff);

	//pass the pointer to function//
	decInfo -> stego_image_fname = ptr ;
	
	printf("%s\n", decInfo->stego_image_fname);
}

//function  to check operation type//
OperationType check_operation_type(char **argv)
{
	//check for encoding//
	if (strncmp(*argv,"-e", 2)== 0)
	{
		return	e_encode;
	}
	//check for decoding//
	if (strncmp(*argv,"-d", 2)== 0)
	{
		return	e_decode;
	}
	else
	{
		return  e_unsupported;
	}

}
//function to get password//
void read_password(EncodeInfo *encInfo ,DecodeInfo *decInfo ,int option )
{
	int size;
	char buff[20];

	//check option for encoding or not//
	if (option == 1)
	{

	//ask the password from user//		
	printf("Enter the password : ");
	scanf ("%s" , buff);

	//find length of buffer//
	size = strlen(buff);

	if(size < 8)
	{
	//copy buffer content to password pointer//		
	strcpy(encInfo -> password, buff);

	//pss the size to function//
	encInfo ->password_size = size;
	}
	else
	printf("Check the password\n");
	}

	//chek option is for decoding//
	if(option == 2)
	{
	//if it is true get password from user and store into buffer//		
	printf("Enter the password : ");
	scanf ("%s" , buff);

	//find size of buffer//
	size = strlen(buff);

	//pass both password and password size to function//
	strcpy(decInfo -> password, buff);

	decInfo ->password_size = size;
	}
		
}
//main program for stegnography// 
int main(int argc , char *argv[])
{
	int option;
	char ch;

	EncodeInfo encInfo;

	DecodeInfo decInfo;

	do
	{

	//if argument count is 1(i.e a.out)ask secect option from user//	
	if (argc == 1)
	{
	printf("Select the option :\n\t 1.Encode\n\t 2.Decode\n");

	//read option from user//
	printf("Enter the option :");
	scanf("%d", &option);

	//if option is 1 pass readed information to ecode function//
	if(option == 1)
		read_info(&encInfo);
	else
		//else pass to decode function//
		read_info_decode(&decInfo);
		
	}

	else
	{
		//else call check operation type function//
		option = check_operation_type(&argv[1]);
	}
	
	switch(option)
	{
		case e_encode:
	
			//function call to read password //
 			read_password(&encInfo , &decInfo , option );

			//function call for decoding//
			do_encoding(&encInfo, argv, argc);

			printf("Encoded successfully\n");

			break;

		case e_decode:

			//function call to read password //
 			read_password(&encInfo , &decInfo , option );

			//function call for decoding//
			do_decoding( &decInfo , argv, argc);

			printf("decoded successfully\n");

			break;

		default:
			printf("Unsupported\n");
	}
	//if you want to continue check for option//
printf("Do you want to continue :");
scanf("\n%c",&ch);

	}while (ch == 'y');

	return 0;
}
