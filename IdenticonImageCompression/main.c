//
//  main.c
//  IdenticonImageCompression
//
//  Created by Paul Touma on 8/15/17.
//  Copyright © 2017 Paul Touma. All rights reserved.
//

#include <math.h>
#include <stdio.h>

const int identicon_pixel_length = 3;

int main(int argc, const char * argv[]) {
   
    int possible_identicons = pow(2,identicon_pixel_length);
    
    for(int i = 0; i < possible_identicons; i++) {
        generate_identicons(i);
    }
    
}

int (*generate_identicons(int identicon_id))[identicon_pixel_length][identicon_pixel_length] {

    for (int i = 0; i < identicon_pixel_length; i++) {
        
        for (int j = 0; j < identicon_pixel_length; j++) {
            
        }
    }
}




