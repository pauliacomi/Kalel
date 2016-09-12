#include "../Forcelib.h"
#include "ManualActionParam.h"


ManualActionParam::ManualActionParam()
{
}

ManualActionParam::ManualActionParam(ManualActionParam * mParam)
{
	this->instrumentType = mParam->instrumentType;
	this->instrumentNumber = mParam->instrumentNumber;
	this->shouldBeActivated = mParam->shouldBeActivated;
}

ManualActionParam::ManualActionParam(int instrType, int instrNumber, bool shouldBeActive)
{
	instrumentType = instrType;
	instrumentNumber = instrNumber;
	shouldBeActivated = shouldBeActive;
}

ManualActionParam::~ManualActionParam()
{
}
