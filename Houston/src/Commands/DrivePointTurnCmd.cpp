// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "DrivePointTurnCmd.h"
#include "../Robot.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

DrivePointTurnCmd::DrivePointTurnCmd(double Angle, double Speed, double Timeout, double Mode): Command() {
    m_Angle = Angle;
    m_Speed = Speed;
    m_Timeout = Timeout;
    m_Mode = Mode;
        // Use requires() here to declare subsystem dependencies
    // eg. requires(Robot::chassis.get());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::drivetrain.get());
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR


// Called just before this Command runs the first time
void DrivePointTurnCmd::Initialize() {
	// Mode 0 = use angle for turn,
	//		1 =look at gps and do a 60degree right turn
	//		2 = 60 degree left turn,
	//		3 = 0 degree turn

	SetTimeout(m_Timeout);
	stopTmr.Reset();
	GetTestData();
	m_Speed = tstSpeed;							// Get speed from netables variable
	Robot::drivetrain->ResetEncoders();
	startHdg = Robot::drivetrain->GetGyroYaw();

	// ------- Determine End Heading from Mode Flag --------
	if (m_Mode == 0)
		m_Angle = Round(m_Angle,2);				// Round to 2 decimal places
	else if (m_Mode == 1){						// Mode 1 = Lt side doing a rt turn CW hdg of 60 degrees
		endHdg = 60;
	}
	else if (m_Mode == 2){						// Mode 2 = Rt side doing a lt turn CCW heading of 300
		endHdg = -60;
	}
	else if (m_Mode == 3){						// Mode 3 = Ctr doing a turn to hdg of 0 degrees
		endHdg = 0;
	}

	// ---- Check for left or Right Turn ----
	// ?? state not init.
	CalcDiff(endHdg);
	CalcDir(endHdg);
	state = 3;									// Default mode = 3 go to final destination hdg
	if (turnDir > 0) std::cout << "Right Turn indicated currHg=" << currHdg << " endHdg=" << endHdg << std::endl;
	if (turnDir < 0) std::cout << "Left Turn indicated currHg=" << currHdg << " endHdg=" << endHdg << std::endl;
	// ---- Check if we need to overshoot and return to ending heading ----
	tmpHdg = 0;
	if ((turnDir > 0) && (diff <= rtOvershoot)) {
		state = 0;
		tmpHdg = endHdg + rtOvershoot + ltOvershoot + 5;
		std::cout << "Right Overshoot required currHg=" << currHdg << " tmpHdg=" << tmpHdg << std::endl;
	}
	if ((turnDir < 0) && (-diff <= ltOvershoot)) {
		state = 0;
		tmpHdg = endHdg - ltOvershoot - rtOvershoot - 5;
		std::cout << "Left Overshoot required currHg=" << currHdg << " tmpHdg=" << tmpHdg << std::endl;
	}
	if (tmpHdg >= 360) tmpHdg -= 360;
	if (tmpHdg <= 0) tmpHdg += 360;

	line = "Debug, Drivetrain, Starting DrivePointTurnCmd";
	line += " StartHdg=" + std::to_string(startHdg) +  " endHdg=" + std::to_string(endHdg);
	line += " tmpHdg=," + std::to_string(tmpHdg);
	Robot::logger->appendLog(line);
}


// ------------------- Main Execute Loop ---------------------
// Called repeatedly when this Command is scheduled to run
// -----------------------------------------------------------

void DrivePointTurnCmd::Execute() {
	// state 0=moving to tmpHdg
	// state 1=Reached tmpHdg time to apply brakes
	// state 2=Stopping at TmpHdg waiting for timeout on brakes
	// state 3=moving to endHdg
	// state 4=Reached endHdg time to apply brakes
	// state 5=Stopping at Final Hdg waiting for timeout on brakes
	// state 6= Time To exit

	std::cout << "Begining of exec section State=" << state <<std::endl;

	// -- Check for arriving at destination
	if (state == 0)  CheckForDest(tmpHdg);					// If we have Not reached dest. keep moving
	if (state == 3)  CheckForDest(endHdg);

	// -----  we have reached our dest hdg time to apply brakes ----
	if ((state == 1) || (state == 4)){
		std::cout << "State 1 or 4 detected currHg=" << currHdg << "state=" << state <<std::endl;
		stopTmr.Reset();
		stopTmr.Start();
		if (state == 1) state = 2;							// we were heading to tmpHdg
		if (state == 4) state = 5;							// we were heading to endHdg
		if (turnDir < 0){									// We're turning left apply right brakes
			std::cout << "Right brake being applied currHg=" << currHdg << "state=" << state <<std::endl;
			Robot::drivetrain->TankDrive(+brakePwr, -brakePwr); }
		else {												// We're turning Right apply left brakes
			std::cout << "Left brake being applied currHg=" << currHdg << std::endl;
			Robot::drivetrain->TankDrive((-brakePwr*1.25), +brakePwr); }	// provide MORE left braking !!
	}

	// ----- Check to see if time to stop brakes ----
	if ((state == 2) || (state == 5)) {
		std::cout << "State 2 or 5 detected currHg=" << currHdg << "state=" << state <<std::endl;
		std::cout << "StopTmr"  << stopTmr.Get() << " brakeTm=" << brakeTm << std::endl;
		if (stopTmr.Get() >= brakeTm) {
			std::cout << "Brake time has been exceeded" << state <<std::endl;

			if (state == 2)	{								// Were now setting up for final leg
				std::cout << "State 2 re calculating diff and dir state=" << state <<std::endl;
				CalcDiff(endHdg);
				CalcDir(endHdg);
				state = 3;
			}
			if (state == 5)	{
				std::cout << "State 5 detected changing it to state=6" << state <<std::endl;
				state = 6;						// were all done
			}
			Robot::drivetrain->TankDrive( 0, 0);
			std::cout << "Brake time reached currHg=" << currHdg << m_Mode << std::endl;
		}
	}

	// --- Keep on Moving in Turn ------
	if ((state == 0 ) || (state == 3)) {
		std::cout << "Keep On Moving State (0 or 3) state=" << state <<std::endl;
		if (turnDir > 0) Robot::drivetrain->TankDrive(m_Speed,-m_Speed);		// Right turn
		if (turnDir < 0) Robot::drivetrain->TankDrive(-m_Speed,m_Speed);		// Left turn
	}
	std::cout << "End of exec section State=" << state <<std::endl;
}

// Make this return true when this Command no longer needs to run execute()
bool DrivePointTurnCmd::IsFinished() {
	if (IsTimedOut()){
		std::cout << "Drive Pt Turn Timed OUT !" << std::endl;
		return true;
	}
	if (state == 6) return true;

	return false;
}

// Called once after isFinished returns true
void DrivePointTurnCmd::End() {
	std::cout << "DrivePTturnCmd Ending Hdg =" << Robot::drivetrain->GetGyroHeading() << std::endl;
	Robot::logger->appendLog("Ending DrivePointTurnCmd");
	Robot::drivetrain->StopMtrs();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DrivePointTurnCmd::Interrupted() {
	End();
}


// ------------ Determine if we have reached target hdg --------------------
void DrivePointTurnCmd::CheckForDest(double destHdg){
	CalcDiff(destHdg);
	// ---- Check if we have reached our destination
	if (((turnDir > 0 ) && ((diff - rtOvershoot) <= 0 )) ||
		((turnDir < 0 ) && ((diff + ltOvershoot) >= 0 ))) {
			if (state == 0)	state = 1;					// Setup to apply brakes
			if (state == 3) state = 4;					// We're moving to endHdg setup to apply brakes
	}
	return;
}

void DrivePointTurnCmd::CalcDiff(double Hdg){
	// Returns diff as (0 -> +180) right turn   (0 -> -180) left turn

	currHdg = Robot::drivetrain->GetGyroYaw();
	tmpCurrHdg = currHdg;
	tmpDestHdg = Hdg;
	if (currHdg < 0) tmpCurrHdg = currHdg + 360;
	if (Hdg < 0) tmpDestHdg = Hdg + 360;

	// ---- Calculate angle difference between current hdg and destination ----
	diff = tmpDestHdg - tmpCurrHdg;
	if (diff > 180)	diff -= 360;
	if (diff < -180) diff += 360;
}

void DrivePointTurnCmd::CalcDir(double Hdg){
	if (diff >= 0) turnDir = +1;							// Right Turn
	if (diff < 0)  turnDir = -1;							// Left Turn
}


float DrivePointTurnCmd::Round(float value, int digits){
	int mult = 1;
	if (digits == 0) mult = 1;
	else if (digits == 1) mult = 10;
	else if (digits == 2) mult = 100;
	else if (digits == 3) mult = 1000;
	else if (digits == 4) mult = 10000;
	else if (digits == 5) mult = 100000;
	else if (digits == 6) mult = 1000000;
	return (round(value * mult) / mult);
}

// ------------------------ Get System Test Default Values --------------------
void DrivePointTurnCmd::GetTestData(){
	netTable = NetworkTable::GetTable("Preferences");

	tstSpeed = netTable->GetNumber("PtSpeed",0);
	brakeTm = netTable->GetNumber("PtBrakeTm",0);
	brakePwr = netTable->GetNumber("PtBrakePwr",0);
	ltOvershoot = netTable->GetNumber("PtLfOS",0);
	rtOvershoot = netTable->GetNumber("PtRtOS",0);
}

