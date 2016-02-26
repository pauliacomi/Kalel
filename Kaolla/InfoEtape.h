#ifndef INFOETAPE_H
#define INFOETAPE_H

class InfoEtape
{
public :
	int Index;
	CString Nom;
	int FctArret;

	InfoEtape(int Index, CString Nom, int FctArret);

	//InfoEtape operator=(const InfoEtape &info);
	//bool operator!=(const InfoEtape &info);
};

//int count(InfoEtape ListeEtape[]);

#endif