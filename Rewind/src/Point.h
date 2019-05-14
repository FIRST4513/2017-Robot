/*
 * Point.h
 *
 *  Created on: Nov 5, 2017
 *      Author: student
 */

#ifndef SRC_POINT_H_
#define SRC_POINT_H_

#include "Math.h"

double pi=3.14159265;

class Point {
public:
	// --- Point Data Elements ---
	double x, y;

	//---- Point Methods -----
	Point(double x1, double y1);					// constructor with params
	Point();										// constructor with no params
	void set(double x1, double y1);
	void set(Point pt);
    double dist(Point pt);							// Calculate distance between two points
    void reset();									// Reset x&y to zero

    Point cartToPolar();
    Point cartToPolar(Point origpt);
    Point polarToCart(double radius, double angle);				// Takes in a radius and angle and returns
																// a cartesian point relative to Point (0,0)

    Point polarToCart(double radius, double angle, Point pt);	// Takes in a radius and angle and returns
    															// a cartesian point relative to Point pt

    bool eq(Point pt);								// Test for equality of 2 points
    bool neq(Point pt);								// Test for not equal of 2 points
    bool operator==(Point pt);
    bool operator!=(Point pt);
    void operator=(Point pt);
    Point operator+(Point pt);
	virtual ~Point();
};


// *************************** Segment Class ****************************
class Segment {
public:
	// --- Segment Data Elements ---
	char type;				// 'N' not active, 'A' Arc,  'S' straight line
    Point startPt;
    Point ctrPt;
    Point endPt;
    double len;
    double radius;
    int dir;                // -1 = counter clockwise, +1 = clokwise
    double startAngle;
    double endAngle;

	//---- Segment Methods -----
    Segment();
    Segment(char type1, Point startPt1, Point endPt1);
    void arc(Point startPt1, Point endPt1, Point ctrPt1, double radius1, int dir1, double len1);
    void strt(Point startPt1, Point endPt1);
    double calcArcAngle(double startAngle, Point ctrPt, Point pt1, Point pt2, double dir);
    void operator=(Segment seg1);
};


// *************************** Route Class ****************************
class Route {
public:
	// --- Route Data Elements ---
	int segCnt;				// Count of active segments
    Segment seg[5];			// Segments Straight or Arc

	//---- Route Methods -----
    Route();
    void setSeg(int i, Segment seg1);
    Segment getSeg(int i);
    void clearSeg(int i);
    void clearAllSeg();
    int append(Segment seg1);
    void operator=(Route rt1);
};


// *************************** Rbt Class ****************************
class Rbt {
public:
	// --- Rbt Data Elements ---
	Point COG;
	double hdg;
	double yaw;
	double angle;
	Point pos;
	Point startPt;
	Point endPt;
	Point xAxisIntPt;
	double distx, disty;
	Segment snglArc;
	Segment dblArc1;
	Segment dblArc2;
	Point ctrPt1;
	Point ctrPt2;
	double radius;
	int arcDir1;
	int arcDir2;
	int ctr1sign;
	int ctr2xsign;
	int ctr2ysign;
	Route rbtRoute;

	Rbt();

};


// ***************************  Hdg Functions ****************************
double addAngleToHdg(double hdg1, double angle);	// Returns new Hdg (0-360) from sum of hdg & angle
double convHdgToAngle(double hdg);					// Returns Angle
double convAngleToHdg(double angle);				// Returns Hdg (0->360) from any angle
double convHdgToYaw(double Hdg);					// Returns Yaw (+ -) 180 from hdg (0->360)
double convYawToHdg(double yaw);					// Returns Hdg (0->360) from (+ -) angle
double calcHdgDiff(double hdg1, double hdg2);		// Returns Hdg with subracted angle (0-360)
double calcHdgSum(double hdg1, double hdg2);		// Returns Hdg with added angle (0-360)

// *************************** Math Functions ****************************

const double DEGREETORADIANCONST = 0.0174533;			// Degrees * Constant = Radians
const double RADIANSTODEGREECONST = 57.295791;			// Constant / Radians = Degrees

double mRound(double nmbr, int digits);					// Returns rounded nmbr to no of digits
int    mGetQuad(double angle);								// Returns Quad 1-4 from any angle
double mQuad(double a, double b, double c);				// Calculates the quadratic formula

double cnvrtDegToRad(double degree);					// Calculates radians from degrees
double cnvrtRadToDeg(double radian);					// Calculates degrees from radians

double mCos(double angle);								// Calculates the cos of an angle
double mSin(double angle);								// Calculates the sin of an angle
double mTan(double angle);								// Calculates the tan of an angle
double mAtan(double opo, double adj);					// Calculates the Angle of triag.
														// from the known opposite and adjacent side


#endif /* SRC_POINT_H_ */
