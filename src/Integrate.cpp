#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;

// a la mierda regex haré yo mismo la expresión xD
int getConst(string polinomio)
{
	string cons = "";
	int pos = polinomio.find("*");
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

int main()
{
	string polinomios[5] = {"-x^2","-123*x^-23","-1*x","x^13","-23*x"};
	int i;
	for (i = 0; i < 5 ; ++i)
	{
		cout <<"Polinomio: " <<polinomios[i] << endl;
		cout << "Constante : " << getConst(polinomios[i]) << "\nExponente : " << getExpo(polinomios[i]) << endl;
		cout <<"==================================="<< endl;
	}
}
/* ==============================Salida ================
Polinomio: -x^2
Constante : -1
Exponente : 2
===================================
Polinomio: -123*x^-23
Constante : -123
Exponente : -23
===================================
Polinomio: -1*x
Constante : -1
Exponente : 1
===================================
Polinomio: x^13
Constante : 1
Exponente : 13
===================================
Polinomio: -23*x
Constante : -23
Exponente : 1
===================================
*/
