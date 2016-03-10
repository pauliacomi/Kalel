#ifndef DEFINE_INSTRUMENT_H
#define DEFINE_INSTRUMENT_H

// type d'instrument
#define APPAREIL_KEITHLEY	10
#define APPAREIL_MENSOR		11
#define AUCUN_APPAREIL		12

#define TYPE_INDEF			-20
#define TYPE_INEXIST		-21

#define LECTURE_BP 0
#define LECTURE_HP 1
// fonctions
#define CALO_V1_BP_V2_KEITHLEY	100
#define CALO_V1_HP_V2_KEITHLEY	108
#define CALO_V1_KEITHLEY		102
#define HP_V2_KEITHLEY			104
#define BP_V2_KEITHLEY			103
#define HP_MENSOR				200
#define BP_MENSOR				201
#define AUCUNE_FONCTION			  0

#define FCT_INDEF				-22
#define FCT_INEXIST				-23


#define KEITHLEY_V1				300
#define KEITHLEY_V2				301
#define MENSOR_VOIE				302
#define VOIE_INDEF				-30

#define nb_instrument 3
#define INDEX_INDEF	  nb_instrument

#endif