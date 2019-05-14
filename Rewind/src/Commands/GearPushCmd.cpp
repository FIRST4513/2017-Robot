// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "GearPushCmd.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

GearPushCmd::GearPushCmd(int Mode): Command() {
    m_Mode = Mode;
        // Use requires() here to declare subsystem dependencies
    // eg. requires(Robot::chassis.get());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::gearHandler.get());
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

// Called just before this Command runs the first time
void GearPushCmd::Initialize() {
		// Mode 0 = manual eject and retract, 1 = Look for contact with probe and only then eject

		line = "Debug, Gear, Starting GearPushCmd Init";
		Robot::logger->appendLog(line);
		std::cout << "Starting GearPushCmd Init" << std::endl;
		SetTimeout(5);
		ejectFlag = 0;
}

// Called repeatedly when this Command is scheduled to run
void GearPushCmd::Execute() {

	if ((m_Mode == 1) && (Robot::drivetrain->GetProbeLmtSw()==true)) {
		// true means switch is still depressed NO probe Contact
		// We are not in contact with probe so dont eject just exit
		ejectFlag = 2;
		return;}

	if (ejectFlag == 0){
		// request to eject gear
		if (Robot::gearHandler->GetEjectFwdLmtSw() != true) {
			//we haven't hit the end so keep moving forward
			Robot::gearHandler->EjectMotorFwd();}
		else {
			// we have hit the end we need to start moving back
			Robot::gearHandler->EjectMotorStop();
			ejectFlag = 1;}
	}

	if (ejectFlag == 1){
		// request to move eject bar back
		if (Robot::gearHandler->GetEjectRearLmtSw() != true){
			//we haven't hit the back yet so keep moving forward
			Robot::gearHandler->EjectMotorBack();
		}
		else{
			// we have hit the end we need to stop moving back
			Robot::gearHandler->EjectMotorStop();
			ejectFlag =2;
		}
	}
}

// Make this return true when this Command no longer needs to run execute()
bool GearPushCmd::IsFinished() {
	if (IsTimedOut()) 		return true;
	if (ejectFlag == 2 ) 	return true;	// we are back so exit

    return false;
}

// Called once after isFinished returns true
void GearPushCmd::End() {
	Robot::gearHandler->EjectMotorStop();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void GearPushCmd::Interrupted() {
	End();
}
