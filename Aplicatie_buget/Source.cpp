#include "Header.h"
#include <malloc.h>
using namespace std;

int main(int nrFis, char* denumireFis[])
{
	Lucru_cu_fisiere l;
	ofstream fisBin;
	l.preluare_fisiere_text(fisBin, nrFis, denumireFis);

	Meniu m;
	//m.testare_clase();
	m.ruleaza();

}