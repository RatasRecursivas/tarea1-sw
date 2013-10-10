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

/*
 * daba un funcion por ejemplo string = "x^2+32*x-3"
 * va cortando por substring por monomio o constante
 * o sea quedaría
 * sub = x^2 string = 32*x-3
 * a sub aplica integral y lo guarda en un stringsteam para ir agregando más valores
 * se aplica lo anterior hasta que string quede vacio
 */
string getIntegral(string funcion)
{
	stringstream result;
	string sub;
	int posmenos,posmas; //acá se guardan las posiciones de los signo para ir separando por cada monomio o constante
	bool primervalor = true;
	do
	{
		posmenos = funcion.find('-',1);
		posmas = funcion.find('+',1);
		// puede que el primer menos sea de un exponenete, si es así buscamos el siguiente
		if( (funcion.find("^-",1) -1 ) == posmenos)
			posmenos = funcion.find('-',posmenos+2);
		//preguntamos, podemos quedarnos sin + o -. Si fuera el caso cortamos el string entrada por completo
		if(posmenos == -1 && posmas == -1)
			posmenos = funcion.length();
		// nos quedamos con el menor no negativo
		if ( (posmas == -1) || (posmenos != -1 && posmenos < posmas) )
			posmas = posmenos;
		sub = funcion.substr(0,posmas);
		sub = integrate(sub);
		if(primervalor != true)
			if(sub[0] != '-' || primervalor)
				result <<" + " <<sub;
			else
				result << " " << sub;
		else
			{
				result << " " << sub;
				primervalor = false;
			}
		funcion = funcion.substr(posmas, funcion.length());
	} while (funcion.length() != 0);
	result<<" + C";
	return  result.str();
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
	string p = "54*x^2 - 478*x + 9";
	string p_ = eliminarSpace(p);
	cout << "Funcion: " << p <<"\nIntegral: " << getIntegral(p_) << endl;
	return 0;
}
