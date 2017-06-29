//#include "energy_ish.h"
#include "image2.h"
#include "match.h"
#include "cmdLine.h"
//#include "match.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//#include <limits>
//#include <cmath>
//#include <ctime>
#include <time.h>

#include "graph.h"


/// Is the color image actually gray?
bool isGray(RGBImage im) {
    const int xsize=imGetXSize(im), ysize=imGetYSize(im);
    for(int y=0; y<ysize; y++)
        for(int x=0; x<xsize; x++)
            if(imRef(im,x,y).c[0] != imRef(im,x,y).c[1] ||
               imRef(im,x,y).c[0] != imRef(im,x,y).c[2])
                return false;
    return true;
}

/// Convert to gray level a color image (extract red channel)
void convert_gray(GeneralImage& im) {
    const int xsize=imGetXSize(im), ysize=imGetYSize(im);
    GrayImage g = (GrayImage)imNew(IMAGE_GRAY, xsize, ysize);
    for(int y=0; y<ysize; y++)
        for(int x=0; x<xsize; x++)
            imRef(g,x,y) = imRef((RGBImage)im,x,y).c[0];
    imFree(im);
    im = (GeneralImage)g;
}

int main(int argc, char **argv)
{
    CmdLine cmd;
    std::string cost, sDisp;
    float lambda=1;
    
    cmd.add( make_option('o', sDisp, "output") );
    cmd.add( make_option('c', cost, "data_cost") );
    cmd.add( make_option('l', lambda, "lambda") );
    //cmd.add( make_option('t', params.edgeThresh, "threshold") );
    cmd.process(argc, argv);
    
    if(argc != 5 && argc != 6) {
        std::cerr << "Usage: " << argv[0] << " [options] "
                  << "im1.png im2.png dMin dMax [dispMap.tif]" << std::endl;
        std::cerr << "General options:" << '\n'
                  << " -o,--output disp.png: scaled disparity map" <<'\n'
                  << "Options for cost:" <<'\n'
                  << " -c,--data_cost dist: L1 or L2" <<'\n'
                  << " -l,--lambda lambda: value of lambda (smoothness)" <<'\n';
        return 1;
    }
	
    int dispMin=0, dispMax=0;
    std::istringstream f(argv[3]), g(argv[4]);
    if(! ((f>>dispMin).eof() && (g>>dispMax).eof())){
        std::cerr << "Error reading dMin or dMax" << std::endl;
        return 1;
    }
    
    GeneralImage im1 = (GeneralImage)imLoad(IMAGE_RGB, argv[1]);
    GeneralImage im2 = (GeneralImage)imLoad(IMAGE_RGB, argv[2]);
    if(!im1 || !im2) {
        std::cerr << "Unable to read image " << argv[im1?2:1] << std::endl;
        return 1;
    }
    bool color=true;
    if(isGray((RGBImage)im1) && isGray((RGBImage)im2)){
        color=false;
        convert_gray(im1);
        convert_gray(im2);
    }
    
    Match m=Match(im1,im2,color);
    
    Match::Parameters p;
    p.dataCost = Match::Parameters::L2;
    p.lambda1 = lambda;
    p.lambda2 = lambda;
    p.edgeThresh = 40;
    
    m.SetParameters(&p);
    m.SetDispRange(dispMin,dispMax);
    m.Run_Ishikawa();
    
    char name_out[255];
	(void)sprintf(name_out,  "%s", "Sol.pgm");
	m.WriteSolution(name_out);
    
    imFree(im1);
    imFree(im2);
	return 0;
}
