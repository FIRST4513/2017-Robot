/*
 * Point.cpp
 *
 *  Created on: Nov 5, 2017
 *      Author: student
 */

#include "Point.h"
#include "Math.h"


// ***************************************************
//              Point Class Routines
// ***************************************************
Point::Point(double x1, double y1) {
	// Constructor with initial values
	x = x1;
	y = y1;
}

Point::Point(){
	// constructor with no parameters
	x = 0;
	y = 0;
}

void Point::set(double x1, double y1){
	// Set self equal to pt
	x = x1;
	y = y1;
}

void Point::set(Point pt){
	x = pt.x;
	y = pt.y;
}

double Point::dist(Point pt){
	// return the Euclidian distance between self and pt
    double dx = x - pt.x;
    double dy = y - pt.y;
    return (sqrt(dx*dx + dy*dy));
}

void Point::reset(){
	// Reset x and y to zero
    x = 0;
    y = 0;
}
// *************************** Cartesian To Polar Conversion **********************

Point Point::cartToPolar(){
	// Convert Cart. Coord Cart.Coord(x,y) to Polar Cord Point(radius, angle)
	double x1, y1, radius, angle;
	x1 = x;
	y1 = y;
	radius = sqrt((x1*x1) + (y1*y1));
	angle = mAtan(y1,x1);
	Point retPt = Point(radius, angle);
	return Point(retPt);
}

Point Point::cartToPolar(Point newPt){
	double x1, y1, radius, angle;
	//		r = sqrt( ( x2 + y2 ))		Cartesian to Polar coord conversion
	//		t = tan-1 ( y / x )
	//		x = r × cos( t )		Polar to Cartesian coord conversion
	//		y = r × sin( t )

	x1 = newPt.x - x;
	y1 = newPt.y - y;

    radius = sqrt((x1*x1) + (y1*y1));
    angle = mAtan(y1,x1);
	Point retPt(radius, angle);
	return (retPt);
}

// *************************** Polar to Cartesian Coversion ***********************

Point Point::polarToCart(double radius, double angle){
	// Convert Polar Coord radius, angle to Cart.Coord(x,y)
	double x,y;
	x = radius * mCos(angle);
	y = radius * mSin(angle);
	Point retPt = Point(x,y);
	return (retPt);
}

Point Point::polarToCart(double radius, double angle, Point pt){
	// Takes in a radius and angle and returns a cartesian point relative to Point pt
    double x = radius * mCos(angle);
    double y = radius * mSin(angle);
    Point retPt = Point(pt.x+x, pt.y+y);
    return (retPt);
}




bool Point::operator==(Point pt){
	if ((x == pt.x) && (y == pt.y))
		return true;
	else
		return false;
}

bool Point::operator!=(Point pt){
	// Test for not equal between self and pt
    if ((x != pt.x) || (y != pt.y))
    	return true;
    else
        return false;
}

void Point::operator=(Point pt){
	x = pt.x;
	y = pt.y;
}

Point Point::operator+(Point pt){
	// return a new point found by adding self and p. This method is
    // called by e.g. p+q for points p and q
    return Point(x+pt.x, y+pt.y);
}

Point::~Point() {
	}


// ***************************************************
//              Segment Class Routines
// ***************************************************
Segment::Segment(){
	type = 'N';
    startPt = Point(0,0);
    ctrPt = startPt;
    endPt = startPt;
    len = 0;
    radius = 0;
    dir = 0;
    startAngle = 0;
    endAngle = 0;
}

Segment::Segment(char type1, Point startPt1, Point endPt1){
	type = type1;
    startPt = startPt1;
    ctrPt = Point(0,0);
    endPt = endPt1;
    len = 0;
    radius = 0;
    dir = 0;
    startAngle = 0;
    endAngle = 0;
}

void Segment::arc(Point startPt1, Point endPt1, Point ctrPt1, double radius1, int dir1){
	type = 'A';
    startPt = startPt1;
    ctrPt = ctrPt1;
    endPt = endPt1;
    len = 0;
    radius = radius1;
    dir = dir1;
    startAngle = 0;
    endAngle = 0;
}

void Segment::strt(Point startPt1, Point endPt1){
	type = 'S';
    startPt = startPt1;
    ctrPt = Point(0,0);
    endPt = endPt1;
    len = startPt1.dist(endPt1);
    radius = 0;
    dir = 1;
    startAngle = 0;
    endAngle = 0;
}

/*
// ********** Calculate angle/length between two arcs points Function **************
double Segment::calcArcAngle(double startAngle, Point ctrPt, Point pt1, Point pt2, double dir){
    // dir > 0 means Counter clockwise so we need to swap the points PT2 to PT1
    // Arc is going right and up down and left
    if (dir > 0){
        tRadius, tAngle1 = CartesianToPolar(ctrPt, pt1);
        tRadius, tAngle2 = CartesianToPolar(ctrPt, pt2);
    }
    else{
        tRadius, tAngle1 = CartesianToPolar(ctrPt, pt2);
        tRadius, tAngle2 = CartesianToPolar(ctrPt, pt1);
    }

    tArcAngle = calcHdgDiff(tAngle1, tAngle2);
    tEndAngle = calcHdgSum(startAngle, tArcAngle);
    tArcLen = ((2 * math.pi * tRadius)/360) * tArcAngle;
    return (tArcAngle, tEndAngle, tArcLen);
}
*/
void Segment::operator=(Segment seg1){
	type = seg1.type;
    startPt = seg1.startPt;
    ctrPt = seg1.ctrPt;
    endPt = seg1.endPt;
    len = seg1.len;
    radius = seg1.radius;
    dir = seg1.dir;
    startAngle = seg1.startAngle;
    endAngle = seg1.endAngle;
}


// ***************************************************
//              Route Class Routines
// ***************************************************
Route::Route(){
	int i;
	while (i < 5){
		seg[i].type = 'N';
	}
	segCnt = 0;			// Zero current active segments in route
}

void Route::setSeg(int i, Segment seg1){
	seg[i] = seg1;
}

Segment Route::getSeg(int i){
	return (seg[i]);
}

void Route::clearSeg(int i){
	seg[i].type = 'N';
	segCnt -= 1;			// Zero current active segments in route
	if (segCnt < 0) segCnt = 0;
}

void Route::clearAllSeg(){
	int i;
	while (i < 5){
		clearSeg(i);
		i++;
	}
	segCnt = 0;
}

int Route::append(Segment seg1){
	if (segCnt > 4) return false;	// Route Full
	if (segCnt < 0) segCnt = 0;		// This shouldnt ever happen ...
	seg[segCnt] = seg1;				// Add segment to route
	segCnt++;
	return true;
}

void Route::operator=(Route rt1){
	int i = 0;
	while ( i < 5){
		seg[i] = rt1.seg[i];
		i++;
	}
	segCnt = rt1.segCnt;
}



// ***************************************************
//              Rbt Class
// ***************************************************
Rbt::Rbt(){
	// --- Rbt Class constructor ----
	COG = Point(0,0);
	hdg = 0;
	yaw = 0;
	pos = Point(0,0);
	startPt = Point(0,0);
	endPt = Point(0,0);
	xAxisIntPt = Point(0,0);
	distx=0;
	disty=0;
	angle = 0;

	ctrPt1 = Point(0,0);
	ctrPt2 = Point(0,0);
	radius = 0;
	arcDir1 = 0;
	arcDir2 = 0;
	ctr1sign = 0;
	ctr2xsign = 0;
	ctr2ysign = 0;
}


// ***************************************************
//              Hdg Routines
// ***************************************************

double addAngleToHdg(double hdg1, double angle){
	// Returns new Hdg (0-360) from sum of hdg & angle
	double newHdg;
	newHdg = hdg1 + angle;
    while (newHdg >= 360) newHdg -= 360;
    while (newHdg < 0)    newHdg -= 360;
    return(newHdg);
}

double convHdgToAngle(double hdg){
	// Returns Angle
	return 0;
}
double convAngleToHdg(double angle){
	// Returns Hdg (0->360) from any angle
	return 0;
}
double convHdgToYaw(double Hdg){
	// Returns Yaw (+ -) 180 from hdg (0->360)
	return 0;
}
double convYawToHdg(double yaw){
	// Returns Hdg (0->360) from (+ -) angle
	return 0;
}
double calcHdgDiff(double hdg1, double hdg2){
	// ********** Calculate diference in compass angles **************
	// Returns angle between 2 headings
	double hdgDiff;
    if (hdg2 >= hdg1)
        hdgDiff = hdg2 - hdg1;
    else
        hdgDiff = (hdg2 - hdg1) + 360;
    while (hdgDiff >= 360) hdgDiff -= 360;
    while (hdgDiff < 0) hdgDiff += 360;
    return(hdgDiff);
}

double calcHdgSum(double hdg1, double hdg2){
    // ********** Calculate new hdg from old heading and angle **************
	// Returns angle between 2 headings
    double hdgSum;
    hdgSum = hdg1 + hdg2;
    while (hdgSum >= 360) hdgSum -= 360;
    while (hdgSum < 0) hdgSum += 360;
    return(hdgSum);
}

// ***************************************************
//              Math Routines
// ***************************************************

double mRound(double nmbr, int digits){
	// Returns rounded nmbr to no of digits
	int mult = 1;
	if (digits == 0) mult = 1;
	else if (digits == 1) mult = 10;
	else if (digits == 2) mult = 100;
	else if (digits == 3) mult = 1000;
	else if (digits == 4) mult = 10000;
	else if (digits == 5) mult = 100000;
	else if (digits == 6) mult = 1000000;
	else if (digits == 7) mult = 10000000;
	else if (digits == 8) mult = 100000000;
	else if (digits == 9) mult = 1000000000;
	return (round(nmbr * mult) / mult);
}

int mGetQuad(double angle){
	// Returns Quad 1-4 from any angle
	int quad;

	while (angle < 0) angle += 360;				// Make sure angle is converted to (0-359.999999)
	while (angle >= 360) angle -= 360;

    if ((angle > 0) && (angle <= 90))
        quad = 1;
    else if ((angle > 90) && (angle <= 180))
        quad = 2;
    else if ((angle > 180) && (angle <= 270))
        quad = 3;
    else if (angle > 270)
        quad = 4;
	return quad;
}

double quad(double a, double b, double c){
	// Calculates the quadratic formula
	double x;
    if (a == 0) a = .0000000001;
    x = (-b + sqrt(b*b - (4 * a * c))) / (2 * a);
    return(x);
}

// **** Trig Functions *******

double cnvrtDegToRad(double degree){
	double rad = degree * DEGREETORADIANCONST;
	return rad;
}

double cnvrtRadToDeg(double radian){
	double deg = RADIANSTODEGREECONST * radian;
	return deg;
}

double mCos(double angle){
	// Calculates the cos of an angle
	// Takes in an angle in degrees and returns a cos value"""
	double x;
	if ((angle == 90) || (angle == 270)){
		// Python didn't return exactly zero .. just carry over
	    x = 0;
	}
	else{
	    x = cos(cnvrtDegToRad(angle));
	}
	return x;
}

double mSin(double angle){
	// Calculates the sin of an angle
	// Takes in an angle in degrees and returns a sin value
	double x;
	if (angle == 180){
		// pyton fails to give zero just close to zero
	      x = 0;
	} else {
		x = sin(cnvrtDegToRad(angle));
	}
    return (x);
}

double mTan(double angle){
	// Calculates the tan of an angle
	// Takes in an angle and returns a tan to the angle
	double x;
	if (angle >= 89.999999 and angle <= 90.000001){
		// avoid return infinity/error
	    x = tan(cnvrtDegToRad(89.99999));
	} else if ((angle >= 179.999999) && (angle <= 180.000001)) {
		// python fails to give zero just close to zero
	    x = 0;
	} else {
        x = tan(cnvrtDegToRad(angle));
	}
	return x;
}

/*
Point mAtan(double opo, double adj){
	// Calculates the Atan of a number
	// Takes in two sides  of a triangle and returns a radius and angle value
	// in a Point object
	double angle, radius, x, y;
	x = opo;
	y = adj;
    radius = sqrt((x*x) + (y*y));
	if ((x >= 0) && (y >= 0)){
		// angle in first quad 0-90 degrees
		if (x == 0)
			angle = 90;
	    else
	        angle = cnvrtRadToDeg(atan(y / x));
	}
	return Point(radius, angle);
}
*/

double mAtan(double opo, double adj){
	// Calculates the Atan of a ratio (opo/adj) or (rise/run)
	// Takes in two sides  of a triangle and returns a radius and angle value
	double angle;
	if ((adj == 0) && (opo >= 0))
			return (90);
	else if ((adj == 0) && (opo <0))
		return (180);

	if 	((opo >= 0) && (adj >= 0)){
		// Quad 1
		angle = cnvrtRadToDeg(atan(opo/adj));
	}
	else if ((opo >= 0) && (adj <= 0)){
		// Quad 2
		angle = cnvrtRadToDeg(atan(opo/adj)) + 180;
	}
	else if ((opo <= 0) && (adj <= 0)){
		// Quad 3
		angle = cnvrtRadToDeg(atan(opo/adj)) + 180;
	}else if ((opo <= 0) && (adj >= 0)){
		// Quad 4
		angle = cnvrtRadToDeg(atan(opo/adj)) + 360;
	}
	return (angle);
}

