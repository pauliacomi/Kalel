////////////////////////////////////////////////
// Modifié le 7 septembre 2009
////////////////////////////////////////////////


#include "StdAfx.h"
#include "ParserXML.h"

#include <string.h>

#include <sstream>

const char* fichierXML = "Donnees.xml";

/////////////////////////////////////////////////////////////
//
//  XML


// On crée le fichierXML s'il n'existe pas
void Initialisation_XML()
{
	// On initialise la déclaration et les éléments de bases
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "iso-8859-1", "" );
	TiXmlElement * element_principal = new TiXmlElement( "experience" );
	TiXmlElement * groupe_experimentateur = new TiXmlElement( "experimentateurs" );
	TiXmlElement * groupe_gaz = new TiXmlElement( "gazs" );
	TiXmlElement * groupe_cellule = new TiXmlElement( "cellules" );
	TiXmlElement * groupe_config = new TiXmlElement( "configs" );


	element_principal->LinkEndChild( groupe_experimentateur );
	element_principal->LinkEndChild( groupe_gaz );
	element_principal->LinkEndChild( groupe_cellule );
	element_principal->LinkEndChild( groupe_config );
	doc.LinkEndChild( decl );
	doc.LinkEndChild( element_principal );

	doc.SaveFile(fichierXML);

	//Rajout des gaz
	//Rajout_Gaz(nom,symbole,masse,temp_critique,pres_critique,temp_ebullition);
	Rajout_Gaz("Azote","N2",28.0134,126.20,33.999,77.35);
	Rajout_Gaz("Helium","He",4.0026,5.20,2.275,4.22);
	Rajout_Gaz("Dioxyde de carbone","CO2",44.0099,304.00,73.825,194.50);
	Rajout_Gaz("Méthane","CH4",16.0430,190.30,45.960,111.40);
	Rajout_Gaz("Monoxyde de carbone","CO",28.0100,132.91,34.987,81.62);
	Rajout_Gaz("Oxygène","O2",31.9900,154.58,50.430,90.18);
	Rajout_Gaz("Propyne","C3H4",40.0650,402.39,56.280,249.94);
	Rajout_Gaz("Ethane","C2H6",31.9900,154.58,50.430,90.18);
	Rajout_Gaz("Propane","C3H8",44.0960,369.82,42.500,231.11);
	Rajout_Gaz("Argon","Ar",39.9480,150.86,48.980,87.29);
	Rajout_Gaz("Hydrogène","H2",2.0160,33.24,12.980,20.38);
	Rajout_Gaz("Butane","C4H10",58.1230,425.18,37.960,272.65);
	Rajout_Gaz("Propylène","C3H6",42.0800,364.75,46.100,225.43);
}


bool ConfigsExists() //Regarde si le fichier XML existe ou pas
{
	TiXmlDocument doc(fichierXML);
	return doc.LoadFile();
}


// La fonction NewElement crée un TiXmlElement ayant :
//     - Comme nom, 'nom_element', le premier paramètre
//     - En texte, 'text_element'le deuxième élément
//     résultat : <nom_element>text_element</nom_element>
//
// Pour les autres fonctions qui utilisent des nombres,
// on converti ces derniers en 'string' et on utilise la fonction
// TiXmlElement * NewElement(string nom_element, string text_element)


TiXmlElement * NewElement(string nom_element, string text_element)
{
	TiXmlElement * elem = new TiXmlElement(nom_element.c_str());
	TiXmlText * text = new TiXmlText(text_element.c_str());
	elem->LinkEndChild(text);
	return elem;
}

TiXmlElement * NewElement(string nom_element, int int_element)
{
	ostringstream text_element;
	text_element << int_element;

	return NewElement(nom_element,text_element.str());
}

TiXmlElement * NewElement(string nom_element, float float_element)
{
	ostringstream text_element;
	text_element << float_element;

	return NewElement(nom_element,text_element.str());
}

TiXmlElement * NewElement(string nom_element, double double_element)
{
	ostringstream text_element;
	text_element << double_element;

	return NewElement(nom_element,text_element.str());
}


///////////////////////////////////////////////////////////////////////////////
//
// Fonctions experimentateur


// retourne la liste des experimentateurs
vector<experimentateur> GetExperimentateurs()
{
	vector<experimentateur> t_experimentateur;
	TiXmlDocument doc(fichierXML);
	if(!doc.LoadFile()){
		return t_experimentateur; // Si le fichier XML ne s'ouvre pas, on renvoie
	}							  // la liste des expérimentateurs vide


	TiXmlHandle docHandle( &doc );
	TiXmlNode *root;

	// On prend le premier 'noeud' experimentateur
	TiXmlHandle handle_root(docHandle.FirstChild("experience").FirstChild("experimentateurs")
					.FirstChild("experimentateur").Node());

	root = handle_root.Node();

	while(root)
	{
		// On crée une variable "experimentateur" et 
		// on récupère le nom et le surnom de l'experimentateur du XML
		experimentateur exp;
		exp.nom = root->FirstChildElement("nom")->GetText();
		exp.surnom = root->FirstChildElement("surnom")->GetText();
		
		// On rajoute la variable "exp" à la fin du 'vector'
		t_experimentateur.push_back(exp);

		// On passe au noeud suivant. Si ce noeud existe, on continue, 
		// Si le noeud suivant est 'NULL', on sort de la boucle
		root = root->NextSibling();
	}

	return t_experimentateur; // On retourne le 'vector' des experimentateurs
}



bool Rajout_Experimentateur(string nom,string surnom,int index)
{
	TiXmlDocument doc(fichierXML);
	if(!doc.LoadFile()){
		return FALSE;
	}

	TiXmlHandle hdl(&doc);
	// On va sur l'élément 'experimentateurs'
	TiXmlElement *elem = hdl.FirstChild("experience").FirstChild("experimentateurs").Element();

	// On crée un nouvel element 'experimentateur' à partir des paramètres
	TiXmlElement nouvel_experimentateur("experimentateur");
	TiXmlElement * element_nom = NewElement("nom", nom);
	TiXmlElement * element_surnom = NewElement("surnom",surnom);
	nouvel_experimentateur.LinkEndChild(element_nom);
	nouvel_experimentateur.LinkEndChild(element_surnom);

	// On va parcourir tous les éléments 'experimentateur' du "groupe" 'experimentateurs'
	TiXmlElement *elem_parcours = hdl.FirstChild("experience").FirstChild("experimentateurs").
									FirstChild("experimentateur").Element();

	for(int i=0;i<index && (elem_parcours);i++)
		elem_parcours = elem_parcours->NextSiblingElement();
	
	// Si l'index est >= au nombre d'élément 'experimentateur'
	// on met le nouvel élément à la fin
	if ((index==-1) || (!elem_parcours))
		elem->InsertEndChild(nouvel_experimentateur);
	// Sinon, on le met juste avant l'élément occupant actuellement 
	// la place numéro "index"
	else
		elem->InsertBeforeChild(elem_parcours,nouvel_experimentateur);

	return doc.SaveFile(fichierXML); // retourne TRUE si tout a été bien enregistré
}


// analogue à la fonction si dessus...
bool Rajout_Experimentateur(experimentateur new_exp,int index)
{
	return Rajout_Experimentateur(new_exp.nom,new_exp.surnom,index);
}


//index : position dans le vector<experimentateur utilisé
bool Suppression_Experimentateur(int index)
{
	TiXmlDocument doc(fichierXML);
	if(!doc.LoadFile()){
		return FALSE;
	}

	TiXmlHandle hdl(&doc);
	// elem : l'élément que l'on va supprimer. On va parcourir les éléments dans 'experimentateurs'
	//        jusqu'à ce qu'on trouve l'élément à la place 'index'
	// f : l'élément 'experimentateurs' d'où on effacera l'élément adéquat
	TiXmlElement *elem = hdl.FirstChild("experience").FirstChild("experimentateurs").
							FirstChild("experimentateur").Element();
	TiXmlElement *f = hdl.FirstChild("experience").FirstChild("experimentateurs").Element();

	for(int i=0;i<index;i++)
		elem=elem->NextSiblingElement();
	// une fois l'élément trouvé, 'f' va l'éliminer
	f->RemoveChild(elem);
	return doc.SaveFile(fichierXML); // retourne TRUE si tout a été enregistré
}


// Pour modifier un élément, on supprime l'ancien pour rajouter le nouveau.
bool Modif_Experimentateur(string nom, string surnom,int index)
{
	Suppression_Experimentateur(index);
	return Rajout_Experimentateur(nom,surnom,index);
}

bool Modif_Experimentateur(experimentateur new_exp,int index)
{
	Suppression_Experimentateur(index);
	return Rajout_Experimentateur(new_exp,index);
}




//////////////////////////////////////////////////////////////////////////
//
// Fonctions Gaz

// Analogues aux fonctions Experimentateur

vector<gaz> GetGazs()
{
	vector<gaz> t_gaz;
	TiXmlDocument doc(fichierXML);
	if(!doc.LoadFile()){
		return t_gaz;
	}

	TiXmlHandle docHandle( &doc );
	TiXmlNode *root;
	TiXmlHandle handle_root(docHandle.FirstChild("experience").FirstChild("gazs")
					.FirstChild("gaz").Node());

	root = handle_root.Node();

	while(root)
	{
		gaz m_gaz;
		m_gaz.nom = root->FirstChildElement("nom")->GetText();
		m_gaz.symbole = root->FirstChildElement("symbole")->GetText();
		m_gaz.masse_moleculaire = atof(root->FirstChildElement("masse_moleculaire")->GetText());
		m_gaz.temperature_critique = atof(root->FirstChildElement("temperature_critique")->GetText());
		m_gaz.pression_critique = atof(root->FirstChildElement("pression_critique")->GetText());
		m_gaz.temperature_ebullition = atof(root->FirstChildElement("temperature_ebullition")->GetText());
		m_gaz.omega = atof(root->FirstChildElement("omega")->GetText());

		t_gaz.push_back(m_gaz);
		root = root->NextSibling();
	}
	return t_gaz;
}



bool Rajout_Gaz(string nom,string symbole,double masse,double temp_critique, 
				double pres_critique, double temp_ebullition, int index)
{
	TiXmlDocument doc(fichierXML);
	if(!doc.LoadFile()){
		return FALSE;
	}

	TiXmlHandle hdl(&doc);
	TiXmlElement *elem = hdl.FirstChild("experience").FirstChild("gazs").Element();

	TiXmlElement nouveau_gaz("gaz");

	TiXmlElement * element_nom = NewElement("nom",nom);
	TiXmlElement * element_symbole = NewElement("symbole",symbole);
	TiXmlElement * element_masse = NewElement("masse_moleculaire",masse);
	TiXmlElement * element_temp_critique = NewElement("temperature_critique",temp_critique);
	TiXmlElement * element_pres_critique = NewElement("pression_critique",pres_critique);
	TiXmlElement * element_temp_ebullition = NewElement("temperature_ebullition",temp_ebullition);
	TiXmlElement * element_omega = NewElement("omega",temp_ebullition/temp_critique);

	nouveau_gaz.LinkEndChild(element_nom);
	nouveau_gaz.LinkEndChild(element_symbole);
	nouveau_gaz.LinkEndChild(element_masse);
	nouveau_gaz.LinkEndChild(element_temp_critique);
	nouveau_gaz.LinkEndChild(element_pres_critique);
	nouveau_gaz.LinkEndChild(element_temp_ebullition);
	nouveau_gaz.LinkEndChild(element_omega);

	TiXmlElement *elem_parcours = hdl.FirstChild("experience").FirstChild("gazs").
									FirstChild("gaz").Element();

	for(int i=0;i<index && (elem_parcours);i++)
		elem_parcours = elem_parcours->NextSiblingElement();
	if ((index==-1) || (!elem_parcours))
		elem->InsertEndChild(nouveau_gaz);
	else
		elem->InsertBeforeChild(elem_parcours,nouveau_gaz);

	return doc.SaveFile(fichierXML);
}


bool Rajout_Gaz(gaz new_gaz,int index)
{
	return Rajout_Gaz(new_gaz.nom,new_gaz.symbole,new_gaz.masse_moleculaire,new_gaz.temperature_critique,
		new_gaz.pression_critique,new_gaz.temperature_ebullition,index);
}



bool Suppression_Gaz(int index)
{
	TiXmlDocument doc(fichierXML);
	if(!doc.LoadFile()){
		return FALSE;
	}

	TiXmlHandle hdl(&doc);
	TiXmlElement *elem = hdl.FirstChild("experience").FirstChild("gazs").
							FirstChild("gaz").Element();
	TiXmlElement *f = hdl.FirstChild("experience").FirstChild("gazs").Element();

	for(int i=0;i<index;i++)
		elem=elem->NextSiblingElement();
	f->RemoveChild(elem);
	return doc.SaveFile(fichierXML);
}



bool Modif_Gaz(string nom,string symbole,double masse,double temp_critique, 
				double pres_critique, double temp_ebullition, int index)
{
	Suppression_Gaz(index);
	return Rajout_Gaz(nom,symbole,masse,temp_critique,
					  pres_critique,temp_ebullition,index);
}


bool Modif_Gaz(gaz new_gaz,int index)
{
	Suppression_Gaz(index);
	return Rajout_Gaz(new_gaz,index);
}


//////////////////////////////////////////////////////////////////////////////////
// Fonctions Cellule


vector<cellule> GetCellules()
{
	vector<cellule> t_cellule;
	TiXmlDocument doc(fichierXML);
	if(!doc.LoadFile()){
		return t_cellule;
	}

	TiXmlHandle docHandle( &doc );
	TiXmlNode *root;
	TiXmlHandle handle_root(docHandle.FirstChild("experience").FirstChild("cellules")
					.FirstChild("cellule").Node());

	root = handle_root.Node();

	while(root)
	{
		cellule cell;
		cell.numero = root->FirstChildElement("numero")->GetText();
		cell.volume_total = atof(root->FirstChildElement("volume_total")->GetText());
		cell.volume_calo = atof(root->FirstChildElement("volume_calo")->GetText());

		t_cellule.push_back(cell);

		root = root->NextSibling();
	}

	return t_cellule;
}



bool Rajout_Cellule(string num, double total, double calo, int index)
{
	TiXmlDocument doc(fichierXML);
	if(!doc.LoadFile()){
		return FALSE;
	}

	TiXmlHandle hdl(&doc);
	TiXmlElement *elem = hdl.FirstChild("experience").FirstChild("cellules").Element();


	TiXmlElement nouvel_cellule("cellule");

	TiXmlElement * element_numero = NewElement("numero", num);
	TiXmlElement * element_total = NewElement("volume_total",total);
	TiXmlElement * element_calo = NewElement("volume_calo",calo);

	nouvel_cellule.LinkEndChild(element_numero);
	nouvel_cellule.LinkEndChild(element_total);
	nouvel_cellule.LinkEndChild(element_calo);

	TiXmlElement *elem_parcours = hdl.FirstChild("experience").FirstChild("cellules").
									FirstChild("cellule").Element();

	for(int i=0;i<index && (elem_parcours);i++)
		elem_parcours = elem_parcours->NextSiblingElement();

	if ((index==-1) || (!elem_parcours))
		elem->InsertEndChild(nouvel_cellule);
	else
		elem->InsertBeforeChild(elem_parcours,nouvel_cellule);

	return doc.SaveFile(fichierXML);
}


bool Rajout_Cellule(cellule new_cellule,int index)
{
	return Rajout_Cellule(new_cellule.numero,new_cellule.volume_total,new_cellule.volume_calo,index);
}


//index : position dans le vector<experimentateur utilisé
bool Suppression_Cellule(int index)
{
	TiXmlDocument doc(fichierXML);
	if(!doc.LoadFile()){
		return FALSE;
	}

	TiXmlHandle hdl(&doc);
	TiXmlElement *elem = hdl.FirstChild("experience").FirstChild("cellules").
							FirstChild("cellule").Element();
	TiXmlElement *f = hdl.FirstChild("experience").FirstChild("cellules").Element();

	for(int i=0;i<index;i++)
		elem=elem->NextSiblingElement();
	f->RemoveChild(elem);
	return doc.SaveFile(fichierXML);
}

bool Modif_Cellule(string num, double total, double calo, int index)
{
	Suppression_Cellule(index);
	return Rajout_Cellule(num,total,calo,index);
}

bool Modif_Cellule(cellule new_cellule,int index)
{
	Suppression_Cellule(index);
	return Rajout_Cellule(new_cellule,index);
}



//////////////////////////////////////////////////////////////////////////////
//
// Fonction doublon

bool Doublon(TiXmlHandle handle_root, string valeur, string type)
{
	/*
	while (elem_parcours)
	{

		elem_parcours = elem_parcours->NextSiblingElement();
	}
	*/

	TiXmlNode *root = handle_root.Node();

	while(root)
	{
		/*
		const char* truc1 = "voilà";
		char* truc2 = "voilà";
		bool essai = (truc1==truc2);
		const char* bidon = root->FirstChildElement(type.c_str())->GetText();
		const char* bidon2=valeur.c_str();
		//if (valeur.c_str() == bidon)
		if(bidon2 == bidon)
			return TRUE;
		*/
		
		char * bidon3 = _strdup(root->FirstChildElement(type.c_str())->GetText());
		char * bidon4 = _strdup(valeur.c_str());

		string temp_nom(_strdup(root->FirstChildElement(type.c_str())->GetText()));

		//if (strdup(valeur.c_str()) == strdup(root->FirstChildElement(type.c_str())->GetText()))
		if (valeur == temp_nom)
		{
			return TRUE;
		}
		

		// On passe au noeud suivant. Si ce noeud existe, on continue, 
		// Si le noeud suivant est 'NULL', on sort de la boucle
		root = root->NextSibling();
	}

	return FALSE;
}

/*
bool DoublonNomExperimentateur(string valeur)
{
	TiXmlDocument doc(fichierXML);
	if(!doc.LoadFile()){
		return FALSE;
	}

	TiXmlHandle hdl(&doc);
	TiXmlHandle handle_root(hdl.FirstChild("experience").FirstChild("experimentateurs")
					.FirstChild("experimentateur").Node());

	return Doublon(handle_root, valeur, "nom");
}
*/

bool DoublonNomExperimentateur(string valeur)
{
	return DoublonExperimentateur(valeur, "nom");
}

bool DoublonSurnomExperimentateur(string valeur)
{
	return DoublonExperimentateur(valeur, "surnom");
}


bool DoublonExperimentateur(string valeur, string type)
{
	TiXmlDocument doc(fichierXML);
	if(!doc.LoadFile()){
		return FALSE;
	}

	TiXmlHandle hdl(&doc);
	TiXmlHandle handle_root(hdl.FirstChild("experience").FirstChild("experimentateurs")
					.FirstChild("experimentateur").Node());

	return Doublon(handle_root, valeur, type);
}




bool DoublonNomGaz(string valeur)
{
	return DoublonGaz(valeur, "nom");
}

bool DoublonSymboleGaz(string valeur)
{
	return DoublonGaz(valeur, "symbole");
}


bool DoublonGaz(string valeur, string type)
{
	TiXmlDocument doc(fichierXML);
	if(!doc.LoadFile()){
		return FALSE;
	}

	TiXmlHandle hdl(&doc);
	TiXmlHandle handle_root(hdl.FirstChild("experience").FirstChild("gazs")
					.FirstChild("gaz").Node());

	return Doublon(handle_root, valeur, type);
}



bool DoublonNumeroCellule(string num)
{
	TiXmlDocument doc(fichierXML);
	if(!doc.LoadFile()){
		return FALSE;
	}

	TiXmlHandle hdl(&doc);
	TiXmlHandle handle_root(hdl.FirstChild("experience").FirstChild("cellules")
					.FirstChild("cellule").Node());

	/*
	// créer un flux de sortie
    ostringstream oss;
    // écrire un nombre dans le flux
    oss << num;
    // récupérer une chaîne de caractères
    string valeur = oss.str();
	*/

	//return Doublon(handle_root, valeur, "numero");
	return Doublon(handle_root, num, "numero");
}