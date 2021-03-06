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
#include <ctime>
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
        sub = polinomio.substr(0, indspac);
        polinomio = polinomio.substr(indspac + 1, polinomio.length());
        sinspacio << sub;
    } while (true);
    sinspacio << polinomio;
    return sinspacio.str();
}

vector<string> splitFuncion(string funcion)
{
    vector<string> p;
    string sub;
    funcion = eliminarSpace(funcion);
    int posmenos, posmas;
    do
    {
        posmenos = funcion.find('-', 1);
        posmas = funcion.find('+', 1);
        // puede que el primer menos sea de un exponenete, si es as� buscamos el siguiente
//		cout << "Posmenos -: " << posmenos << endl;
//		cout << "Posmenos ^-: " << funcion.find("^-",1) + 1 << endl;
        if( (funcion.find("^-", 1) + 1 ) == posmenos)
            posmenos = funcion.find('-', posmenos + 2);

        //si nos quedamos sin - o + terminas de iterar cortando al maximo el string
        if(posmenos == -1 && posmas == -1)
            posmas = funcion.length();
//		cout << "Posmenos: " << posmenos << endl;
//		cout << "posmas: " << posmas << endl;
        // nos quedamos con el menor no negativo
        if ( (posmas == -1) || (posmenos != -1 && posmenos < posmas) )
            posmas = posmenos;
//		cout << "Corte : " << posmas << endl;
        sub = funcion.substr(0, posmas);
        p.push_back(sub);
        funcion = funcion.substr(posmas, funcion.length());
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
        cons = polinomio.substr(0, pos);

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
        expo = polinomio.substr(pos + 1, polinomio.length());
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
            expo *= -1 ;
            cons *= -1;
        }
        result << cons << "/" << expo << "*x^" << expo ;
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
    result << integrate(p[0]) << " ";
    for (i = 1; i != p.size(); ++i)
    {
        if(integrate(p[i])[0] != '-')
            result << "+ ";
        result << integrate(p[i]) << " ";
    }
    result << "+ C";
    return  result.str();
}

float evaluarPunto(string miniFuncion, float x)
{
    int pos = miniFuncion.find('x');
    if (pos < miniFuncion.length())
    {
        float cont = getCons(miniFuncion);
        int expo = getExpo(miniFuncion);
        return cont * pow(x, expo);
    }
    else
        return atoi(miniFuncion.c_str());
}

float evaluarUnpuntopolinomio(string polinomio, float x)
{
    float resultado = 0;
    vector<string> polinomioseparado = splitFuncion(polinomio);
    vector<string>::size_type it;
    for (it = 0; it != polinomioseparado.size(); ++it)
    {
        resultado += evaluarPunto(polinomioseparado[it], x);
    }
    return resultado;
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
            sum += evaluarPunto(separafuncion[it], x_puntos[i]);
        y_puntos.push_back(sum);
        sum = 0;
    }
    return y_puntos;
}

void graficar(string funcion, int inicio, int fin)
{
    int cant_puntos = fin - inicio + 3; // Recordar ver una distribucion de puntos del 105% u otra
    string integral = getIntegral(funcion);
    stringstream nombre;
    nombre << "f(x) = " << integral << " Desde x = " << inicio << " Hasta x = " << fin;
    int it = 0; // Itera por los arreglos x e y

    // Nombre de fichero de salida
    char filename[100];
    stringstream f_name;
    time_t rawtime;
    tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    // Ahora si que si, escribimos la info al string
    strftime(filename, 100, "%Y-%m-%d-%H-%M-%S", timeinfo);
    f_name << filename << ".png";

    // Arreglos de puntos (x,y)
    PLFLT x[cant_puntos], y[cant_puntos];
    
    // Hackish, genera un punto en eje x al principio
    x[0] = inicio;
    y[0] = 0;
    it++;

    vector<float> x_valores = x_puntos(inicio, fin); // Obtener coordenadas x
    vector<float> y_valores = y_puntos(integral, x_valores);
    
    int bound = (fin - inicio) * 0.1; // Para que se alcancen a notar los limites
    PLFLT ymin = 0, ymax = y_valores[0]; // Valores maximos y minimos de y, se modificaran
    PLFLT xmin = inicio - bound, xmax = fin + bound; // Max y min de eje x

    // Pasar datos de los vectores a los arreglos correspondientes, calcular maximo y minimo valor de eje y
    for(vector<float>::iterator x_it = x_valores.begin(), y_it = y_valores.begin(); x_it != x_valores.end(); x_it++, y_it++, it++)
    {
        if(*y_it > ymax)
            ymax = *y_it;
        x[it] = *x_it;
        y[it] = *y_it;
    }
    
    ymax += ymax * 0.1; // Para que se alcance a notar los limites

    PLINT just = 0, axis = 1;

    // Generar el punto en eje x para cerrar los poligonos generados por el polinomio
    x[it] = fin;
    y[it] = 0;

    plstream *pls; //Objeto de plot
    pls = new plstream();
    plsdev("png"); // Guardamos en png
    plsfnam(f_name.str().c_str()); // Aca se guarda

    pls->init();
    pls->env(xmin, xmax, ymin, ymax, just, axis );
    pls->lab("(x)", "(y)", nombre.str().c_str());
    pls->fill(cant_puntos, x, y);

    delete pls;
    cout << "Grafico guardado en " << f_name.str() << endl;
}

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        if (strcmp(argv[1], "-i") == 0)
        {
            if(argc == 3)
                cout << "La integral es " << getIntegral((string)argv[2]) << endl;
            else
                cout << "Falta el polinomio!" << endl;
        }
        else if(strcmp(argv[1], "-g") == 0)
        {
            if(argc == 5)
            {
                int inicio = atoi(argv[3]);
                int fin = atoi(argv[4]);
                if(inicio >= 0 && fin >= 0 && fin > inicio)
                {
                    cout << "La integral es " << getIntegral((string)argv[2]) << endl;
                    graficar((string)argv[2], inicio, fin);
                }
                else
                {
                    cout << "Problema de rango, recuerde que desde y hasta deben ser mayores a 0, y obviamente hasta debe ser mayor a desde" << endl;
                }
            }
            else
                cout << "Le faltaron parametros, el polinomio, desde o hasta" << endl;
        }
        else if(strcmp(argv[1], "-v") == 0)
        {
            cout << "Integracion de Polinomio por ratas recursivas" << endl;
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

