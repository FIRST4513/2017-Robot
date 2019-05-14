// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#ifndef DRIVESTRAIGHTDISTCMD_H
#define DRIVESTRAIGHTDISTCMD_H


#include "Commands/Subsystem.h"
#include "../Robot.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class DriveStraightDistCmd: public Command {
public:
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR
	DriveStraightDistCmd(double Speed, double Distance, double Timeout, double Mode);

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();

private:
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=VARIABLES

    double m_Speed;
    double m_Distance;
    double m_Timeout;
    double m_Mode;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=VARIABLES

    // --- Test Profile Constants --
	double cruisePwr;
	double tgtDist;
	int TO;
	int mode;

	double accelPwr;
	double accelDist;
	double decelDistFromTgt;
	double stopDistFromTgt;
	double decelPwr;
	double slowPwr;
	double stopPwr;

	std::string line;

	std::shared_ptr<NetworkTable> netTable;			// NetTables to talk to RoboRealm
	std::vector<std::string> keys;

	Timer stallTmr;
	int driveModeFlag, driveProfileFlag;

	double currPwr;
	double m_RemainTgtDist, m_CurrTgtDist, currDist, currSpeed;
	char m_Zone;

	void GetTestData();
	void SendPwrToDrivetrain(double speed, int mode);

};

#endif