#include "StdAfx.h"
#include "InfoEtape.h"

using namespace std;


InfoEtape::InfoEtape(int Index, CString Nom, int FctArret)
{
	this->Index = Index;
	this->Nom = Nom;
	this->FctArret = FctArret;
}
/*
InfoEtape InfoEtape::operator=(const InfoEtape &info)
{
	Index = info.Index;
	Nom = info.Nom;
	FctArret = info.FctArret;

	return *this;
}

bool InfoEtape::operator!=(const InfoEtape &info)
{
	if (Index != info.Index ||
		Nom != info.Nom ||
		FctArret != info.FctArret)
		return TRUE;
	return FALSE;
}
*/

/*
int count(InfoEtape ListeEtape[])
{
	int i=0;
	while (ListeEtape[i]!=NULL){i++;}
	return i;
} 
*/