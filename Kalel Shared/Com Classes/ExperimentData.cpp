#include "../Forcelib.h"
#include "ExperimentData.h"

#include "../Resources/DefineStages.h"	// For the names of the stages/experiment types

ExperimentData::ExperimentData()
{
	ResetData();
}

ExperimentData::~ExperimentData()
{
}

ExperimentData::ExperimentData(const ExperimentData & p)
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
	experimentGraphPoints			= p.GetexperimentGraphPoints();
	verificationStep				= p.GetverificationStep();

	timeStart						= p.GettimeStart();
	timeElapsed						= p.GettimeElapsed();
	measurementsMade				= p.GetmeasurementsMade();
	timeToEquilibrate				= p.GettimeToEquilibrate();
	timeToEquilibrateCurrent		= p.GettimeToEquilibrateCurrent();

	injectionAttemptCounter			= p.GetinjectionAttemptCounter();
	adsorptionCounter				= p.GetadsorptionCounter();
	desorptionCounter				= p.GetdesorptionCounter();

	pressureInitial					= p.GetpressureInitial();
	pressureFinal					= p.GetpressureFinal();
	pressureHighOld					= p.GetpressureHighOld();

	///*******************
	///		Recorded Data
	///*******************

	resultCalorimeter				= p.GetresultCalorimeter();

	pressureHigh					= p.GetpressureHigh();
	pressureLow						= p.GetpressureLow();

	temperatureCalo					= p.GettemperatureCalo();
	temperatureCage					= p.GettemperatureCage();
	temperatureRoom					= p.GettemperatureRoom();

}

void ExperimentData::ResetData()
{
	experimentInProgress		= false;
	experimentRecording			= false;
	experimentWaiting			= false;
	experimentCommandsRequested = true;

	experimentStage				= STAGE_UNDEF;
	experimentPreviousStage		= STAGE_UNDEF;
	experimentDose				= 0;
	experimentStepStatus		= STEP_STATUS_UNDEF;
	experimentSubstepStage		= STEP_STATUS_START;
	experimentGraphPoints		= 0;
	verificationStep			= STEP_VERIFICATIONS_UNDEF;

	timeStart					= 0;
	timeElapsed					= 0.f;
	measurementsMade			= 0;
	timeToEquilibrate			= 0.f;
	timeToEquilibrateCurrent	= 0.f;

	injectionAttemptCounter		= 0;
	adsorptionCounter			= 0;
	desorptionCounter			= 0;

	pressureInitial				= 0.f;
	pressureFinal				= 0.f;
	pressureHighOld				= 0.f;

	resultCalorimeter			= 0.f;

	pressureHigh				= 0.f;
	pressureLow					= 0.f;

	temperatureCalo				= 0.f;
	temperatureCage				= 0.f;
	temperatureRoom				= 0.f;
}


bool ExperimentData::GetexperimentInProgress()							const { return experimentInProgress			; }
bool ExperimentData::GetexperimentRecording()							const { return experimentRecording			; }
bool ExperimentData::GetexperimentWaiting()								const { return experimentWaiting			; }
bool ExperimentData::GetexperimentCommandsRequested()					const { return experimentCommandsRequested	; }
int ExperimentData::GetexperimentStage()								const { return experimentStage				; }
int ExperimentData::GetverificationStep()								const { return verificationStep				; }
int ExperimentData::GetexperimentStepStatus()							const { return experimentStepStatus			; }
int ExperimentData::GetexperimentSubstepStage()							const { return experimentSubstepStage		; }
int ExperimentData::GetexperimentDose()									const { return experimentDose				; }
int ExperimentData::GetexperimentGraphPoints()							const { return experimentGraphPoints		; }
int ExperimentData::GetexperimentPreviousStage()						const { return experimentPreviousStage		; }
unsigned long long ExperimentData::GettimeStart()						const { return timeStart					; }
unsigned long long ExperimentData::GettimeElapsed()						const { return timeElapsed					; }
long ExperimentData::GetmeasurementsMade()								const { return measurementsMade				; }
double ExperimentData::GettimeToEquilibrate()							const { return timeToEquilibrate			; }
double ExperimentData::GettimeToEquilibrateCurrent()					const { return timeToEquilibrateCurrent		; }
int ExperimentData::GetinjectionAttemptCounter()						const { return injectionAttemptCounter		; }
int ExperimentData::GetadsorptionCounter()								const { return adsorptionCounter			; }
int ExperimentData::GetdesorptionCounter()								const { return desorptionCounter			; }
double ExperimentData::GetpressureInitial()								const { return pressureInitial				; }
double ExperimentData::GetpressureFinal()								const { return pressureFinal				; }
double ExperimentData::GetpressureHighOld()								const { return pressureHighOld				; }
double ExperimentData::GetresultCalorimeter()							const { return resultCalorimeter			; }
double ExperimentData::GetpressureHigh()								const { return pressureHigh					; }
double ExperimentData::GetpressureLow()									const { return pressureLow					; }
double ExperimentData::GettemperatureCalo()								const { return temperatureCalo				; }
double ExperimentData::GettemperatureCage()								const { return temperatureCage				; }
double ExperimentData::GettemperatureRoom()								const { return temperatureRoom				; }


void ExperimentData::SetexperimentInProgress(bool a)						{ experimentInProgress			= a ; }
void ExperimentData::SetexperimentRecording(bool a)							{ experimentRecording			= a ; }
void ExperimentData::SetexperimentWaiting(bool a)							{ experimentWaiting				= a ; }
void ExperimentData::SetexperimentCommandsRequested(bool a)					{ experimentCommandsRequested	= a ; }
void ExperimentData::SetexperimentStage(int a)								{ experimentStage				= a ; }
void ExperimentData::SetverificationStep(int a)								{ verificationStep				= a ; }
void ExperimentData::SetexperimentStepStatus(int a)							{ experimentStepStatus			= a ; }
void ExperimentData::SetexperimentSubstepStage(int a)						{ experimentSubstepStage		= a ; }
void ExperimentData::SetexperimentDose(int a)								{ experimentDose				= a ; }
void ExperimentData::SetexperimentGraphPoints(int a)						{ experimentGraphPoints			= a ; }
void ExperimentData::SetexperimentPreviousStage(int a)						{ experimentPreviousStage		= a ; }
void ExperimentData::SetmeasurementsMade(long a)							{ measurementsMade				= a ; }
void ExperimentData::SettimeStart(unsigned long long  a)					{ timeStart						= a ; }
void ExperimentData::SettimeElapsed(unsigned long long  a)					{ timeElapsed					= a ; }
void ExperimentData::SettimeToEquilibrate(double a)							{ timeToEquilibrate				= a ; }
void ExperimentData::SettimeToEquilibrateCurrent(double a)					{ timeToEquilibrateCurrent		= a ; }
void ExperimentData::SetinjectionAttemptCounter(int a)						{ injectionAttemptCounter		= a ; }
void ExperimentData::SetadsorptionCounter(int a)							{ adsorptionCounter				= a ; }
void ExperimentData::SetdesorptionCounter(int a)							{ desorptionCounter				= a ; }
void ExperimentData::SetpressureInitial(double a)							{ pressureInitial				= a ; }
void ExperimentData::SetpressureFinal(double a)								{ pressureFinal					= a ; }
void ExperimentData::SetpressureHighOld(double a)							{ pressureHighOld				= a ; }
void ExperimentData::SetresultCalorimeter(double a)							{ resultCalorimeter				= a ; }
void ExperimentData::SetpressureHigh(double a)								{ pressureHigh					= a ; }
void ExperimentData::SetpressureLow(double a)								{ pressureLow					= a ; }
void ExperimentData::SettemperatureCalo(double a)							{ temperatureCalo				= a ; }
void ExperimentData::SettemperatureCage(double a)							{ temperatureCage				= a ; }
void ExperimentData::SettemperatureRoom(double a)							{ temperatureRoom				= a ; }