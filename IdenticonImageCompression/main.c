//
//  main.c
//  IdenticonImageCompression
//
//  Created by Paul Touma on 8/15/17.
//  Copyright © 2017 Paul Touma. All rights reserved.
//

#include <math.h>
#include <stdio.h>
#include <string.h>

const int identicon_pixel_length = 3;
const int unicode_zero = 48;

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

//fill with ending zeros
void post_fill_binary_identicon_string (char binary_identicon_string_rep [identicon_pixel_length * identicon_pixel_length + 1]) {
    
    size_t length = strlen(binary_identicon_string_rep);
    
    int current_position = (int)((ssize_t)length);
    while (current_position < identicon_pixel_length * identicon_pixel_length) {
        binary_identicon_string_rep[current_position] = '0';
        current_position++;
    }
}

void generate_identicon(int identicon [identicon_pixel_length] [identicon_pixel_length], int dec_identicon_id) {
    
    int binary_identicon_id = decimal_to_binary (dec_identicon_id);
    
    char binary_identicon_string_rep[identicon_pixel_length * identicon_pixel_length + 1];
    sprintf(binary_identicon_string_rep, "%d", binary_identicon_id);
    
    post_fill_binary_identicon_string(binary_identicon_string_rep);
    
    int current_id_pos = 0;
    
    for (int row = 0; row < identicon_pixel_length; row++) {
        
        for (int col = 0; col < identicon_pixel_length; col++) {
            
            int c = binary_identicon_string_rep[current_id_pos];
            identicon[row][col] = c == unicode_zero ? 0 : 1; //converting unicode char to integer
            
            current_id_pos++;
        }
    }
}

void print_identicon(int arr[identicon_pixel_length][identicon_pixel_length])
{
    for (int r = 0; r < identicon_pixel_length; r++)
    {
        for (int c = 0; c < identicon_pixel_length; c++)
        {
            printf("%d", arr[r][c]);
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
        print_identicon(current_identicon);
        
    }
    
    return 0;
}
