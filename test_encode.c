/**********************************************WELCOME TO STEGANOGRAPHY********************************************************************/

/**************PROGRAM:To implement the steganography for a given image and text file*/
/***************AUTHOR: SACHIN L ************************************************/
/*****************DATE:19-11-2018 **********************************************/

/*Include all the sub file which contains various all the declaration and some other definations*/
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include "encode.h"
#include "common.h"
#include "types.h"

/*Declaration of global variables to hold the size of files*/
uint img_size;
short header_size;

/*The main program started from here*/
int main(int argc,char *argv[])
{
    /*Declaring structure variables fo encode and decode*/
    EncodeInfo encInfo;
    EncodeInfo decInfo;

    /*A switch case to perform either encoding or decoding*/
    switch(check_operation_type(argv))
    {
	case e_encode:
	   
	    /*Validate the User given arguments*/
	    if(read_and_validate_encode_args(argc,argv,&encInfo) == e_success)
	    {
		/*Perform decoding*/
		if(do_encoding(&encInfo) == e_success)
		{
		    sleep(1);
		    printf("%sINFO: ##Your file has been successfully encrypted#%s\n",GREEN,RESET);
		}
		else
		{
		    sleep(1);
		    printf("%sERROR: Encryption failed%s\n",RED,RESET);
		    exit(1);
		}
	    }
	    else
	    {
		disp_error();
		exit(1);
	    }
	    break;
	    /*Case 2 to perform decoding*/
	case e_decode:
	    printf("%sINFO: ##Decoding Procedure Started##\n",BLUE);

	    /*Validate the user given arguments*/
	    if(read_and_validate_decode_args(argv,&decInfo) == e_success)
	    {
		/*go through the decoding*/
		if(do_decoding(argc,argv,&decInfo) == e_success)
		{
		    sleep(1);
		    printf("%sINFO: ##Your file has been successfully decoded##%s\n",GREEN,RESET);
		}
		else
		{
		    sleep(1);
		    printf("%sERROR: Decryption failed%s\n",RED,RESET);
		    exit(1);
		}
	    }
	    else
	    {
		disp_error();
		exit(1);
	    }
	    break;

    }

    return 0;
}

/*A function declaraton to check which operation has to perform*/
OperationType check_operation_type(char *argv[])
{
    if( argv[1] != NULL)
    {
	/*If user given argument as -e perform encoding*/
	if(!(strcmp(argv[1],"-e")))
	{
	    return e_encode;
	}
	/*If user given argument as -d perform decoding*/
	else if(!(strcmp(argv[1],"-d")))
	{
	    return e_decode;
	}
	/*the wrong argument causes to terminate the program*/
	else
	{
	    return e_unsupported;
	}
    }
    else
    {
	disp_error();
	exit(1);
    }
}


/*A function defination to perform to validate the user given arguments*/
Status read_and_validate_encode_args(int argc,char *argv[],EncodeInfo *encInfo)
{
    char *ptr;
    int iter;
    /*Ensure that the all the required arguments are provided*/
    if(argv[1] != NULL && argv[2] != NULL && argv[3] != NULL)
    {
	/*Check for file extnsion*/
	if(strstr(argv[2],".bmp") && strstr(argv[3],".txt" ))
	{
	    /*Allocate memory to to store the file names*/
	    encInfo->src_image_fname=malloc(20);
	    encInfo->secret_fname=malloc(20);
	    encInfo->stego_image_fname=malloc(20);
	    strcpy(encInfo->src_image_fname,argv[2]);
	    strcpy(encInfo->secret_fname,argv[3]);
	    ptr=strchr(argv[3],'.');
	    for(iter=0;*(ptr+iter)!='\0';iter++)
	    {
		encInfo->extn_secret_file[iter]=*(ptr+iter);
	    }
	    encInfo->secret_file_extn_size=iter;
	    /*Searching for output file*/
	    if( argc == 4 ) 
	    {
		sleep(1);
		printf("%sINFO: Output file not mentioned.Creating steged_1mg.bmp as default\n",BLUE);
		encInfo->stego_image_fname="steged_1mg.bmp";
	    }
	    else
	    {
		encInfo->stego_image_fname=argv[4];
	    }
	    return e_success;
	}
	else
	{
	    return e_failure;
	}
    }
    else
    {
	return e_failure;
    }
}

/*Actual encoding started from here*/
Status do_encoding(EncodeInfo *encInfo)
{
    sleep(1);
    printf("%sINFO: Opening files required\n",BLUE);
    if (open_files(encInfo) == e_failure)
    {
	sleep(1);
	printf("%sERROR: %s function failed%s\n",RED,"open_files",RESET );
	return 1;
    }
    else
    {
	sleep(1);
	printf("%sINFO: Opened beautifull.bmp\n",GREEN);
	sleep(1);
	printf("%sINFO: Opened secret.txt\n",GREEN);
    }
    sleep(1);
    printf("%sINFO: ## Encoding procedure started ##\n",BLUE);
    sleep(1);
    printf("%sINFO: Checking for secret.txt size\n",BLUE);
    if(check_capacity(encInfo) == e_success)
    {
	sleep(1);
	printf("%sINFO: Done. Not Empty\n",GREEN);
    }
    else
    {
	sleep(1);
	printf("%sERROR: Secret.txt file should not be empty%s\n",RED,RESET);
	exit(1);
    }


    sleep(1);
    printf("%sINFO: Checking %s capacity to handle %s\n",BLUE,encInfo->src_image_fname,encInfo->secret_fname);
    img_size = get_image_size_for_bmp(encInfo->fptr_src_image);
    
    /*Check the capacity of image file to encode text file*/
    if((img_size-header_size-1) >= ((1+sizeof(long)+encInfo->secret_file_extn_size+sizeof(long)+encInfo->size_secret_file)*8))
    {
	sleep(1);
	printf("%sINFO: Done. Found OK\n",GREEN);
    }
    else
    {
	sleep(1);
	printf("%sERROR: The image file size should be larger to store secret message%s\n",RED,RESET);
	exit(1);
    }

    sleep(1);
    printf("%sINFO: Copying header\n",BLUE);

    /*Copy header informtion*/
    if((copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)) == e_success)
    {
	sleep(1);
	printf("%sINFO: Done\n",GREEN);
    }
    else
    {
	sleep(1);
	printf("%sERROR: Header copy is failed%s\n",RED,RESET);
	exit(1);
    }

    /*Encode the magic string*/
    sleep(1);
    printf("%sINFO: Encoding magic string signature\n",BLUE);
    if((encode_magic_string(MAGIC_STRING,encInfo))== e_success)
    {
	sleep(1);
	printf("%sINFO: Done\n",GREEN);	
    }
    else
    {
	sleep(1);
	printf("%sERROR: Magic string not copied%s\n",RED,RESET);
	exit(1);
    }

    /*encoding of the secret file extn size*/
    printf("%sINFO: Encoding secret file extension size\n",BLUE);
    if(encode_secret_file_extn_size(encInfo->secret_file_extn_size,encInfo) == e_success)
    {
	sleep(1);
	printf("%sINFO: Done\n",GREEN);
    }
    else
    {
	sleep(1);
	printf("%sERROR: Encryption of secret file extension size is failed%s\n",RED,RESET);
	exit(1);
    }
    /*Encoding of secret file extension*/
    sleep(1);
    printf("%sINFO: Encoding secret file extension\n",BLUE);
    if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo) == e_success)
    {
	sleep(1);
	printf("%sINFO: Done\n",GREEN);
    }
    else
    {
	sleep(1);
	printf("%sERROR: Encryption of secret file extension is failed%s\n",RED,RESET);
	exit(1);
    }

    /*Encode secret file size*/
    sleep(1);
    printf("%sINFO: Encoding secret file size\n",BLUE);
    if(encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_success)
    {
	sleep(1);
	printf("%sINFO: Done\n",GREEN);
    }
    else
    {
	sleep(1);
	printf("%sERROR: Encryption of secret file size is failed%s\n",RED,RESET);
	exit(1);
    }

    /*Encode the secret file data*/
    sleep(1);
    printf("%sINFO: Encoding secret file data\n",BLUE);
    if(encode_secret_file_data(encInfo)== e_success)
    {
	sleep(1);
	printf("%sINFO: Done\n",GREEN);
    }
    else
    {
	sleep(1);
	printf("%sERROR: Encryption of secret file data is failed%s\n",RED,RESET);
	exit(1);
    }


    /*copy the remaining left over data*/
    sleep(1);
    printf("%sINFO: Encoding left over data\n",BLUE);
    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
    {
	sleep(1);
	printf("%sINFO: Done\n",GREEN);
    }
    else
    {
	sleep(1);
	printf("%sERROR: Copy of remaining data is failed%s\n",RED,RESET);
	exit(1);
    }

    sleep(1);
    printf("%sINFO: ##Encryption successfully done ##\n",GREEN);

    return 0;
}

/*A function defination to open files*/
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
	sleep(1);
	perror("fopen");
	fprintf(stderr, "%sERROR: Unable to open file %s\n",RED,encInfo->src_image_fname);

	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
	sleep(1);
	perror("fopen");
	fprintf(stderr, "%sERROR: Unable to open file %s%s\n",RED,encInfo->secret_fname,RESET);

	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
	sleep(1);
	perror("fopen");
	fprintf(stderr, "%sERROR: Unable to open file %s%s\n",RED,encInfo->stego_image_fname,RESET);

	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

/* A function defination to Check capacity to to handle secret file */
Status check_capacity(EncodeInfo *encInfo)
{
    fseek(encInfo->fptr_secret,0L,SEEK_END);
    if((encInfo->size_secret_file=ftell(encInfo->fptr_secret))== 0)
    {
	return e_failure;
    }
    else
    {
	encInfo->size_secret_file=ftell(encInfo->fptr_secret)+1;
	rewind(encInfo->fptr_secret);

	return e_success;
    }
}

/*get the size of bmp file*/
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);

    // Return image capacity
    return width * height * 3;
}

/*copy the header informtion*/
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    long posi;
    fseek(fptr_src_image,10L,SEEK_SET);
    fread(&header_size,4,1,fptr_src_image);
    char temp[header_size];

    rewind(fptr_src_image);
    fread(temp,header_size,1,fptr_src_image);
    fwrite(temp,header_size,1,fptr_dest_image);

    return e_success;
}

/*Encoding of magic string to assure rever about encrypted data*/
Status encode_magic_string(char magic_string,EncodeInfo *encInfo)
{
    long posi;
    fseek(encInfo->fptr_src_image,header_size,SEEK_SET);
    fread(encInfo->image_data,8,1,encInfo->fptr_src_image);
    encode_byte_to_lsb(magic_string,encInfo->image_data);
    fwrite(encInfo->image_data,8,1,encInfo->fptr_stego_image);
    return e_success;
}

/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    int iter;
    for(iter=0;iter<4;iter++)
    {
	fread(encInfo->image_data,8,1,encInfo->fptr_src_image);
	encode_byte_to_lsb(file_extn[iter],encInfo->image_data);
	fwrite(encInfo->image_data,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}

/*Encoding of secret file extn data*/
Status encode_secret_file_extn_size(long file_size_extn,EncodeInfo *encInfo)
{
    int iter;
    long posi;
    char *ptr=(char *)&file_size_extn;
    for(iter=0;iter<sizeof(long);iter++)
    {
	fread(encInfo->image_data,8,1,encInfo->fptr_src_image);
	encode_byte_to_lsb(*(ptr+iter),encInfo->image_data);
	posi=ftell(encInfo->fptr_stego_image);
	fwrite(encInfo->image_data,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}

/* Encode secret file size */
Status encode_secret_file_size(long file_size,EncodeInfo *encInfo)
{
    int iter;
    char *ptr=(char *)&file_size;
    for(iter=0;iter<sizeof(long);iter++)
    {
	fread(encInfo->image_data,8,1,encInfo->fptr_src_image);
	encode_byte_to_lsb(*(ptr+iter),encInfo->image_data);
	fwrite(encInfo->image_data,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    int iter,ch;
    for(iter=0; iter<encInfo->size_secret_file; iter++ )
    {
	fread(&ch,1,1,encInfo->fptr_secret);
	fread(encInfo->image_data,8,1,encInfo->fptr_src_image);
	encode_byte_to_lsb(ch,encInfo->image_data);
	fwrite(encInfo->image_data,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    int current_posi,end_posi,nob;
    current_posi=ftell(fptr_src);
    fseek(fptr_src,0L,SEEK_END);
    end_posi=ftell(fptr_src);
    fseek(fptr_src,current_posi,SEEK_SET);
    current_posi=ftell(fptr_src);
    int size = end_posi-current_posi;
    char temp[size];

    nob=fread(temp,1,size,fptr_src);
    nob=fwrite(temp,1,size,fptr_dest);

    return e_success;
}


/*Actual encryption is done in this function*/
void encode_byte_to_lsb(char ch,char *buffer_image)
{
    uint res1,res2,mask,iter;
    for(iter=0;iter<8;iter++)
    {

	mask=1<<iter;
	res1=!(!(ch&mask));
	res2=buffer_image[iter] & 1;

	buffer_image[iter] = res1 == 1?res2 == 0?(buffer_image[iter]+1):(buffer_image[iter]):res2 == 0?(buffer_image[iter]):(buffer_image[iter]-1);
    }
}

/*A error prompt to user for wrong arguments*/
void disp_error()
{
    sleep(1);
    printf("%s./lsb_steg: Encoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]%s\n",RED,RESET);
    sleep(1);
    printf("%s./lsb_steg: Decoding: ./lsb_steg -d <.bmp_file> [output file]%s\n",RED,RESET);
}


/***********************************************************DECRYPTION********************************************************************/

/*validate tha arguments to decrypt data*/
Status read_and_validate_decode_args(char *argv[],EncodeInfo *decInfo)
{
    if(argv[1] != NULL && argv[2] != NULL)
    {
	if(strstr(argv[2],".bmp"))
	{
	    decInfo->stego_image_fname=malloc(20);
	    decInfo->decode_text_fname=malloc(20);
	    strcpy(decInfo->stego_image_fname,argv[2]);
	    return e_success;
	}
	else
	{
	    return e_failure;
	}
    }
    else
    {
	return e_failure;
    }
}

/*Decryption is started from here*/
Status do_decoding(int argc,char *argv[],EncodeInfo *decInfo)
{
    // Test open_files
    sleep(1);
    printf("%sINFO: Opening files required\n",BLUE);
    if (open_files_decode(decInfo) == e_failure)
    {
	sleep(1);
	printf("%sERROR: %s function failed%s\n",RED,"open_files",RESET );
	return 1;
    }
    else
    {
	sleep(1);
	printf("%sINFO: Opened steged_img.bmp\n",GREEN);
    }

    sleep(1);
    printf("%sINFO: Decoding Magic String Signature\n",BLUE);

    /*Decode the magic string*/
    if(decode_magic_string(decInfo->fptr_stego_image) == e_success)
    {
	sleep(1);
	printf("%sINFO: Done\n",GREEN);
    }
    else
    {
	sleep(1);
	printf("%sERROR: Magic string not matched So no need to decode image%s\n",RED,RESET);
	exit(1);
    }

    /*Decode the size of the file extension*/
    decInfo->decode_file_extn_size=decode_output_file_extn_size(decInfo->fptr_stego_image);

    /*get the output file extension*/
    if(decode_output_file_extn(decInfo->fptr_stego_image,decInfo->extn_decode_file,decInfo->decode_file_extn_size) == e_success)
    {
	if(argc == 3)
	{
	    sleep(1);
	    printf("%sINFO: Output file not mensioned, Create decode%s as default\n",BLUE,decInfo->extn_decode_file);
	    strcpy(decInfo->decode_text_fname,"decode");
	    strcat(decInfo->decode_text_fname,decInfo->extn_decode_file);
	}
	else
	{
	    strcpy(decInfo->decode_text_fname,argv[3]);
	}
    }

    /*create the output file*/
    decInfo->fptr_decode_text = fopen(decInfo->decode_text_fname, "w+");
    
    // Do Error handling
    if (decInfo->fptr_decode_text == NULL)
    {
	sleep(1);
	perror("fopen");
	fprintf(stderr, "%sERROR: Unable to open file %s\n",RED,decInfo->decode_text_fname);
	exit(1);
    }
    else
    {
	sleep(1);
	printf("%sINFO: Opened %s\n",GREEN,decInfo->decode_text_fname);
	sleep(1);
	printf("%sINFO: Done. Opened all required files\n",GREEN);
    }

    sleep(1);
    printf("%sINFO: Decoding file size\n",BLUE);
    decInfo->decode_file_size=decode_output_file_size(decInfo->fptr_stego_image);
    sleep(1);
    printf("%sINFO: Done\n",GREEN);

    /*decode the original message*/
    if(decoding_file_data(decInfo->fptr_stego_image,decInfo->fptr_decode_text,decInfo->decode_file_size) == e_success)
    {
	sleep(1);
	printf("%sINFO: Decoding done successfully##\n",GREEN);
	return e_success;
    }
    else
    {
	return e_failure;
    }
}

/*A function to  oprn the files*/
Status open_files_decode(EncodeInfo *decInfo)
{
    // Src Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
	sleep(1);
	perror("fopen");
	fprintf(stderr, "%sERROR: Unable to open file %s\n",RED,decInfo->stego_image_fname);

	return e_failure;
    }
    // No failure return e_success
    return e_success;
}

/*A function to decode magic string*/
Status decode_magic_string(FILE *fptr_stego_image)
{   char ch;
    fseek(fptr_stego_image,10L,SEEK_SET);
    fread(&header_size,4,1,fptr_stego_image);
    fseek(fptr_stego_image,header_size,SEEK_SET);
    ch=decode_byte(fptr_stego_image);
    return ch == 'A'? e_success:e_failure;
}

/*decode output file extension size*/
long decode_output_file_extn_size(FILE *fptr_stego_image)
{
    long size=0,base=1;
    char ch;
    int iter;
    for(iter = 0; iter < sizeof(long); iter++)
    {
	ch=decode_byte(fptr_stego_image);
	size=size+(int)ch*base;
	base=base*10;
    }
    return size;
}

/*Decode the output file ectension*/
Status decode_output_file_extn(FILE *fptr_stego_image,char extn_decode_file[],long extn_size)
{
    char ch;
    int iter;
    for(iter = 0; iter <extn_size; iter++)
    {
	ch=decode_byte(fptr_stego_image);
	extn_decode_file[iter]=ch;
    }
    extn_decode_file[iter] ='\0';

    return e_success;
}

/*Decode the output file size*/
long decode_output_file_size(FILE *fptr_stego_image)
{
    long size=0,base=1;
    char ch;
    int iter;
    for(iter = 0; iter<sizeof(long); iter++)
    {
	ch=decode_byte(fptr_stego_image);
	size=size+(int)ch*base;
	base=base*10;
    }
    return size;
}

/*Decode the actual data*/
Status decoding_file_data(FILE *fptr_stego_image,FILE *fptr_decode_text,long decode_file_size)
{
    int iter;
    char ch;
    for(iter=0;iter<decode_file_size;iter++)
    {
	ch=decode_byte(fptr_stego_image);
	fwrite(&ch,1,1,fptr_decode_text);
    }
    return e_success;
}

char decode_byte(FILE *fptr_stego_image)
{
    long int posi=ftell(fptr_stego_image);
    //printf("current position=%ld\n",posi);
    unsigned char ch=0,temp[8];
    int iter;
    fread(temp,8,1,fptr_stego_image);
    for(iter=7;iter>=0; iter--)
    {
	ch= ((temp[iter] & 1) | ch);
	if(iter >= 1)
	    ch=ch << 1;
    }
    return ch;
}

/**********************************************************FINISHED************************************************************************/
