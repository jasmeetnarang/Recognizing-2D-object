// Class for representing a 2D gray-scale image,
// with support for reading/writing pgm images.
// To be used in Computer Vision class.

#include "image.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>

struct record {
    int label;
    float rowX;
    float colY;
    float E;
    float orientation;
    int lasti;
    int lastj;
};

using namespace std;

namespace ComputerVisionProjects {

    Image::Image(const Image &an_image){
        AllocateSpaceAndSetSize(an_image.num_rows(), an_image.num_columns());
        SetNumberGrayLevels(an_image.num_gray_levels());

        for (size_t i = 0; i < num_rows(); ++i)
            for (size_t j = 0; j < num_columns(); ++j){
                SetPixel(i,j, an_image.GetPixel(i,j));
            }
    }

    Image::~Image(){
        DeallocateSpace();
    }

    void Image::AllocateSpaceAndSetSize(size_t num_rows, size_t num_columns) {
        if (pixels_ != nullptr) DeallocateSpace();
        pixels_ = new int*[num_rows];
        for (size_t i = 0; i < num_rows; ++i)
            pixels_[i] = new int[num_columns];

        num_rows_ = num_rows;
        num_columns_ = num_columns;
    }

    void Image::DeallocateSpace() {
        for (size_t i = 0; i < num_rows_; i++)
            delete pixels_[i];
        delete pixels_;
        pixels_ = nullptr;
        num_rows_ = 0;
        num_columns_ = 0;
    }

    bool ReadImage(const string &filename, Image *an_image) {
        if (an_image == nullptr) abort();
        FILE *input = fopen(filename.c_str(),"rb");
        if (input == 0) {
            cout << "ReadImage: Cannot open file" << endl;
            return false;
        }

        // Check for the right "magic number".
        char line[1024];
        if (fread(line, 1, 3, input) != 3 || strncmp(line,"P5\n",3)) {
            fclose(input);
            cout << "ReadImage: Expected .pgm file" << endl;
            return false;
        }

        // Skip comments.
        do
            fgets(line, sizeof line, input);
        while(*line == '#');

        // Read the width and height.
        int num_columns,num_rows;
        sscanf(line,"%d %d\n", &num_columns, &num_rows);
        an_image->AllocateSpaceAndSetSize(num_rows, num_columns);


        // Read # of gray levels.
        fgets(line, sizeof line, input);
        int levels;
        sscanf(line,"%d\n", &levels);
        an_image->SetNumberGrayLevels(levels);

        // read pixel row by row.
        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0;j < num_columns; ++j) {
                const int byte=fgetc(input);
                if (byte == EOF) {
                    fclose(input);
                    cout << "ReadImage: short file" << endl;
                    return false;
                }
                an_image->SetPixel(i, j, byte);
            }
        }

        fclose(input);
        return true;
    }

    bool WriteImage(const string &filename, const Image &an_image) {
        FILE *output = fopen(filename.c_str(), "w");
        if (output == 0) {
            cout << "WriteImage: cannot open file" << endl;
            return false;
        }
        const int num_rows = an_image.num_rows();
        const int num_columns = an_image.num_columns();
        const int colors = an_image.num_gray_levels();

        // Write the header.
        fprintf(output, "P5\n"); // Magic number.
        fprintf(output, "#\n");  // Empty comment.
        fprintf(output, "%d %d\n%03d\n", num_columns, num_rows, colors);

        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0; j < num_columns; ++j) {
                const int byte = an_image.GetPixel(i , j);
                if (fputc(byte,output) == EOF) {
                    fclose(output);
                    cout << "WriteImage: could not write" << endl;
                    return false;
                }
            }
        }

        fclose(output);
        return true;
    }

//Coverts the gray scale image to a binay image
//given a threshold value

//    void printImageInfo(Image *an_image){
//        const int num_rows = an_image->num_rows();
//        const int num_columns = an_image->num_columns();
//        const int num_gray_levels =an_image->num_gray_levels();
//
//        if(an_image == nullptr) abort();
//
//        int binsSize = 1;
//
//        if(num_gray_levels > 0){
//            binsSize =num_gray_levels+1;
//        }
//
//
//        int bins[binsSize] ={0};
//        int min =binsSize;
//        int max = -1;
//
//        for (int i = 0; i < num_rows; ++i) {
//            for (int j = 0;j < num_columns; ++j) {
//                int pixelVal = an_image->GetPixel(i,j);
//                cout<<pixelVal<<" ";
//
//                 if(pixelVal<min) {
//                     min = pixelVal;
//                 }
//                 if(pixelVal>max) {
//                     max = pixelVal;
//                 }
//
//                bins[pixelVal]++;
//
//            }
//            cout<<endl;
//        }
//
//        cout<<"Bins : "<<endl;
//
//        for(int i=0;i<binsSize;i++){
//            cout<<i<<" - "<<bins[i]<<endl;
//        }
//
//
//        cout<<"Min : "<<min<<endl;
//        cout<<"Max : "<<max<<endl;
//
//    }




    void p1(Image *an_image, int T){
        // int num_columns,num_rows;

        const int num_rows = an_image->num_rows();
        const int num_columns = an_image->num_columns();

        if(an_image == nullptr) abort();

        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0;j < num_columns; ++j) {
                int pixelVal = an_image->GetPixel(i,j);
                if(pixelVal < T) an_image->SetPixel(i,j,0);
                else an_image->SetPixel(i,j,1);
            }
        }

        an_image->SetNumberGrayLevels(1);
    }

    //sequential labeling equivalence case method
    void saveEquivalence(vector<set<int>>& equivalenceMap, int label1, int label2){
        if(label1==label2) {
            return;
        }

        int found = -1;

        vector<int> groups2Merge;

        for(int i =0;i<equivalenceMap.size();i++) {
            set<int> groupSet = equivalenceMap.at(i);
            if(groupSet.find(label1)!=groupSet.end()){
                groupSet.insert(label2);
                equivalenceMap.at(i) = groupSet;
                if(found==-1) {
                    found = i;
                }else{
                    groups2Merge.push_back(i);
                }
            } else if(groupSet.find(label2)!=groupSet.end()) {
                groupSet.insert(label1);
                equivalenceMap.at(i) = groupSet;
                if(found==-1) {
                    found = i;
                }else{
                    groups2Merge.push_back(i);
                }
            }
        }

        if(found==-1){
            set<int> newSet;
            newSet.insert(label1);
            newSet.insert(label2);
            equivalenceMap.push_back(newSet);
        } else {
            if(!groups2Merge.empty()){
                set<int> foundSet = equivalenceMap.at(found);
                for(int k=groups2Merge.size()-1;k>-1;k--){
                    int indexOfGroup2merge = groups2Merge.at(k);
                    set<int> set2merge = equivalenceMap.at(indexOfGroup2merge);
                    foundSet.insert(set2merge.begin(),set2merge.end());
                    equivalenceMap.erase(equivalenceMap.begin()+indexOfGroup2merge);
                }
                equivalenceMap.at(found) = foundSet;
            }
        }

    }

    int getEquivClass(std::vector<std::set<int>> equivalenceMap, int label){
        for(int i =0;i<equivalenceMap.size();i++) {
            set<int> groupSet = equivalenceMap.at(i);
            if(groupSet.find(label)!=groupSet.end()){
                return i+1;
            }
        }
    }

//    void printEquivClassInfo(std::vector<std::set<int>> equivalenceMap){
//        int max = equivalenceMap.at(0).size();
//        int min = max;
//
//        for(int i=0;i<equivalenceMap.size();i++){
//            set<int> groupSet = equivalenceMap.at(i);
//            if(groupSet.size()>max) max = groupSet.size();
//            if(groupSet.size()<min) min = groupSet.size();
//        }
//
//        cout<<"Max members : "<<max<<endl;
//        cout<<"Min members : "<<min<<endl;
//
//    }

    void p2(Image *an_image){

        const int num_rows = an_image->num_rows();
        const int num_columns = an_image->num_columns();

        vector<set<int>> equivalenceMap;

        if(an_image == nullptr) abort();
        int k =1;
        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0;j < num_columns; ++j) {
                int pixelVal = an_image->GetPixel(i,j);

                int upPixelVal = 0;
                if(i>0){
                    upPixelVal = an_image->GetPixel(i-1,j);
                }

                int leftPixelVal = 0;
                if(j>0){
                    leftPixelVal = an_image->GetPixel(i,j-1);
                }

                if(pixelVal == 1) {
                    if(upPixelVal >= 1 && leftPixelVal==0){
                        an_image->SetPixel(i,j,upPixelVal);
                    }
                    else if(upPixelVal == 0&& leftPixelVal >=1 ){
                        an_image->SetPixel(i,j,leftPixelVal);
                    } else if (upPixelVal >= 1 && leftPixelVal >=1 ){
                        saveEquivalence(equivalenceMap,leftPixelVal,upPixelVal);
                        an_image->SetPixel(i,j,leftPixelVal);
                    } else {
                        an_image->SetPixel(i,j,k);
                        k++;
                    }
                }
            }
        }


        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0;j < num_columns; ++j) {
                int pixelVal = an_image->GetPixel(i,j);

                if(pixelVal >= 1) {
                    an_image->SetPixel(i,j,getEquivClass(equivalenceMap,pixelVal));
                }
            }
        }

        //printEquivClassInfo(equivalenceMap);

        an_image->SetNumberGrayLevels(equivalenceMap.size());
    }


    vector<record> getObjParams(Image *an_image) {
        const int num_rows = an_image->num_rows();
        const int num_columns = an_image->num_columns();

        float positionX =0.0, positionY =0.0, E=0.0,orientation =0.0;
        int numberofObjects = 0;
        numberofObjects = an_image->num_gray_levels();

        vector<record> records;

        for(int k=1;k<=numberofObjects;k++) {
            float Area = 0.0;
            float iVal = 0.0, jVal = 0.0;
            int momenta = 0, momentb = 0, momentc = 0;

            int lasti=0,lastj=0;

            for (int i = 0; i < num_rows; ++i) {
                for (int j = 0; j < num_columns; ++j) {
                    int pixelVal = an_image->GetPixel(i, j);

                    if (pixelVal == k) {
                        Area = pixelVal + Area;
                        iVal = iVal + (i * pixelVal);
                        jVal = jVal + (j * pixelVal);
                        momenta = momenta + (i * i * 1);
                        momentb = momentb + (i * j * 1);
                        momentc = momentc + (j * j * 1);
                        lasti = i;
                        lastj = j;
                    }

                }
            }

            float fract = 1 / Area;

            positionX = fract * iVal;
            positionY = fract * jVal;

            momentb = 2 * momentb;

            orientation = (atan((float) momentb / ((float) momenta - (float) momentc)) / 2);

            float sin_theta = sin(orientation);
            float cos_theta = cos(orientation);

            float parta = (float) momenta * sin_theta * sin_theta;
            float partb = (float) momentb * sin_theta * cos_theta;
            float partc = (float) momentc * cos_theta * cos_theta;

            E = parta - partb + partc;

            record record1{};

            record1.label = k;
            record1.rowX = positionX;
            record1.colY = positionY;
            record1.E = E;
            record1.orientation = orientation;
            record1.lasti = lasti;
            record1.lastj = lastj;
            records.push_back(record1);
        };

        return records;
    }

    void p3(Image *an_image, string database_file){
        const int num_rows = an_image->num_rows();
        const int num_columns = an_image->num_columns();
        ofstream out;

        float positionX =0.0, positionY =0.0, E=0.0,orientation =0.0;
        out.open(database_file);
        int numberofObjects = 0;
        int lasti,lastj;
        numberofObjects = an_image->num_gray_levels();

        vector<record> records = getObjParams(an_image);

        for(int i=0;i<records.size();i++) {
            record record1 = records.at(i);

            string space = " ";

            out<<record1.label<<space<<record1.rowX<<space<<record1.colY<<space<<record1.E<<space<<record1.orientation<<endl;
            DrawLine(record1.rowX,record1.colY,record1.lasti,record1.lastj,200,
                     an_image);
        }
        out.close();

    }

    void p4(Image *an_image, string input_file) {
        ifstream in;

        vector<record> prevRecords;

        in.open(input_file);
        if (in.is_open()) {
            while (!in.eof()) {

                record record1{};
                in >> record1.label;

                if(in.eof()){
                    break;
                }

                in >> record1.rowX;
                in >> record1.colY;
                in >> record1.E;
                in >> record1.orientation;
                prevRecords.push_back(record1);
            }

            in.close();
        }

        vector<record> records = getObjParams(an_image);

        for(int i =0;i<prevRecords.size();i++){
            for (int j = 0; j < records.size(); j++) {
                record prevRecord = prevRecords.at(i);
                record record = records.at(j);
                float differenceOfRow = abs(prevRecord.rowX - record.rowX);
                float differenceOfcolumn = abs(prevRecord.colY - record.colY);
                float differenceOfE = abs(prevRecord.E - record.E);
                float differenceofOrientation = abs(prevRecord.orientation - record.orientation);
                cout<<"For label "<<prevRecord.label<<" "<<record.label<<endl;
                cout<<"Diff of X "<<differenceOfRow<<"\n"<<"Diff of Y "<<differenceOfcolumn<<"\n";
                cout<<"Diff of Inertia "<<differenceOfE<<"\n"<<"Diff of Orientation "<<differenceofOrientation<<endl<<endl;

            }
        }

    }
//    void extracting_line(std::string input_file){
//        ifstream in;
//        string text_line;
//
//        in.open(input_file);
//        if(in.is_open()){
//            while(!in.eof()){
//                getline(in,text_line);
//                extract_text(text_line);
//            }
//        }
//
//    }
//
//    void extract_text(std::string text_line){
//       int length = text_line.length();
//       const char space = ' ';
//
//       char label = text_line[0];
//       cout<<label<<endl;
//
//
//
//
////       for(int i =0;i<length;i++){
////           if(text_line[i] == ' ')
////               space = text_line[i];
////       }
//

// Implements the Bresenham's incremental midpoint algorithm;
// (adapted from J.D.Foley, A. van Dam, S.K.Feiner, J.F.Hughes
// "Computer Graphics. Principles and practice", 
// 2nd ed., 1990, section 3.2.2);  
    void DrawLine(int x0, int y0, int x1, int y1, int color,
                  Image *an_image) {
        if (an_image == nullptr) abort();

#ifdef SWAP
#undef SWAP
#endif
#define SWAP(a,b) {a^=b; b^=a; a^=b;}

        const int DIR_X = 0;
        const int DIR_Y = 1;

        // Increments: East, North-East, South, South-East, North.
        int incrE,
                incrNE,
                incrS,
                incrSE,
                incrN;
        int d;         /* the D */
        int x,y;       /* running coordinates */
        int mpCase;    /* midpoint algorithm's case */
        int done;      /* set to 1 when done */

        int xmin = x0;
        int xmax = x1;
        int ymin = y0;
        int ymax = y1;

        int dx = xmax - xmin;
        int dy = ymax - ymin;
        int dir;

        if (dx * dx > dy * dy) {  // Horizontal scan.
            dir=DIR_X;
            if (xmax < xmin) {
                SWAP(xmin, xmax);
                SWAP(ymin , ymax);
            }
            dx = xmax - xmin;
            dy = ymax - ymin;

            if (dy >= 0) {
                mpCase = 1;
                d = 2 * dy - dx;
            } else {
                mpCase = 2;
                d = 2 * dy + dx;
            }

            incrNE = 2 * (dy - dx);
            incrE = 2 * dy;
            incrSE = 2 * (dy + dx);
        } else {// vertical scan.
            dir = DIR_Y;
            if (ymax < ymin) {
                SWAP(xmin, xmax);
                SWAP(ymin, ymax);
            }
            dx = xmax - xmin;
            dy = ymax-ymin;

            if (dx >=0 ) {
                mpCase = 1;
                d = 2 * dx - dy;
            } else {
                mpCase = 2;
                d = 2 * dx + dy;
            }

            incrNE = 2 * (dx - dy);
            incrE = 2 * dx;
            incrSE = 2 * (dx + dy);
        }

        /// Start the scan.
        x = xmin;
        y = ymin;
        done = 0;

        while (!done) {
            an_image->SetPixel(x,y,color);

            // Move to the next point.
            switch(dir) {
                case DIR_X:
                    if (x < xmax) {
                        switch(mpCase) {
                            case 1:
                                if (d <= 0) {
                                    d += incrE;
                                    x++;
                                } else {
                                    d += incrNE;
                                    x++;
                                    y++;
                                }
                                break;

                            case 2:
                                if (d <= 0) {
                                    d += incrSE;
                                    x++;
                                    y--;
                                } else {
                                    d += incrE;
                                    x++;
                                }
                                break;
                        }
                    } else {
                        done=1;
                    }
                    break;

                case DIR_Y:
                    if (y < ymax) {
                        switch(mpCase) {
                            case 1:
                                if (d <= 0) {
                                    d += incrE;
                                    y++;
                                } else {
                                    d += incrNE;
                                    y++;
                                    x++;
                                }
                                break;

                            case 2:
                                if (d <= 0) {
                                    d += incrSE;
                                    y++;
                                    x--;
                                } else {
                                    d += incrE;
                                    y++;
                                }
                                break;
                        } // mpCase
                    } // y < ymin
                    else {
                        done=1;
                    }
                    break;
            }
        }
    }

}  // namespace ComputerVisionProjects







