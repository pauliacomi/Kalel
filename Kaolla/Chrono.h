#pragma once
#ifndef CHRONO_H
#define CHRONO_H


class CChrono
{
public:
	CChrono(void);
	~CChrono(void);

private:
	LARGE_INTEGER start, end, freq;
	float temps;
	LARGE_INTEGER pause,reprise;
	bool started;
	
public:
	void TopChrono();
	float TempsActuel();
	float TempsActuel(LARGE_INTEGER endEtranger);
	int TempsActuelMinute();
	int TempsActuelMinute(LARGE_INTEGER endEtranger);
	float fTempsActuelMinute();
	float fTempsActuelMinute(LARGE_INTEGER endEtranger);
	float TempsActuelMillis();
	float TempsActuelMillis(LARGE_INTEGER endEtranger);
	CString StrTempsActuel();
	void ArretTemps();
	void RepriseTemps();
};

#endif // !CHRONO_H