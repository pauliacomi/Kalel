#include "StdAfx.h"
#include "Vannes.h"

#define nb_essais 3
#define temps_ms 500



CVannes::CVannes(void) : NI_USB_6008()
{
	NI_USB_6008::SetDevNI_USB_6008(PortUSBVannes());

	ToutFermer();
}

CVannes::~CVannes(void)
{
}



bool CVannes::VanneEstOuvert(int num)
{return NI_USB_6008::EstOuvertPort0(num-1);}

bool CVannes::VanneEstFerme(int num)
{return NI_USB_6008::EstFermePort0(num-1);}

bool CVannes::EV1EstActive()
{return NI_USB_6008::EstOuvertPort1(0);}

bool CVannes::EV1EstDesactive()
{return NI_USB_6008::EstFermePort1(0);}

bool CVannes::EV2EstActive()
{return NI_USB_6008::EstOuvertPort1(1);}

bool CVannes::EV2EstDesactive()
{return NI_USB_6008::EstFermePort1(1);}

bool CVannes::PompeEstActive()
{return NI_USB_6008::EstOuvertPort1(2);}

bool CVannes::PompeEstDesactive()
{return NI_USB_6008::EstFermePort1(2);}


bool CVannes::Ouvrir(int num)
{
	int tentative = 0;
	bool action_reussie;
	do{
		tentative++;
		action_reussie = NI_USB_6008::OuvrirPort0(num-1);
		Sleep(temps_ms);
	}while(!action_reussie && tentative <= nb_essais);

	return action_reussie;
}

bool CVannes::Fermer(int num)
{
	int tentative = 0;
	bool action_reussie;
	do{
		tentative++;
		action_reussie = NI_USB_6008::FermerPort0(num-1);
		Sleep(temps_ms);
	}while(!action_reussie && tentative <= nb_essais);

	return action_reussie;
}

bool CVannes::FermerToutesLesVannes()
{
	int tentative = 0;
	bool action_reussie;
	do{
		tentative++;
		action_reussie = NI_USB_6008::FermerPort0Tous();
		Sleep(temps_ms);
	}while(!action_reussie && tentative <= nb_essais);

	return action_reussie;
}

bool CVannes::ActiverEV(int num)
{
	int tentative = 0;
	bool action_reussie;
	do{
		tentative++;
		action_reussie = NI_USB_6008::OuvrirPort1(0 + num);
		Sleep(temps_ms);
	}while(!action_reussie && tentative <= nb_essais);

	return action_reussie;
}

bool CVannes::DesactiverEV(int num)
{
	int tentative = 0;
	bool action_reussie;
	do {
		tentative++;
		action_reussie = NI_USB_6008::FermerPort1(0 + num);
		Sleep(temps_ms);
	} while (!action_reussie && tentative <= nb_essais);

	return action_reussie;
}

bool CVannes::ActiverPompe()
{
	int tentative = 0;
	bool action_reussie;
	do{
		tentative++;
		action_reussie = NI_USB_6008::OuvrirPort1(2);
		Sleep(temps_ms);
	}while(!action_reussie && tentative <= nb_essais);

	return action_reussie;
}

bool CVannes::DesactiverPompe()
{
	int tentative = 0;
	bool action_reussie;
	do{
		tentative++;
		action_reussie = NI_USB_6008::FermerPort1(2);
		Sleep(temps_ms);
	}while(!action_reussie && tentative <= nb_essais);

	return action_reussie;
}

bool CVannes::FermerLesValvesEtLaPompe()
{
	int tentative = 0;
	bool action_reussie;
	do{
		tentative++;
		action_reussie = NI_USB_6008::FermerPort1Tous();
		Sleep(temps_ms);
	}while(!action_reussie && tentative <= nb_essais);

	return action_reussie;
}

bool CVannes::ToutFermer()
{
	if(FermerToutesLesVannes() && FermerLesValvesEtLaPompe())
		return TRUE;
	return FALSE;
}



int CVannes::PortUSBVannes()
{
	return GetPortVannes();
}




