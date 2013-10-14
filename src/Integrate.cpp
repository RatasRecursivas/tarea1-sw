//============================================================================
// Name        : Integral.cpp
// Author      : Seba || Pato || Naty
// Version     :
// Copyright   :
// Description : Programa integral (integro)
//============================================================================

#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <plstream.h>

using namespace std;

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

vector<string> splitFuncion(string funcion)
{
	vector<string> p;
	string sub;
	funcion = eliminarSpace(funcion);
	int posmenos,posmas;
	do
	{
		posmenos = funcion.find('-',1);
		posmas = funcion.find('+',1);
		// puede que el primer menos sea de un exponenete, si es as� buscamos el siguiente
//		cout << "Posmenos -: " << posmenos << endl;
//		cout << "Posmenos ^-: " << funcion.find("^-",1) + 1 << endl;
		if( (funcion.find("^-",1) + 1 ) == posmenos)
			posmenos = funcion.find('-',posmenos+2);

		//si nos quedamos sin - o + terminas de iterar cortando al maximo el string
		if(posmenos == -1 && posmas == -1)
			posmas = funcion.length();
//		cout << "Posmenos: " << posmenos << endl;
//		cout << "posmas: " << posmas << endl;
		// nos quedamos con el menor no negativo
		if ( (posmas == -1) || (posmenos != -1 && posmenos < posmas) )
			posmas = posmenos;
		cout << "Corte : " << posmas << endl;
		sub = funcion.substr(0,posmas);
		p.push_back(sub);
		funcion = funcion.substr(posmas,funcion.length());
	} while (funcion.length() != 0);
	return p;
}

// a la mierda regex har� yo mismo la expresi�n xD
float getCons(string polinomio)
{
	string cons;
	int pos = polinomio.find("*");
	if (pos < polinomio.length())
	{
		//se obtiene el substring desde el inicio hasta donde llega el por
		// 123412*x...
		cons = polinomio.substr(0,pos);
        
        // Ahora compruebo si hay un /, indicando una division
        pos = cons.find("/");
        if(pos < cons.length()) // Encontre una division, realizarla!
        {
            float numerador = atoi(cons.substr(0, pos).c_str());
            int denominador = atoi(cons.substr(pos + 1, cons.length()).c_str());
            return numerador / denominador;
        }
        else
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

string integrate(string funcion)
{
	stringstream result;
	int pos = funcion.find('x');
	if(pos < funcion.length())
	{
		float cons = getCons(funcion);
		int expo = getExpo(funcion);
		expo += 1;
		if (expo < 0 && cons < 0)
		{
			expo *= -1 ; cons *= -1;
		}
		result << cons <<"/" << expo <<"*x^" << expo ;
	}
	else
		result << atoi(funcion.c_str()) << "*x";
	return result.str();
}

/*
 * daba un funcion por ejemplo string = "x^2+32*x-3"
 * va cortando por substring por monomio o constante
 * o sea quedar�a
 * sub = x^2 string = 32*x-3
 * a sub aplica integral y lo guarda en un stringsteam para ir agregando m�s valores
 * se aplica lo anterior hasta que string quede vacio
 */
string getIntegral(string funcion)
{
	stringstream result;
	vector<string> p = splitFuncion(funcion);
	vector<string>::size_type i;
	if(0  == p.size())
		return "";
	result << integrate(p[0]) <<" ";
	for (i = 1; i != p.size(); ++i)
	{
		if(integrate(p[i])[0] != '-')
			result << "+ ";
		result << integrate(p[i]) <<" ";
	}
	result<<"+ C";
	return  result.str();
}

float evaluarPunto(string miniFuncion, int x)
{
	int pos = miniFuncion.find('x');
	if (pos < miniFuncion.length())
	{
		float cont = getCons(miniFuncion);
		int expo = getExpo(miniFuncion);
		return cont * pow(x,expo);
	}
	else
		return atoi(miniFuncion.c_str());
}

vector<float> x_puntos(int inicio, int fin)
{
	vector<float> res;
	for(float f = (float)inicio; f <= (float)fin; f += 1.0)
		res.push_back(f);
	return res;
}

vector<float> y_puntos(string funcion, vector<float> x_puntos)
{
	vector<float> y_puntos;
	vector<float>::size_type i;
	vector<string> separafuncion = splitFuncion(funcion);
	float sum = 0;
	vector<string>::size_type it;
	for (i = 0; i != x_puntos.size(); ++i)
	{
		for (it = 0; it != separafuncion.size(); ++it)
			sum += evaluarPunto(separafuncion[it],x_puntos[i]);
		y_puntos.push_back(sum);
		sum = 0;
	}
	return y_puntos;
}

void graficarFuncion(string funcionEntrada,int inicio, int fin)
{
    stringstream nombre;
    string integral = getIntegral(funcionEntrada);
    nombre << "f(x) = " << integral;
	vector<float> x_valores = x_puntos(inicio, fin);
	vector<float> y_valores = y_puntos(integral, x_valores);

	PLFLT xmin = inicio, xmax = fin, x[fin - inicio], y[fin - inicio];
	int i = 0;
	for (vector<float>::iterator it = x_valores.begin(); it != x_valores.end(); ++it)
	{
		x[i] = *it;
		i++;
	}
	i = 0;
	PLFLT ymin = y_valores[0], ymax = y_valores[0];
	for (vector<float>::iterator it = y_valores.begin(); it != y_valores.end(); ++it)
	{
		if(*it > ymax)
			ymax = *it;
		if(*it < ymin)
			ymin = *it;
		y[i] = *it;
		i++;
	}
	PLINT just = 0, axis = 1;
	plstream *pls; //Objeto de plot

	// Inicializar el plstream
	pls = new plstream();

	// Decirle que guarde en png
	plsdev("png");

	// Fichero donde se guarda
	plsfnam("test2.png");

	// Demosle
	pls->init();
	pls->env(xmin, xmax, ymin, ymax, just, axis );
	pls->lab("(x)", "(y)", nombre.str().c_str());
	pls->line(fin - inicio, x, y);
    pls->fill(fin -inicio, x, y);

	// Lo terminamos de usar
	delete pls;
}

int main(int argc, char *argv[])
{
	if(argc > 1)
	{
		if (strcmp(argv[1],"-i")==0)
		{
			if(argc == 3)
				cout << "La integral es " << getIntegral((string)argv[2])<< endl;
			else
				cout << "Falta el polinomio!" << endl;
		}
		else if(strcmp(argv[1],"-g")==0)
		{
			if(argc == 5)
			{
				cout << "La integral es " << getIntegral((string)argv[2])<< endl;
				graficarFuncion((string)argv[2], atoi(argv[3]), atoi(argv[4]));
				cout << "Se grafico, vealo!" << endl;
			}
			else
				cout << "Le faltaron parametros, el polinomio, desde o hasta" << endl;
		}
		else if(strcmp(argv[1],"-v")==0)
		{
			cout << "Natalia Tarifeño" << endl;
			cout << "Sebastian Rocha" << endl;
			cout << "Patricio Pérez" << endl;
			cout << "Fecha de compilacion: " << __DATE__ << " " << __TIME__ << endl;
		}
		else
			cout << "Opcion invalida" << endl;
	}
	else
		cout << "Tiene que pasarle algun parametro!" << endl;
	return 0;
}
