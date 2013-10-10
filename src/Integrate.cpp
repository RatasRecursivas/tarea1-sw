//============================================================================
// Name        : Integral.cpp
// Author      : Seba
// Version     :
// Copyright   :
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>

using namespace std;


// a la mierda regex haré yo mismo la expresión xD
int getCons(string polinomio)
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
	//se busca el ^, de ahí en adelante será el número del exponente
	if(pos < polinomio.length())
	{
		expo = polinomio.substr(pos+1,polinomio.length());
		return atoi(expo.c_str());
	}
	else
		//si no contiene ^ entonces el exponente es 1
		return 1;
}



string integrate(string funcion)
{
	stringstream result;
	int pos = funcion.find('x');
	if(pos < funcion.length())
	{
		int cons = getCons(funcion);
		int expo = getExpo(funcion);
		expo += 1;
		if (expo < 0 && cons < 0)
		{
		expo *= -1 ; cons *= -1;
		}
		result << cons <<"/" << expo <<"*x^" << expo ;
	}
	else
		result << atoi(funcion.c_str()) << "x";
	return result.str();
}

string integral(string funcion)
{
	stringstream salida;
	string sub;
	int posmenos,posmas;
	while(true)
	{
		stringstream substring;
		posmenos = funcion.find('-',1);
		posmas = funcion.find('+',1);
		if((posmenos == -1 && posmas == 0) || (posmenos == -1 && posmas == 0) || (posmenos == -1 && posmas == -1))
			break;
		if(posmas == -1 )
			posmas = posmenos;
		else
			if(posmenos != -1 )
				if(posmenos < posmas )
					posmas = posmenos;
		sub = funcion.substr(0,posmas);
		sub = integrate(sub);
		if(sub[0] != '-')
			substring << "+ " << sub << " ";
		else
			substring << sub << " ";
		salida << substring.str();
		funcion = funcion.substr(posmas+1,funcion.length());
	}
	stringstream substring;
	sub = integrate(funcion);
	if(sub[0] != '-')
		substring << " + " << sub << " ";
	else
		substring << " "<< sub << " ";
	salida << substring.str() << " + C";
	return salida.str();
}

//odio los espacios ahora
string eliminarSpace(string polinomio)
{
	stringstream sinspacio;
	string sub;
	int indspac;
	do {
		indspac = polinomio.find(' ');
		if(indspac == -1)
			break;
		sub = polinomio.substr(0,indspac);
		polinomio = polinomio.substr(indspac+1,polinomio.length());
		sinspacio << sub;
	} while (true);
	sinspacio<< polinomio;
	return sinspacio.str();
}

int main()
{
	string p = " x^23 + 12 + 2x+ 54x ";
	string p_ = eliminarSpace(p);
	cout << "Funcion: " << p <<"\nIntegral: " << integral(p_) << endl;
	return 0;
}
