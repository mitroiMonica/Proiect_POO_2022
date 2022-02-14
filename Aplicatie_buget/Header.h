#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <ctime>
#include <string>

using namespace std;
class Abstracta {
public:
	virtual void afisare_cota_TVA() = 0;
	virtual void afisare_tip() = 0;
};

enum Necesitate {
	DelocNecesar,
	PutinNecesar,
	FoarteNecesar
};
//Clasa parinte comun din ierarhia claselor - cea de pe nivelul 1 din familia de clase (cea de pe nivelul 0 este cea abstracta)
class Comun :public Abstracta {
	float TVA = 0.19; //utilizatorul nu trebuie sa introduca TVA-ul nici macar in fisierele text/CSV
	Necesitate nivel_necesitate = FoarteNecesar;
	static int vector_necesitate[3]; //vector care retine de cate ori utilizatorul a introdus delocNecesar, PutinNecesar sau FoarteNecesar care va ajuta la sugestii

public:
	Comun()
	{
		vector_necesitate[this->nivel_necesitate] += 1;
	}
	Comun(float TVA, Necesitate nivel_necesitate)
	{
		this->TVA = TVA;
		this->nivel_necesitate = nivel_necesitate;
		vector_necesitate[this->nivel_necesitate] += 1;
	}
	Comun(Necesitate nivel_necesitate)
	{
		this->nivel_necesitate = nivel_necesitate;
		vector_necesitate[this->nivel_necesitate] += 1;
	}
	Comun(const Comun& c)
	{
		this->TVA = c.TVA;
		this->nivel_necesitate = c.nivel_necesitate;
		vector_necesitate[this->nivel_necesitate] += 1;
	}
	Comun& operator=(const Comun& c)
	{
		this->TVA = c.TVA;
		vector_necesitate[this->nivel_necesitate] -= 1;
		this->nivel_necesitate = c.nivel_necesitate;
		vector_necesitate[this->nivel_necesitate] += 1;

		return*this;
	}
	~Comun()
	{
		//vector_necesitate[this->nivel_necesitate] -= 1;
	}
	//getteri
	static void get_vector_necesitate()
	{
		for (int i = 0; i < 3; i++)
			cout << vector_necesitate[i] << "\t";
	}
	float get_TVA()
	{
		return this->TVA;
	}
	Necesitate get_necesitate()
	{
		return this->nivel_necesitate;
	}
	//setteri
	void set_cota_TVA(float TVA)
	{
		this->TVA = TVA;
	}
	virtual void set_TVA()
	{
		this->TVA = 0.19;
	}
	//Metoda care ii arata utilizatorului ce cote foloseste programul. 
	//Acesta il va si ajuta pe utilizator ca in cazul in care doreste sa modifice el cota 
	//(momentan nu am implementat asta) sa stie ce cota sa utilizeze pentru produsul/serviciul cumparat.
	//Este logic ca pentru fiecare categorie de produse/servicii cota sa poata sa difere de aceea este o metoda virtuala.
	void afisare_cota_TVA()
	{
		cout << "\nCota generala este de " << get_TVA() * 100 << "%.";
	}
	void set_necesitate(Necesitate nivel_necesitate)
	{
		vector_necesitate[this->nivel_necesitate] -= 1;
		this->nivel_necesitate = nivel_necesitate;
		vector_necesitate[this->nivel_necesitate] += 1;
	}
	virtual void necesitate_default()
	{
		this->nivel_necesitate = FoarteNecesar;
	}
	void afisare_tip()
	{
		cout << "\nElement general\n";
	}
	static int get_necesitate_pe_cat(Necesitate n)
	{
		return vector_necesitate[n];
	}
	void stergeNecesitate()
	{
		vector_necesitate[this->nivel_necesitate] -= 1;
	}
	friend ostream& operator<<(ostream&, Comun&);
	friend istream& operator>>(istream&, Comun&);
	friend ofstream& operator<<(ofstream&, Comun&);
	friend ifstream& operator>>(ifstream&, Comun&);
	friend class Lucru_cu_fisiere;
};
int Comun::vector_necesitate[3] = { 0 };

ostream& operator<<(ostream& out, Comun& c)
{
	//out << "\nTVA: " << c.TVA * 100 << "%";
	out << "\nNivel de necesitate: " << c.nivel_necesitate << endl;
	return out;
}
istream& operator>>(istream& in, Comun& c)
{
	//cout << "TVA: "; in >> c.TVA;
	cout << "Nivel de necesitate: [0-Deloc necesar; 1-Putin necesar; 2-Foarte necesar] => ";
	c.vector_necesitate[c.nivel_necesitate] -= 1;
	int niv;
	in >> niv;
	switch (niv)
	{
	case 0:
		c.nivel_necesitate = DelocNecesar;
		break;
	case 1:
		c.nivel_necesitate = PutinNecesar;
		break;
	case 2:
		c.nivel_necesitate = FoarteNecesar;
		break;
	default:
		cout << "Nu ai ales niciuna din cele trei categorii!\n";
		c.nivel_necesitate = FoarteNecesar;
		break;
	}
	c.vector_necesitate[c.nivel_necesitate] += 1;
	return in;
}
ofstream& operator<<(ofstream& out, Comun& c)
{
	//out << c.TVA;
	out << c.nivel_necesitate << endl;
	return out;
}
ifstream& operator>>(ifstream& in, Comun& c)
{
	c.vector_necesitate[c.nivel_necesitate] -= 1;
	int niv;
	in >> niv;
	switch (niv)
	{
	case 0:
		c.nivel_necesitate = DelocNecesar;
		break;
	case 1:
		c.nivel_necesitate = PutinNecesar;
		break;
	case 2:
		c.nivel_necesitate = FoarteNecesar;
		break;
	default:
		cout << "Nu ai ales niciuna din cele trei categorii!\n";
		c.nivel_necesitate = FoarteNecesar;
		break;
	}
	c.vector_necesitate[c.nivel_necesitate] += 1;
	return in;
}

//in functie de nivelul de necesitate(daca este intre x si y) incercati sa renuntati la produsele care nu va sunt strict necesare si sa ... 
//in general produsele cumparate au fost deloc necesare. Daca doriti sa economisiti... La facilitati/rapoarte sa am chestia asta cu "Sugestii"
enum CategorieAlimente {
	Fructe,
	Legume,
	Lactate,
	Carne,
	Derivate_din_cereale,
	Alta
};
//CLASA ALIMENTE:
class Alimente :public Comun {
	char* denumire = nullptr;
	float pret = 0;
	CategorieAlimente categorie = Alta;
	static float cheltuialaPeCategorie[6];
public:
	Alimente()
		:Comun()
	{}
	Alimente(const char* denumire, float pret)
		:Comun()
	{
		if (denumire != nullptr)
		{
			this->denumire = new char[strlen(denumire) + 1];
			strcpy(this->denumire, denumire);
		}
		else cout << "Alimentul trebuie sa aiba o denumire!";
		if (pret > 0)
			this->pret = pret;
		else cout << "Pretul nu poate fi negativ";
		cheltuialaPeCategorie[5] += pret;
	}
	Alimente(const char* denumire, float pret, CategorieAlimente categorie)
		:Comun()
	{
		if (denumire != nullptr)
		{
			this->denumire = new char[strlen(denumire) + 1];
			strcpy(this->denumire, denumire);
		}
		else cout << "Alimentul trebuie sa aiba o denumire!";
		if (pret > 0)
			this->pret = pret;
		else cout << "Pretul nu poate fi negativ";
		this->categorie = categorie;
		cheltuialaPeCategorie[categorie] += pret;
	}
	Alimente(const char* denumire, float pret, CategorieAlimente categorie, Necesitate nivel_necesitate)
		:Comun(nivel_necesitate)
	{
		if (denumire != nullptr)
		{
			this->denumire = new char[strlen(denumire) + 1];
			strcpy(this->denumire, denumire);
		}
		else cout << "Alimentul trebuie sa aiba o denumire!";
		if (pret > 0)
			this->pret = pret;
		else cout << "Pretul nu poate fi negativ";
		this->categorie = categorie;
		cheltuialaPeCategorie[categorie] += pret;
	}
	Alimente(const char* denumire, float pret, CategorieAlimente categorie, float TVA, Necesitate nivel_necesitate)
		:Comun(TVA, nivel_necesitate)
	{
		if (denumire != nullptr)
		{
			this->denumire = new char[strlen(denumire) + 1];
			strcpy(this->denumire, denumire);
		}
		else cout << "Alimentul trebuie sa aiba o denumire!";
		if (pret > 0)
			this->pret = pret;
		else cout << "Pretul nu poate fi negativ";
		this->categorie = categorie;
		cheltuialaPeCategorie[categorie] += pret;
	}
	Alimente(const Alimente& a)
		:Comun(a)
	{
		if (a.denumire != nullptr)
		{
			this->denumire = new char[strlen(a.denumire) + 1];
			strcpy(this->denumire, a.denumire);
		}
		if (a.pret > 0)
			this->pret = a.pret;
		this->categorie = a.categorie;
		cheltuialaPeCategorie[this->categorie] += pret;
	}
	~Alimente()
	{
		if (this->denumire != nullptr)
			delete[] denumire;
		//cheltuialaPeCategorie[this->categorie] -= this->pret;
	}
	Alimente& operator=(const Alimente& a)
	{
		if (this->denumire != nullptr)
			delete[] this->denumire;
		if (a.denumire != nullptr)
		{
			this->denumire = new char[strlen(a.denumire) + 1];
			strcpy(this->denumire, a.denumire);
		}
		else this->denumire = nullptr;
		cheltuialaPeCategorie[this->categorie] -= this->pret;
		this->categorie = a.categorie;
		this->pret = a.pret;
		cheltuialaPeCategorie[this->categorie] += this->pret;
		Comun::operator=(a);
		return *this;
	}
	//CALCULEAZA CHELTUIALA TOTALA A ALIMENTELOR
	static float calculeazaSumaTotala()
	{
		return cheltuialaPeCategorie[0] + cheltuialaPeCategorie[1] +
			cheltuialaPeCategorie[2] + cheltuialaPeCategorie[3] + cheltuialaPeCategorie[4] + cheltuialaPeCategorie[5];
	}
	const char* get_denumire()
	{
		if (this->denumire != nullptr)
			return this->denumire;
		else return "-";
	}
	float get_pret()
	{
		return this->pret;
	}
	CategorieAlimente get_categorie()
	{
		return this->categorie;
	}
	static float cheltuiala_pe_categorie(CategorieAlimente categorie)
	{
		return cheltuialaPeCategorie[categorie];
	}
	void set_denumire(const char* denumire)
	{
		if (denumire != nullptr)
		{
			if (this->denumire != nullptr)
				delete[] this->denumire;
			this->denumire = new char[strlen(denumire) + 1];
			strcpy(this->denumire, denumire);
		}
		else cerr << "Alimentul trebuie sa aiba o denumire!";
	}
	void set_pret(float pret)
	{
		if (pret >= 0)
		{
			float dif = pret - this->pret;
			cheltuialaPeCategorie[this->categorie] += dif;
			this->pret = pret;
		}
		else
			cerr << "Pretul nu poate fi negativ!";
	}
	void set_pret_CSV(float pret)
	{
		if (pret >= 0)
		{
			this->pret = pret;
		}
		else
			cerr << "Pretul nu poate fi negativ!";
	}
	void set_categorie(CategorieAlimente cat)
	{
		if (cat != Fructe && cat != Legume && cat != Lactate && cat != Carne && cat != Derivate_din_cereale && cat != Alta)
			cerr << "Nu exista categoria introdusa!";
		else
		{
			cheltuialaPeCategorie[this->categorie] -= this->pret;
			this->categorie = cat;
			cheltuialaPeCategorie[this->categorie] += this->pret;
		}
	}
	void set_categorie_CSV(CategorieAlimente cat)
	{
		if (cat != Fructe && cat != Legume && cat != Lactate && cat != Carne && cat != Derivate_din_cereale && cat != Alta)
			cerr << "Nu exista categoria introdusa!";
		else
		{
			this->categorie = cat;
			cheltuialaPeCategorie[this->categorie] += this->pret;
		}
	}

	//AFISEAZA CHELTUIALA TOTALA PE O CATEGORIE
	void operator[](int index)
	{
		if (index >= 0 && index < 3)
			cout << "\nCheltuiala totala pe categoria " << index << " este " << cheltuialaPeCategorie[index];
		else
			cerr << "\nOut of range!";

	}
	friend ostream& operator<<(ostream&, Alimente&);
	friend istream& operator>>(istream&, Alimente&);
	friend ofstream& operator<<(ofstream&, Alimente&);
	friend ifstream& operator>>(ifstream&, Alimente&);
	//CALCULEAZA SUMA DINTRE DOUA ALIMENTE:
	float operator+(const Alimente a)
	{
		return this->pret + a.pret;
	}
	//ADAUGA LA PRETUL UNUI ALIMENT UN LEU: 
	//(preincrementare)
	Alimente& operator++()
	{
		this->pret++;
		cheltuialaPeCategorie[this->categorie]++;
		return *this;
	}
	//(postincrementare)
	Alimente operator++(int)
	{
		Alimente copie = *this;
		this->pret++;
		cheltuialaPeCategorie[this->categorie]++;
		return copie;
	}
	// SCADE LA PRETUL UNUI ALIMENT UN LEU :
	Alimente& operator--()
	{
		this->pret--;
		cheltuialaPeCategorie[this->categorie]--;
		return *this;
	}
	//(postincrementare)
	Alimente operator--(int)
	{
		Alimente copie = *this;
		this->pret--;
		cheltuialaPeCategorie[this->categorie]--;
		return copie;
	}
	//cast pentru Aliment
	explicit operator float()
	{
		return this->pret;
	}
	//Supraincarcarea operatorului ! care schimba categoria unui aliment in Alta
	Alimente& operator!()
	{
		cheltuialaPeCategorie[this->categorie] -= this->pret;
		this->categorie = Alta;
		cheltuialaPeCategorie[this->categorie] += this->pret;
		return *this;
	}
	//Supraincarcarea operatorului < care compara preturile a doua alimente
	void operator<(Alimente a)
	{
		if (this->pret < a.pret && this->denumire != nullptr && a.denumire != nullptr)
			cout << "\nPretul alimentului " << this->denumire
			<< " este mai mic decat cel al alimentului " << a.denumire;
		else if (a.pret < this->pret && this->denumire != nullptr && a.denumire != nullptr)
			cout << "\nPretul alimentului " << a.denumire
			<< " este mai mic decat cel al alimentului " << this->denumire;
		else if (this->denumire != nullptr && a.denumire != nullptr)
			cout << "\nPretul alimentului " << a.denumire
			<< " este egal cu cel al alimentului " << this->denumire;
		else cerr << "\nCel putin un aliment nu are denumire!";
	}
	//Operatorul == care testeaza egalitatea dintre 2 obiecte 
	bool operator==(Alimente a)
	{
		if ((this->denumire == nullptr && a.denumire != nullptr) || (this->denumire != nullptr && a.denumire == nullptr))
			return false;
		else if (this->denumire != nullptr && a.denumire != nullptr)
			if (strcmp(this->denumire, a.denumire) != 0)
				return false;
		if (this->pret != a.pret)
			return false;
		if (this->categorie != a.categorie)
			return false;
		return true;
	}
	void StergeObiect()
	{
		cheltuialaPeCategorie[this->categorie] -= this->pret;
		this->pret = 0;
		this->categorie = Alta;
		if (this->denumire != nullptr)
		{
			delete[] this->denumire;
			this->denumire = nullptr;
		}
	}
	void sterge_pret_aliment()
	{
		cheltuialaPeCategorie[this->categorie] -= this->pret;
	}
	void set_TVA()
	{
		this->set_cota_TVA(0.09);
	}
	void necesitate_default()
	{
		this->set_necesitate(FoarteNecesar);
	}
	void afisare_cota_TVA()
	{
		cout << "\nPentru majoritatea produselor sau serviciilor din aceasta categorie cota TVA este de 9%. Exceptiile sunt:"
			<< "\n\tBauturile alcoolice, destinate consumului uman sau animal - TVA 19%";

	}
	void afisare_tip()
	{
		cout << "\nAlimente\n";
	}
	friend class Lucru_cu_fisiere;
};
float Alimente::cheltuialaPeCategorie[6] = { 0 };
ostream& operator<<(ostream& out, Alimente& a)
{
	out << "\n---------------------";
	out << "\nCategorie: " << a.categorie << "\nDenumire aliment: ";
	if (a.denumire != nullptr)
		out << a.denumire;
	else
		out << "-";
	out << "\nPret: " << a.pret;
	//out << (Comun&)a;
	out << (Comun&)a;
	return out;
}
istream& operator>>(istream& in, Alimente& a)
{
	cout << "\nDenumire: ";
	char buffer[10000];
	in >> ws;
	in.getline(buffer, 10000, '\n');
	if (a.denumire != nullptr)
		delete[] a.denumire;
	if (buffer != nullptr)
	{
		a.denumire = new char[strlen(buffer) + 1];
		strcpy(a.denumire, buffer);
	}
	else {
		cerr << "Alimentul nu are denumire!\n";
		a.denumire = nullptr;
	}
	a.cheltuialaPeCategorie[a.categorie] -= a.pret;
	float pret;
	cout << "Pret: ";
	in >> pret;
	if (pret > 0)
		a.pret = pret;
	else {
		a.pret = 0;
		cerr << "Pretul nu poate fi negativ!\n";
	}
	int cat;
	cout << "Categorie: [0-Fructe; 1-Legume; 2-Lactate; 3-Carne; 4-Derivate din cereale; 5-Alta] => ";
	in >> cat;
	switch (cat)
	{
	case 0:
		a.categorie = Fructe;
		break;
	case 1:
		a.categorie = Legume;
		break;
	case 2:
		a.categorie = Lactate;
		break;
	case 3:
		a.categorie = Carne;
		break;
	case 4:
		a.categorie = Derivate_din_cereale;
		break;
	case 5:
		a.categorie = Alta;
		break;
	default:
		cerr << "Nu ai ales niciuna din cele sase categorii!\n";
		a.categorie = Alta;
		break;
	}
	a.cheltuialaPeCategorie[a.categorie] += a.pret;
	in >> (Comun&)a;
	return in;
}
ofstream& operator<<(ofstream& out, Alimente& a)
{
	if (a.denumire != nullptr)
		out << a.denumire << endl;
	else
		out << "-" << endl;
	out << a.pret << endl;
	out << a.categorie << endl;
	out << (Comun&)a;
	return out;
}
ifstream& operator>>(ifstream& in, Alimente& a)
{
	char buffer[10000];
	in >> ws;
	in.getline(buffer, 10000, '\n');
	if (a.denumire != nullptr)
		delete[] a.denumire;
	if (buffer != nullptr)
	{
		a.denumire = new char[strlen(buffer) + 1];
		strcpy(a.denumire, buffer);
	}
	else
		a.denumire = nullptr;
	a.cheltuialaPeCategorie[a.categorie] -= a.pret;
	in >> a.pret;
	if (a.pret < 0)
		a.pret = 0;
	int cat;
	in >> cat;
	switch (cat)
	{
	case 0:
		a.categorie = Fructe;
		break;
	case 1:
		a.categorie = Legume;
		break;
	case 2:
		a.categorie = Lactate;
		break;
	case 3:
		a.categorie = Carne;
		break;
	case 4:
		a.categorie = Derivate_din_cereale;
		break;
	case 5:
		a.categorie = Alta;
		break;
	default:
		cerr << "Nu ai ales niciuna din cele sase categorii!\n";
		a.categorie = Alta;
		break;
	}
	in >> (Comun&)a;
	a.cheltuialaPeCategorie[a.categorie] += a.pret;
	return in;
}

enum CategorieUtilitati {
	Energie,
	Gaz,
	Apa,
	Telefonie,
	Internet,
	TV,
	Asigurari,
	Altele
};
//CLASA UTILITAIT:
class Utilitati :public Comun {
	char* denumire = nullptr;
	float pret = 0;
	CategorieUtilitati categorie = Altele;
	static float cheltuialaPeCategorie[8];
public:
	Utilitati()
		:Comun()
	{}
	Utilitati(float pret, CategorieUtilitati categorie)
		:Comun()
	{
		if (pret >= 0)
			this->pret = pret;
		else
			cerr << "Pretul nu poate fi negativ!";
		this->categorie = categorie;
		cheltuialaPeCategorie[categorie] += pret;
	}
	Utilitati(const char* denumire, float pret, CategorieUtilitati categorie, Necesitate nivel_necesitate)
		:Comun(nivel_necesitate)
	{
		if (denumire != nullptr)
		{
			this->denumire = new char[strlen(denumire) + 1];
			strcpy(this->denumire, denumire);
		}
		else cerr << "Trebuie sa introduceti si denumirea firmei ce va ofera utilitatea!";
		if (pret >= 0)
			this->pret = pret;
		else
			cerr << "Pretul nu poate fi negativ!";
		this->categorie = categorie;
		cheltuialaPeCategorie[categorie] += pret;
	}
	Utilitati(const char* denumire, float pret, CategorieUtilitati categorie)
		:Comun()
	{
		if (denumire != nullptr)
		{
			this->denumire = new char[strlen(denumire) + 1];
			strcpy(this->denumire, denumire);
		}
		else cerr << "Trebuie sa introduceti si denumirea firmei ce va ofera utilitatea!";
		if (pret >= 0)
			this->pret = pret;
		else
			cerr << "Pretul nu poate fi negativ!";
		this->categorie = categorie;
		cheltuialaPeCategorie[categorie] += pret;
	}
	Utilitati(const char* denumire, float pret, CategorieUtilitati categorie, float TVA, Necesitate nivel_necesitate)
		:Comun(TVA, nivel_necesitate)
	{
		if (denumire != nullptr)
		{
			this->denumire = new char[strlen(denumire) + 1];
			strcpy(this->denumire, denumire);
		}
		else cerr << "Trebuie sa introduceti si denumirea firmei ce va ofera utilitatea!";
		if (pret >= 0)
			this->pret = pret;
		else
			cerr << "Pretul nu poate fi negativ!";
		this->categorie = categorie;
		cheltuialaPeCategorie[categorie] += pret;
	}
	Utilitati(const Utilitati& u)
		:Comun(u)
	{
		if (u.denumire != nullptr)
		{
			this->denumire = new char[strlen(u.denumire) + 1];
			strcpy(this->denumire, u.denumire);
		}
		if (u.pret >= 0)
			this->pret = u.pret;
		this->categorie = u.categorie;
		cheltuialaPeCategorie[this->categorie] += this->pret;
	}
	~Utilitati()
	{
		if (this->denumire != nullptr)
			delete[] this->denumire;
		//cheltuialaPeCategorie[this->categorie] -= this->pret;
	}
	Utilitati& operator=(const Utilitati& u)
	{
		if (this->denumire != nullptr)
			delete[] this->denumire;
		if (u.denumire != nullptr)
		{
			this->denumire = new char[strlen(u.denumire) + 1];
			strcpy(this->denumire, u.denumire);
		}
		else this->denumire = nullptr;
		cheltuialaPeCategorie[this->categorie] -= this->pret;
		this->categorie = u.categorie;
		this->pret = u.pret;
		cheltuialaPeCategorie[this->categorie] += this->pret;
		Comun::operator=(u);
		return *this;
	}
	const char* get_denumire()
	{
		return this->denumire;
	}
	float get_pret()
	{
		return this->pret;
	}
	CategorieUtilitati get_categorie()
	{
		return this->categorie;
	}
	static float get_cheltuialaPeCategorie(CategorieUtilitati cat)
	{
		return cheltuialaPeCategorie[cat];
	}
	void set_denumire(const char* denumire)
	{
		if (denumire != nullptr)
		{
			if (this->denumire != nullptr)
				delete[] this->denumire;
			this->denumire = new char[strlen(denumire) + 1];
			strcpy(this->denumire, denumire);
		}
		else
			cerr << "Utilitatea trebuie sa aiba denumirea firmei care v-o ofera!";
	}
	void set_pret(float pret)
	{
		if (pret >= 0)
		{
			float dif = pret - this->pret;
			cheltuialaPeCategorie[this->categorie] += dif;
			this->pret = pret;
		}
		else
			cerr << "Pretul nu poate fi negativ!";
	}
	void set_pret_CSV(float pret)
	{
		if (pret >= 0)
		{
			this->pret = pret;
		}
		else
			cerr << "Pretul nu poate fi negativ!";
	}
	void set_categorie(CategorieUtilitati categorie)
	{
		if (categorie != Energie && categorie != Apa && categorie != Telefonie
			&& categorie != Internet && categorie != Altele && categorie != Gaz
			&& categorie != TV && categorie != Asigurari)
			cerr << "Nu exista categoria introdusa!";
		else
		{
			cheltuialaPeCategorie[this->categorie] -= this->pret;
			this->categorie = categorie;
			cheltuialaPeCategorie[this->categorie] += this->pret;
		}
	}
	void set_categorie_CSV(CategorieUtilitati categorie)
	{
		if (categorie != Energie && categorie != Apa && categorie != Telefonie
			&& categorie != Internet && categorie != Altele && categorie != Gaz
			&& categorie != TV && categorie != Asigurari)
			cerr << "Nu exista categoria introdusa!";
		else
		{
			this->categorie = categorie;
			cheltuialaPeCategorie[this->categorie] += this->pret;
		}
	}
	friend ostream& operator<<(ostream&, Utilitati&);
	friend istream& operator>>(istream&, Utilitati&);
	friend ofstream& operator<<(ofstream&, Utilitati&);
	friend ifstream& operator>>(ifstream&, Utilitati&);
	//supraincarcare operator[] care afiseaza cheltuiala pe o categorie
	void operator[](int index)
	{
		if (index >= 0 && index < 8)
			cout << "\nCheltuiala totala pe categoria " << index << " este " << cheltuialaPeCategorie[index];
		else
			cerr << "\nOut of range!";
	}
	//CALCULEAZA SUMA DINTRE DOUA UTILITATI:
	float operator+(const Utilitati u)
	{
		return this->pret + u.pret;
	}
	//ADAUGA LA PRETUL UNEI UTILIATI UN LEU: 
	//(preincrementare)
	Utilitati& operator++()
	{
		this->pret++;
		cheltuialaPeCategorie[this->categorie]++;
		return *this;
	}
	//(postincrementare)
	Utilitati operator++(int)
	{
		Utilitati copie = *this;
		this->pret++;
		cheltuialaPeCategorie[this->categorie]++;
		return copie;
	}
	// SCADE LA PRETUL UNEI UTILITATI UN LEU :
	Utilitati& operator--()
	{
		this->pret--;
		cheltuialaPeCategorie[this->categorie]--;
		return *this;
	}
	//(postincrementare)
	Utilitati operator--(int)
	{
		Utilitati copie = *this;
		this->pret--;
		cheltuialaPeCategorie[this->categorie]--;
		return copie;
	}
	//cast pentru Utilitati
	explicit operator float()
	{
		return this->pret;
	}
	//Schimba categoria unei utilitati in Altele:
	Utilitati& operator!()
	{
		cheltuialaPeCategorie[this->categorie] -= this->pret;
		this->categorie = Altele;
		cheltuialaPeCategorie[this->categorie] += this->pret;
		return *this;
	}
	//Compara preturile a doua utiliati:
	void operator<(Utilitati u)
	{
		if (this->pret < u.pret && this->denumire != nullptr && u.denumire != nullptr)
			cout << "\nPretul utilitatii " << this->denumire
			<< " este mai mic decat cel al utilitatii " << u.denumire;
		else if (u.pret < this->pret && this->denumire != nullptr && u.denumire != nullptr)
			cout << "\nPretul utilitatii " << u.denumire
			<< " este mai mic decat cel al utilitatii " << this->denumire;
		else if (this->denumire != nullptr && u.denumire != nullptr)
			cout << "\nPretul utilitatii " << u.denumire
			<< " este egal cu cel al utilitatii " << this->denumire;
		else cerr << "\nCel putin o utilitate nu are denumire!";
	}
	//Operatorul == care testeaza egalitatea dintre 2 obiecte 
	bool operator==(Utilitati u)
	{

		if ((this->denumire == nullptr && u.denumire != nullptr) || (this->denumire != nullptr && u.denumire == nullptr))
			return false;
		else if (this->denumire != nullptr && u.denumire != nullptr)
			if (strcmp(this->denumire, u.denumire) != 0)
				return false;
		if (this->pret != u.pret)
			return false;
		if (this->categorie != u.categorie)
			return false;
		return true;
	}
	void StergeObiect()
	{
		cheltuialaPeCategorie[this->categorie] -= this->pret;
		this->pret = 0;
		this->categorie = Altele;
		if (this->denumire != nullptr)
		{
			delete[] this->denumire;
			this->denumire = nullptr;
		}
	}
	void sterge_pret_utilitate()
	{
		cheltuialaPeCategorie[this->categorie] -= this->pret;
	}
	//CALCULARE SUMA TOTALA
	static float calculeazaSumaTotala()
	{
		return cheltuialaPeCategorie[0] + cheltuialaPeCategorie[1] +
			cheltuialaPeCategorie[2] + cheltuialaPeCategorie[3] +
			cheltuialaPeCategorie[4] + cheltuialaPeCategorie[5] +
			cheltuialaPeCategorie[6] + cheltuialaPeCategorie[7];
	}
	void set_TVA()
	{
		if (this->categorie == Apa)
			this->set_cota_TVA(0.09);
		else
			this->set_cota_TVA(0.19);
	}
	void afisare_cota_TVA()
	{
		cout << "\nPentru majoritatea produselor sau serviciilor din aceasta categorie cota TVA este de 19%. Exceptiile sunt:"
			<< "\n\tServiciile de alimentare cu apa si de canalizare - TVA 9%";

	}
	void necesitate_default()
	{
		this->set_necesitate(FoarteNecesar);
	}
	void afisare_tip()
	{
		cout << "\nUtilitati\n";
	}
	friend class Lucru_cu_fisiere;
};
float Utilitati::cheltuialaPeCategorie[8] = { 0 };
ostream& operator<<(ostream& out, Utilitati& u)
{
	out << "\n---------------------";
	out << "\nCategorie: " << u.categorie << "\nDenumire utilitate: ";
	if (u.denumire != nullptr)
		out << u.denumire;
	else
		out << "-";
	out << "\nPret: " << u.pret;
	out << (Comun&)u;
	return out;
}
istream& operator>>(istream& in, Utilitati& u)
{
	cout << "\nDenumire: ";
	char buffer[10000];
	in >> ws;
	in.getline(buffer, 10000, '\n');
	if (u.denumire != nullptr)
		delete[] u.denumire;
	if (buffer != nullptr)
	{
		u.denumire = new char[strlen(buffer) + 1];
		strcpy(u.denumire, buffer);
	}
	else {
		cerr << "Utilitatea nu are denumirea firmei introdusa!\n";
		u.denumire = nullptr;
	}
	u.cheltuialaPeCategorie[u.categorie] -= u.pret;
	float pret;
	cout << "Pret: ";
	in >> pret;
	if (pret > 0)
		u.pret = pret;
	else {
		u.pret = 0;
		cerr << "Pretul nu poate fi negativ!\n";
	}
	int cat;
	cout << "Categorie: [0-Energie, 1-Gaz, 2-Apa, 3-Telefonie, 4-Internet, 5-TV, 6-Asigurari, 7-Altele] => ";
	in >> cat;
	switch (cat)
	{
	case 0:
		u.categorie = Energie;
		break;
	case 1:
		u.categorie = Gaz;
		break;
	case 2:
		u.categorie = Apa;
		break;
	case 3:
		u.categorie = Telefonie;
		break;
	case 4:
		u.categorie = Internet;
		break;
	case 5:
		u.categorie = TV;
		break;
	case 6:
		u.categorie = Asigurari;
		break;
	case 7:
		u.categorie = Altele;
		break;
	default:
		cerr << "Nu ai ales niciuna din cele trei categorii!\n";
		u.categorie = Altele;
		break;
	}
	u.cheltuialaPeCategorie[u.categorie] += u.pret;
	in >> (Comun&)u;
	return in;
}
ofstream& operator<<(ofstream& out, Utilitati& u)
{
	if (u.denumire != nullptr)
		out << u.denumire << endl;
	else
		out << "-" << endl;
	out << u.pret << endl;
	out << u.categorie << endl;
	out << (Comun&)u;
	return out;
}
ifstream& operator>>(ifstream& in, Utilitati& u)
{
	char buffer[10000];
	in >> ws;
	in.getline(buffer, 10000, '\n');
	if (u.denumire != nullptr)
		delete[] u.denumire;
	if (buffer != nullptr)
	{
		u.denumire = new char[strlen(buffer) + 1];
		strcpy(u.denumire, buffer);
	}
	else {
		u.denumire = nullptr;
	}
	u.cheltuialaPeCategorie[u.categorie] -= u.pret;
	in >> u.pret;
	if (u.pret < 0)
		u.pret = 0;
	int cat;
	in >> cat;
	switch (cat)
	{
	case 0:
		u.categorie = Energie;
		break;
	case 1:
		u.categorie = Gaz;
		break;
	case 2:
		u.categorie = Apa;
		break;
	case 3:
		u.categorie = Telefonie;
		break;
	case 4:
		u.categorie = Internet;
		break;
	case 5:
		u.categorie = TV;
		break;
	case 6:
		u.categorie = Asigurari;
		break;
	case 7:
		u.categorie = Altele;
		break;
	default:
		u.categorie = Altele;
		break;
	}
	u.cheltuialaPeCategorie[u.categorie] += u.pret;
	in >> (Comun&)u;
	return in;
}
enum CategorieDiverse {
	Sanatate,
	Educatie,
	Copil,
	Animale,
	Imbracaminte,
	Electrocasnice,
	Mobilier,
	Alte
};
//CLASA DIVERSE
class Diverse :public Comun {
	char* denumire = nullptr;
	float pret = 0;
	CategorieDiverse categorie = Alte;
	static float cheltuialaPeCategorie[8];
public:
	Diverse()
		:Comun()
	{}
	Diverse(float pret, CategorieDiverse categorie)
		:Comun()
	{
		if (pret >= 0)
			this->pret = pret;
		else
			cerr << "Pretul nu poate fi negativ!";
		this->categorie = categorie;
		cheltuialaPeCategorie[categorie] += pret;
	}
	Diverse(const char* denumire, float pret, CategorieDiverse categorie, Necesitate nivel_necesitate)
		:Comun(nivel_necesitate)
	{
		if (denumire != nullptr)
		{
			this->denumire = new char[strlen(denumire) + 1];
			strcpy(this->denumire, denumire);
		}
		else cerr << "Trebuie sa introduceti denumirea!";
		if (pret >= 0)
			this->pret = pret;
		else
			cerr << "Pretul nu poate fi negativ!";
		this->categorie = categorie;
		cheltuialaPeCategorie[categorie] += pret;
	}
	Diverse(const char* denumire, float pret, CategorieDiverse categorie)
		:Comun()
	{
		if (denumire != nullptr)
		{
			this->denumire = new char[strlen(denumire) + 1];
			strcpy(this->denumire, denumire);
		}
		else cerr << "Trebuie sa introduceti denumirea!";
		if (pret >= 0)
			this->pret = pret;
		else
			cerr << "Pretul nu poate fi negativ!";
		this->categorie = categorie;
		cheltuialaPeCategorie[categorie] += pret;
	}
	Diverse(const char* denumire, float pret, CategorieDiverse categorie, float TVA, Necesitate nivel_necesitate)
		:Comun(TVA, nivel_necesitate)
	{
		if (denumire != nullptr)
		{
			this->denumire = new char[strlen(denumire) + 1];
			strcpy(this->denumire, denumire);
		}
		else cerr << "Trebuie sa introduceti denumirea!";
		if (pret >= 0)
			this->pret = pret;
		else
			cerr << "Pretul nu poate fi negativ!";
		this->categorie = categorie;
		cheltuialaPeCategorie[categorie] += pret;
	}
	Diverse(const Diverse& diverse)
		:Comun(diverse)
	{
		if (diverse.denumire != nullptr)
		{
			this->denumire = new char[strlen(diverse.denumire) + 1];
			strcpy(this->denumire, diverse.denumire);
		}
		if (diverse.pret >= 0)
			this->pret = diverse.pret;
		this->categorie = diverse.categorie;
		cheltuialaPeCategorie[this->categorie] += this->pret;
	}
	~Diverse()
	{
		if (this->denumire != nullptr)
			delete[] this->denumire;
		//cheltuialaPeCategorie[this->categorie] -= this->pret;
	}
	Diverse& operator=(const Diverse& diverse)
	{
		if (this->denumire != nullptr)
			delete[] this->denumire;
		if (diverse.denumire != nullptr)
		{
			this->denumire = new char[strlen(diverse.denumire) + 1];
			strcpy(this->denumire, diverse.denumire);
		}
		else this->denumire = nullptr;
		cheltuialaPeCategorie[this->categorie] -= this->pret;
		this->categorie = diverse.categorie;
		this->pret = diverse.pret;
		cheltuialaPeCategorie[this->categorie] += this->pret;
		Comun::operator=(diverse);
		return *this;
	}
	const char* get_denumire()
	{
		return this->denumire;
	}
	float get_pret()
	{
		return this->pret;
	}
	CategorieDiverse get_categorie()
	{
		return this->categorie;
	}
	static float get_cheltuialaPeCategorie(CategorieDiverse cat)
	{
		return cheltuialaPeCategorie[cat];
	}
	void set_denumire(const char* denumire)
	{
		if (denumire != nullptr)
		{
			if (this->denumire != nullptr)
				delete[] this->denumire;
			this->denumire = new char[strlen(denumire) + 1];
			strcpy(this->denumire, denumire);
		}
		else
			cerr << "Trebuie sa introduceti o denumire!";
	}
	void set_pret(float pret)
	{
		if (pret >= 0)
		{
			float dif = pret - this->pret;
			cheltuialaPeCategorie[this->categorie] += dif;
			this->pret = pret;
		}
		else
			cerr << "Pretul nu poate fi negativ!";
	}
	void set_pret_CSV(float pret)
	{
		if (pret >= 0)
		{
			this->pret = pret;
		}
		else
			cerr << "Pretul nu poate fi negativ!";
	}
	void set_categorie(CategorieDiverse categorie)
	{
		if (categorie != Sanatate && categorie != Educatie && categorie != Copil
			&& categorie != Imbracaminte && categorie != Animale && categorie != Mobilier
			&& categorie != Alte && categorie != Electrocasnice)
			cerr << "Nu exista categoria introdusa!";
		else
		{
			cheltuialaPeCategorie[this->categorie] -= this->pret;
			this->categorie = categorie;
			cheltuialaPeCategorie[this->categorie] += this->pret;
		}
	}
	void set_categorie_CSV(CategorieDiverse categorie)
	{
		if (categorie != Sanatate && categorie != Educatie && categorie != Copil
			&& categorie != Imbracaminte && categorie != Animale && categorie != Mobilier
			&& categorie != Alte && categorie != Electrocasnice)
			cerr << "Nu exista categoria introdusa!";
		else
		{
			this->categorie = categorie;
			cheltuialaPeCategorie[this->categorie] += this->pret;
		}
	}
	//supraincarcare operator[] care afiseaza cheltuiala pe o categorie
	void operator[](int index)
	{
		if (index >= 0 && index < 8)
			cout << "\nCheltuiala totala pe categoria " << index << " este " << cheltuialaPeCategorie[index];
		else
			cerr << "\nOut of range!";
	}
	//Suma dintre doua diverse:
	float operator+(const Diverse diverse)
	{
		return this->pret + diverse.pret;
	}
	//ADAUGA LA PRETUL UNEI "Diverse" 5 LEi: 
	//(preincrementare)
	Diverse& operator++()
	{
		this->pret++;
		cheltuialaPeCategorie[this->categorie]++;
		return *this;
	}
	//(postincrementare)
	Diverse operator++(int)
	{
		Diverse copie = *this;
		this->pret++;
		cheltuialaPeCategorie[this->categorie]++;
		return copie;
	}
	// SCADE LA PRETUL UNEI "Diverse" 5 LEi :
	Diverse& operator--()
	{
		this->pret--;
		cheltuialaPeCategorie[this->categorie]--;
		return *this;
	}
	//(postincrementare)
	Diverse operator--(int)
	{
		Diverse copie = *this;
		this->pret--;
		cheltuialaPeCategorie[this->categorie]--;
		return copie;
	}
	//cast pentru Diverse
	explicit operator float()
	{
		return this->pret;
	}
	//Schimba categoria unei diverse in Alte:
	Diverse& operator!()
	{
		cheltuialaPeCategorie[this->categorie] -= this->pret;
		this->categorie = Alte;
		cheltuialaPeCategorie[this->categorie] += this->pret;
		return *this;
	}
	//Compara preturile a doua diverse:
	void operator<(Diverse diverse)
	{
		if (this->pret < diverse.pret && this->denumire != nullptr && diverse.denumire != nullptr)
			cout << "\nPretul activitatii " << this->denumire
			<< " este mai mic decat cel al activitatii " << diverse.denumire;
		else if (diverse.pret < this->pret && this->denumire != nullptr && diverse.denumire != nullptr)
			cout << "\nPretul activitatii " << diverse.denumire
			<< " este mai mic decat cel al activitatii " << this->denumire;
		else if (this->denumire != nullptr && diverse.denumire != nullptr)
			cout << "\nPretul activitatii " << diverse.denumire
			<< " este egal cu cel al activitatii " << this->denumire;
		else cerr << "\nCel putin una dintre diverse nu are denumire!";
	}
	//Operatorul == care testeaza egalitatea dintre 2 obiecte 
	bool operator==(Diverse diverse)
	{

		if ((this->denumire == nullptr && diverse.denumire != nullptr) || (this->denumire != nullptr && diverse.denumire == nullptr))
			return false;
		else if (this->denumire != nullptr && diverse.denumire != nullptr)
			if (strcmp(this->denumire, diverse.denumire) != 0)
				return false;
		if (this->pret != diverse.pret)
			return false;
		if (this->categorie != diverse.categorie)
			return false;
		return true;
	}
	void StergeObiect()
	{
		cheltuialaPeCategorie[this->categorie] -= this->pret;
		this->pret = 0;
		this->categorie = Alte;
		if (this->denumire != nullptr)
		{
			delete[] this->denumire;
			this->denumire = nullptr;
		}
	}
	//CALCULARE SUMA TOTALA
	static float calculeazaSumaTotala()
	{
		return cheltuialaPeCategorie[0] + cheltuialaPeCategorie[1] +
			cheltuialaPeCategorie[2] + cheltuialaPeCategorie[3] +
			cheltuialaPeCategorie[4] + cheltuialaPeCategorie[5] +
			cheltuialaPeCategorie[6] + cheltuialaPeCategorie[7];
	}
	void sterge_pret_diversa()
	{
		cheltuialaPeCategorie[this->categorie] -= this->pret;
	}
	void set_TVA()
	{
		if (this->categorie == Educatie)
			this->set_cota_TVA(0.05);
		else
			this->set_cota_TVA(0.19);
	}
	void necesitate_default()
	{
		this->set_necesitate(PutinNecesar);
	}
	void afisare_cota_TVA()
	{
		cout << "\nPentru majoritatea produselor sau serviciilor din aceasta categorie cota TVA este de 19%. Exceptiile sunt:"
			<< "\n\tMedicamentele de uz uman si veterinar - TVA 9%"
			<< "\n\tManualele scolare, cartile, ziarele si revistele - TVA 5%";
	}
	void afisare_tip()
	{
		cout << "\nDiverse\n";
	}
	friend ostream& operator<<(ostream&, Diverse&);
	friend istream& operator>>(istream&, Diverse&);
	friend ofstream& operator<<(ofstream&, Diverse&);
	friend ifstream& operator>>(ifstream&, Diverse&);
	friend class Lucru_cu_fisiere;
};
float Diverse::cheltuialaPeCategorie[8] = { 0 };
ostream& operator<<(ostream& out, Diverse& diverse)
{
	out << "\n---------------------";
	out << "\nCategorie: " << diverse.categorie << "\nDenumire diverse: ";
	if (diverse.denumire != nullptr)
		out << diverse.denumire;
	else
		out << "-";
	out << "\nPret: " << diverse.pret;
	out << (Comun&)diverse;
	return out;
}
istream& operator>>(istream& in, Diverse& diverse)
{
	cout << "\nDenumire: ";
	char buffer[10000];
	in >> ws;
	in.getline(buffer, 10000, '\n');
	if (diverse.denumire != nullptr)
		delete[] diverse.denumire;
	if (buffer != nullptr)
	{
		diverse.denumire = new char[strlen(buffer) + 1];
		strcpy(diverse.denumire, buffer);
	}
	else {
		cerr << "Denumirea nu a fost introdusa!\n";
		diverse.denumire = nullptr;
	}
	diverse.cheltuialaPeCategorie[diverse.categorie] -= diverse.pret;
	float pret;
	cout << "Pret: ";
	in >> pret;
	if (pret > 0)
		diverse.pret = pret;
	else {
		diverse.pret = 0;
		cerr << "Pretul nu poate fi negativ!\n";
	}
	int cat;
	cout << "Categorie: [0-Sanatate, 1-Educatie, 2-Copil, 3-Animale, 4-Imbracaminte, 5-Electrocasnice, 6-Mobilier, 7-Altele] => ";
	in >> cat;
	switch (cat)
	{
	case 0:
		diverse.categorie = Sanatate;
		break;
	case 1:
		diverse.categorie = Educatie;
		break;
	case 2:
		diverse.categorie = Copil;
		break;
	case 3:
		diverse.categorie = Animale;
		break;
	case 4:
		diverse.categorie = Imbracaminte;
		break;
	case 5:
		diverse.categorie = Electrocasnice;
		break;
	case 6:
		diverse.categorie = Mobilier;
		break;
	case 7:
		diverse.categorie = Alte;
		break;
	default:
		cerr << "Nu ai ales niciuna din cele trei categorii!\n";
		diverse.categorie = Alte;
		break;
	}
	diverse.cheltuialaPeCategorie[diverse.categorie] += diverse.pret;
	in >> (Comun&)diverse;
	return in;
}
ofstream& operator<<(ofstream& out, Diverse& diverse)
{
	if (diverse.denumire != nullptr)
		out << diverse.denumire << endl;
	else
		out << "-" << endl;
	out << diverse.pret << endl;
	out << diverse.categorie << endl;
	out << (Comun&)diverse;
	return out;
}
ifstream& operator>>(ifstream& in, Diverse& diverse)
{
	char buffer[10000];
	in >> ws;
	in.getline(buffer, 10000, '\n');
	if (diverse.denumire != nullptr)
		delete[] diverse.denumire;
	if (buffer != nullptr)
	{
		diverse.denumire = new char[strlen(buffer) + 1];
		strcpy(diverse.denumire, buffer);
	}
	else {
		diverse.denumire = nullptr;
	}
	diverse.cheltuialaPeCategorie[diverse.categorie] -= diverse.pret;
	in >> diverse.pret;
	if (diverse.pret < 0)
		diverse.pret = 0;
	int cat;
	in >> cat;
	switch (cat)
	{
	case 0:
		diverse.categorie = Sanatate;
		break;
	case 1:
		diverse.categorie = Educatie;
		break;
	case 2:
		diverse.categorie = Copil;
		break;
	case 3:
		diverse.categorie = Animale;
		break;
	case 4:
		diverse.categorie = Imbracaminte;
		break;
	case 5:
		diverse.categorie = Electrocasnice;
		break;
	case 6:
		diverse.categorie = Mobilier;
		break;
	case 7:
		diverse.categorie = Alte;
		break;
	default:
		diverse.categorie = Alte;
		break;
	}
	diverse.cheltuialaPeCategorie[diverse.categorie] += diverse.pret;
	in >> (Comun&)diverse;
	return in;
}
enum CategorieDivertisment {
	Excursii,
	Petreceri,
	Iesiri,
	Alte_cheltuieli
};
//CLASA DIVERTISMENT creata pe baza enum CategorieDivertisment, elemente care nu sunt incluse la diverse, 
//deoarece la diverse sunt lucruri din care nu prea poti sa scazi din bugetul alocat lor
//In schimb aceasta clasa isi poate modifica usor bugetul => si va ajuta la diverse sugestii la raport
class Divertisment :public Comun {
	char* denumire = nullptr;
	float pret = 0;
	CategorieDivertisment categorie = Alte_cheltuieli;
	static float cheltuialaPeCategorie[4];
public:
	Divertisment()
		:Comun()
	{}
	Divertisment(float pret, CategorieDivertisment categorie)
		:Comun()
	{
		if (pret >= 0)
			this->pret = pret;
		else
			cerr << "Pretul nu poate fi negativ!";
		this->categorie = categorie;
		cheltuialaPeCategorie[categorie] += pret;
	}
	Divertisment(const char* denumire, float pret, CategorieDivertisment categorie, float TVA, Necesitate nivel_necesitate)
		:Comun(TVA, nivel_necesitate)
	{
		if (denumire != nullptr)
		{
			this->denumire = new char[strlen(denumire) + 1];
			strcpy(this->denumire, denumire);
		}
		else cerr << "Trebuie sa introduceti si denumirea!";
		if (pret >= 0)
			this->pret = pret;
		else
			cerr << "Pretul nu poate fi negativ!";
		this->categorie = categorie;
		cheltuialaPeCategorie[categorie] += pret;
	}
	Divertisment(const char* denumire, float pret, CategorieDivertisment categorie, Necesitate nivel_necesitate)
		:Comun(nivel_necesitate)
	{
		if (denumire != nullptr)
		{
			this->denumire = new char[strlen(denumire) + 1];
			strcpy(this->denumire, denumire);
		}
		else cerr << "Trebuie sa introduceti si denumirea!";
		if (pret >= 0)
			this->pret = pret;
		else
			cerr << "Pretul nu poate fi negativ!";
		this->categorie = categorie;
		cheltuialaPeCategorie[categorie] += pret;
	}
	Divertisment(const char* denumire, float pret, CategorieDivertisment categorie)
		:Comun()
	{
		if (denumire != nullptr)
		{
			this->denumire = new char[strlen(denumire) + 1];
			strcpy(this->denumire, denumire);
		}
		else cerr << "Trebuie sa introduceti si denumirea!";
		if (pret >= 0)
			this->pret = pret;
		else
			cerr << "Pretul nu poate fi negativ!";
		this->categorie = categorie;
		cheltuialaPeCategorie[categorie] += pret;
	}
	Divertisment(const Divertisment& d)
		:Comun(d)
	{
		if (d.denumire != nullptr)
		{
			this->denumire = new char[strlen(d.denumire) + 1];
			strcpy(this->denumire, d.denumire);
		}
		if (d.pret >= 0)
			this->pret = d.pret;
		this->categorie = d.categorie;
		cheltuialaPeCategorie[this->categorie] += this->pret;
	}
	~Divertisment()
	{
		if (this->denumire != nullptr)
			delete[] this->denumire;
		//cheltuialaPeCategorie[this->categorie] -= this->pret;
	}
	Divertisment& operator=(const Divertisment& d)
	{
		if (this->denumire != nullptr)
			delete[] this->denumire;
		if (d.denumire != nullptr)
		{
			this->denumire = new char[strlen(d.denumire) + 1];
			strcpy(this->denumire, d.denumire);
		}
		else this->denumire = nullptr;
		cheltuialaPeCategorie[this->categorie] -= this->pret;
		this->categorie = d.categorie;
		this->pret = d.pret;
		cheltuialaPeCategorie[this->categorie] += this->pret;
		Comun::operator=(d);
		return *this;
	}
	const char* get_denumire()
	{
		return this->denumire;
	}
	float get_pret()
	{
		return this->pret;
	}
	CategorieDivertisment get_categorie()
	{
		return this->categorie;
	}
	static float get_cheltuialaPeCategorie(CategorieDivertisment cat)
	{
		return cheltuialaPeCategorie[cat];
	}
	void set_denumire(const char* denumire)
	{
		if (denumire != nullptr)
		{
			if (this->denumire != nullptr)
				delete[] this->denumire;
			this->denumire = new char[strlen(denumire) + 1];
			strcpy(this->denumire, denumire);
		}
		else
			cerr << "Trebuie sa introduceti o denumire!";
	}
	void set_pret(float pret)
	{
		if (pret >= 0)
		{
			float dif = pret - this->pret;
			cheltuialaPeCategorie[this->categorie] += dif;
			this->pret = pret;
		}
		else
			cerr << "Pretul nu poate fi negativ!";
	}
	void set_pret_CSV(float pret)
	{
		if (pret >= 0)
		{
			this->pret = pret;
		}
		else
			cerr << "Pretul nu poate fi negativ!";
	}
	void set_categorie(CategorieDivertisment categorie)
	{
		if (categorie != Petreceri && categorie != Iesiri && categorie != Alte_cheltuieli && categorie != Excursii)
			cerr << "Nu exista categoria introdusa!";
		else
		{
			cheltuialaPeCategorie[this->categorie] -= this->pret;
			this->categorie = categorie;
			cheltuialaPeCategorie[this->categorie] += this->pret;
		}
	}
	void set_categorie_CSV(CategorieDivertisment categorie)
	{
		if (categorie != Petreceri && categorie != Iesiri && categorie != Alte_cheltuieli && categorie != Excursii)
			cerr << "Nu exista categoria introdusa!";
		else
		{
			this->categorie = categorie;
			cheltuialaPeCategorie[this->categorie] += this->pret;
		}
	}
	//supraincarcare operator[] care afiseaza cheltuiala pe o categorie
	void operator[](int index)
	{
		if (index >= 0 && index < 4)
			cout << "\nCheltuiala totala pe categoria " << index << " este " << cheltuialaPeCategorie[index];
		else
			cerr << "\nOut of range!";
	}
	//CALCULEAZA SUMA DINTRE DOUA OBIECTE ALLE DIVERTISMENTULUI:
	float operator+(const Divertisment d)
	{
		return this->pret + d.pret;
	}
	//ADAUGA LA PRETUL UNUI DIVERTISMENT 10 LEi: 
	//(preincrementare)
	Divertisment& operator++()
	{
		this->pret += 10;
		cheltuialaPeCategorie[this->categorie] += 10;
		return *this;
	}
	//(postincrementare)
	Divertisment operator++(int)
	{
		Divertisment copie = *this;
		this->pret += 10;
		cheltuialaPeCategorie[this->categorie] += 10;
		return copie;
	}
	// SCADE LA PRETUL UNUI DIVERTISMENT 10 LEI :
	Divertisment& operator--()
	{
		this->pret -= 10;
		cheltuialaPeCategorie[this->categorie] -= 10;
		return *this;
	}
	//(postincrementare)
	Divertisment operator--(int)
	{
		Divertisment copie = *this;
		this->pret -= 10;
		cheltuialaPeCategorie[this->categorie] -= 10;
		return copie;
	}
	//cast pentru Divertisment
	explicit operator float()
	{
		return this->pret;
	}
	//Schimba categoria unei Divertisment in Alte_cheltuieli:
	Divertisment& operator!()
	{
		cheltuialaPeCategorie[this->categorie] -= this->pret;
		this->categorie = Alte_cheltuieli;
		cheltuialaPeCategorie[this->categorie] += this->pret;
		return *this;
	}
	//Compara preturile a doua elemente Divertisment:
	void operator<(Divertisment d)
	{
		if (this->pret < d.pret && this->denumire != nullptr && d.denumire != nullptr)
			cout << "\nPretul activitatii" << this->denumire
			<< " este mai mic decat cel al activitatii " << d.denumire;
		else if (d.pret < this->pret && this->denumire != nullptr && d.denumire != nullptr)
			cout << "\nPretul activitatii " << d.denumire
			<< " este mai mic decat cel al activivtatii" << this->denumire;
		else if (this->denumire != nullptr && d.denumire != nullptr)
			cout << "\nPretul activitatii " << d.denumire
			<< " este egal cu cel al activitatii " << this->denumire;
		else cerr << "\nCel putin un divertisment nu are denumire!";
	}
	//Operatorul == care testeaza egalitatea dintre 2 obiecte 
	bool operator==(Divertisment d)
	{

		if ((this->denumire == nullptr && d.denumire != nullptr) || (this->denumire != nullptr && d.denumire == nullptr))
			return false;
		else if (this->denumire != nullptr && d.denumire != nullptr)
			if (strcmp(this->denumire, d.denumire) != 0)
				return false;
		if (this->pret != d.pret)
			return false;
		if (this->categorie != d.categorie)
			return false;
		return true;
	}
	void sterge_pret_divertisment()
	{
		cheltuialaPeCategorie[this->categorie] -= this->pret;
	}
	friend ostream& operator<<(ostream&, Divertisment&);
	friend istream& operator>>(istream&, Divertisment&);
	friend ofstream& operator<<(ofstream&, Divertisment);
	friend ifstream& operator>>(ifstream&, Divertisment&);
	//Actualizare pret cu 0 si denumire cu nullptr => echivalent cu stergerea datelor obictelor
	void StergeObiect()
	{
		cheltuialaPeCategorie[this->categorie] -= this->pret;
		this->pret = 0;
		this->categorie = Alte_cheltuieli;
		if (this->denumire != nullptr)
		{
			delete[] this->denumire;
			this->denumire = nullptr;
		}
	}
	void necesitate_default()
	{
		this->set_necesitate(DelocNecesar);
	}
	void afisare_cota_TVA()
	{
		cout << "\nPentru majoritatea produselor sau serviciilor din aceasta categorie cota TVA este de 19%. Exceptiile sunt:"
			<< "\n\tServiciile constand in permiterea accesului la castele, muzee, case memoriale, monumente istorice, monumente de arhitectura si arheologice,"
			<< "gradini zoologice si botanice, balciuri, parcuri de distractii si parcuri recreative - TVA 5%";
	}
	void afisare_tip()
	{
		cout << "\nDivertisment\n";
	}
	//CALCULEAZA SUMA PE TOATE CATEGORIILE
	static float calculeazaSumaTotala()
	{
		return cheltuialaPeCategorie[0] + cheltuialaPeCategorie[1]
			+ cheltuialaPeCategorie[2] + cheltuialaPeCategorie[3];
	}
	friend class Lucru_cu_fisiere;
};
float Divertisment::cheltuialaPeCategorie[4] = { 0,0,0,0 };
ostream& operator<<(ostream& out, Divertisment& div)
{
	out << "\n---------------------";
	out << "\nCategorie: " << div.categorie << "\nDenumire divertisment: ";
	if (div.denumire != nullptr)
		out << div.denumire;
	else
		out << "-";
	out << "\nPret: " << div.pret;
	out << (Comun&)div;
	return out;
}
istream& operator>>(istream& in, Divertisment& div)
{
	cout << "\nDenumire: ";
	char buffer[10000];
	in >> ws;
	in.getline(buffer, 10000, '\n');
	if (div.denumire != nullptr)
		delete[] div.denumire;
	if (buffer != nullptr)
	{
		div.denumire = new char[strlen(buffer) + 1];
		strcpy(div.denumire, buffer);
	}
	else {
		cerr << "Activitatea nu are denumire!\n";
		div.denumire = nullptr;
	}
	div.cheltuialaPeCategorie[div.categorie] -= div.pret;
	float pret;
	cout << "Pret: ";
	in >> pret;
	if (pret > 0)
		div.pret = pret;
	else {
		div.pret = 0;
		cerr << "Pretul nu poate fi negativ!\n";
	}
	int cat;
	cout << "Categorie: [0-Excursii, 1-Petreceri, 2-Iesiri, 3-Altele] => ";
	in >> cat;
	switch (cat)
	{
	case 0:
		div.categorie = Excursii;
		break;
	case 1:
		div.categorie = Petreceri;
		break;
	case 2:
		div.categorie = Iesiri;
		break;
	case 3:
		div.categorie = Alte_cheltuieli;
		break;
	default:
		cerr << "Nu ai ales niciuna din cele patru categorii!\n";
		div.categorie = Alte_cheltuieli;
		break;
	}
	div.cheltuialaPeCategorie[div.categorie] += div.pret;
	in >> (Comun&)div;
	return in;
}
ofstream& operator<<(ofstream& out, Divertisment div)
{
	if (div.denumire != nullptr)
		out << div.denumire << endl;
	else
		out << "-" << endl;
	out << div.pret << endl;
	out << div.categorie << endl;
	out << (Comun&)div;
	return out;
}
ifstream& operator>>(ifstream& in, Divertisment& div)
{
	char buffer[10000];
	in >> ws;
	in.getline(buffer, 10000, '\n');
	if (div.denumire != nullptr)
		delete[] div.denumire;
	if (buffer != nullptr)
	{
		div.denumire = new char[strlen(buffer) + 1];
		strcpy(div.denumire, buffer);
	}
	else {
		div.denumire = nullptr;
	}
	div.cheltuialaPeCategorie[div.categorie] -= div.pret;
	in >> div.pret;
	if (div.pret < 0)
		div.pret = 0;
	int cat;
	in >> cat;
	switch (cat)
	{
	case 0:
		div.categorie = Excursii;
		break;
	case 1:
		div.categorie = Petreceri;
		break;
	case 2:
		div.categorie = Iesiri;
		break;
	case 3:
		div.categorie = Alte_cheltuieli;
		break;
	default:
		div.categorie = Alte_cheltuieli;
		break;
	}
	div.cheltuialaPeCategorie[div.categorie] += div.pret;
	in >> (Comun&)div;
	return in;
}
//CLASA CHELTUIELI:
// clasa mare de cheltuieli care va contine elemente din toate clasele mici
class Cheltuieli {
	static float suma_TVA;
	time_t now = time(0);
	tm* ltm = localtime(&now);
	const int data;		//ziua pentru fiecare cheltuiala inregistrata,\
						    care nu se va afisa utilizatorului, dar  \
							care va putea fi utilizata pentru raport
	int nr_alimente = 0;
	Alimente* alimente = nullptr;
	int nr_utilitati = 0;
	Utilitati* utilitati = nullptr;
	int nr_divertisment = 0;
	Divertisment* divertisment = nullptr;
	int nr_diverse = 0;
	Diverse* diverse = nullptr;
	float suma_cheltuiala = 0;
	static float cheltuieli_pe_date[31];

public:
	Cheltuieli()
		:data(ltm->tm_mday)
	{}
	Cheltuieli(int nr_alimente, Alimente* alimente, int nr_utilitati, Utilitati* utilitati)
		:data(ltm->tm_mday)
	{
		if (nr_alimente > 0)
		{
			this->nr_alimente = nr_alimente;
			if (alimente != nullptr)
			{
				this->alimente = new Alimente[nr_alimente];
				for (int i = 0; i < nr_alimente; i++)
				{
					this->alimente[i] = alimente[i];
					this->suma_cheltuiala += this->alimente[i].get_pret();
				}
			}
		}
		else if (nr_alimente < 0)
			cerr << "Nu puteti introduce un numar negativ de alimente!\n";
		if (nr_utilitati > 0)
		{
			this->nr_utilitati = nr_utilitati;
			if (utilitati != nullptr)
			{
				this->utilitati = new Utilitati[nr_utilitati];
				for (int i = 0; i < nr_utilitati; i++)
				{
					this->utilitati[i] = utilitati[i];
					this->suma_cheltuiala += this->utilitati[i].get_pret();
				}
			}
		}
		else if (nr_utilitati < 0)
			cerr << "Nu puteti introduce un numar negativ de utilitati!\n";
		cheltuieli_pe_date[this->data] += this->suma_cheltuiala;
		suma_TVA += this->calculeazaTVA();
	}
	Cheltuieli(int nr_alimente, Alimente* alimente, int nr_utilitati, Utilitati* utilitati,
		int nr_divertisment, Divertisment* divertisment, int nr_diverse, Diverse* diverse)
		:data(ltm->tm_mday)
	{
		if (nr_alimente > 0)
		{
			this->nr_alimente = nr_alimente;
			if (alimente != nullptr)
			{
				this->alimente = new Alimente[nr_alimente];
				for (int i = 0; i < nr_alimente; i++)
				{
					this->alimente[i] = alimente[i];
					this->suma_cheltuiala += this->alimente[i].get_pret();
				}
			}
		}
		else if (nr_alimente < 0)
			cerr << "Nu puteti introduce un numar negativ de alimente!\n";

		if (nr_utilitati > 0)
		{
			this->nr_utilitati = nr_utilitati;
			if (utilitati != nullptr)
			{
				this->utilitati = new Utilitati[nr_utilitati];
				for (int i = 0; i < nr_utilitati; i++)
				{
					this->utilitati[i] = utilitati[i];
					this->suma_cheltuiala += this->utilitati[i].get_pret();
				}
			}
		}
		else if (nr_utilitati < 0)
			cerr << "Nu puteti introduce un numar negativ de utilitati!\n";

		if (nr_divertisment > 0)
		{
			this->nr_divertisment = nr_divertisment;
			if (divertisment != nullptr)
			{
				this->divertisment = new Divertisment[nr_divertisment];
				for (int i = 0; i < nr_divertisment; i++)
				{
					this->divertisment[i] = divertisment[i];
					this->suma_cheltuiala += this->divertisment[i].get_pret();
				}
			}
		}
		else if (nr_divertisment < 0)
			cerr << "Nu puteti introduce un numar negativ de divertismente!\n";

		if (nr_diverse > 0)
		{
			this->nr_diverse = nr_diverse;
			if (diverse != nullptr)
			{
				this->diverse = new Diverse[nr_diverse];
				for (int i = 0; i < nr_diverse; i++)
				{
					this->diverse[i] = diverse[i];
					this->suma_cheltuiala += this->diverse[i].get_pret();
				}
			}
		}
		else if (nr_divertisment < 0)
			cerr << "Nu puteti introduce un numar negativ de diverse!\n";
		cheltuieli_pe_date[this->data - 1] += this->suma_cheltuiala;
		suma_TVA += this->calculeazaTVA();
	}
	Cheltuieli(int data, int nr_alimente, Alimente* alimente, int nr_utilitati, Utilitati* utilitati,
		int nr_divertisment, Divertisment* divertisment, int nr_diverse, Diverse* diverse)
		:data(data)
	{
		if (nr_alimente > 0)
		{
			this->nr_alimente = nr_alimente;
			if (alimente != nullptr)
			{
				this->alimente = new Alimente[nr_alimente];
				for (int i = 0; i < nr_alimente; i++)
				{
					this->alimente[i] = alimente[i];
					this->suma_cheltuiala += this->alimente[i].get_pret();
				}
			}
		}
		else if (nr_alimente < 0)
			cerr << "Nu puteti introduce un numar negativ de alimente!\n";

		if (nr_utilitati > 0)
		{
			this->nr_utilitati = nr_utilitati;
			if (utilitati != nullptr)
			{
				this->utilitati = new Utilitati[nr_utilitati];
				for (int i = 0; i < nr_utilitati; i++)
				{
					this->utilitati[i] = utilitati[i];
					this->suma_cheltuiala += this->utilitati[i].get_pret();
				}
			}
		}
		else if (nr_utilitati < 0)
			cerr << "Nu puteti introduce un numar negativ de utilitati!\n";

		if (nr_divertisment > 0)
		{
			this->nr_divertisment = nr_divertisment;
			if (divertisment != nullptr)
			{
				this->divertisment = new Divertisment[nr_divertisment];
				for (int i = 0; i < nr_divertisment; i++)
				{
					this->divertisment[i] = divertisment[i];
					this->suma_cheltuiala += this->divertisment[i].get_pret();
				}
			}
		}
		else if (nr_divertisment < 0)
			cerr << "Nu puteti introduce un numar negativ de divertismente!\n";

		if (nr_diverse > 0)
		{
			this->nr_diverse = nr_diverse;
			if (diverse != nullptr)
			{
				this->diverse = new Diverse[nr_diverse];
				for (int i = 0; i < nr_diverse; i++)
				{
					this->diverse[i] = diverse[i];
					this->suma_cheltuiala += this->diverse[i].get_pret();
				}
			}
		}
		else if (nr_divertisment < 0)
			cerr << "Nu puteti introduce un numar negativ de diverse!\n";
		cheltuieli_pe_date[this->data] += this->suma_cheltuiala;
		suma_TVA += this->calculeazaTVA();
	}
	Cheltuieli(const Cheltuieli& ch)
		:data(ltm->tm_mday)
	{

		this->nr_alimente = ch.nr_alimente;
		if (ch.nr_alimente > 0)
		{
			this->alimente = new Alimente[ch.nr_alimente];
			for (int i = 0; i < ch.nr_alimente; i++)
				this->alimente[i] = ch.alimente[i];
		}
		this->nr_utilitati = ch.nr_utilitati;
		if (ch.nr_utilitati > 0)
		{
			this->utilitati = new Utilitati[ch.nr_utilitati];
			for (int i = 0; i < ch.nr_utilitati; i++)
				this->utilitati[i] = ch.utilitati[i];
		}
		this->nr_divertisment = ch.nr_divertisment;
		if (ch.nr_divertisment > 0)
		{
			this->divertisment = new Divertisment[ch.nr_divertisment];
			for (int i = 0; i < ch.nr_divertisment; i++)
				this->divertisment[i] = ch.divertisment[i];
		}
		this->nr_diverse = ch.nr_diverse;
		if (ch.nr_diverse > 0)
		{
			this->diverse = new Diverse[ch.nr_diverse];
			for (int i = 0; i < ch.nr_diverse; i++)
				this->diverse[i] = ch.diverse[i];
		}
		this->suma_cheltuiala = ch.suma_cheltuiala;
		cheltuieli_pe_date[ch.data] += ch.suma_cheltuiala;
		suma_TVA += this->calculeazaTVA();
	}
	Cheltuieli& operator=(const Cheltuieli& ch)
	{
		cheltuieli_pe_date[ch.data] -= ch.suma_cheltuiala;
		suma_TVA -= this->calculeazaTVA();
		if (this->alimente != nullptr)
			delete[] this->alimente;
		this->nr_alimente = ch.nr_alimente;
		if (ch.nr_alimente > 0)
		{
			this->alimente = new Alimente[ch.nr_alimente];
			for (int i = 0; i < ch.nr_alimente; i++)
				this->alimente[i] = ch.alimente[i];
		}
		else this->alimente = nullptr;
		if (this->utilitati != nullptr)
			delete[] utilitati;
		this->nr_utilitati = ch.nr_utilitati;
		if (ch.nr_utilitati > 0)
		{
			this->utilitati = new Utilitati[ch.nr_utilitati];
			for (int i = 0; i < ch.nr_utilitati; i++)
				this->utilitati[i] = ch.utilitati[i];
		}
		else this->utilitati = nullptr;
		if (this->divertisment != nullptr)
			delete[] this->divertisment;
		this->nr_divertisment = ch.nr_divertisment;
		if (ch.nr_divertisment > 0)
		{
			this->divertisment = new Divertisment[ch.nr_divertisment];
			for (int i = 0; i < ch.nr_divertisment; i++)
				this->divertisment[i] = ch.divertisment[i];
		}
		else this->divertisment = nullptr;
		if (this->diverse != nullptr)
			delete[] this->diverse;
		this->nr_diverse = ch.nr_diverse;
		if (ch.nr_diverse > 0)
		{
			this->diverse = new Diverse[ch.nr_diverse];
			for (int i = 0; i < ch.nr_diverse; i++)
				this->diverse[i] = ch.diverse[i];
		}
		else this->diverse = nullptr;
		this->suma_cheltuiala = ch.suma_cheltuiala;
		cheltuieli_pe_date[ch.data] += ch.suma_cheltuiala;
		suma_TVA += this->calculeazaTVA();
		return *this;
	}
	~Cheltuieli()
	{
		if (this->alimente != nullptr)
		{
			/*for (int i = 0; i < this->nr_alimente; i++)
				cheltuieli_pe_date[data] -= this->alimente[i].get_pret();*/
			delete[] this->alimente;
		}
		if (this->utilitati != nullptr)
		{
			/*for (int i = 0; i < this->nr_utilitati; i++)
				cheltuieli_pe_date[data] -= this->utilitati[i].get_pret();*/
			delete[] this->utilitati;
		}
		if (this->divertisment != nullptr)
		{
			/*for (int i = 0; i < this->nr_divertisment; i++)
				cheltuieli_pe_date[data] -= this->divertisment[i].get_pret();*/
			delete[] this->divertisment;
		}
		if (this->diverse != nullptr)
		{
			/*for (int i = 0; i < this->nr_diverse; i++)
				cheltuieli_pe_date[data] -= this->diverse[i].get_pret();*/
			delete[] this->diverse;
		}
		//suma_TVA -= this->calculeazaTVA();
	}
	//getteri
	static float get_suma_TVA()
	{
		return suma_TVA;
	}
	float get_suma_cheltuiala()
	{
		return this->suma_cheltuiala;
	}
	const int get_data()
	{
		return this->data;
	}
	int get_nr_alimente()
	{
		return this->nr_alimente;
	}
	void get_alimente()
	{
		if (this->nr_alimente > 0)
		{
			for (int i = 0; i < this->nr_alimente; i++)
				cout << this->alimente[i];
		}
		else cerr << "\nNu sunt alimente adaugate.\n";
	}
	int get_nr_utilitati()
	{
		return this->nr_utilitati;
	}
	void get_utilitati()
	{
		if (this->nr_utilitati > 0)
		{
			for (int i = 0; i < this->nr_utilitati; i++)
				cout << this->utilitati[i];
		}
		else cerr << "\nNu sunt utilitati adugate.\n";
	}
	int get_nr_divertismente()
	{
		return this->nr_divertisment;
	}
	void get_divertisment()
	{
		if (this->nr_divertisment > 0)
		{
			for (int i = 0; i < this->nr_divertisment; i++)
				cout << this->divertisment[i];
		}
		else cerr << "\nNu sunt divertismente adaugate.\n";
	}
	int get_nr_diverse()
	{
		return this->nr_diverse;
	}
	void get_diverse()
	{
		if (this->nr_diverse > 0)
		{
			for (int i = 0; i < this->nr_diverse; i++)
				cout << this->diverse[i];
		}
		else cerr << "\nNu sunt diverse adaugate.\n";
	}
	static void get_cheltuiala_pe_date()
	{
		bool j = 0;
		for (int i = 0; i <= 30; i++)
			if (cheltuieli_pe_date[i] != 0)
			{
				cout << "In ziua: " << i + 1 << " ati cheltuit: " << cheltuieli_pe_date[i] << " u.m." << endl;
				j = true;
			}
		if (!j)
			cout << "Nu aveti cheltuieli!\n";
	}
	static float get_suma_totala_pe_zile()
	{
		float suma = 0;
		for (int i = 0; i < 31; i++)
			if (cheltuieli_pe_date[i] != 0)
				suma += cheltuieli_pe_date[i];
		return suma;
	}
	//setteri

	//setarea numarului de alimente; in cazul in care numarul setat este mai mare \
	decat numarul intial de alimente se va citi un aliment de la tastatura
	Cheltuieli& set_nr_alimente(int nr_alimente)
	{
		if (nr_alimente >= 0)
		{
			if (nr_alimente < this->nr_alimente)
			{
				Alimente* copie;
				copie = new Alimente[this->nr_alimente];
				for (int i = 0; i < this->nr_alimente; i++)
					copie[i] = this->alimente[i];
				if (this->alimente != nullptr)
					delete[] this->alimente;
				this->alimente = new Alimente[nr_alimente];
				for (int i = 0; i < nr_alimente; i++)
					this->alimente[i] = copie[i];
				int diferenta = this->nr_alimente - nr_alimente;
				while (diferenta)
				{
					this->suma_cheltuiala -= copie[this->nr_alimente - diferenta].get_pret();
					cheltuieli_pe_date[this->data - 1] -= copie[this->nr_alimente - diferenta].get_pret();
					diferenta--;
				}
				this->nr_alimente = nr_alimente;
			}
			else if (nr_alimente > this->nr_alimente)
			{
				Alimente* copie;
				copie = new Alimente[this->nr_alimente];
				for (int i = 0; i < this->nr_alimente; i++)
					copie[i] = this->alimente[i];
				if (this->alimente != nullptr)
					delete[] this->alimente;
				this->alimente = new Alimente[nr_alimente];
				for (int i = 0; i < this->nr_alimente; i++)
					this->alimente[i] = copie[i];
				int diferenta = nr_alimente - this->nr_alimente;
				this->nr_alimente = nr_alimente;
				while (diferenta)
				{
					Alimente a;
					cout << "\nAliment adaugat:\n";
					cin >> a;
					this->alimente[nr_alimente - diferenta] = a;
					this->suma_cheltuiala += a.get_pret();
					cheltuieli_pe_date[this->data - 1] += a.get_pret();
					diferenta--;
				}
			}
		}
		else cerr << "\nNu puteti introduce un numar negativ de alimente.\n";
		return *this;
	}
	//setarea numarului de utilitati; in cazul in care numarul setat este mai mare \
	decat numarul intial de utilitati se va citi o utilitate de la tastatura
	Cheltuieli& set_nr_utilitati(int nr_utilitati)
	{
		if (nr_utilitati >= 0)
		{
			if (nr_utilitati < this->nr_utilitati)
			{
				Utilitati* copie;
				copie = new Utilitati[this->nr_utilitati];
				for (int i = 0; i < this->nr_utilitati; i++)
					copie[i] = this->utilitati[i];
				if (this->utilitati != nullptr)
					delete[] this->utilitati;
				this->utilitati = new Utilitati[nr_utilitati];
				for (int i = 0; i < nr_utilitati; i++)
					this->utilitati[i] = copie[i];
				int diferenta = this->nr_utilitati - nr_utilitati;
				while (diferenta)
				{
					this->suma_cheltuiala -= copie[this->nr_utilitati - diferenta].get_pret();
					cheltuieli_pe_date[this->data - 1] -= copie[this->nr_utilitati - diferenta].get_pret();
					diferenta--;
				}
				this->nr_utilitati = nr_utilitati;
			}
			else if (nr_utilitati > this->nr_utilitati)
			{
				Utilitati* copie;
				copie = new Utilitati[this->nr_utilitati];
				for (int i = 0; i < this->nr_utilitati; i++)
					copie[i] = this->utilitati[i];
				if (this->utilitati != nullptr)
					delete[] this->utilitati;
				this->utilitati = new Utilitati[nr_utilitati];
				for (int i = 0; i < this->nr_utilitati; i++)
					this->utilitati[i] = copie[i];
				int diferenta = nr_utilitati - this->nr_utilitati;
				this->nr_utilitati = nr_utilitati;
				while (diferenta)
				{
					Utilitati u;
					cout << "\nUtilitate adaugata:\n";
					cin >> u;
					this->utilitati[nr_utilitati - diferenta] = u;
					this->suma_cheltuiala += u.get_pret();
					cheltuieli_pe_date[this->data - 1] += u.get_pret();
					diferenta--;
				}
			}
		}
		else cerr << "\nNu puteti introduce un numar negativ de utilitati.\n";
		return *this;
	}
	//setarea numarului de divertismente; in cazul in care numarul setat este mai mare \
	decat numarul intial de divertismente se va citi un divertisment de la tastatura
	Cheltuieli& set_nr_divertisment(int nr_divertisment)
	{
		if (nr_divertisment >= 0)
		{
			if (nr_divertisment < this->nr_divertisment)
			{
				Divertisment* copie;
				copie = new Divertisment[this->nr_divertisment];
				for (int i = 0; i < this->nr_divertisment; i++)
					copie[i] = this->divertisment[i];
				if (this->divertisment != nullptr)
					delete[] this->divertisment;
				this->divertisment = new Divertisment[nr_divertisment];
				for (int i = 0; i < nr_divertisment; i++)
					this->divertisment[i] = copie[i];
				int diferenta = this->nr_divertisment - nr_divertisment;
				while (diferenta)
				{
					this->suma_cheltuiala -= copie[this->nr_divertisment - diferenta].get_pret();
					cheltuieli_pe_date[this->data - 1] -= copie[this->nr_divertisment - diferenta].get_pret();
					diferenta--;
				}
				this->nr_divertisment = nr_divertisment;
			}
			else if (nr_divertisment > this->nr_divertisment)
			{
				Divertisment* copie;
				copie = new Divertisment[this->nr_divertisment];
				for (int i = 0; i < this->nr_divertisment; i++)
					copie[i] = this->divertisment[i];
				if (this->divertisment != nullptr)
					delete[] this->divertisment;
				this->divertisment = new Divertisment[nr_divertisment];
				for (int i = 0; i < this->nr_divertisment; i++)
					this->divertisment[i] = copie[i];
				int diferenta = nr_divertisment - this->nr_divertisment;
				this->nr_divertisment = nr_divertisment;
				while (diferenta)
				{
					Divertisment d;
					cout << "\nDivertisment adaugat:\n";
					cin >> d;
					this->divertisment[nr_divertisment - diferenta] = d;
					this->suma_cheltuiala += d.get_pret();
					cheltuieli_pe_date[this->data - 1] += d.get_pret();
					diferenta--;
				}
			}
		}
		else cerr << "\nNu puteti introduce un numar negativ de divertismente.\n";
		return *this;
	}
	//setarea numarului de diverse; in cazul in care numarul setat este mai mare \
	decat numarul intial de diverse se va citi o "diversa" de la tastatura
	Cheltuieli& set_nr_diverse(int nr_diverse)
	{
		if (nr_diverse >= 0)
		{
			if (nr_diverse < this->nr_diverse)
			{
				Diverse* copie;
				copie = new Diverse[this->nr_diverse];
				for (int i = 0; i < this->nr_diverse; i++)
					copie[i] = this->diverse[i];
				if (this->diverse != nullptr)
					delete[] this->diverse;
				this->diverse = new Diverse[nr_diverse];
				for (int i = 0; i < nr_diverse; i++)
					this->diverse[i] = copie[i];
				int diferenta = this->nr_diverse - nr_diverse;
				while (diferenta)
				{
					this->suma_cheltuiala -= copie[this->nr_diverse - diferenta].get_pret();
					cheltuieli_pe_date[this->data - 1] -= copie[this->nr_diverse - diferenta].get_pret();
					diferenta--;
				}
				this->nr_diverse = nr_diverse;
			}
			else if (nr_diverse > this->nr_diverse)
			{
				Diverse* copie;
				copie = new Diverse[this->nr_diverse];
				for (int i = 0; i < this->nr_diverse; i++)
					copie[i] = this->diverse[i];
				if (this->diverse != nullptr)
					delete[] this->diverse;
				this->diverse = new Diverse[nr_diverse];
				for (int i = 0; i < this->nr_diverse; i++)
					this->diverse[i] = copie[i];
				int diferenta = nr_diverse - this->nr_diverse;
				this->nr_diverse = nr_diverse;
				while (diferenta)
				{
					Diverse div;
					cout << "\nDiverse adaugate:\n";
					cin >> div;
					this->diverse[nr_diverse - diferenta] = div;
					this->suma_cheltuiala += div.get_pret();
					cheltuieli_pe_date[this->data - 1] += div.get_pret();
					diferenta--;
				}
			}
		}
		else cerr << "\nNu puteti introduce un numar negativ de diverse.\n";
		return *this;
	}
	//modifica vectorul de alimente initial prin inlocuirea cu altul dat:
	Cheltuieli& set_alimente(int nr_alimente, Alimente* a)
	{
		if (nr_alimente > 0 && a != nullptr)
		{
			cheltuieli_pe_date[this->data - 1] -= this->suma_cheltuiala;
			for (int i = 0; i < this->nr_alimente; i++)
				suma_cheltuiala -= this->alimente[i].get_pret();
			if (this->alimente != nullptr)
				delete[] this->alimente;
			this->alimente = new Alimente[nr_alimente];
			this->nr_alimente = nr_alimente;
			for (int i = 0; i < nr_alimente; i++)
			{
				this->alimente[i] = a[i];
				suma_cheltuiala += a[i].get_pret();
			}
			cheltuieli_pe_date[this->data - 1] += this->suma_cheltuiala;
		}
		else if (nr_alimente == 0 || a == nullptr)
		{
			cheltuieli_pe_date[this->data - 1] -= this->suma_cheltuiala;
			for (int i = 0; i < this->nr_alimente; i++)
				suma_cheltuiala -= this->alimente[i].get_pret();
			if (this->alimente != nullptr)
				delete[] this->alimente;
			this->alimente = nullptr;
			this->nr_alimente = 0;
		}
		else cerr << "\nNumarul de alimente nu poate fi negativ!\n";
		return *this;
	}
	//modifica vectorul de utilitati initial prin inlocuirea cu altul dat:
	Cheltuieli& set_utilitati(int nr_utilitati, Utilitati* u)
	{
		if (nr_utilitati > 0 && u != nullptr)
		{
			cheltuieli_pe_date[this->data - 1] -= this->suma_cheltuiala;
			for (int i = 0; i < this->nr_utilitati; i++)
				suma_cheltuiala -= this->utilitati[i].get_pret();
			if (this->utilitati != nullptr)
				delete[] this->utilitati;
			this->utilitati = new Utilitati[nr_utilitati];
			this->nr_utilitati = nr_utilitati;
			for (int i = 0; i < nr_utilitati; i++)
			{
				this->utilitati[i] = u[i];
				suma_cheltuiala += u[i].get_pret();
			}
			cheltuieli_pe_date[this->data - 1] += this->suma_cheltuiala;
		}
		else if (nr_utilitati == 0 || u == nullptr)
		{
			cheltuieli_pe_date[this->data - 1] -= this->suma_cheltuiala;
			for (int i = 0; i < this->nr_utilitati; i++)
				suma_cheltuiala -= this->utilitati[i].get_pret();
			if (this->utilitati != nullptr)
				delete[] this->utilitati;
			this->utilitati = nullptr;
			this->nr_utilitati = 0;
		}
		else cerr << "\nNumarul de utilitati nu poate fi negativ!\n";
		return *this;
	}
	//modifica vectorul de divertisment initial prin inlocuirea cu altul dat:
	Cheltuieli& set_divertisment(int nr_divertisment, Divertisment* d)
	{
		if (nr_divertisment > 0 && d != nullptr)
		{
			cheltuieli_pe_date[this->data - 1] -= this->suma_cheltuiala;
			for (int i = 0; i < this->nr_divertisment; i++)
				suma_cheltuiala -= this->divertisment[i].get_pret();
			if (this->divertisment != nullptr)
				delete[] this->divertisment;
			this->divertisment = new Divertisment[nr_divertisment];
			this->nr_divertisment = nr_divertisment;
			for (int i = 0; i < nr_divertisment; i++)
			{
				this->divertisment[i] = d[i];
				suma_cheltuiala += d[i].get_pret();
			}
			cheltuieli_pe_date[this->data - 1] += this->suma_cheltuiala;
		}
		else if (nr_divertisment == 0 || d == nullptr)
		{
			cheltuieli_pe_date[this->data - 1] -= this->suma_cheltuiala;
			for (int i = 0; i < this->nr_divertisment; i++)
				suma_cheltuiala -= this->divertisment[i].get_pret();
			if (this->divertisment != nullptr)
				delete[] this->divertisment;
			this->divertisment = nullptr;
			this->nr_divertisment = 0;
		}
		else cerr << "\nNumarul de utilitati nu poate fi negativ!\n";
		return *this;
	}
	//modifica vectorul de diverse initial prin inlocuirea cu altul dat:
	Cheltuieli& set_diverse(int nr_diverse, Diverse* div)
	{
		if (nr_diverse > 0 && div != nullptr)
		{
			cheltuieli_pe_date[this->data - 1] -= this->suma_cheltuiala;
			for (int i = 0; i < this->nr_diverse; i++)
				suma_cheltuiala -= this->diverse[i].get_pret();
			if (this->diverse != nullptr)
				delete[] this->diverse;
			this->diverse = new Diverse[nr_diverse];
			this->nr_diverse = nr_diverse;
			for (int i = 0; i < nr_diverse; i++)
			{
				this->diverse[i] = div[i];
				suma_cheltuiala += div[i].get_pret();
			}
			cheltuieli_pe_date[this->data - 1] += this->suma_cheltuiala;
		}
		else if (nr_diverse == 0 || div == nullptr)
		{
			cheltuieli_pe_date[this->data - 1] -= this->suma_cheltuiala;
			for (int i = 0; i < this->nr_diverse; i++)
				suma_cheltuiala -= this->diverse[i].get_pret();
			if (this->diverse != nullptr)
				delete[] this->diverse;
			this->diverse = nullptr;
			this->nr_diverse = 0;
		}
		else cerr << "\nNumarul de utilitati nu poate fi negativ!\n";
		return *this;
	}
	void set_cheltuieli_pe_data(int data)
	{
		cheltuieli_pe_date[data - 1] += this->suma_cheltuiala;
	}
	//operator++ care adauga un aliment dat de utilizator 
	//		pre
	Cheltuieli& operator++()
	{
		if (this->alimente != nullptr)
		{
			Alimente* copie; copie = new Alimente[this->nr_alimente];
			for (int i = 0; i < this->nr_alimente; i++)
				copie[i] = this->alimente[i];
			delete[] this->alimente;
			this->nr_alimente++;
			this->alimente = new Alimente[this->nr_alimente];
			Alimente a;
			cout << "\nAliment adaugat:\n";
			cin >> a;
			for (int i = 0; i < this->nr_alimente - 1; i++)
				this->alimente[i] = copie[i];
			this->alimente[nr_alimente - 1] = a;
			this->suma_cheltuiala += a.get_pret();
			cheltuieli_pe_date[this->data - 1] += a.get_pret();
		}
		else if (this->alimente == nullptr)
		{
			this->nr_alimente = 1;
			this->alimente = new Alimente[this->nr_alimente];
			Alimente a;
			cout << "\nAliment adaugat:\n";
			cin >> a;
			this->alimente[0] = a;
			this->suma_cheltuiala += a.get_pret();
			cheltuieli_pe_date[this->data - 1] += a.get_pret();
		}
		return *this;
	}
	//		post
	Cheltuieli operator++(int)
	{
		Cheltuieli copy = *this;
		if (this->alimente != nullptr)
		{
			Alimente* copie; copie = new Alimente[this->nr_alimente];
			for (int i = 0; i < this->nr_alimente; i++)
				copie[i] = this->alimente[i];
			delete[] this->alimente;
			this->nr_alimente++;
			this->alimente = new Alimente[this->nr_alimente];
			Alimente a;
			cout << "\nAliment adaugat:\n";
			cin >> a;
			for (int i = 0; i < this->nr_alimente - 1; i++)
				this->alimente[i] = copie[i];
			this->alimente[this->nr_alimente - 1] = a;
			this->suma_cheltuiala += a.get_pret();
			cheltuieli_pe_date[this->data - 1] += a.get_pret();
		}
		else if (this->alimente == nullptr)
		{
			this->nr_alimente = 1;
			this->alimente = new Alimente[this->nr_alimente];
			Alimente a;
			cout << "\nAliment adaugat:\n";
			cin >> a;
			this->alimente[0] = a;
			this->suma_cheltuiala += a.get_pret();
			cheltuieli_pe_date[this->data - 1] += a.get_pret();
		}
		return copy;
	}
	//operator+ care adauga o suma la cheltuiala pe ziua cheltuielii
	float operator+(float sum)
	{
		return Cheltuieli::cheltuieli_pe_date[this->data - 1] += sum;
	}
	//supraincarcare operator[] care extrage cheltuiala totala \
					facuta intr-o anumita zi
	float operator[](const int index)
	{
		if (index > 31 || index < 1)
		{
			throw "Zi invalida!";

		}
		else if (cheltuieli_pe_date[index - 1] != 0)
		{
			return cheltuieli_pe_date[index - 1];
		}
		else {
			throw "Nu aveti nicio cheltuiala in aceasta zi!";
			//Sau ziua introdusa nu exista in luna actuala!";
		}
	}
	//operatorul cast:
	explicit operator float()
	{
		return this->suma_cheltuiala;
	}
	//operator! care sa spuna daca o cheltuiala are sau nu alimente
	void operator!()
	{
		if (this->nr_alimente == 0)
			cout << "\nCheltuiala introdusa nu contine alimente.\n";
		else
			cout << "\nCheltuiala introdusa contine alimente.\n";
	}
	//operator conditional > care compara cheltuiala dintre doua date/zile
	void operator>(Cheltuieli c)
	{
		if (c.data == this->data)
			cerr << "\nComparati aceeasi zi!\n";
		else
			if (c.cheltuieli_pe_date[c.data] > this->cheltuieli_pe_date[this->data])
				cout << "\nCheltuiala din ziua " << c.data << " este mai mare decat cheltuiala din ziua " << this->data << endl;
			else
				cout << "\nCheltuiala din ziua " << this->data << " este mai mare decat cheltuiala din ziua " << c.data << endl;
	}
	//operator== pentru testarea dintre doua obiecte
	bool operator==(Cheltuieli c)
	{
		if (this->data != c.data)
			return false;
		else if (this->nr_alimente != c.nr_alimente)
			return false;
		else if (this->nr_alimente == c.nr_alimente) {
			for (int i = 0; i < this->nr_alimente; i++)
				if ((this->alimente[i] == c.alimente[i]) == false)
					return false;
		}
		else if (this->nr_utilitati != c.nr_utilitati)
			return false;
		else if (this->nr_utilitati == c.nr_utilitati) {
			for (int i = 0; i < this->nr_utilitati; i++)
				if ((this->utilitati[i] == c.utilitati[i]) == false)
					return false;
		}
		else if (this->nr_divertisment != c.nr_divertisment)
			return false;
		else if (this->nr_divertisment == c.nr_divertisment) {
			for (int i = 0; i < this->nr_divertisment; i++)
				if ((this->divertisment[i] == c.divertisment[i]) == false)
					return false;
		}
		else if (this->nr_diverse != c.nr_diverse)
			return false;
		else if (this->nr_diverse == c.nr_diverse) {
			for (int i = 0; i < this->nr_diverse; i++)
				if ((this->diverse[i] == c.diverse[i]) == false)
					return false;
		}
		else if (c.suma_cheltuiala != this->suma_cheltuiala) //redundant, deoarece se verfica preturile mai sus pt toate obiectele de \
															 tip alimente/diverse/divertisment/utilitati
			return false;
		return true;
	}
	void sterge_suma_pe_data()
	{
		cheltuieli_pe_date[this->data - 1] -= this->suma_cheltuiala;
	}
	void sterge_pret_aliment(int i)
	{
		this->alimente[i].sterge_pret_aliment();
	}
	void sterge_pret_utilitate(int i)
	{
		this->utilitati[i].sterge_pret_utilitate();
	}
	void sterge_pret_divertisment(int i)
	{
		this->divertisment[i].sterge_pret_divertisment();
	}
	void sterge_pret_diversa(int i)
	{
		this->diverse[i].sterge_pret_diversa();
	}
	static float get_chel_pe_data(int i)
	{
		return cheltuieli_pe_date[i - 1];
	}
	//Metoda care calculeaza suma TVA
	float calculeazaTVA()
	{
		float suma_TVA = 0;
		for (int i = 0; i < this->nr_alimente; i++)
		{
			this->alimente[i].set_TVA();
			suma_TVA += this->alimente[i].get_TVA() * this->alimente[i].get_pret();
		}
		for (int i = 0; i < this->nr_utilitati; i++)
		{
			this->utilitati[i].set_TVA();
			suma_TVA += this->utilitati[i].get_TVA() * this->utilitati[i].get_pret();
		}
		for (int i = 0; i < this->nr_divertisment; i++)
		{
			this->divertisment[i].set_TVA();
			suma_TVA += this->divertisment[i].get_TVA() * this->divertisment[i].get_pret();
		}
		for (int i = 0; i < this->nr_diverse; i++)
		{
			this->diverse[i].set_TVA();
			suma_TVA += this->diverse[i].get_TVA() * this->diverse[i].get_pret();
		}
		return suma_TVA;
	}
	//Metoda care sterge TVA-ul unei cheltuieli
	void sterge_TVA()
	{
		Cheltuieli::suma_TVA -= this->calculeazaTVA();
	}
	//Metoda care sterge necesitatile din vectorul de necesitati
	void sterge_necesitati()
	{
		for (int i = 0; i < this->nr_alimente; i++)
		{
			this->alimente[i].stergeNecesitate();
		}
		for (int i = 0; i < this->nr_utilitati; i++)
		{
			this->utilitati[i].stergeNecesitate();
		}
		for (int i = 0; i < this->nr_divertisment; i++)
		{
			this->divertisment[i].stergeNecesitate();
		}
		for (int i = 0; i < this->nr_diverse; i++)
		{
			this->diverse[i].stergeNecesitate();
		}
	}
	friend ostream& operator<<(ostream&, const Cheltuieli&);
	friend istream& operator>>(istream&, Cheltuieli&);
	friend ofstream& operator<<(ofstream&, Cheltuieli&);
	friend ifstream& operator>>(ifstream&, Cheltuieli&);
	friend class Lucru_cu_fisiere;
};
float Cheltuieli::cheltuieli_pe_date[31] = { 0 };
float Cheltuieli::suma_TVA = 0;

ostream& operator<<(ostream& out, const Cheltuieli& ch)
{
	//out << "\nDATA:\n" << ch.data;
	out << "\nALIMENTE:\n";
	if (ch.alimente != nullptr)
		for (int i = 0; i < ch.nr_alimente; i++)
			out << ch.alimente[i];
	else out << " - ";
	out << "\n\nUTILITATI:\n";
	if (ch.utilitati != nullptr)
		for (int i = 0; i < ch.nr_utilitati; i++)
			out << ch.utilitati[i];
	else out << " - ";
	out << "\n\nDIVERTISMENT:\n";
	if (ch.divertisment != nullptr)
		for (int i = 0; i < ch.nr_divertisment; i++)
			out << ch.divertisment[i];
	else out << " - ";
	out << "\n\nDIVERSE:\n";
	if (ch.diverse != nullptr)
		for (int i = 0; i < ch.nr_diverse; i++)
			out << ch.diverse[i];
	else out << " - ";
	return out;
}
istream& operator>>(istream& in, Cheltuieli& ch) {
	time_t now = time(0);
	tm* data = localtime(&now);
	int data_curenta = data->tm_mday;
	//stergem cheltuielile aferente vechiului obiect
	ch.cheltuieli_pe_date[ch.data - 1] -= ch.suma_cheltuiala;
	Cheltuieli::suma_TVA -= ch.calculeazaTVA();
	ch.suma_cheltuiala = 0;
	cout << "\nNumar alimente: ";
	in >> ch.nr_alimente;
	if (ch.nr_alimente < 0)
	{
		cerr << "Nu puteti introduce un numar negativ de alimente!";
		ch.nr_alimente = 0;
		if (ch.alimente != nullptr)
			delete[] ch.alimente;
	}
	if (ch.nr_alimente > 0)
	{
		cout << "\nAlimentele:\n";
		if (ch.alimente != nullptr)
			delete[] ch.alimente;
		ch.alimente = new Alimente[ch.nr_alimente];
		for (int i = 0; i < ch.nr_alimente; i++)
		{
			in >> ch.alimente[i];
			ch.suma_cheltuiala += ch.alimente[i].get_pret();
		}

	}
	else if (ch.nr_alimente == 0)
	{
		if (ch.alimente != nullptr)
			delete[] ch.alimente;
		ch.alimente = nullptr;
	}
	cout << "\nNumar utilitati: ";
	in >> ch.nr_utilitati;
	if (ch.nr_utilitati < 0)
	{
		cerr << "Nu puteti introduce un numar negativ de utilitati!";
		ch.nr_utilitati = 0;
		if (ch.utilitati != nullptr)
			delete[] ch.utilitati;
		ch.utilitati = nullptr;
	}
	if (ch.nr_utilitati > 0)
	{
		cout << "\nUtilitatile:\n";
		if (ch.utilitati != nullptr)
			delete[] ch.utilitati;
		ch.utilitati = new Utilitati[ch.nr_utilitati];
		for (int i = 0; i < ch.nr_utilitati; i++)
		{
			in >> ch.utilitati[i];
			ch.suma_cheltuiala += ch.utilitati[i].get_pret();
		}
	}
	else if (ch.nr_utilitati == 0)
	{
		if (ch.utilitati != nullptr)
			delete[] ch.utilitati;
		ch.utilitati = nullptr;
	}
	cout << "\nNumar divertismente: ";
	in >> ch.nr_divertisment;
	if (ch.nr_divertisment < 0)
	{
		cerr << "Nu puteti introduce un numar negativ de divertismente!";
		ch.nr_divertisment = 0;
		if (ch.divertisment != nullptr)
			delete[] ch.divertisment;
		ch.divertisment = nullptr;
	}
	if (ch.nr_divertisment > 0)
	{
		cout << "\nDivertismentele:\n";
		if (ch.divertisment != nullptr)
			delete[] ch.divertisment;
		ch.divertisment = new Divertisment[ch.nr_divertisment];
		for (int i = 0; i < ch.nr_divertisment; i++)
		{
			in >> ch.divertisment[i];
			ch.suma_cheltuiala += ch.divertisment[i].get_pret();
		}
	}
	else if (ch.nr_divertisment == 0)
	{
		if (ch.divertisment != nullptr)
			delete[] ch.divertisment;
		ch.divertisment = nullptr;
	}
	cout << "\nNumar diverse: ";
	in >> ch.nr_diverse;
	if (ch.nr_diverse < 0)
	{
		cerr << "Nu puteti introduce un numar negativ de diverse!";
		ch.nr_diverse = 0;
		if (ch.diverse != nullptr)
			delete[] ch.diverse;
		ch.diverse = nullptr;
	}
	if (ch.nr_diverse > 0)
	{
		cout << "\nDiversele:\n";
		if (ch.diverse != nullptr)
			delete[] ch.diverse;
		ch.diverse = new Diverse[ch.nr_diverse];
		for (int i = 0; i < ch.nr_diverse; i++)
		{
			in >> ch.diverse[i];
			ch.suma_cheltuiala += ch.diverse[i].get_pret();
		}
	}
	else if (ch.nr_diverse == 0)
	{
		if (ch.diverse != nullptr)
			delete[] ch.diverse;
		ch.diverse = nullptr;
	}
	ch.cheltuieli_pe_date[data_curenta - 1] += ch.suma_cheltuiala;
	Cheltuieli::suma_TVA += ch.calculeazaTVA();
	return in;
}
ofstream& operator<<(ofstream& out, Cheltuieli& ch)
{
	out << ch.data << endl;
	if (ch.alimente != nullptr)
	{
		out << ch.nr_alimente << endl;
		for (int i = 0; i < ch.nr_alimente; i++)
			out << ch.alimente[i];
	}
	else out << 0 << endl;
	if (ch.utilitati != nullptr)
	{
		out << ch.nr_utilitati << endl;
		for (int i = 0; i < ch.nr_utilitati; i++)
			out << ch.utilitati[i];
	}
	else out << 0 << endl;
	if (ch.divertisment != nullptr)
	{
		out << ch.nr_divertisment << endl;
		for (int i = 0; i < ch.nr_divertisment; i++)
			out << ch.divertisment[i];
	}
	else out << 0 << endl;
	if (ch.diverse != nullptr)
	{
		out << ch.nr_diverse << endl;
		for (int i = 0; i < ch.nr_diverse; i++)
			out << ch.diverse[i];
	}
	else out << 0 << endl;
	return out;
}
ifstream& operator>>(ifstream& in, Cheltuieli& ch)
{
	//stergem cheltuielile aferente vechiului obiect
	int data;
	in >> data;
	//	ch.cheltuieli_pe_date[ch.data] -= ch.suma_cheltuiala;
	//Cheltuieli::suma_TVA -= ch.calculeazaTVA();
	ch.suma_cheltuiala = 0;
	in >> ch.nr_alimente;
	if (ch.alimente != nullptr)
	{
		delete[] ch.alimente;
		ch.alimente = nullptr;
	}
	if (ch.nr_alimente > 0)
	{
		ch.alimente = new Alimente[ch.nr_alimente];
		for (int i = 0; i < ch.nr_alimente; i++)
		{
			in >> ch.alimente[i];
			ch.suma_cheltuiala += ch.alimente[i].get_pret();
		}
	}
	in >> ch.nr_utilitati;
	if (ch.utilitati != nullptr)
	{
		delete[] ch.utilitati;
		ch.utilitati = nullptr;
	}
	if (ch.nr_utilitati > 0)
	{
		ch.utilitati = new Utilitati[ch.nr_utilitati];
		for (int i = 0; i < ch.nr_utilitati; i++)
		{
			in >> ch.utilitati[i];
			ch.suma_cheltuiala += ch.utilitati[i].get_pret();
		}
	}
	in >> ch.nr_divertisment;
	if (ch.divertisment != nullptr)
	{
		delete[] ch.divertisment;
		ch.divertisment = nullptr;
	}
	if (ch.nr_divertisment > 0)
	{
		ch.divertisment = new Divertisment[ch.nr_divertisment];
		for (int i = 0; i < ch.nr_divertisment; i++)
		{
			in >> ch.divertisment[i];
			ch.suma_cheltuiala += ch.divertisment[i].get_pret();
		}
	}
	in >> ch.nr_diverse;
	if (ch.diverse != nullptr)
	{
		delete[] ch.diverse;
		ch.diverse = nullptr;
	}
	if (ch.nr_diverse > 0)
	{
		ch.diverse = new Diverse[ch.nr_diverse];
		for (int i = 0; i < ch.nr_diverse; i++)
		{
			in >> ch.diverse[i];
			ch.suma_cheltuiala += ch.diverse[i].get_pret();
		}
	}
	if (ch.suma_cheltuiala != 0)
		Cheltuieli::cheltuieli_pe_date[data - 1] += ch.suma_cheltuiala;
	Cheltuieli::suma_TVA += ch.calculeazaTVA();
	return in;
}
class Venit {
	static float venit;
public:
	Venit() {}
	Venit(const Venit& ven)
	{
		this->venit = ven.venit;
	}
	Venit& operator=(const Venit& ven)
	{
		this->venit = ven.venit;
		return *this;
	}
	~Venit() {}
	static float get_venit()
	{
		return venit;
	}
	static void set_venit(float ven)
	{
		if (ven > 0)
			venit = ven;
		else cerr << "\nVenitul nu poate fi negativ sau nul!\n";
	}
};
float Venit::venit = 0;

class Lucru_cu_fisiere
{
public:
	//Metoda care preia argumentele main si care introduce si cheltuielile din fisierele text in fisier binar
	void preluare_fisiere_text(ofstream& fisBin, int nrFis, char* denumireFis[])
	{
		if (nrFis > 1)
		{
			cout << nrFis << " argumente";
			cout << "\nFisierele: ";
			for (int i = 1; i < nrFis; i++)
				cout << endl << denumireFis[i];
			cout << endl;
		}
		//if (nrFis == 1)
		fisBin.open("Toate_cheltuielile.dat", ofstream::binary);
		if (fisBin.is_open())
		{
			for (int i = 1; i < nrFis; i++)
			{
				ifstream Fis(denumireFis[i]);
				if (Fis.is_open())
				{
					Cheltuieli ch;
					Fis.seekg(0, ios::beg);
					while (Fis.good())
					{
						Fis >> ch;
						cout << ch << "\nTotal: " << ch.get_suma_cheltuiala() << endl;
						this->scrieCheltuiala(fisBin, ch);
					}
					Fis.close();
				}
				else cout << "\nFisierul " << denumireFis[i] << " nu a putut fi deschis!";
			}
			fisBin.close();
		}
		else cout << "Fisierul binar nu s-a putut deschide!";
	}
	//Metoda care citeste o cheltuiala din CSV
	void citeste_din_CSV(ifstream& fisierCSV, Cheltuieli& ch)
	{
		char valoare[100];
		/*fisierCSV.getline(valoare, 100, ',');
		stringstream data_ch(valoare);
		int data;
		data_ch >> data;*/
		fisierCSV.getline(valoare, 100, ',');
		if (ch.alimente != nullptr)
			delete[] ch.alimente;
		ch.nr_alimente = 1;
		ch.alimente = new Alimente[ch.nr_alimente];
		if (strlen(valoare) != 0)
		{
			(ch.alimente)->set_denumire(valoare);
		}
		fisierCSV.getline(valoare, 100, ',');
		stringstream pretul_al(valoare);
		float pret_al = 0;
		pretul_al >> pret_al;
		ch.alimente->set_pret_CSV(pret_al);
		fisierCSV.getline(valoare, 100, ',');
		stringstream cat_al(valoare);
		int categorie_al;
		cat_al >> categorie_al;
		switch (categorie_al)
		{
		case 0:
			ch.alimente[0].set_categorie_CSV(Fructe);
			break;
		case 1:
			ch.alimente[0].set_categorie_CSV(Legume);
			break;
		case 2:
			ch.alimente[0].set_categorie_CSV(Lactate);
			break;
		case 3:
			ch.alimente[0].set_categorie_CSV(Carne);
			break;
		case 4:
			ch.alimente[0].set_categorie_CSV(Derivate_din_cereale);
			break;
		case 5:
			ch.alimente[0].set_categorie_CSV(Alta);
			break;
		default:
			ch.alimente[0].set_categorie_CSV(Alta);
			break;
		}
		fisierCSV.getline(valoare, 100, ',');
		stringstream nec_al(valoare);
		int necesitate_al;
		nec_al >> necesitate_al;
		switch (necesitate_al)
		{
		case 0:
			ch.alimente[0].set_necesitate(DelocNecesar);
			break;
		case 1:
			ch.alimente[0].set_necesitate(PutinNecesar);
			break;
		case 2:
			ch.alimente[0].set_necesitate(FoarteNecesar);
			break;
		default:
			ch.alimente[0].set_necesitate(FoarteNecesar);
			break;
		}
		ch.suma_cheltuiala += ch.alimente[0].get_pret();

		fisierCSV.getline(valoare, 100, ',');
		if (ch.utilitati != nullptr)
			delete[] ch.utilitati;
		ch.nr_utilitati = 1;
		ch.utilitati = new Utilitati[ch.nr_alimente];
		if (strlen(valoare) != 0)
		{
			(ch.utilitati)->set_denumire(valoare);
		}
		fisierCSV.getline(valoare, 100, ',');
		stringstream pretul_ut(valoare);
		float pret_ut = 0;
		pretul_ut >> pret_ut;
		ch.utilitati->set_pret_CSV(pret_ut);
		fisierCSV.getline(valoare, 100, ',');
		stringstream cat_ut(valoare);
		int categorie_ut;
		cat_ut >> categorie_ut;
		switch (categorie_ut)
		{
		case 0:
			ch.utilitati[0].set_categorie_CSV(Energie);
			break;
		case 1:
			ch.utilitati[0].set_categorie_CSV(Gaz);
			break;
		case 2:
			ch.utilitati[0].set_categorie_CSV(Apa);
			break;
		case 3:
			ch.utilitati[0].set_categorie_CSV(Telefonie);
			break;
		case 4:
			ch.utilitati[0].set_categorie_CSV(Internet);
			break;
		case 5:
			ch.utilitati[0].set_categorie_CSV(TV);
			break;
		case 6:
			ch.utilitati[0].set_categorie_CSV(Asigurari);
			break;
		case 7:
			ch.utilitati[0].set_categorie_CSV(Altele);
			break;
		default:
			ch.utilitati[0].set_categorie_CSV(Altele);
			break;
		}
		fisierCSV.getline(valoare, 100, ',');
		stringstream nec_ut(valoare);
		int necesitate_ut;
		nec_ut >> necesitate_ut;
		switch (necesitate_ut)
		{
		case 0:
			ch.utilitati[0].set_necesitate(DelocNecesar);
			break;
		case 1:
			ch.utilitati[0].set_necesitate(PutinNecesar);
			break;
		case 2:
			ch.utilitati[0].set_necesitate(FoarteNecesar);
			break;
		default:
			ch.utilitati[0].set_necesitate(FoarteNecesar);
			break;
		}
		ch.suma_cheltuiala += ch.utilitati[0].get_pret();

		fisierCSV.getline(valoare, 100, ',');
		if (ch.divertisment != nullptr)
			delete[] ch.divertisment;
		ch.nr_divertisment = 1;
		ch.divertisment = new Divertisment[ch.nr_divertisment];
		if (strlen(valoare) != 0)
		{
			(ch.divertisment)->set_denumire(valoare);
		}
		fisierCSV.getline(valoare, 100, ',');
		stringstream pretul_dt(valoare);
		float pret_dt = 0;
		pretul_dt >> pret_dt;
		ch.divertisment->set_pret_CSV(pret_dt);
		fisierCSV.getline(valoare, 100, ',');
		stringstream cat_dt(valoare);
		int categorie_dt;
		cat_dt >> categorie_dt;
		switch (categorie_dt)
		{
		case 0:
			ch.divertisment[0].set_categorie_CSV(Excursii);
			break;
		case 1:
			ch.divertisment[0].set_categorie_CSV(Petreceri);
			break;
		case 2:
			ch.divertisment[0].set_categorie_CSV(Iesiri);
			break;
		case 3:
			ch.divertisment[0].set_categorie_CSV(Alte_cheltuieli);
			break;
		default:
			ch.divertisment[0].set_categorie_CSV(Alte_cheltuieli);
			break;
		}
		fisierCSV.getline(valoare, 100, ',');
		stringstream nec_dt(valoare);
		int necesitate_dt;
		nec_dt >> necesitate_dt;
		switch (necesitate_dt)
		{
		case 0:
			ch.divertisment[0].set_necesitate(DelocNecesar);
			break;
		case 1:
			ch.divertisment[0].set_necesitate(PutinNecesar);
			break;
		case 2:
			ch.divertisment[0].set_necesitate(FoarteNecesar);
			break;
		default:
			ch.divertisment[0].set_necesitate(FoarteNecesar);
			break;
		}
		ch.suma_cheltuiala += ch.divertisment[0].get_pret();

		fisierCSV.getline(valoare, 100, ',');
		if (ch.diverse != nullptr)
			delete[] ch.diverse;
		ch.nr_diverse = 1;
		ch.diverse = new Diverse[ch.nr_diverse];
		if (strlen(valoare) != 0)
		{
			(ch.diverse)->set_denumire(valoare);
		}
		fisierCSV.getline(valoare, 100, ',');
		stringstream pretul_dv(valoare);
		float pret_dv = 0;
		pretul_dv >> pret_dv;
		ch.diverse->set_pret_CSV(pret_dv);
		fisierCSV.getline(valoare, 100, ',');
		stringstream cat_dv(valoare);
		int categorie_dv;
		cat_dv >> categorie_dv;
		switch (categorie_dv)
		{
		case 0:
			ch.diverse[0].set_categorie_CSV(Sanatate);
			break;
		case 1:
			ch.diverse[0].set_categorie_CSV(Educatie);
			break;
		case 2:
			ch.diverse[0].set_categorie_CSV(Copil);
			break;
		case 3:
			ch.diverse[0].set_categorie_CSV(Animale);
			break;
		case 4:
			ch.diverse[0].set_categorie_CSV(Imbracaminte);
			break;
		case 5:
			ch.diverse[0].set_categorie_CSV(Electrocasnice);
			break;
		case 6:
			ch.diverse[0].set_categorie_CSV(Mobilier);
			break;
		case 7:
			ch.diverse[0].set_categorie_CSV(Alte);
			break;
		default:
			ch.diverse[0].set_categorie_CSV(Alte);
			break;
		}
		fisierCSV.getline(valoare, 100, '\n');
		stringstream nec_dv(valoare);
		int necesitate_dv;
		nec_dv >> necesitate_dv;
		switch (necesitate_dv)
		{
		case 0:
			ch.diverse[0].set_necesitate(DelocNecesar);
			break;
		case 1:
			ch.diverse[0].set_necesitate(PutinNecesar);
			break;
		case 2:
			ch.diverse[0].set_necesitate(FoarteNecesar);
			break;
		default:
			ch.diverse[0].set_necesitate(FoarteNecesar);
			break;
		}
		ch.suma_cheltuiala += ch.diverse[0].get_pret();
		Cheltuieli::suma_TVA += ch.calculeazaTVA();
		//cheltuieli_pe_date[data] += this->suma_cheltuiala;
	}
	//Metoda care ajuta la citirea din fisiere text - se diferentiaza prin faptul ca nu aloca cheltuieli pe date
	void citeste_din_txt(ifstream& in, Cheltuieli& ch)
	{
		int data;
		in >> data;
		ch.suma_cheltuiala = 0;
		in >> ch.nr_alimente;
		if (ch.alimente != nullptr)
		{
			delete[] ch.alimente;
			ch.alimente = nullptr;
		}
		if (ch.nr_alimente > 0)
		{
			ch.alimente = new Alimente[ch.nr_alimente];
			for (int i = 0; i < ch.nr_alimente; i++)
			{
				in >> ch.alimente[i];
				ch.suma_cheltuiala += ch.alimente[i].get_pret();
			}
		}
		in >> ch.nr_utilitati;
		if (ch.utilitati != nullptr)
		{
			delete[] ch.utilitati;
			ch.utilitati = nullptr;
		}
		if (ch.nr_utilitati > 0)
		{
			ch.utilitati = new Utilitati[ch.nr_utilitati];
			for (int i = 0; i < ch.nr_utilitati; i++)
			{
				in >> ch.utilitati[i];
				ch.suma_cheltuiala += ch.utilitati[i].get_pret();
			}
		}
		in >> ch.nr_divertisment;
		if (ch.divertisment != nullptr)
		{
			delete[] ch.divertisment;
			ch.divertisment = nullptr;
		}
		if (ch.nr_divertisment > 0)
		{
			ch.divertisment = new Divertisment[ch.nr_divertisment];
			for (int i = 0; i < ch.nr_divertisment; i++)
			{
				in >> ch.divertisment[i];
				ch.suma_cheltuiala += ch.divertisment[i].get_pret();
			}
		}
		in >> ch.nr_diverse;
		if (ch.diverse != nullptr)
		{
			delete[] ch.diverse;
			ch.diverse = nullptr;
		}
		if (ch.nr_diverse > 0)
		{
			ch.diverse = new Diverse[ch.nr_diverse];
			for (int i = 0; i < ch.nr_diverse; i++)
			{
				in >> ch.diverse[i];
				ch.suma_cheltuiala += ch.diverse[i].get_pret();
			}
		}
		if (ch.suma_cheltuiala != 0)
			Cheltuieli::cheltuieli_pe_date[ch.data - 1] += ch.suma_cheltuiala;
	}
	void optiune11(float& suma_totala)
	{
		float sum = 0;
		string cale;
		cout << "Calea fisierului: ";
		cin >> cale;
		ofstream fisBin;
		fisBin.open("Toate_cheltuielile.dat", ofstream::binary | ios::app);
		if (fisBin.is_open())
		{
			if (fisBin.is_open())
			{
				ifstream Fis(cale);
				if (Fis.is_open())
				{
					Cheltuieli c;
					Fis.seekg(0, ios::beg);
					while (Fis.good())
					{
						this->citeste_din_txt(Fis, c);
						sum += c.get_suma_cheltuiala();
						c.sterge_suma_pe_data();
						for (int i = 0; i < c.get_nr_alimente(); i++)
							c.sterge_pret_aliment(i);
						for (int i = 0; i < c.get_nr_utilitati(); i++)
							c.sterge_pret_utilitate(i);
						for (int i = 0; i < c.get_nr_divertismente(); i++)
							c.sterge_pret_divertisment(i);
						for (int i = 0; i < c.get_nr_diverse(); i++)
							c.sterge_pret_diversa(i);
					}
					if (sum > (Venit::get_venit() - suma_totala))
					{
						cout << "\nSuma totala a cheltuielilor este mai mare decat venitul ramas!";
						cout << " Venitul ramas este de " << Venit::get_venit() - suma_totala << " u.m.";
					}
					else
					{
						Fis.seekg(0, ios::beg);
						while (Fis.good())
						{
							Fis >> c;
							cout << c << "\nTotal: " << c.get_suma_cheltuiala() << endl;
							ofstream fisierBinar("Toate_cheltuielile.dat", ios::app);
							this->scrieCheltuiala(fisierBinar, c);
							if (fisierBinar.is_open())
								fisierBinar.close();
						}
						suma_totala += sum;
						cout << "\n##  Venitul ramas este de " << Venit::get_venit() - suma_totala << " u.m.  ##\n";
					}
					Fis.close();
				}
				else cout << "\nFisierul " << cale << " nu a putut fi deschis!";
			}
			fisBin.close();
		}
		else cout << "Fisierul binar nu s-a putut deschide!";
	}

	//Metoda care citeste din fisiere binare 
	void citesteComun(ifstream& fisierBinar, Comun& c)
	{
		//	fisierBinar.read((char*)&TVA, sizeof(float));
		fisierBinar.read((char*)&c.nivel_necesitate, sizeof(Necesitate));
	}
	//Metoda care citeste alimente din fisiere binare 
	void citesteAliment(ifstream& fisierBinar, Alimente& a)
	{
		if (a.denumire != nullptr)
			delete[] a.denumire;
		int dimensiune;
		fisierBinar.read((char*)&dimensiune, sizeof(int));
		if (dimensiune == 1) a.denumire = nullptr;
		else
		{
			a.denumire = new char[dimensiune];
			fisierBinar.read(a.denumire, sizeof(char) * dimensiune);
		}
		fisierBinar.read((char*)&a.pret, sizeof(float));
		fisierBinar.read((char*)&a.categorie, sizeof(CategorieAlimente));
		citesteComun(fisierBinar, a);
	}
	//Metoda care citeste utilitati din fisiere binare
	void citesteUtilitate(ifstream& fisierBinar, Utilitati& u)
	{
		if (u.denumire != nullptr)
			delete[] u.denumire;
		int dimensiune;
		fisierBinar.read((char*)&dimensiune, sizeof(int));
		if (dimensiune == 1) u.denumire = nullptr;
		else
		{
			u.denumire = new char[dimensiune];
			fisierBinar.read(u.denumire, sizeof(char) * dimensiune);
		}
		fisierBinar.read((char*)&u.pret, sizeof(float));
		fisierBinar.read((char*)&u.categorie, sizeof(CategorieUtilitati));
		citesteComun(fisierBinar, u);
	}
	//Metoda care citeste divertismente din fisiere binare
	void citesteDivertisment(ifstream& fisierBinar, Divertisment& d)
	{
		if (d.denumire != nullptr)
			delete[] d.denumire;
		int dimensiune;
		fisierBinar.read((char*)&dimensiune, sizeof(int));
		if (dimensiune == 1) d.denumire = nullptr;
		else
		{
			d.denumire = new char[dimensiune];
			fisierBinar.read(d.denumire, sizeof(char) * dimensiune);
		}
		fisierBinar.read((char*)&d.pret, sizeof(float));
		fisierBinar.read((char*)&d.categorie, sizeof(CategorieDivertisment));
		citesteComun(fisierBinar, d);
	}
	//Metoda care citeste diverse din fisiere binare
	void citesteDiverse(ifstream& fisierBinar, Diverse& d)
	{
		if (d.denumire != nullptr)
			delete[] d.denumire;
		int dimensiune;
		fisierBinar.read((char*)&dimensiune, sizeof(int));
		if (dimensiune == 1) d.denumire = nullptr;
		else
		{
			d.denumire = new char[dimensiune];
			fisierBinar.read(d.denumire, sizeof(char) * dimensiune);
		}
		fisierBinar.read((char*)&d.pret, sizeof(float));
		fisierBinar.read((char*)&d.categorie, sizeof(CategorieDiverse));
		citesteComun(fisierBinar, d);
	}

	//Metoda care scrie în fisiere binare
	void scrieComun(ofstream& fisierBinar, Comun& c)
	{
		//	fisierBinar.write((char*)&TVA, sizeof(float));
		fisierBinar.write((char*)&c.nivel_necesitate, sizeof(Necesitate));
	}
	//Metoda care scrie alimente în fisiere binare
	void scrieAliment(ofstream& fisierBinar, Alimente& a)
	{
		if (a.denumire != nullptr)
		{
			int dimensiune = strlen(a.denumire) + 1;
			fisierBinar.write((char*)&dimensiune, sizeof(int));
			fisierBinar.write(a.denumire, sizeof(char) * dimensiune);
		}
		else
		{
			int dimensiune = 2;
			fisierBinar.write((char*)&dimensiune, sizeof(int));
			fisierBinar.write("-", sizeof(char) * dimensiune);
		}
		fisierBinar.write((char*)&a.pret, sizeof(float));
		fisierBinar.write((char*)&a.categorie, sizeof(CategorieAlimente));
		scrieComun(fisierBinar, a);
	}
	//Metoda care scrie diverse în fisiere binare
	void scrieDiverse(ofstream& fisierBinar, Diverse& d)
	{
		if (d.denumire != nullptr)
		{
			int dimensiune = strlen(d.denumire) + 1;
			fisierBinar.write((char*)&dimensiune, sizeof(int));
			fisierBinar.write(d.denumire, sizeof(char) * dimensiune);
		}
		else
		{
			int dimensiune = 2;
			fisierBinar.write((char*)&dimensiune, sizeof(int));
			fisierBinar.write("-", sizeof(char) * dimensiune);
		}
		fisierBinar.write((char*)&d.pret, sizeof(float));
		fisierBinar.write((char*)&d.categorie, sizeof(CategorieDiverse));
		scrieComun(fisierBinar, d);
	}
	//Metoda care scrie divertismente în fisiere binare
	void scrieDivertisment(ofstream& fisierBinar, Divertisment& d)
	{
		if (d.denumire != nullptr)
		{
			int dimensiune = strlen(d.denumire) + 1;
			fisierBinar.write((char*)&dimensiune, sizeof(int));
			fisierBinar.write(d.denumire, sizeof(char) * dimensiune);
		}
		else
		{
			int dimensiune = 2;
			fisierBinar.write((char*)&dimensiune, sizeof(int));
			fisierBinar.write("-", sizeof(char) * dimensiune);
		}
		fisierBinar.write((char*)&d.pret, sizeof(float));
		fisierBinar.write((char*)&d.categorie, sizeof(CategorieDivertisment));
		scrieComun(fisierBinar, d);
	}
	//Metoda care scrie utilitati în fisiere binare
	void scrieUtilitate(ofstream& fisierBinar, Utilitati& u)
	{
		if (u.denumire != nullptr)
		{
			int dimensiune = strlen(u.denumire) + 1;
			fisierBinar.write((char*)&dimensiune, sizeof(int));
			fisierBinar.write(u.denumire, sizeof(char) * dimensiune);
		}
		else
		{
			int dimensiune = 2;
			fisierBinar.write((char*)&dimensiune, sizeof(int));
			fisierBinar.write("-", sizeof(char) * dimensiune);
		}
		fisierBinar.write((char*)&u.pret, sizeof(float));
		fisierBinar.write((char*)&u.categorie, sizeof(CategorieUtilitati));
		scrieComun(fisierBinar, u);
	}

	//Metoda care scrie cheltuieli în fisiere binare
	void scrieCheltuiala(ofstream& fisierBinar, Cheltuieli& ch)
	{
		//fisierBinar.write((char*)&this->data, sizeof(int));
		fisierBinar.write((char*)&ch.nr_alimente, sizeof(int));
		for (int i = 0; i < ch.nr_alimente; i++)
			this->scrieAliment(fisierBinar, ch.alimente[i]);

		fisierBinar.write((char*)&ch.nr_utilitati, sizeof(int));
		for (int i = 0; i < ch.nr_utilitati; i++)
			this->scrieUtilitate(fisierBinar, ch.utilitati[i]);

		fisierBinar.write((char*)&ch.nr_divertisment, sizeof(int));
		for (int i = 0; i < ch.nr_divertisment; i++)
			this->scrieDivertisment(fisierBinar, ch.divertisment[i]);

		fisierBinar.write((char*)&ch.nr_diverse, sizeof(int));
		for (int i = 0; i < ch.nr_diverse; i++)
			this->scrieDiverse(fisierBinar, ch.diverse[i]);
	}
	//Metoda care citeste cheltuieli din fisiere binare
	void citesteCheltuiala(ifstream& fisierBinar, Cheltuieli& ch)
	{
		//fisierBinar.read((char*)&this->data, sizeof(int));
		fisierBinar.read((char*)&ch.nr_alimente, sizeof(int));
		if (ch.alimente != nullptr)
			delete[] ch.alimente;
		if (ch.nr_alimente == 0) ch.alimente = nullptr;
		else
		{
			ch.alimente = new Alimente[ch.nr_alimente];
			for (int i = 0; i < ch.nr_alimente; i++)
			{
				ch.alimente[i].stergeNecesitate();
				this->citesteAliment(fisierBinar, ch.alimente[i]);
			}

		}

		fisierBinar.read((char*)&ch.nr_utilitati, sizeof(int));
		if (ch.utilitati != nullptr)
			delete[] ch.utilitati;
		if (ch.nr_utilitati == 0) ch.utilitati = nullptr;
		else
		{
			ch.utilitati = new Utilitati[ch.nr_utilitati];
			for (int i = 0; i < ch.nr_utilitati; i++)
			{
				ch.utilitati[i].stergeNecesitate();
				this->citesteUtilitate(fisierBinar, ch.utilitati[i]);
			}
		}

		fisierBinar.read((char*)&ch.nr_divertisment, sizeof(int));
		if (ch.divertisment != nullptr)
			delete[] ch.divertisment;
		if (ch.nr_divertisment == 0) ch.divertisment = nullptr;
		else
		{
			ch.divertisment = new Divertisment[ch.nr_divertisment];
			for (int i = 0; i < ch.nr_divertisment; i++)
			{
				ch.divertisment[i].stergeNecesitate();
				this->citesteDivertisment(fisierBinar, ch.divertisment[i]);
			}
		}

		fisierBinar.read((char*)&ch.nr_diverse, sizeof(int));
		if (ch.diverse != nullptr)
			delete[] ch.diverse;
		if (ch.nr_diverse == 0) ch.diverse = nullptr;
		else
		{
			ch.diverse = new Diverse[ch.nr_diverse];
			for (int i = 0; i < ch.nr_diverse; i++)
			{
				ch.diverse[i].stergeNecesitate();
				this->citesteDiverse(fisierBinar, ch.diverse[i]);
			}
		}

	}

	//Metode ce ajuta in meniu
	void sume_totale_pe_cat(string cale)
	{
		ofstream FisierUtilizator(cale);
		if (FisierUtilizator.is_open())
		{
			FisierUtilizator << "SUMELE TOTALE:";
			FisierUtilizator << "\nTotal alimente: " << Alimente::calculeazaSumaTotala() << " u.m.";
			FisierUtilizator << "\n\tFructe: " << Alimente::cheltuiala_pe_categorie(Fructe) << " u.m.";
			FisierUtilizator << "\n\tLegume: " << Alimente::cheltuiala_pe_categorie(Legume) << " u.m.";
			FisierUtilizator << "\n\tLactate: " << Alimente::cheltuiala_pe_categorie(Lactate) << " u.m.";
			FisierUtilizator << "\n\tCarne: " << Alimente::cheltuiala_pe_categorie(Carne) << " u.m.";
			FisierUtilizator << "\n\tDerivate din cereale: " << Alimente::cheltuiala_pe_categorie(Derivate_din_cereale) << " u.m.";
			FisierUtilizator << "\n\tAlta: " << Alimente::cheltuiala_pe_categorie(Alta) << " u.m.";
			FisierUtilizator << endl;
			FisierUtilizator << "\nTotal utilitati: " << Utilitati::calculeazaSumaTotala() << " u.m.";
			FisierUtilizator << "\n\tEnergie: " << Utilitati::get_cheltuialaPeCategorie(Energie) << " u.m.";
			FisierUtilizator << "\n\tGaz: " << Utilitati::get_cheltuialaPeCategorie(Gaz) << " u.m.";
			FisierUtilizator << "\n\tApa: " << Utilitati::get_cheltuialaPeCategorie(Apa) << " u.m.";
			FisierUtilizator << "\n\tTelefonie: " << Utilitati::get_cheltuialaPeCategorie(Telefonie) << " u.m.";
			FisierUtilizator << "\n\tInternet: " << Utilitati::get_cheltuialaPeCategorie(Internet) << " u.m.";
			FisierUtilizator << "\n\tTV: " << Utilitati::get_cheltuialaPeCategorie(TV) << " u.m.";
			FisierUtilizator << "\n\tAsigurari: " << Utilitati::get_cheltuialaPeCategorie(Asigurari) << " u.m.";
			FisierUtilizator << "\n\tAltele: " << Utilitati::get_cheltuialaPeCategorie(Altele) << " u.m.";
			FisierUtilizator << endl;
			FisierUtilizator << "\nTotal divertisment: " << Divertisment::calculeazaSumaTotala() << " u.m.";
			FisierUtilizator << "\n\tExcursii: " << Divertisment::get_cheltuialaPeCategorie(Excursii) << " u.m.";
			FisierUtilizator << "\n\tPetreceri: " << Divertisment::get_cheltuialaPeCategorie(Petreceri) << " u.m.";
			FisierUtilizator << "\n\tIesiri: " << Divertisment::get_cheltuialaPeCategorie(Iesiri) << " u.m.";
			FisierUtilizator << "\n\tAlte_cheltuieli: " << Divertisment::get_cheltuialaPeCategorie(Alte_cheltuieli) << " u.m.";
			FisierUtilizator << endl;
			FisierUtilizator << "\nTotal diverse: " << Diverse::calculeazaSumaTotala() << " u.m.";
			FisierUtilizator << "\n\tSanatate: " << Diverse::get_cheltuialaPeCategorie(Sanatate) << " u.m.";
			FisierUtilizator << "\n\tEducatie: " << Diverse::get_cheltuialaPeCategorie(Educatie) << " u.m.";
			FisierUtilizator << "\n\tCopil: " << Diverse::get_cheltuialaPeCategorie(Copil) << " u.m.";
			FisierUtilizator << "\n\tAnimale: " << Diverse::get_cheltuialaPeCategorie(Animale) << " u.m.";
			FisierUtilizator << "\n\tImbracaminte: " << Diverse::get_cheltuialaPeCategorie(Imbracaminte) << " u.m.";
			FisierUtilizator << "\n\tElectrocasnice: " << Diverse::get_cheltuialaPeCategorie(Electrocasnice) << " u.m.";
			FisierUtilizator << "\n\tMobilier: " << Diverse::get_cheltuialaPeCategorie(Mobilier) << " u.m.";
			FisierUtilizator << "\n\tAlte: " << Diverse::get_cheltuialaPeCategorie(Alte) << " u.m.";
			cout << "S-a scris ceva in fisierul text!";
			FisierUtilizator.close();
		}
		else
			cout << "\nCalea este invalida!";
	}
	void sume_totale(string cale)
	{
		ofstream FisierUtilizator(cale);
		if (FisierUtilizator.is_open())
		{
			FisierUtilizator << "SUMELE TOTALE:";
			FisierUtilizator << "\nAlimente: " << Alimente::calculeazaSumaTotala() << " u.m.";
			FisierUtilizator << "\nUtilitati: " << Utilitati::calculeazaSumaTotala() << " u.m.";
			FisierUtilizator << "\nDivertisment: " << Divertisment::calculeazaSumaTotala() << " u.m.";
			FisierUtilizator << "\nDiverse: " << Diverse::calculeazaSumaTotala() << " u.m.";
			cout << "S-a scris ceva in fisierul text!";
			FisierUtilizator.close();
		}
		else
			cout << "\nCalea este invalida!";
	}

};


class Meniu :private Venit
{
public:
	void ruleaza() {
		cout << "--------------------------BINE ATI VENIT!---------------------------\n";
		cout << "Venitul net lunar al dumneavoastra este: => ";
		float ven = 0;
		float suma_totala = Cheltuieli::get_suma_totala_pe_zile();
		float suma_totala_TVA = 0;
		cout << fixed << setprecision(2);  //face ca precizia fiecarui numar(fie el float/double/int etc) sa fie de 2
		while (ven <= 0 || ven < suma_totala)
		{
			cin >> ven;
			if (ven <= 0)
				cerr << "\nVenitul nu poate fi negativ sau nul!\nReincercati! => ";
			else if (ven < suma_totala)
				cerr << "\nVenitul introdus este mai mic decat totalul cheltuielilor facute!\nReincercati! => ";
		}
		set_venit(ven);
		bool inchide = false;
		while (inchide == false)
		{
			cout << "\n\nCE DORITI SA FACETI?\n";
			cout << "\nI. Sa adaug o cheltuiala [apasati 1].";
			cout << "\nII. Sa adaug o suma la venit. [apasati 2] \n[Daca este negativa se va scadea suma din venit]";
			cout << "\n\nRapoarte: ";
			cout << "\nA. Sa se afiseze suma de bani cheltuita. [apasati 3]";
			cout << "\nB. Sa se afiseze suma de bani cheltuita pe zilele lunii. [apasati 4]";
			cout << "\nC. Sa se afiseze toate chetuielile facute. [apasati 5]"; //citire din fisierul binar care are toate chetuielile
			cout << "\nD. Sa se afiseze suma totala de pe fiecare categorie. [apasati 6]";
			cout << "\nE. Sa se afiseze suma fiecarei categorii si subcategorii. [apasati 7]";
			cout << "\nF. Sa se afiseze venitul ramas. [apasati 8]";
			cout << "\nG. Sa se afiseze totalul TVA impozitat. [apasati 9]";
			cout << "\n\nFacilitati: ";
			cout << "\na) Introduceti un fisier CSV cu cheltuieli. [apasati 10]"; //se va incarca doar daca suma tuturor cheltuielilor este mai mica decat venitul ramas
			cout << "\nb) Introduceti un fisier text cu cheltuieli. [apasati 11]"; //se va incarca doar daca suma tuturor cheltuielilor este mai mica decat venitul ramas
			cout << "\n\nRapoarte si sugestii: ";
			cout << "\n1. privind gradul de necesitate al produselor cumparate [apasati 12]";
			cout << "\n2. privind recurenta zilelor de cumparaturi [apasati 13]";
			cout << "\n\nAnexa cota TVA din Romania [apasati 14]";
			cout << "\n\nEXIT [apasati 15]";
			int alegere;
			cout << "\nOptiunea dumneavoastra este: => ";
			cin >> alegere;
			while (alegere != 13 && alegere != 1 && alegere != 14 && alegere != 2 && alegere != 3 && alegere != 4 && alegere != 5 && alegere != 6 &&
				alegere != 7 && alegere != 8 && alegere != 9 && alegere != 10 && alegere != 11 && alegere != 12 && alegere != 15)
			{
				cout << "\nNu ati ales nicio optiune valida!\nReincercati => ";
				cin >> alegere;
			}
			if (alegere == 1)
			{
				Cheltuieli c;
				cin >> c;
				cout << "Total cheltuiala: " << c.get_suma_cheltuiala() << " u.m.";
				if (c.get_suma_cheltuiala() > (Venit::get_venit() - suma_totala))
				{
					cout << "\nSuma totala a cheltuielii este mai mare decat venitul ramas!";
					cout << " Venitul ramas este de " << Venit::get_venit() - suma_totala << " u.m.";
					c.sterge_suma_pe_data();
					for (int i = 0; i < c.get_nr_alimente(); i++)
						c.sterge_pret_aliment(i);
					for (int i = 0; i < c.get_nr_utilitati(); i++)
						c.sterge_pret_utilitate(i);
					for (int i = 0; i < c.get_nr_divertismente(); i++)
						c.sterge_pret_divertisment(i);
					for (int i = 0; i < c.get_nr_diverse(); i++)
						c.sterge_pret_diversa(i);
				}
				else
				{
					Lucru_cu_fisiere l;
					ofstream fisierBinar("Toate_cheltuielile.dat", ios::app, ios::binary);
					l.scrieCheltuiala(fisierBinar, c);
					suma_totala += c.get_suma_cheltuiala();
					cout << "\n##  Venitul ramas este de " << get_venit() - suma_totala << " u.m.  ##\n";
					if (fisierBinar.is_open())
						fisierBinar.close();
				}

			}
			else if (alegere == 2)
			{
				float suma;
				cout << "\nSuma este => ";
				cin >> suma;
				Venit::set_venit(ven + suma);
				ven = Venit::get_venit();
				cout << "\nNoul venit este: " << get_venit() << endl;
			}
			else if (alegere == 3)
			{
				cout << "\nSuma totala este de " << suma_totala << " u.m.";
				cout << "\n\nDoriti acest raport intr-un fisier text? [apasati 1 pentru DA si 0 pentru NU]";
				cout << "\nOptiune: ";
				int al;
				cin >> al;
				while (al != 1 && al != 0)
				{
					cout << "Nu ati introdus nici DA nici NU.Reincercati => ";
					cin >> al;
				}
				if (al == 1)
				{
					string cale;
					cout << "Calea fisierului text: ";
					cin >> cale;
					ofstream FisierUtilizator(cale);
					if (FisierUtilizator.is_open())
					{
						FisierUtilizator << "Suma totala este de " << suma_totala << " u.m.\n";
						cout << "S-a scris ceva in fisierul text!";
						FisierUtilizator.close();
					}
					else
						cout << "\nCalea este invalida!";
				}
			}
			else if (alegere == 4)
			{
				Cheltuieli::get_cheltuiala_pe_date();
				cout << "\nDoriti acest raport intr-un fisier text? [apasati 1 pentru DA si 0 pentru NU]";
				cout << "\nOptiune: ";
				int al;
				cin >> al;
				while (al != 1 && al != 0)
				{
					cout << "Nu ati introdus nici DA nici NU. Reincercati => ";
					cin >> al;
				}
				if (al == 1)
				{
					string cale;
					cout << "Calea fisierului text: ";
					cin >> cale;
					ofstream FisierUtilizator(cale);
					if (FisierUtilizator.is_open())
					{
						bool j = 0;
						for (int i = 1; i <= 31; i++)
							if (Cheltuieli::get_chel_pe_data(i) != 0)
							{
								FisierUtilizator << "In ziua: " << i << " ati cheltuit: " << Cheltuieli::get_chel_pe_data(i) << " u.m." << endl;
								j = true;
							}
						cout << "S-a scris ceva in fisierul text!";
						if (!j)
							cout << "Nu aveti cheltuieli!\n";
						FisierUtilizator.close();
					}
					else
						cout << "\nCalea este invalida!";
				}

			}
			else if (alegere == 5)
			{
				Cheltuieli cc;
				ifstream fisierBinar("Toate_cheltuielile.dat");
				if (fisierBinar.is_open())
				{
					Lucru_cu_fisiere l;
					fisierBinar.seekg(0, ios::end);
					int end = fisierBinar.tellg();
					fisierBinar.seekg(0, ios::beg);
					while (end > fisierBinar.tellg())
					{
						l.citesteCheltuiala(fisierBinar, cc);
						cout << cc;
					}
					fisierBinar.close();
				}
				else
					cout << "A aparut o eroare la deschidrea fisierului de back-up!";
			}
			else if (alegere == 6)
			{
				cout << "\nSUMELE TOTALE:";
				cout << "\nAlimente: " << Alimente::calculeazaSumaTotala() << " u.m.";
				cout << "\nUtilitati: " << Utilitati::calculeazaSumaTotala() << " u.m.";
				cout << "\nDivertisment: " << Divertisment::calculeazaSumaTotala() << " u.m.";
				cout << "\nDiverse: " << Diverse::calculeazaSumaTotala() << " u.m.";
				cout << "\n\nDoriti acest raport intr-un fisier text? [apasati 1 pentru DA si 0 pentru NU]";
				cout << "\nOptiune: ";
				int al;
				cin >> al;
				while (al != 1 && al != 0)
				{
					cout << "Nu ati introdus nici DA nici NU.Reincercati => ";
					cin >> al;
				}
				if (al == 1)
				{
					string cale;
					cout << "Calea fisierului text: ";
					cin >> cale;
					Lucru_cu_fisiere l;
					l.sume_totale(cale);

				}
			}
			else if (alegere == 7)
			{
				cout << "\nSUMELE TOTALE:";
				cout << "\nTotal alimente: " << Alimente::calculeazaSumaTotala() << " u.m.";
				cout << "\n\tFructe: " << Alimente::cheltuiala_pe_categorie(Fructe) << " u.m.";
				cout << "\n\tLegume: " << Alimente::cheltuiala_pe_categorie(Legume) << " u.m.";
				cout << "\n\tLactate: " << Alimente::cheltuiala_pe_categorie(Lactate) << " u.m.";
				cout << "\n\tCarne: " << Alimente::cheltuiala_pe_categorie(Carne) << " u.m.";
				cout << "\n\tDerivate din cereale: " << Alimente::cheltuiala_pe_categorie(Derivate_din_cereale) << " u.m.";
				cout << "\n\tAlta: " << Alimente::cheltuiala_pe_categorie(Alta) << " u.m.";
				cout << endl;
				cout << "\nTotal utilitati: " << Utilitati::calculeazaSumaTotala() << " u.m.";
				cout << "\n\tEnergie: " << Utilitati::get_cheltuialaPeCategorie(Energie) << " u.m.";
				cout << "\n\tGaz: " << Utilitati::get_cheltuialaPeCategorie(Gaz) << " u.m.";
				cout << "\n\tApa: " << Utilitati::get_cheltuialaPeCategorie(Apa) << " u.m.";
				cout << "\n\tTelefonie: " << Utilitati::get_cheltuialaPeCategorie(Telefonie) << " u.m.";
				cout << "\n\tInternet: " << Utilitati::get_cheltuialaPeCategorie(Internet) << " u.m.";
				cout << "\n\tTV: " << Utilitati::get_cheltuialaPeCategorie(TV) << " u.m.";
				cout << "\n\tAsigurari: " << Utilitati::get_cheltuialaPeCategorie(Asigurari) << " u.m.";
				cout << "\n\tAltele: " << Utilitati::get_cheltuialaPeCategorie(Altele) << " u.m.";
				cout << endl;
				cout << "\nTotal divertisment: " << Divertisment::calculeazaSumaTotala() << " u.m.";
				cout << "\n\tExcursii: " << Divertisment::get_cheltuialaPeCategorie(Excursii) << " u.m.";
				cout << "\n\tPetreceri: " << Divertisment::get_cheltuialaPeCategorie(Petreceri) << " u.m.";
				cout << "\n\tIesiri: " << Divertisment::get_cheltuialaPeCategorie(Iesiri) << " u.m.";
				cout << "\n\tAlte_cheltuieli: " << Divertisment::get_cheltuialaPeCategorie(Alte_cheltuieli) << " u.m.";
				cout << endl;
				cout << "\nTotal diverse: " << Diverse::calculeazaSumaTotala() << " u.m.";
				cout << "\n\tSanatate: " << Diverse::get_cheltuialaPeCategorie(Sanatate) << " u.m.";
				cout << "\n\tEducatie: " << Diverse::get_cheltuialaPeCategorie(Educatie) << " u.m.";
				cout << "\n\tCopil: " << Diverse::get_cheltuialaPeCategorie(Copil) << " u.m.";
				cout << "\n\tAnimale: " << Diverse::get_cheltuialaPeCategorie(Animale) << " u.m.";
				cout << "\n\tImbracaminte: " << Diverse::get_cheltuialaPeCategorie(Imbracaminte) << " u.m.";
				cout << "\n\tElectrocasnice: " << Diverse::get_cheltuialaPeCategorie(Electrocasnice) << " u.m.";
				cout << "\n\tMobilier: " << Diverse::get_cheltuialaPeCategorie(Mobilier) << " u.m.";
				cout << "\n\tAlte: " << Diverse::get_cheltuialaPeCategorie(Alte) << " u.m.";
				cout << "\n\nDoriti acest raport intr-un fisier text? [apasati 1 pentru DA si 0 pentru NU]";
				cout << "\nOptiune: ";
				int al;
				cin >> al;
				while (al != 1 && al != 0)
				{
					cout << "Nu ati introdus nici DA nici NU.Reincercati => ";
					cin >> al;
				}
				if (al == 1)
				{
					string cale;
					cout << "Calea fisierului text: ";
					cin >> cale;
					Lucru_cu_fisiere l;
					l.sume_totale_pe_cat(cale);
				}
			}
			else if (alegere == 8)
			{
				cout << "\nSuma de bani ramasa este de " << get_venit() - suma_totala << " u.m.";
				cout << "\n\nDoriti acest raport intr-un fisier text? [apasati 1 pentru DA si 0 pentru NU]";
				cout << "\nOptiune: ";
				int al;
				cin >> al;
				while (al != 1 && al != 0)
				{
					cout << "Nu ati introdus nici DA nici NU.Reincercati => ";
					cin >> al;
				}
				if (al == 1)
				{
					string cale;
					cout << "Calea fisierului text: ";
					cin >> cale;
					ofstream FisierUtilizator(cale);
					if (FisierUtilizator.is_open())
					{
						FisierUtilizator << "\nSuma de bani ramasa este de " << get_venit() - suma_totala << " u.m.";
						cout << "S-a scris ceva in fisierul text!";
						FisierUtilizator.close();
					}
					else
						cout << "\nCalea este invalida!";
				}
			}
			else if (alegere == 9)
			{
				cout << "\nSuma totala a taxei pe valoare adaugata (TVA) este: " << Cheltuieli::get_suma_TVA() << " u.m.";
				cout << "\n**Suma este una aproximativa. Unele cote utilizate folosite pentru anumite produse sau servicii cumparate pot sa difere!\n";
				//cout<<"\nDaca doriti modificarea unei cote apasati 20."
				//Nu am mai apucat sa realizez si modificarea cotelor, dar ideea era ca atunci cand se afiseaza cheltuielile sa se afiseze si cotele utilizate \
				si daca doreste sa o schimbe pentru un produs sa aiba aceasta posibilitate.[i s-ar fi afisat si cotele pentru toate elementele: cum e in functia virtuala afisare_cota_TVA\
                Dar per total, diferentele nu ar fi fost semnificative.
				cout << "\nDoriti acest raport intr-un fisier text? [apasati 1 pentru DA si 0 pentru NU]";
				cout << "\nOptiune: ";
				int al;
				cin >> al;
				while (al != 1 && al != 0)
				{
					cout << "Nu ati introdus nici DA nici NU.Reincercati => ";
					cin >> al;
				}
				if (al == 1)
				{
					string cale;
					cout << "Calea fisierului text: ";
					cin >> cale;
					ofstream FisierUtilizator(cale);
					if (FisierUtilizator.is_open())
					{
						FisierUtilizator << "Suma totala a taxei pe valoare adaugata (TVA) este: " << Cheltuieli::get_suma_TVA() << " u.m."
							<< "\n**Suma este una aproximativa. Unele cote utilizate folosite pentru anumite produse sau servicii cumparate pot sa difere!\n";
						cout << "S-a scris ceva in fisierul text!";
						FisierUtilizator.close();
					}
					else
						cout << "\nCalea este invalida!";
				}
			}
			else if (alegere == 10)
			{
				string cale;
				cout << "Calea fisierului text: ";
				cin >> cale;
				ifstream FisierUtilizator(cale);
				if (FisierUtilizator.is_open())
				{
					float suma_totala_ch = 0;
					FisierUtilizator.seekg(0, ios::end);
					int end = FisierUtilizator.tellg();
					FisierUtilizator.seekg(0, ios::beg);
					while (end > FisierUtilizator.tellg())
					{
						Cheltuieli che;
						Lucru_cu_fisiere l;
						char valoare[100];
						FisierUtilizator.getline(valoare, 100, ',');
						stringstream data_ch(valoare);
						int data;
						data_ch >> data;
						l.citeste_din_CSV(FisierUtilizator, che);
						suma_totala_ch += che.get_suma_cheltuiala();
						che.sterge_pret_aliment(0);
						che.sterge_pret_utilitate(0);
						che.sterge_pret_divertisment(0);
						che.sterge_pret_diversa(0);
						che.sterge_TVA();
						che.sterge_necesitati();
					}
					if (suma_totala_ch > (get_venit() - suma_totala))
						cerr << "\nSuma tuturor cheltuielilor din CSV este mai mare decat suma venitului ramasa!"
						<< "\nIn consecinta, nu a fost introdusa nici o cheltuiala.";
					else
					{
						Lucru_cu_fisiere l;
						FisierUtilizator.seekg(0, ios::beg);
						while (end > FisierUtilizator.tellg())
						{
							Cheltuieli che;
							char valoare[100];
							FisierUtilizator.getline(valoare, 100, ',');
							stringstream data_ch(valoare);
							int data;
							data_ch >> data;
							l.citeste_din_CSV(FisierUtilizator, che);
							ofstream fisierBinar("Toate_cheltuielile.dat", ios::app | ios::binary);
							l.scrieCheltuiala(fisierBinar, che);
							suma_totala += che.get_suma_cheltuiala();
							che.set_cheltuieli_pe_data(data);
							if (fisierBinar.is_open())
								fisierBinar.close();
						}
						cout << "\nS-au incarcat cu succes toate cheltuielile.";
					}

					cout << "\n##  Venitul ramas este de " << get_venit() - suma_totala << " u.m.  ##\n";
					FisierUtilizator.close();
				}
				else
					cout << "\nCalea este invalida!";
			}
			else if (alegere == 11)
			{
				Lucru_cu_fisiere l;
				l.optiune11(suma_totala);
			}
			else if (alegere == 12)
			{
				cout << "\nAti achizitionat:\n"
					<< Comun::get_necesitate_pe_cat(DelocNecesar) << " produse si servicii deloc necesare\n"
					<< Comun::get_necesitate_pe_cat(PutinNecesar) << " produse si servicii putin necesare\n"
					<< Comun::get_necesitate_pe_cat(FoarteNecesar) << " produse si servicii foarte necesare\n";
				if (Comun::get_necesitate_pe_cat(DelocNecesar) > Comun::get_necesitate_pe_cat(PutinNecesar)
					&& Comun::get_necesitate_pe_cat(DelocNecesar) > Comun::get_necesitate_pe_cat(FoarteNecesar))
					cout << "Dupa cum se observa ati cumparat foarte multe din categoria deloc necesare.\n"
					<< "Daca doriti sa economisiti, incercati sa nu mai cumparati atat de multe lucruri deloc necesare."
					<< " Axati-va pe cele putin si foarte necesare.\n";
				else if (Comun::get_necesitate_pe_cat(PutinNecesar) > Comun::get_necesitate_pe_cat(FoarteNecesar)
					&& Comun::get_necesitate_pe_cat(PutinNecesar) > Comun::get_necesitate_pe_cat(DelocNecesar))
					cout << "Dupa cum se observa ati cumparat foarte multe din categoria putin necesare.\n"
					<< "Acest lucru este bun, demonstrand ca nu cheltuiti banii pe lucruri ce nu va trebuie.\n";
				else if (Comun::get_necesitate_pe_cat(FoarteNecesar) > Comun::get_necesitate_pe_cat(PutinNecesar)
					&& Comun::get_necesitate_pe_cat(PutinNecesar) > Comun::get_necesitate_pe_cat(DelocNecesar))
					cout << "Dupa cum se observa ati cumparat foarte multe din categoria foarte necesare.\n"
					<< "Acest lucru este foarte bine, demonstrand ca va focusati suma de bani doar pe lucrurile care va trebuie.\n";
				else if (Comun::get_necesitate_pe_cat(PutinNecesar) > 5 || Comun::get_necesitate_pe_cat(DelocNecesar) > 3)
					cout << "Daca doriti sa economisiti incercati sa diminuati banii cheltuiti pe produsele si serviciile din categoria deloc sau putin necesare.\n";
				else
					cout << "Nu avem inca sugestii. :(\n";
			}
			else if (alegere == 13)
			{
				int j = 0;
				for (int i = 0; i < 31; i++)
					if (Cheltuieli::get_chel_pe_data(i) != 0)
						j++;
				time_t now = time(0);
				tm* ltm = localtime(&now);
				int ziua = ltm->tm_mday;
				if (ziua > 25)
				{
					if (j > 23)
						cout << "\nAti cumparat aproape zilnic alimente sau servicii."
						<< "Daca doriti sa economisiti, incercati sa nu mai cheltuiti zilnic.";
					else if (j >= 14 && j <= 23)
						cout << "\nAti cumparat destul de normal. Continuati asa.";
					else
						cout << "\nAti cumparat rareori pe parcursul lunii.";
				}
				else cout << "\n** Nu va putem da inca un raport, deoarece nu este finalul lunii. **";
			}
			else if (alegere == 14)
			{
				cotele_TVA_Ro();
			}
			else if (alegere == 15)
			{
				cout << "\n----------------------------VA MULTUMIM!----------------------------\n";
				inchide = true;
			}
		}
	}
	void testare_clase()
	{
		Lucru_cu_fisiere ll;
		Alimente a1, a2("Banane", 7), a3("Apa", 5, Alta), a4, a5;
		cout << a1 << a2 << a3 << a4 << a5;
		cout << a4;
		a4 = a2;
		cout << a4;
		cout << endl << "Suma:" << Alimente::calculeazaSumaTotala();
		//cin >> a4;
		cout << a4;
		a4[2];
		cout << endl << a2.get_denumire() << " + " << a3.get_denumire() << " => " << a2 + a3 << " lei.";
		++a2;
		a5 = a2--;
		cout << a2;
		cout << a5;
		a2[2];
		cout << endl << "Pret: " << (float)a5;
		Alimente a6("Paine", 1.3, Derivate_din_cereale);
		a6[0];
		!a6;
		cout << a6;
		a6[0];
		a6[2];
		a2 < a6;
		a6 < a2;
		a4 = a6;
		a6 < a4;
		Cheltuieli::get_cheltuiala_pe_date();
		Alimente a(nullptr, 5, Alta), b;
		//cin >> a;
		ofstream fisBin("fisierBinar.dat", ios::binary);
		ll.scrieAliment(fisBin, a);
		fisBin.close();
		ifstream fissBin("fisierBinar.dat", ios::binary);
		ll.citesteAliment(fissBin, b);
		cout << endl << b;
		fissBin.close();
		cout << "\nUTILITATI: \n";
		Utilitati u1, u2(100, Energie), u3("Telekom", 35, Telefonie), u4 = u2, u5;
		cout << u1 << u2 << u3;
		cout << endl << "Suma: " << Utilitati::calculeazaSumaTotala();
		u1 = u3;
		cout << u1 << u2 << u3 << u4;
		cout << endl << u1.get_categorie();
		cout << endl << Utilitati::get_cheltuialaPeCategorie(Energie);
		if (u4 == u2)
			cout << "\nSunt egale!";
		else
			cout << "\nNu sunt egale!";
		u2.set_denumire("Enel");
		if (u4 == u2)
			cout << "\nSunt egale!";
		else
			cout << "\nNu sunt egale!";
		u2.set_pret(135.5);
		cout << endl << "Suma: " << Utilitati::calculeazaSumaTotala();
		cout << endl << Utilitati::get_cheltuialaPeCategorie(Energie);
		//cin >> u5;
		cout << u5;
		u5[Energie];
		cout << "\nDIVERTISMENT:\n ";
		Divertisment d1, d2(100, Iesiri), d3("Zi nastere", 250, Petreceri), d4 = d3, d5;
		d4.set_pret(200);
		cout << d1 << d2 << d3 << d4;
		cout << d4;
		cout << "\nSume: ";
		d1[0];
		d1[1];
		d1[2];
		d1[3];
		d5 = d4;
		d5.set_denumire("Excursie draguta in Mexic");
		d5.set_pret(9501.3);
		d5.set_categorie(Excursii);
		cout << "\n\nCategorie: " << d5.get_categorie() << "\nPret: " << d5.get_pret() << "\nDenumire: " << d5.get_denumire();
		//cin >> d5;
		cout << d5;
		d1[0];
		d5.StergeObiect();
		d1[0];
		cout << d5;

		cout << endl << "\nDiverse:\n";
		Diverse div1, div2(100, Educatie), div3(70, Animale);
		div1.set_categorie(Sanatate);
		div1.set_pret(500);
		div1.set_denumire("Plomba");
		div1[1];
		div1.StergeObiect();
		cout << endl << div1.get_categorie() << endl;
		cout << div1;
		div1++;
		cout << endl << (float)div1;
		--div1;
		!div1;
		div1 < div3;
		cout << endl << (div1 == div2);
		cout << endl << div1 + div2 << endl;
		cout << div1 << div2 << div3;
		time_t now = time(0);
		tm* data = localtime(&now);
		cout << "Day: " << data->tm_mday << endl;
		Alimente al[] = { a1,a2,a3 };
		Utilitati ul[] = { u1,u2,u3 };
		Divertisment div[] = { d1,d2,d3 };
		Diverse diverse[] = { div1,div2,div3 };
		Cheltuieli c1, c2(2, al, 1, ul), c3(3, al, 3, ul, 1, div, 2, diverse), c4 = c3;
		cout << c4 << endl;
		c3.get_data();
		cout << endl << endl;
		c3.get_alimente();
		cout << endl << endl;
		c3.get_diverse();
		cout << endl << endl;
		c3.get_utilitati();
		cout << endl << endl;
		c3.get_divertisment();
		cout << endl << endl;
		Cheltuieli::get_cheltuiala_pe_date();
		try {
			cout << c3[18];
		}
		catch (const char* err) {
			cout << err;
		}
		cout << endl;
		//c4 = c3++;
		//c4 = ++c3;
		cout << c4;
		cout << "Venit: "; float ve;
		//cin >> ve; Cheltuieli::set_venit(ve);
		c3 + 100;
		cout << (float)c3;
		c3 > c1;
		c4 = c2;
		cout << c4.get_suma_cheltuiala();
		cout << endl << (c3 == c4);
		!c3;
		cout << c3.get_suma_cheltuiala();
		//cin >> c3;
		cout << c3;
		cout << endl << c3.get_suma_cheltuiala() << endl;
		c3.get_cheltuiala_pe_date();
		c3.set_nr_alimente(1);
		cout << c3;
		//c3.set_nr_utilitati(4);
		cout << c3;
		cout << c3.get_suma_cheltuiala() << endl;
		c3.get_cheltuiala_pe_date();
		cout << c3;
		c3.set_utilitati(2, ul);
		c3.set_diverse(3, diverse);
		c3.set_utilitati(2, ul);
		c3.set_alimente(1, al);
		c3.set_utilitati(1, ul);
		c3.set_divertisment(3, div);
		cout << endl << c3.get_suma_cheltuiala() << endl;
		c3.get_cheltuiala_pe_date();
		cout << c3;
		ofstream Fisier("cheltuieli.txt");
		if (Fisier.is_open())
		{
			Fisier << c3;
			cout << "\nS-a scris ceva in fisierul text!\n";
			Fisier.close();
		}
		cout << endl << (c3 == c4) << "\t" << (c3 == c1) << endl;
		cout << endl;
		Cheltuieli::get_cheltuiala_pe_date();
		Cheltuieli d, e;
		//cin >> d;
		ofstream fis("fisierBinar.dat", ofstream::binary);
		ll.scrieCheltuiala(fis, d);
		fis.close();
		ifstream fiss("fisierBinar.dat", ifstream::binary);
		ll.citesteCheltuiala(fiss, d);
		cout << endl << e;
		fiss.close();
		Comun co1, co2(0.09, PutinNecesar), co3 = co2;
		//cout << co1<<endl<<co2<<endl<<co3;
		co3 = co1;
		//cin >> co1;
		cout << endl << endl << endl;
		cout << co1 << endl << co2 << endl << co3;
		cout << endl;
		Comun::get_vector_necesitate();
		cout << endl;
		Comun* co4;
		Alimente ali;
		Utilitati uti(200, Apa);
		Diverse divers(55, Educatie);
		Divertisment dd;
		co4 = new Comun[1];
		co4 = &dd;
		co4->necesitate_default();
		co4->set_TVA();
		cout << endl << (*co4).get_TVA() * 100 << "%" << endl;
		(*co4).afisare_tip();
		cout << (*co4);
		(*co4).afisare_cota_TVA();
		//cin >> ali;
		cout << ali;
		Cheltuieli chhh, chl = chhh;
		//cin >> chhh;
		cout << chhh << endl;
		cout << Cheltuieli::get_suma_TVA();
		chl = chhh;
		cout << chl << chhh << endl;
		cout << Cheltuieli::get_suma_TVA();

		Lucru_cu_fisiere l;
		ifstream fisierDeScos("Toate_cheltuielile.dat");
		Cheltuieli c[100];
		int i = 0;
		l.citesteCheltuiala(fisierDeScos, c[i]);
		while (fisierDeScos)
		{
			i++;
			l.citesteCheltuiala(fisierDeScos, c[i]);
		}
		cout << endl << "\nASTA>\n ";
		for (int j = 0; j < i; j++)
			cout << c[j];
		fisierDeScos.close();
		cout << endl;
		Cheltuieli::get_cheltuiala_pe_date();
		Cheltuieli cc1;
		ifstream fisierBinar("Toate_cheltuielile.dat");
		fisierBinar.seekg(0, ios::end);
		int end = fisierBinar.tellg();
		fisierBinar.seekg(0, ios::beg);
		while (end > fisierBinar.tellg())
		{
			l.citesteCheltuiala(fisierDeScos, cc1);
			cout << cc1;
		}
		fisierBinar.close();
		ofstream fisi("C:/Users/Monica/Desktop/Fisier.txt");
		if (fisi.is_open()) cout << "DA";
		fisi.close();
		Cheltuieli che;
		ifstream csv("C:/Users/Monica/Desktop/POO.csv");
		if (csv.is_open())
		{
			l.citeste_din_CSV(csv, che);
			l.citeste_din_CSV(csv, che);
			cout << che;
			csv.close();
		}
		else cout << "Fisierul nu s-a putut deschide!";

	}
	void cotele_TVA_Ro()
	{
		Alimente al;
		Utilitati ut;
		Divertisment dt;
		Diverse dv;
		Abstracta* a = &al;
		cout << "Ce cote doriti sa cunoasteti?";
		cout << "\n1. Ale alimentelor [apasati 1]";
		cout << "\n2. Ale utilitatilor [apasati 2]";
		cout << "\n3. Ale divertismentelor [apasati 3]";
		cout << "\n4. Ale diverselor [apasati 4]";
		cout << "\nOptiune: ";
		int optiun;
		cin >> optiun;
		while (optiun != 1 && optiun != 2 && optiun != 3 && optiun != 4)
		{
			cout << "\nNu ati ales nicio optiune! Reincercati => ";
			cin >> optiun;
		}
		if (optiun == 1)
			a = &al;
		else if (optiun == 2)
			a = &ut;
		else if (optiun == 3)
			a = &dt;
		else if (optiun == 4)
			a = &dv;
		(*a).afisare_tip();
		(*a).afisare_cota_TVA();
	}
};

