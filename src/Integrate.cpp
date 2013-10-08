#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>

using namespace std;

// a la mierda regex har� yo mismo la expresi�n xD
int getConst(string polinomio)
{
	string cons = "";
	unsigned int pos = polinomio.find("*");
	if ( pos < polinomio.length() )
	{
		//se obtiene el substring desde el inicio hasta donde llega el por
		// 123412*x...
		cons = polinomio.substr(0,pos);
		return atoi(cons.c_str());
	}
	else
	{
		//puede que sea x^i o -x^i,  hay que capturar el - si fuera negativo
		if (polinomio[0] == '-')
			return -1;
		return 1;
	}
}

int getExpo(string polinomio)
{
	string expo = "";
	int pos = polinomio.find("^");
	//se busca el ^, de ah� en adelante ser� el n�mero del exponente
	if(pos < polinomio.length())
	{
		expo = polinomio.substr(pos+1,polinomio.length());
		return atoi(expo.c_str());
	}
	else
		//si no contiene ^ entonces el exponente es 1
		return 1;
}

// Discrimina si es un polinomio o una constante
bool esPolinomio(string polinomio)
{
	int pos = polinomio.find("x");
	if (pos < polinomio.length())
		return true;
	return false;
}

string integrate(string polinomio)
{
	stringstream result;
	if(esPolinomio(polinomio))
	{
		int cons = getConst(polinomio);
		int expo = getExpo(polinomio);
		expo += 1;
		//menos con menos m�s
		if (expo < 0 && cons < 0)
		{
			expo *= -1 ; cons *= -1;
		}
		result << cons <<"/" << expo <<"*x^" << expo ;
	}
	else
	{
		int valor = atoi(polinomio.c_str());
		result << valor << "*x";
	}
	return result.str();
}

int main()
{
	string funciones[] = {" -x^2 "," +123*x^23 "," -1*x "," +x^13 "," -23*x"," +123 "," -12 "};
	int i;
	for (i = 0; i < 7 ; ++i)
	{
		cout <<"funcion:" <<funciones[i] << endl;
		cout << "Integral : " << integrate(funciones[i]) << endl;
		cout <<"==================================="<< endl;
	}
}
