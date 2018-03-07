#include "ExperimentStatus.h"
#include "../Forcelib.h"
#include "ExperimentStatus.h"

#include "../Resources/DefineStages.h"

ExperimentStatus::ExperimentStatus()
{
	ResetData();
}

ExperimentStatus::~ExperimentStatus(void)
{
}


ExperimentStatus::ExperimentStatus(const ExperimentStatus & p)
{
	experimentInProgress			= p.GetexperimentInProgress();
	experimentRecording				= p.GetexperimentRecording();
	experimentWaiting				= p.GetexperimentWaiting();
	experimentCommandsRequested		= p.GetexperimentCommandsRequested();

	///*******************
	///		Parameters for storing where program has reached
	///*******************

	experimentStage					= p.GetexperimentStage();
	experimentPreviousStage			= p.GetexperimentPreviousStage();
	experimentDose					= p.GetexperimentDose();
	experimentStepStatus			= p.GetexperimentStepStatus();
	experimentSubstepStage			= p.GetexperimentSubstepStage();
	verificationStep				= p.GetverificationStep();

	timeStart						= p.GettimeStart();
	timeEquilibrationStart			= p.GettimeEquilibrateStart();
	timeToEquilibrate				= p.GettimeToEquilibrate();

	injectionAttemptCounter			= p.GetinjectionAttemptCounter();
	adsorptionCounter				= p.GetadsorptionCounter();
	desorptionCounter				= p.GetdesorptionCounter();

	pressureInitial					= p.GetpressureInitial();
	pressureFinal					= p.GetpressureFinal();
	pressureHighOld					= p.GetpressureHighOld();

}

void ExperimentStatus::ResetData()
{
	experimentInProgress = false;
	experimentRecording = false;
	experimentWaiting = false;
	experimentCommandsRequested = true;

	experimentStage = STAGE_UNDEF;
	experimentPreviousStage = STAGE_UNDEF;
	experimentDose = 0;
	experimentStepStatus = STEP_STATUS_UNDEF;
	experimentSubstepStage = STEP_STATUS_START;
	verificationStep = STEP_VERIFICATIONS_UNDEF;

	timeStart = 0;
	timeEquilibrationStart = 0;
	timeToEquilibrate = 0.f;

	injectionAttemptCounter = 0;
	adsorptionCounter = 0;
	desorptionCounter = 0;

	pressureInitial = 0.f;
	pressureFinal = 0.f;
	pressureHighOld = 0.f;
}

ExperimentStatus & ExperimentStatus::operator=(const ExperimentStatus * p)
{
	if (this != p) {  // make sure not same object

		experimentInProgress = p->GetexperimentInProgress();
		experimentRecording = p->GetexperimentRecording();
		experimentWaiting = p->GetexperimentWaiting();
		experimentCommandsRequested = p->GetexperimentCommandsRequested();

		///*******************
		///		Parameters for storing where program has reached
		///*******************

		experimentStage = p->GetexperimentStage();
		experimentPreviousStage = p->GetexperimentPreviousStage();
		experimentDose = p->GetexperimentDose();
		experimentStepStatus = p->GetexperimentStepStatus();
		experimentSubstepStage = p->GetexperimentSubstepStage();
		verificationStep = p->GetverificationStep();

		timeStart = p->GettimeStart();
		timeEquilibrationStart = p->GettimeEquilibrateStart();
		timeToEquilibrate = p->GettimeToEquilibrate();

		injectionAttemptCounter = p->GetinjectionAttemptCounter();
		adsorptionCounter = p->GetadsorptionCounter();
		desorptionCounter = p->GetdesorptionCounter();

		pressureInitial = p->GetpressureInitial();
		pressureFinal = p->GetpressureFinal();
		pressureHighOld = p->GetpressureHighOld();

	}
	return *this;    // Return ref for multiple assignment
}


bool ExperimentStatus::GetexperimentInProgress()						const { return experimentInProgress					; }
bool ExperimentStatus::GetexperimentRecording()							const { return experimentRecording					; }
bool ExperimentStatus::GetexperimentWaiting()							const { return experimentWaiting					; }
bool ExperimentStatus::GetexperimentCommandsRequested()					const { return experimentCommandsRequested			; }
int ExperimentStatus::GetexperimentStage()								const { return experimentStage						; }
int ExperimentStatus::GetverificationStep()								const { return verificationStep						; }
int ExperimentStatus::GetexperimentStepStatus()							const { return experimentStepStatus					; }
int ExperimentStatus::GetexperimentSubstepStage()						const { return experimentSubstepStage				; }
int ExperimentStatus::GetexperimentDose()								const { return experimentDose						; }
int ExperimentStatus::GetexperimentPreviousStage()						const { return experimentPreviousStage				; }
unsigned long long ExperimentStatus::GettimeStart()						const { return timeStart							; }
unsigned long long ExperimentStatus::GettimeEquilibrateStart()			const { return timeEquilibrationStart				; }
double ExperimentStatus::GettimeToEquilibrate()							const { return timeToEquilibrate					; }
int ExperimentStatus::GetinjectionAttemptCounter()						const { return injectionAttemptCounter				; }
int ExperimentStatus::GetadsorptionCounter()							const { return adsorptionCounter					; }
int ExperimentStatus::GetdesorptionCounter()							const { return desorptionCounter					; }
double ExperimentStatus::GetpressureInitial()							const { return pressureInitial						; }
double ExperimentStatus::GetpressureFinal()								const { return pressureFinal						; }
double ExperimentStatus::GetpressureHighOld()							const { return pressureHighOld						; }


void ExperimentStatus::SetexperimentInProgress(bool a)						{ experimentInProgress			= a ; }
void ExperimentStatus::SetexperimentRecording(bool a)						{ experimentRecording			= a ; }
void ExperimentStatus::SetexperimentWaiting(bool a)							{ experimentWaiting				= a ; }
void ExperimentStatus::SetexperimentCommandsRequested(bool a)				{ experimentCommandsRequested	= a ; }
void ExperimentStatus::SetexperimentStage(int a)							{ experimentStage				= a ; }
void ExperimentStatus::SetverificationStep(int a)							{ verificationStep				= a ; }
void ExperimentStatus::SetexperimentStepStatus(int a)						{ experimentStepStatus			= a ; }
void ExperimentStatus::SetexperimentSubstepStage(int a)						{ experimentSubstepStage		= a ; }
void ExperimentStatus::SetexperimentDose(int a)								{ experimentDose				= a ; }
void ExperimentStatus::SetexperimentPreviousStage(int a)					{ experimentPreviousStage		= a ; }
void ExperimentStatus::SettimeStart(unsigned long long  a)					{ timeStart						= a ; }
void ExperimentStatus::SettimeEquilibrateStart(unsigned long long a)		{ timeEquilibrationStart		= a ; }
void ExperimentStatus::SettimeToEquilibrate(double a)						{ timeToEquilibrate				= a ; }
void ExperimentStatus::SetinjectionAttemptCounter(int a)					{ injectionAttemptCounter		= a ; }
void ExperimentStatus::SetadsorptionCounter(int a)							{ adsorptionCounter				= a ; }
void ExperimentStatus::SetdesorptionCounter(int a)							{ desorptionCounter				= a ; }
void ExperimentStatus::SetpressureInitial(double a)							{ pressureInitial				= a ; }
void ExperimentStatus::SetpressureFinal(double a)							{ pressureFinal					= a ; }
void ExperimentStatus::SetpressureHighOld(double a)							{ pressureHighOld				= a ; }