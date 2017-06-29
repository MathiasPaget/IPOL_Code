#ifndef MATCH_H
#define MATCH_H

#include "image2.h"
#include "image.h"
#include "energy_ish.h"
#include "graph.h"
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

class Energy;


class Match {
public:
	Match(GeneralImage left, GeneralImage right, bool color=false);
    ~Match();
    
    void SetDispRange(int dMin, int dMax);
    
    /// Parameters of algorithm.
    struct Parameters
    {
        enum {L1,L2} dataCost; ///< Data term
        // Smoothness term
        int edgeThresh; ///< Intensity level diff for 'edge'
        int lambda1; ///< Smoothness cost not across edge
        int lambda2; ///< Smoothness cost across edge (should be <=lambda1)
    };
    
    // Run optimization
    void Run_Ishikawa();
    void SetParameters(Parameters *params);
    void InitSubPixel();
    
    void WriteSolution(char *name_file);
    
    private :
    
	Coord imSizeL, imSizeR; ///< image dimensions
	int originalHeightL; ///< true left image height before possible crop
	GrayImage imLeft, imRight;          ///< original images (if gray)
	RGBImage imColorLeft, imColorRight; ///< original images (if color)
	GrayImage imLeftMin, imLeftMax;     ///< range of gray based on neighbors
	GrayImage imRightMin, imRightMax;   ///< range of gray based on neighbors
	RGBImage imColorLeftMin, imColorLeftMax; ///< For color images
	RGBImage imColorRightMin, imColorRightMax;
	int dispMin, dispMax; ///< range of disparities
	
	static const int OCCLUDED; ///< Special value of disparity meaning occlusion
	/// If (p,q) is an active assignment
	/// q == p + Coord(IMREF(d_left,  p), p.y)
	/// p == q + Coord(IMREF(d_right, q), q.y)
	//IntImage  d_left, d_right;
	Parameters  params; ///< Set of parameters    

	// Data penalty functions
	int  data_penalty_gray (Coord l, Coord r) const;
	int  data_penalty_color(Coord l, Coord r) const;
	
	int  data_penalty_color2(Coord l, Coord r) const;
	
	// Smoothness penalty functions
	int  smoothness_penalty_gray (Coord p, Coord np, int d) const;
	int  smoothness_penalty_color(Coord p, Coord np, int d) const;
	
	int  smoothness_penalty_color(Coord p, Coord np) const;
    
    int dimg;
    short *Solution;
};
#endif
