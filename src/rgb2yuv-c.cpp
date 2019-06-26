#include <ctype.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

using namespace std;

int menu(int argc, char **argv, char **RGB, char **YUV, int *exit){
    int authors = 0;
    int help = 0;
    *RGB = NULL;
    *YUV = NULL;
    *exit = 0;

    int opt;

    while ((opt = getopt(argc, argv, "i:o:ha")) != -1) {

        switch (opt) {
            case 'h':
                help = 1;
                break;
            case 'a':
                authors = 1;
                break;
            case 'i':
                *RGB = optarg;
                break;
            case 'o':
                *YUV = optarg;
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }
    }

    printf("      _____   _____ ____     _         __     ___    ___      __ \n ");
    printf("    |  __ \\ / ____|  _ \\   | |        \\ \\   / / |  | \\ \\    / / \n ");
    printf("    | |__) | |  __| |_) |  | |_ ___    \\ \\_/ /| |  | |\\ \\  / /  \n ");
    printf("    |  _  /| | |_ |  _ <   | __/ _ \\    \\   / | |  | | \\ \\/ /   \n ");
    printf("    | | \\ \\| |__| | |_) |  | || (_) |    | |  | |__| |  \\  /    \n ");
    printf("    |_|  \\_\\\\_____|____/    \\__\\___/     |_|   \\____/    \\/     \n\n\n" );

    if (authors) {
        printf("Authors: Boris Altamirano - Daniel Jimenez - Jose Andres Pacheco\n\n");
        *exit = 1;
    } else if (help == 1) {
        printf("RGB to YUV images files converter.\n\n");
        printf("Options: \n\t -a \t\t Displays authors names. \n\t -h \t\t Displays this menu. \n");
        printf("\t -i 'RGBFile' \t Specifies the RGB file to be converted. \n\t -o 'YUVFile' \t Specifies the output YUV file name.\n\n");
        *exit = 1;
    } else {
        if (*RGB != NULL) {
            printf("Using RGB File: %s.\n",*RGB);
        } else {
            printf("No RGB File specified.\n\n");
            return 1;
        }

        if (*YUV != NULL) {
            printf("Writing output YUV file name as: %s.\n\n",*YUV);
        } else {
            printf("No output YUV File name specified.\n\n");
            return 1;
        }
    }
    return 0;
}

char* convert_pixel(char buf[3]) {

    float buf2[3];
    float result_float[3];
    static char result_pixel [3];

    //Casting signed char to float
    for (int i=0; i<3; i++){
        buf2[i] = float(static_cast<unsigned char>(buf[i]));
    }

    //Doing convertion rgb_888 to yuv_444
    result_float [0] =  (0.257 * buf2[0]) + (0.504 * buf2[1]) + (0.098 * buf2[2]) + 16;
    result_float [1] = -(0.148 * buf2[0]) - (0.291 * buf2[1]) + (0.439 * buf2[2]) + 128;
    result_float [2] =  (0.439 * buf2[0]) - (0.368 * buf2[1]) - (0.071 * buf2[2]) + 128;

    //Casting float to signed char
    for (int i=0; i<3; i++){
        result_pixel[i] = char(result_float[i]);
    }

    return result_pixel;
}

void rgb2yuv (char *input_image, char *output_image){
    ifstream in_image;
    ofstream out_image;

    char pixels_yuv[640*480*3];

    in_image.open(input_image, ios::in | ios::binary); //Example image input: "sample_640x480.rgb"
    out_image.open(output_image, ios::out | ios::binary); //Example image output: "sample_639x480.yuv"

    int i = 0;
    char buf_rgb[3];
    char* buf_yuv;

    while (in_image.read(buf_rgb, sizeof(buf_rgb))) {
        buf_yuv = convert_pixel(buf_rgb);

        memcpy(&pixels_yuv[i*3], buf_yuv, sizeof(buf_rgb));

        i++;
    }

    in_image.close();
    out_image.write(pixels_yuv, sizeof(pixels_yuv));
    out_image.close();
}

int main (int argc, char **argv) {

    char *RGB_file_name;
    char *YUV_file_name;
    int exit;

    struct timeval time1;
    struct timeval time2;
    long int elapsed_time;

    if (menu(argc,argv,&RGB_file_name,&YUV_file_name,&exit)){
        return 1;
    }
    if (exit == 1) {
        return 0;
    }

    printf("Executing conversion!!\n");

    gettimeofday(&time1, NULL);
    rgb2yuv(RGB_file_name,YUV_file_name);
    gettimeofday(&time2, NULL);

    elapsed_time = time2.tv_usec - time1.tv_usec;

    printf("Elapsed time: %ld us\n", elapsed_time);

    return 0;
}
