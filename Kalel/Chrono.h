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
	LARGE_INTEGER pause, reprise;
	float temps;
	bool started;
	bool paused;
	
public:
	void TopChrono();
	double TempsActuel();
	double TempsActuel(LARGE_INTEGER endEtranger);
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