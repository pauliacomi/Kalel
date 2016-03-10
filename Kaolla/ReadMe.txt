================================================================================
    BIBLIOTH�QUE MICROSOFT FOUNDATION CLASS : Vue d'ensemble du projet Kaolla
===============================================================================

L'Assistant Application a cr�� cette application Kaolla pour  
vous.  Cette application d�crit les principes de base de l'utilisation de  
Microsoft Foundation Classes et vous permet de cr�er votre application.

Ce fichier contient un r�sum� du contenu de chacun des fichiers qui 
constituent votre application Kaolla.

Kaolla.vcproj
    Il s'agit du fichier projet principal pour les projets VC++ g�n�r�s � l'aide 
d'un Assistant Application. 
    Il contient les informations sur la version de Visual C++ qui a g�n�r� le 
fichier et  
    des informations sur les plates-formes, configurations et fonctionnalit�s du 
projet s�lectionn�es avec 
    l'Assistant Application.

Kaolla.h
    Il s'agit du fichier d'en-t�te principal de l'application.  Il contient 
d'autres 
    en-t�tes de projet sp�cifiques (y compris Resource.h) et d�clare la 
    classe d'application CKaollaApp.

Kaolla.cpp
    Il s'agit du fichier source principal de l'application qui contient la 
    classe d'application CKaollaApp.

Kaolla.rc
    Il s'agit de la liste de toutes les ressources Microsoft Windows que le 
    programme utilise.  Elle comprend les ic�nes, les bitmaps et les curseurs 
qui sont stock�s 
    dans le sous-r�pertoire RES.  Ce fichier peut �tre modifi� directement dans 
Microsoft 
    Visual C++. Vos ressources de projet sont dans 1036.

res\Kaolla.ico
    Il s'agit d'un fichier ic�ne, qui est utilis� comme ic�ne de l'application.  
Cette 
    ic�ne est incluse par le fichier de ressource principal Kaolla.rc.

res\Kaolla.rc2
    Ce fichier contient les ressources qui ne sont pas modifi�es par Microsoft  
    Visual C++. Vous devez placer toutes les ressources non modifiables par 
    l'�diteur de ressources dans ce fichier.

/////////////////////////////////////////////////////////////////////////////

Pour la fen�tre frame principale :
    Le projet comprend une interface MFC standard.

MainFrm.h, MainFrm.cpp
    Ces fichiers contiennent la classe de frame CMainFrame 
d�riv�e de
    CFrameWnd et qui contr�le toutes les fonctionnalit�s des frames SDI.

res\Toolbar.bmp
    Ce fichier bitmap sert � cr�er des images en mosa�que pour la barre 
d'outils.
    La barre d'outils et la barre d'�tat initiales sont construites dans la 
classe  
CMainFrame. Modifiez cette image de barre d'outils avec 
l'�diteur de ressources et 
    actualisez le tableau IDR_MAINFRAME TOOLBAR dans Kaolla.rc 
pour ajouter 
    les boutons de barre d'outils.
/////////////////////////////////////////////////////////////////////////////

L'Assistant Application cr�e un type de document et une vue :

KaollaDoc.h, KaollaDoc.cpp - le document
    Ces fichiers contiennent votre classe CKaollaDoc.  Modifiez ces 
fichiers pour 
    ajouter les donn�es de document sp�ciales et impl�menter l'enregistrement et 
le 
    chargement des fichiers (via CKaollaDoc::Serialize).

KaollaView.h, KaollaView.cpp - la vue du document
    Ces fichiers contiennent votre classe CKaollaView.
    Les objets CKaollaView servent � afficher les objets CKaollaDoc.





/////////////////////////////////////////////////////////////////////////////

Autres fonctionnalit�s :

Contr�les ActiveX
    L'application comprend la prise en charge des contr�les ActiveX.

Fen�tre fractionn�e
    L'Assistant Application a ajout� la prise en charge des fen�tre fractionn�es 
pour les documents de votre application.

/////////////////////////////////////////////////////////////////////////////

Autres fichiers standard :

StdAfx.h, StdAfx.cpp
    Ces fichiers sont utilis�s pour g�n�rer un fichier d'en-t�te pr�compil� 
(PCH) 
    nomm� Kaolla.pch et un fichier de type pr�compil� nomm� 
Stdafx.obj.

Resource.h
    Il s'agit du ficher d'en-t�te standard, qui d�finit les nouveaux ID de 
ressources.
    Microsoft Visual C++ lit et met � jour ce fichier.

Kaolla.manifest
	Les fichiers manifestes d'application sont utilis�s par Windows XP pour 
d�crire les d�pendances des applications  
	sur des versions sp�cifiques des assemblys c�te � c�te. Le chargeur 
utilise ces  
	informations pour charger l'assembly appropri� � partir du cache de 
l'assembly ou directement  
	� partir de l'application. Le manifeste de l'application peut �tre inclus 
pour redistribution  
	comme fichier .manifest externe install� dans le m�me dossier que 
l'ex�cutable de l'application  
	ou �tre inclus dans l'ex�cutable sous la forme d'une ressource. 
/////////////////////////////////////////////////////////////////////////////

Autres remarques :

L'Assistant Application utilise "TODO:" pour indiquer les parties du code source 
o� 
vous devrez ajouter ou modifier du code.

Si votre application utilise les MFC dans une DLL partag�e vous devez redistribuer 
les DLL MFC. Si la langue de votre application n'est pas celle du syst�me 
d'exploitation, vous devez �galement redistribuer le fichier des  ressources 
localis�es MFC90XXX.DLL. Pour plus d'informations, consultez la section relative 
� la redistribution des applications Visual C++ dans la documentation MSDN.

/////////////////////////////////////////////////////////////////////////////
