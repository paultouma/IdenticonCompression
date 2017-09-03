//
//  main.c
//  IdenticonImageCompression
//
//  Created by Paul Touma on 8/15/17.
//  Copyright © 2017 Paul Touma. All rights reserved.
//

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

//constants
const int identicon_pixel_length = 3;
const int unicode_zero = 48;

/*
 *  Function: decimal_to_binary
 *  ------------------------------------------------
 *  computes binary representation of decimal number
 *
 *      dec: decimal number
 *
 *  returns: binary representation of decimal number
 */
int decimal_to_binary(int dec)
{
    int bin = 0, k = 1;
    while (dec)
    {
        bin += (dec % 2) * k;
        k *= 10;
        dec /= 2;
    }
    
    return bin;
}

/*
 *  Function: pre_fill_binary_identicon_string
 *  ------------------------------------------------
 *  fill beginning of string (representing an identicon id in binary) with zeros
 *  to ensure that string representation fills up to (identicon_pixel_length)^2
 *  total characters
 *      binary_identicon_string_rep: pointer to stream of chars representing binary_id
 *
 *  returns: nothing
 */
void post_fill_binary_identicon_string (char binary_identicon_string_rep []) {
    
    size_t length = strlen(binary_identicon_string_rep);
    
    int original_bin_len = (int)((ssize_t)length);
    
    int current_bin_position = 0;
    
    char adjuted_binary_string_rep [identicon_pixel_length * identicon_pixel_length + 1];
    
    for (int current_position = 0; current_position < identicon_pixel_length * identicon_pixel_length + 1; current_position++) {
        
        if (current_position == identicon_pixel_length * identicon_pixel_length) {
            adjuted_binary_string_rep[current_position] = '\0'; //end of the char array marker
        }
        if (current_position < identicon_pixel_length * identicon_pixel_length - original_bin_len) {
            adjuted_binary_string_rep[current_position] = '0';
        } else {
            adjuted_binary_string_rep[current_position] = binary_identicon_string_rep[current_bin_position];
            current_bin_position++;
        }
    }
    
    strcpy(binary_identicon_string_rep, adjuted_binary_string_rep);
}

/*
 *  Function: generate_identicon
 *  ------------------------------------------------
 *  fill identicon (2-d array) indeces based on the binary represention of given
 *  base 10 identicon id
 *      identicon: pointer to 2-d array representing empty identicon
 *      dec_identicon_id: decimal representation of identicon_id
 *
 *  returns: nothing
 */
void generate_identicon(int identicon [identicon_pixel_length] [identicon_pixel_length], int dec_identicon_id) {
    
    //convert to binary
    int binary_identicon_id = decimal_to_binary (dec_identicon_id);

    
    //initialize binary string and fill ending zeros if neeeded
    char binary_identicon_string_rep[identicon_pixel_length * identicon_pixel_length + 1];
    sprintf(binary_identicon_string_rep, "%d", binary_identicon_id);
    post_fill_binary_identicon_string(binary_identicon_string_rep);
    
    int current_id_pos = 0;
    
    //fill identicon based on binary string
    for (int row = 0; row < identicon_pixel_length; row++) {
        
        for (int col = 0; col < identicon_pixel_length; col++) {
            
            int c = binary_identicon_string_rep[current_id_pos];
            identicon[row][col] = c == unicode_zero ? 0 : 1; //converting unicode char to integer
            
            current_id_pos++;
        }
    }
}

/*
 *  Function: print_identicon
 *  ------------------------------------------------
 *  print the contents of an identicon
 *      identicon: pointer to 2-d array representing empty identicon
 *
 *  returns: nothing
 */
void print_identicon(int identicon[identicon_pixel_length][identicon_pixel_length])
{
    for (int r = 0; r < identicon_pixel_length; r++)
    {
        for (int c = 0; c < identicon_pixel_length; c++)
        {
            printf("%d", identicon[r][c]);
        }
        printf("\n");
    }
    printf("\n");

}

int main(int argc, const char * argv[]) {
   
    int possible_identicons = pow(2,identicon_pixel_length * identicon_pixel_length);
    
    for(int current_dec_id = 0; current_dec_id < possible_identicons; current_dec_id++) {
        
        int current_identicon [identicon_pixel_length] [identicon_pixel_length];
        generate_identicon(current_identicon, current_dec_id);
        //print_identicon(current_identicon);
                
    }

    //file creation
    FILE *file;
    char *buffer;
    size_t fileLen;
    char *filename = "/Users/paul2/Desktop/pattern.tiff";

    //open file
    file = fopen(filename, "rb");
    if (!file)
    {
        fprintf(stderr, "Unable to open file %s", filename);
        return 1;
    }

    //get file length
    fseek(file, 0, SEEK_END);
    fileLen=ftell(file);
    fseek(file, 0, SEEK_SET);

    //allocate memory
    buffer=(char *)malloc(fileLen);
    if (!buffer)
    {
        fprintf(stderr, "memory error!");
        fclose(file);
        return 1;
    }
    
    //read the file into buffer
    fread(buffer,fileLen,sizeof(unsigned char),file);
    fclose(file);

    int current_pos = 8;
    
    int pixel_width = 518;
    int pixel_height = 604;
    
    int identicon_count = 0;
    
    while (current_pos < fileLen && current_pos < pixel_height * pixel_width) {
        
        int potential_identicon [identicon_pixel_length] [identicon_pixel_length];
        
        
        //only do so for the top left of every identicon
        if (current_pos % identicon_pixel_length == 0 && (current_pos / pixel_width) % identicon_pixel_length == 0) {
            
            //fill up the potential identicon
            for (int row = 0; row < identicon_pixel_length; row++) {
                
                for (int col = 0; col < identicon_pixel_length; col++) {
                    
                    int current_position = current_pos + (row) + (col * pixel_width); //adding the row and column components independently
                    
                    unsigned char c = (unsigned char) buffer[current_position];
                    
                    int i = (int) c; //converting char to integer
                    
                    potential_identicon[row][col] = i == 255 ? 1 : 0;
                }
            }
            
            identicon_count++;
        }
        
        current_pos = current_pos + 1;
    }
    
    //print out the results of the compression
    printf("Using an identicon pixel length of %d, %d bytes of tiff image read compressed into %d bytes. \n", identicon_pixel_length, current_pos, identicon_count);
}
