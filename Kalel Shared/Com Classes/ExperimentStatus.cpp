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
	*this = rhs;							// Copy values from reference
}

ExperimentStatus::~ExperimentStatus(void)
{
}


void ExperimentStatus::ResetData()
{
	experimentInProgress					.store_nostamp( false );
	experimentRecording						.store_nostamp( false );
	experimentWaiting						.store_nostamp( false );
	experimentCommandsRequested				.store_nostamp( true );

	experimentStage							.store_nostamp( STAGE_UNDEF );
	experimentPreviousStage					.store_nostamp( STAGE_UNDEF );
	experimentDose							.store_nostamp( 0 );
	experimentStepStatus					.store_nostamp( STEP_STATUS_UNDEF );
	experimentSubstepStage					.store_nostamp( STEP_STATUS_START );
	verificationStep						.store_nostamp( STEP_VERIFICATIONS_UNDEF );

	timeStart								.store_nostamp( 0 );
	timeEquilibrationStart					.store_nostamp( 0 );
	timeToEquilibrate						.store_nostamp( 0.f );

	injectionAttemptCounter					.store_nostamp( 0 );
	adsorptionCounter						.store_nostamp( 0 );
	desorptionCounter						.store_nostamp( 0 );

	pressureInitial							.store_nostamp( 0.f );
	pressureFinal							.store_nostamp( 0.f );
	pressureHighOld							.store_nostamp( 0.f );

	tp = timeh::NowTime();
}


ExperimentStatus & ExperimentStatus::operator=(const ExperimentStatus & p)
{
	if (this != &p) {  // make sure not same object
		
		// To be efficient, we won't generate a timestamp for each allocation
		// Instead, we allocate without timestamping, then generate the new time 
		// at the end

		///*******************
		///		Global flags
		///*******************

		experimentInProgress.store_nostamp(					p.experimentInProgress.load());
		experimentRecording.store_nostamp(					p.experimentRecording.load());
		experimentWaiting.store_nostamp(					p.experimentWaiting.load());
		experimentCommandsRequested.store_nostamp(			p.experimentCommandsRequested.load());

		///*******************
		///		Parameters for storing where program has reached
		///*******************

		experimentStage.store_nostamp(						p.experimentStage.load());
		experimentPreviousStage.store_nostamp(				p.experimentPreviousStage.load());
		experimentDose.store_nostamp(						p.experimentDose.load());
		experimentStepStatus.store_nostamp(					p.experimentStepStatus.load());
		experimentSubstepStage.store_nostamp(				p.experimentSubstepStage.load());
		verificationStep.store_nostamp(						p.verificationStep.load());

		timeStart.store_nostamp(							p.timeStart.load());
		timeEquilibrationStart.store_nostamp(				p.timeEquilibrationStart.load());
		timeToEquilibrate.store_nostamp(					p.timeToEquilibrate.load());

		injectionAttemptCounter.store_nostamp(				p.injectionAttemptCounter.load());
		adsorptionCounter.store_nostamp(					p.adsorptionCounter.load());
		desorptionCounter.store_nostamp(					p.desorptionCounter.load());

		pressureInitial.store_nostamp(						p.pressureInitial.load());
		pressureFinal.store_nostamp(						p.pressureFinal.load());
		pressureHighOld.store_nostamp(						p.pressureHighOld.load());
	
		// timestamp
		tp = timeh::NowTime();
	}
	return *this;    // Return ref for multiple assignment
}