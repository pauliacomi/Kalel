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
}



void ExperimentStatus::Replace(const ExperimentStatus & rhs)
{

	tp = rhs.tp.load();

	///*******************
	///		Global flags
	///*******************

	experimentInProgress				.store_nostamp(rhs.experimentInProgress.load());
	experimentRecording					.store_nostamp(rhs.experimentRecording.load());
	experimentWaiting					.store_nostamp(rhs.experimentWaiting.load());
	experimentCommandsRequested			.store_nostamp(rhs.experimentCommandsRequested.load());

	///*******************
	///		Parameters for storing where program has reached
	///*******************

	experimentStage						.store_nostamp(rhs.experimentStage.load());
	experimentPreviousStage				.store_nostamp(rhs.experimentPreviousStage.load());
	experimentDose						.store_nostamp(rhs.experimentDose.load());
	experimentStepStatus				.store_nostamp(rhs.experimentStepStatus.load());
	experimentSubstepStage				.store_nostamp(rhs.experimentSubstepStage.load());
	verificationStep					.store_nostamp(rhs.verificationStep.load());

	timeStart							.store_nostamp(rhs.timeStart.load());
	timeEquilibrationStart				.store_nostamp(rhs.timeEquilibrationStart.load());
	timeToEquilibrate					.store_nostamp(rhs.timeToEquilibrate.load());

	injectionAttemptCounter				.store_nostamp(rhs.injectionAttemptCounter.load());
	adsorptionCounter					.store_nostamp(rhs.adsorptionCounter.load());
	desorptionCounter					.store_nostamp(rhs.desorptionCounter.load());

	pressureInitial						.store_nostamp(rhs.pressureInitial.load());
	pressureFinal						.store_nostamp(rhs.pressureFinal.load());
	pressureHighOld						.store_nostamp(rhs.pressureHighOld.load());
}
