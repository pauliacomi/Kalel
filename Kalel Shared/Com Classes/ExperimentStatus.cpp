#include "../Forcelib.h"

#include "ExperimentStatus.h"

#include "../timeHelpers.h"
#include "../Resources/DefineStages.h"

ExperimentStatus::ExperimentStatus()
{
	ResetData();							// Do initialisation of all variables
}

ExperimentStatus::ExperimentStatus(const ExperimentStatus & rhs)
{
	Replace(rhs);							// Copy values from reference
}

ExperimentStatus & ExperimentStatus::operator=(const ExperimentStatus & p)
{
	if (this != &p) {  // make sure not same object

		// To be efficient, we won't generate a timestamp for each allocation
		// Instead, we allocate without timestamping, then generate the new time 
		// at the end
		Replace(p);

		// timestamp
		tp = timeh::NowTime();
	}
	return *this;    // Return ref for multiple assignment
}

void ExperimentStatus::ResetData()
{
	tp = timeh::NowTime();

	inProgress						.store_nostamp( false );
	isRecording						.store_nostamp( false );
	isWaiting						.store_nostamp( false );
	isWaitingUser					.store_nostamp( false );
	isRunningAutomation				.store_nostamp( true );

	mainStage						.store_nostamp( STAGE_UNDEF );
	stepStatus						.store_nostamp( STEP_STATUS_UNDEF );
	substepStatus					.store_nostamp( SUBSTEP_STATUS_UNDEF );
	injectionDose					.store_nostamp( 0 );

	timeStart						.store_nostamp( 0 );
	timeWaitStart					.store_nostamp( 0 );
	timeToWait						.store_nostamp( 0.f );

	injectionAttemptCounter			.store_nostamp( 0 );
	stepCounter						.store_nostamp( 0 );

	pressureInitial					.store_nostamp( 0.f );
	pressureFinal					.store_nostamp( 0.f );
	pressureHighOld					.store_nostamp( 0.f );
}



void ExperimentStatus::Replace(const ExperimentStatus & rhs)
{

	tp = rhs.tp.load();

	///*******************
	///		Global flags
	///*******************

	inProgress							.store_nostamp(rhs.inProgress.load());
	isRecording							.store_nostamp(rhs.isRecording.load());
	isWaiting							.store_nostamp(rhs.isWaiting.load());
	isWaitingUser						.store_nostamp(rhs.isWaiting.load());
	isRunningAutomation					.store_nostamp(rhs.isRunningAutomation.load());

	///*******************
	///		Parameters for storing where program has reached
	///*******************

	mainStage							.store_nostamp(rhs.mainStage.load());
	injectionDose						.store_nostamp(rhs.injectionDose.load());
	stepStatus							.store_nostamp(rhs.stepStatus.load());
	substepStatus						.store_nostamp(rhs.substepStatus.load());

	timeStart							.store_nostamp(rhs.timeStart.load());
	timeWaitStart						.store_nostamp(rhs.timeWaitStart.load());
	timeToWait							.store_nostamp(rhs.timeToWait.load());

	injectionAttemptCounter				.store_nostamp(rhs.injectionAttemptCounter.load());
	stepCounter							.store_nostamp(rhs.stepCounter.load());

	pressureInitial						.store_nostamp(rhs.pressureInitial.load());
	pressureFinal						.store_nostamp(rhs.pressureFinal.load());
	pressureHighOld						.store_nostamp(rhs.pressureHighOld.load());
}
