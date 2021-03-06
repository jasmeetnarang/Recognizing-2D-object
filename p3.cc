// Sample test file for the image class.
// Reads a given pgm image, draws a line, and saves it to
// another pgm image.
#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;
using namespace ComputerVisionProjects;

int
main(int argc, char **argv){

    if (argc!=4) {
        printf("Usage: %s file1 file2 file3\n", argv[0]);
        return 0;
    }
    const string input_file(argv[1]);
    const string attributes_file(argv[2]);
    const string output_file(argv[3]);


    Image an_image;
    if (!ReadImage(input_file, &an_image)) {
        cout <<"Can't open file " << input_file << endl;
        return 0;
    }

    // Draw a line from (0, 0) to (100, 240) having gray-value
    // 200.
    //DrawLine(0, 0, 100, 240, 200,
//	   &an_image);
    //p2(&an_image);
    p3(&an_image, attributes_file);
//    if(!p3(&an_image, attributes_file)){
//        cout<<"Can't open file "<<attributes_file<<endl;
//        return 0;
//    }
    //printImageInfo(&an_image);

    if (!WriteImage(output_file, an_image)){
        cout << "Can't write to file " << output_file << endl;
        return 0;
    }
}
