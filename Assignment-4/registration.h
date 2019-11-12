//
//  registration.h
//  Assignment_4
//
//  Created by Mayra Sanchez on 10/29/19.
//  Copyright © 2019 Mayra Sanchez. All rights reserved.
//

#ifndef registration_h
#define registration_h

/* Header file to be used with
 * shmp1.c and shmc1.c
 */

/*
 created the structure CLASS that holds info such as the class number, date, title
 and most importantly the amount of seats left
 
 Sets the premise for which the other classes will be basing their processes off of
 */
struct CLASS {
    char class_number[6];
    char date[7];
    char title[50];
    int seats_left;
};


#endif /* registration_h */
