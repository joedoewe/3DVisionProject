//============================================================================
// Name        : detectRepPoints.h
// Author      : Ryen Elith
// Version     : 6. April 2016
// Copyright   :
// Description : Headerfile for Class to find repetitive 3D points
//============================================================================
#ifndef DETECTREPPOINTS_H
#define DETECTREPPOINTS_H

#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <fstream>

using namespace std;

class detectRepPoints{

private:

        // generic data info
        int siftFeatureDim;                                                // dimension: 128 for sift

        // point visibility stuff

            // class own container with images each point is seen in
            Eigen::MatrixXf pointsInImage;

            // points to compare against eachother (2 points that can be seen in same image - any image)
            Eigen::MatrixXf pointsToTest;

            // function to get point visibilities in images (reads point file and fills pointsToSift (partly) and pointsInImage)
            int get3DPointVisibility();

            // builds binary matrix pointsToTest with comparisons to execute
            int getPointsToTest();


        // Sift stuff

            // struct to store 3D point information
            struct siftFeatures {
                int pointIndex;                     // point index -> trivial, equal to position of struct in pointsToSift vector
                Eigen::Vector3f pos;                // point position in 3D
                vector<int> imIndex;                // image indexes where point is seen
                vector<Eigen::Vector2f> siftPos;    // position of sift features in corresponding image
            };

            // container storing: lookup point -> sift descriptors
            vector<vector<Eigen::MatrixXf> > siftFeatureVector;

            // container storing: 3d point -> point's information (siftFeature struct)
            vector<struct siftFeatures> pointsToSift;

            // function to calculate angle between two descriptors
            double angleOfTwoSift(Eigen::MatrixXf sift1, Eigen::MatrixXf sift2);

            // function to computes all sift descriptors and fill siftFeatureVector and complete pointsToSift structs (with sift indexes)
            int get3DPointSiftRepresentations();

            // function to compute siftDescriptor of one image using openCV
            int computeSiftDescriptor(int image,Eigen::Vector2f pos,Eigen::MatrixXf &outSingleFeatureVector);

        // grouping stuff

            // group organisation variables
            int highestGroupIdx;                                        // highest group index in use
            vector<int> nextGroupIdx;                                   // next group index to assign for new groups (holds recycled groups)
            double tol_angle;                                           // decision criteria angle for repetitive points
            int countComparisons;                                       // count of comparisons executed to find groups

            // function to compare two 3D points based on their sift descriptors
            int compare3DPoints(int pointIdx1, int pointIdx2);

            // function to add new group for point index
            int addGroup(int pointIdx);

            // update group hierarchy given two compared points: result of comparison: 0->no match, 1->match
            void updateGroups(int pointIdx1, int pointIdx2, int comparisonResult);

            // function to ouput content of group given a group index
            void coutGroupContent(int groupIdx);

            //matrix pointToGroup with 1 to 1 relation: point index -> group index
            vector<int> pointToGroup;

            // 2d vector with 1 to n relation: group index -> point index
            vector<vector<int> > groupToPoints;

public:
        // constructor
        detectRepPoints();
        // destructor
        ~detectRepPoints();

        // number of points and images
        int n_img;
        int n_points;

        // main function to get repetitive points
        int getRepetitivePoints(vector<int> &outPointToGroup, vector<vector<int> > &outGroupToPoints);

        // getter method to get 3d location of given point index
        Eigen::Vector3f get3dFromPointIdx(int pointIndex);

        // function to print group results with some statistics
        int printGroupMembers();
};


#endif // detectRepPoints_H