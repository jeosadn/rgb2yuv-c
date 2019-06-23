#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

char* convert(char buf[3]) {

   float buf2[3];
   float result_float[3];	
   static char result_pixel [3];
 
   //Casting signed char to float
   for (int i=0; i<3; i++){
      buf2[i] = float(static_cast<unsigned char>(buf[i]));
      //Scaling to 0..1 values
      buf2[i] = buf2[i]/255;
   }
 
   //Doing convertion rgb_888 to yuv_444
   result_float [0] =  (0.257 * buf2[0]) + (0.504 * buf2[1]) + (0.098 * buf2[2]) + 16;
   result_float [1] = -(0.148 * buf2[0]) - (0.291 * buf2[1]) + (0.439 * buf2[2]) + 128;
   result_float [2] =  (0.439 * buf2[0]) - (0.368 * buf2[1]) - (0.071 * buf2[2]) + 128;

   //Casting float to signed char
   for (int i=0; i<3; i++){
      //Scaling again to 0..255 values
      result_float[i] = result_float[i]*255;
      result_pixel[i] = char(result_float[i]);
   } 

   //printf("CHAR1: %d %d %d\n", buf[0], buf[1], buf[2]);
   //cout << "INT: " << buf2[0] << " " << buf2[1] << " " << buf2[2] << endl;
   //printf("CHAR2: %d %d %d\n", result_pixel[0], result_pixel[1], result_pixel[2]);
  
   return result_pixel; 
}

int main() {
   ifstream image;
   ofstream output_image;

   char pixels_rgb[640*480*3];
   char pixels_yuv[640*480*3];

   printf("Executing convertion!!\n");
   
   image.open("sample_640x480.rgb", ios::in | ios::binary);
   output_image.open("sample_640x480.yuv", ios::out | ios::binary);

   int i = 0;
   char buf_rgb[3];
   char* buf_yuv;

   while (image.read(buf_rgb, sizeof(buf_rgb)))
    {
	buf_yuv = convert(buf_rgb);
	
	memcpy(&pixels_rgb[i*3], buf_rgb, sizeof(buf_rgb));
	memcpy(&pixels_yuv[i*3], buf_yuv, sizeof(buf_rgb));

	i++;
    }

   image.close();
   output_image.write(pixels_yuv, sizeof(pixels_yuv)); 
   output_image.close();

   return 0;
}
