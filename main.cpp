#include <allegro.h>
#include <string>
#include <conio.h>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winalleg.h>
#include <fstream>
#include <map>
#include <exception>
#include <list>
#include <vector>
#include <cmath>
#include <typeinfo>
#include <time.h>

//#define CATCH_CONFIG_MAIN //Esto es para hacer pruebas, no sé si llegaremos a hacer alguna
//#include "catch.hpp"

using namespace std;


/**EXCEPCIONES*/

class AllegroNoFuncionaException: public exception
{
public:
    AllegroNoFuncionaException(const string& msg): msg_(msg) {}
    virtual ~AllegroNoFuncionaException() throw () {}

    virtual const char* what() const throw()
    {
        string msgPpal = "Error de Allegro: ";
        return (msgPpal + msg_).c_str();
    }

private:
    string msg_;
};

class SonidoNoEncontradoException: public exception
{
private:
    string index_;
public:
    SonidoNoEncontradoException(const string& index): index_(index) {}
    virtual ~SonidoNoEncontradoException() throw () {}

    virtual const char* what() const throw()
    {
        string msgPrincipal = "No se encontro el sonido solicitado: ";
        return (msgPrincipal + index_).c_str();
    }

};

class AudioArchivoNoEncontradoException: public exception
{
private:
    string index_;
public:
    AudioArchivoNoEncontradoException(const string& index): index_(index) {}
    virtual ~AudioArchivoNoEncontradoException() throw () {}

    virtual const char* what() const throw()
    {
        string msgPrincipal = "No se encontro el Archivo de audio solicitado: " ;
        return (msgPrincipal + index_).c_str();
    }

};

class ImagenNoEncontradaException: public exception
{
private:
    string index_;
public:
    ImagenNoEncontradaException(const string& index): index_(index) {}
    virtual ~ImagenNoEncontradaException() throw () {}

    virtual const char* what() const throw()
    {
        string msgPrincipal = "No se encontro la imagen solicitada Nro. ";
        return (msgPrincipal + index_).c_str();
    }
};

class ImagenArchivoNoEncontradoException: public exception
{
private:
    string index_;
public:
    ImagenArchivoNoEncontradoException(const string& index): index_(index) {}
    virtual ~ImagenArchivoNoEncontradoException() throw () {}

    virtual const char* what() const throw()
    {
        string msgPrincipal = "No se encontro el Archivo de imagen solicitado: ";
        return (msgPrincipal + index_).c_str();
    }

};

class FunteArchivoNoEncontradoException: public exception
{
private:
    string index_;
public:
    FunteArchivoNoEncontradoException(const string& index): index_(index) {}
    virtual ~FunteArchivoNoEncontradoException() throw () {}

    virtual const char* what() const throw()
    {
        string msgPrincipal = "No se encontro el Archivo de Fuente tipográfica solicitado: ";
        return (msgPrincipal + index_).c_str();
    }

};

class ErrorInicializarSonidoException: public exception
{
public:
    ErrorInicializarSonidoException(const string& msg): msg_(msg) {}
    virtual ~ErrorInicializarSonidoException() throw () {}

    virtual const char* what() const throw()
    {
        string msgPrincipal = "No se pudo inicializar el sistema de sonido: ";
        return ( msgPrincipal + msg_).c_str();
    }

private:
    string msg_;

};

class ErrorLectoEscrituraException: public exception
{
public:
    ErrorLectoEscrituraException(const string& msg): msg_(msg) {}
    virtual ~ErrorLectoEscrituraException() throw () {}

    virtual const char* what() const throw()
    {
        string msgPrincipal = "Error de LectoEscritura: ";
        return (msgPrincipal + msg_).c_str();
    }

private:
    string msg_;

};

class CustomException : public exception
{
public:
    CustomException(const string& msg): msg_(msg) {}
    virtual ~CustomException() throw () {}

    virtual const char* what() const throw()
    {
        string msgPrincipal = "Error de programacion: ";
        return (msgPrincipal + msg_).c_str();
    }

private:
    string msg_;
};


/**FIN EXCEPCIONES*/
class ColorDTO
{
private:
    int r;
    int g;
    int b;
public:
    ColorDTO(int r, int g, int b) : r(r), g(g), b(b) {}
    int getR() const
    {
        return r;
    }
    int getG() const
    {
        return g;
    }
    int getB() const
    {
        return b;
    }
    void setR(int color)
    {
        r=color;
    }
    void setG(int color)
    {
        g=color;
    }
    void setB(int color)
    {
        b=color;
    }
};


class IUsaAllegro
{
public:
    virtual void activarAllegro()=0;
};


class AllegroEjecuta
{
private:
    static bool isAllegroActivado;
    static void iniciarAllegro();
public:
    static bool getIsAllegroActivado();
    static void setIsAllegroActivado(bool);
    static void activarAllegro();

};
bool AllegroEjecuta::isAllegroActivado = false;

bool AllegroEjecuta::getIsAllegroActivado()
{
    return isAllegroActivado;
}

void AllegroEjecuta::setIsAllegroActivado(bool flag)
{
    isAllegroActivado = flag;
}

void AllegroEjecuta::iniciarAllegro()
{
    try
    {
        setIsAllegroActivado(allegro_init() == 0 && install_keyboard() == 0);
        if(!getIsAllegroActivado())
            throw AllegroNoFuncionaException("No inicializaron las funciones de Allegro");
    }
    catch(AllegroNoFuncionaException& e)
    {
        cout << e.what() << endl;
    }
    catch(exception& e)
    {
        cout << "Standard exception: " << e.what() << endl;
    }
}

void AllegroEjecuta::activarAllegro()
{
    if (!getIsAllegroActivado())
        iniciarAllegro();
}

class Audio : public IUsaAllegro
{
protected:

    map<string,MIDI*> miMusica;
    map<string,SAMPLE*> miSonido;
    static Audio* unicaInstancia;
    void activarAllegro() override
    {
        AllegroEjecuta::activarAllegro();
    }

    Audio()
    {

        try
        {
            activarAllegro();

            if(install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL)!= 0)
                throw AllegroNoFuncionaException("No inicializo sistema de sonido. " + string(allegro_error));


            this->miMusica.insert(pair<string,MIDI*>("inicio",load_midi("sonidos/sub.mid")));


            for (map<string,MIDI*>::iterator it=miMusica.begin(); it!=miMusica.end(); ++it)
            {
                MIDI* musica = it->second;
                if(!musica)
                    throw new AudioArchivoNoEncontradoException("Falta archivo de musica");
            }

            this->miSonido.insert(pair<string,SAMPLE*>("inicioNivel",load_wav("sonidos/inicioNivel.wav")));
            this->miSonido.insert(pair<string,SAMPLE*>("ladrilloRoto",load_wav("sonidos/ladrilloRoto.wav")));
            this->miSonido.insert(pair<string,SAMPLE*>("rebotePelota",load_wav("sonidos/rebotePelota.wav")));
            this->miSonido.insert(pair<string,SAMPLE*>("vidaExtra",load_wav("sonidos/vidaExtra.wav")));
            this->miSonido.insert(pair<string,SAMPLE*>("rebotaBase",load_wav("sonidos/reboteBase.wav")));
            this->miSonido.insert(pair<string,SAMPLE*>("vidaPerdida",load_wav("sonidos/fallo.wav")));
            this->miSonido.insert(pair<string,SAMPLE*>("gameOver",load_wav("sonidos/game-over.wav")));
            for (map<string,SAMPLE*>::iterator it=miSonido.begin(); it!=miSonido.end(); ++it)
            {
                SAMPLE* sonido = it->second;
                if(!sonido)
                    throw new AudioArchivoNoEncontradoException("Falta archivo de sonido");
            }
        }
        catch(AllegroNoFuncionaException& e)
        {
            cout << e.what() << endl;
        }
        catch(AudioArchivoNoEncontradoException& e)
        {
            cout << e.what() << endl;
        }
        catch(exception& e)
        {
            cout << "Standard exception: " << e.what() << endl;
        }


    }

    void validarMusica(string index)
    {
        map<string,MIDI*>::iterator it;
        it = miMusica.find(index);
        if(it == miMusica.end())
            throw SonidoNoEncontradoException(index);
    }
    void validarSonido(string index)
    {
        map<string,SAMPLE*>::iterator it;
        it = miSonido.find(index);
        if(it == miSonido.end())
            throw SonidoNoEncontradoException(index);
    }

public:
    virtual ~Audio()
    {
        miMusica.clear();
        miSonido.clear();
        delete unicaInstancia;
    }

    static Audio* getInstancia();
    Audio(Audio &otroAudio) = delete;
    void operator = (const Audio &) = delete;


    void ejecutarMusica(string index, int repetir)
    {
        try
        {
            validarMusica(index);
            play_midi(miMusica[index],repetir);
        }
        catch(exception& e)
        {
            cout << e.what() << endl;
        }
    }
    void ejecutarSonido(string index)
    {
        try
        {
            validarSonido(index);
            play_sample(miSonido[index],200,150,500,0);// int play_sample(const SAMPLE *spl, int vol, int pan, int freq, int loop);
        }
        catch(exception& e)
        {
            cout << e.what() << endl;
        }
    }

    void setearVolumen(unsigned int sample_volume, unsigned int midi_volume)
    {
        set_volume(sample_volume,midi_volume); // 230 - 209
    }

    void detenerSonido(string index)
    {
        try
        {
            validarSonido(index);
            stop_sample(miSonido[index]);
        }
        catch(exception& e)
        {
            cout << e.what() << '\n';
        }
    }

    void pausarMusica()
    {
        midi_pause();
    }

};
Audio* Audio::unicaInstancia = 0;

Audio *Audio::getInstancia()
{
    if(unicaInstancia == 0)
    {
        unicaInstancia = new Audio();
    }
    return unicaInstancia;
}




class Puntaje
{

protected:
    unsigned int highScore;
    unsigned int puntajeActual;
    string archivoHighScore;

    unsigned int _leerArchivoPuntaje(string);
    void _escribirArchivoPuntaje(unsigned int);
    static Puntaje* unicaInstancia;

    Puntaje(string archivoPuntaje) //"high.dat"
    {
        this->archivoHighScore = archivoPuntaje;
        this->highScore = this->_leerArchivoPuntaje(this->archivoHighScore);
        this->puntajeActual = 0;
    }

public:
    virtual ~Puntaje()
    {
        delete unicaInstancia;
    }
    static Puntaje *getInstancia(string);
    Puntaje(Puntaje &otroPuntaje) = delete;
    void operator = (const Puntaje &) = delete;



    unsigned int getHighScore()
    {
        return highScore;
    }

    void setHighScore(unsigned int puntaje)
    {
        highScore = puntaje;
        _escribirArchivoPuntaje(puntaje);
    }

    unsigned int getPuntajeActual()
    {
        return puntajeActual;
    }

    void setPuntajeActual(unsigned int puntaje)
    {
        puntajeActual = puntaje;
    }
};

Puntaje* Puntaje::unicaInstancia = 0;
Puntaje* Puntaje::getInstancia(string archivoPuntaje)
{
    if(unicaInstancia == 0)
    {
        unicaInstancia = new Puntaje(archivoPuntaje);
    }
    return unicaInstancia;
}

unsigned int Puntaje::_leerArchivoPuntaje(string archivoHighScore) //"high.dat"
{
    try
    {
        ifstream lector;
        lector.open(archivoHighScore.c_str());
        if(lector.fail())
        {
            lector.close();
            throw ErrorLectoEscrituraException("Lectura -> No se pudo abrir el archivo o este no existe.");
        }
        else
        {
            char textoPuntaje [sizeof(lector)];

            if(!lector.eof())
            {
                lector.getline(textoPuntaje,sizeof(lector));
                string str = string(textoPuntaje);
                lector.close();
                return strtoul(str.c_str(), NULL, 10); // strtoul en C++ convierte la parte inicial de un array de char en un valor unsigned long
            }
            else
            {
                lector.close();
                throw ErrorLectoEscrituraException("Lectura -> El archivo esta vacio.");
            }
        }
    }
    catch(ErrorLectoEscrituraException& e)
    {
        cout << e.what() << endl;
        return 0;
    }
    catch(exception& e)
    {
        cout << "Standard exception: " << e.what() << endl;
        return 0;
    }
}

void Puntaje::_escribirArchivoPuntaje(unsigned int numPuntaje)
{
    try
    {
        ofstream escritor;
        escritor.open(archivoHighScore.c_str());
        if(escritor.fail())
        {
            escritor.close();
            throw ErrorLectoEscrituraException("Escritura -> No se pudo abrir el archivo o este no existe.");
        }
        escritor<< highScore << endl;
        escritor.close();
    }
    catch(ErrorLectoEscrituraException& e)
    {
        cout << e.what() << endl;
    }
    catch(exception& e)
    {
        cout << "Standard exception: " << e.what() << endl;
    }
}

class Punto
{
private:
    int x;
    int y;
public:
    Punto(int xp, int yp) : x(xp), y(yp) {}
    int getX()const
    {
        return x;
    }
    int getY()const
    {
        return y;
    }
    void setX(int valor)
    {
        x = valor;
    }
    void setY(int valor)
    {
        y = valor;
    }
    bool operator == (const Punto& otroPunto) /** Sobrecarga de operadores **/
    {
        return x == otroPunto.getX() && y == otroPunto.getY();
    }
};

class PuntoConstante
{
private:
    const int x;
    const int y;
public:
    PuntoConstante (int xp, int yp) : x(xp), y(yp) {}
    int getX()const
    {
        return x;
    }
    int getY()const
    {
        return y;
    }
};

class Pieza
{
protected:
    Punto* puntoCritico; //El puntoCritico (pc) es el vértice superior? izquierdo desde el cual comienza el dibujo, o bien, el centro de un círculo
    ColorDTO* colorPieza;
    list<Punto> puntosEnContorno;
public:
    Pieza(int x, int y) : puntoCritico(new Punto(x,y))
    {
        colorPieza = new ColorDTO(185,185,185); //Gris por defecto
    }
    virtual ~Pieza()
    {
        delete puntoCritico;
        delete colorPieza;
        puntosEnContorno.clear();
    }
    virtual void dibujar(BITMAP*)=0;
    virtual list<Punto> puntosEnContornoPieza()=0;
    Punto* getPuntoSiHaySuperposicion(Pieza* otraPieza)
    {
        bool flag = false;
        Punto* result = 0;
        list<Punto> PuntosOtraPieza = otraPieza->puntosEnContornoPieza();
        list<Punto> PuntosLocal = puntosEnContorno.size()==0 ? this->puntosEnContornoPieza() : this->puntosEnContorno; //A las piezas inmóviles se les va a calcular sus puntos por única vez en el constructor de la clase derivada

        for (list<Punto>::iterator itOtraPieza=PuntosOtraPieza.begin(); itOtraPieza != PuntosOtraPieza.end() && !flag; ++itOtraPieza)
        {
            for (list<Punto>::iterator itLocal=PuntosLocal.begin(); itLocal != PuntosLocal.end() && !flag; ++itLocal)
            {
                if(*itOtraPieza == *itLocal)
                {
                    flag=true;
                    result= new Punto((*itLocal).getX(),(*itLocal).getY());
                    break;
                }
            }
        }
        return result;
    }

    Punto* getPuntoCritico()
    {
        return puntoCritico;
    }
    void setPuntoCritico(int x, int y)
    {
        puntoCritico = new Punto(x,y);
    }
    ColorDTO* getColorPieza()
    {
        return colorPieza;
    }
    void setColorPieza(int r, int g, int b)
    {
        colorPieza->setR(r);
        colorPieza->setG(g);
        colorPieza->setB(b);
    }
};



class PiezaRectangular : public Pieza, public IUsaAllegro
{
protected:
    Punto* puntoOpuesto;            /** puntoOpuesto es el vértice contrario a la diagonal del punto crítico en un rectángulo/cuadrado

                                        P crítico -> * * * * * * *
                                                     *           *
                                                     *           *
                                                     * * * * * * * <- P opuesto

                                    **/
    void activarAllegro() override
    {
        AllegroEjecuta::activarAllegro();
    }
public:
    PiezaRectangular(int pcX, int pcY,int poX, int poY) : Pieza(pcX,pcY), puntoOpuesto(new Punto(poX,poY))
    {
        activarAllegro();
    }
    virtual ~PiezaRectangular()
    {
        delete puntoOpuesto;
    }

    Punto* getPuntoOpuesto() const
    {
        return puntoOpuesto;
    }
    virtual void setPuntoOpuesto(int poX, int poY) = 0;

    void dibujar (BITMAP* buffer) override
    {
        rectfill(buffer,puntoCritico->getX(),puntoCritico->getY(),puntoOpuesto->getX(),puntoOpuesto->getY(),makecol(colorPieza->getR(),colorPieza->getG(),colorPieza->getB()));
    }
    list<Punto> puntosEnContornoPieza() override
    {

        list<Punto> pContorno;
        int pcX = puntoCritico->getX();
        int pcY = puntoCritico->getY();
        int poX = puntoOpuesto->getX();
        int poY = puntoOpuesto->getY();

        //lados verticales
        for (int y = pcY; y<= poY; y++)
        {
            pContorno.push_back(Punto(pcX,y));
            pContorno.push_back(Punto(poX,y));
        }
        //lados horizontales
        for (int x = pcX; x <= poX; x++)
        {
            pContorno.push_back(Punto(x,pcY));
            pContorno.push_back(Punto(x,poY));
        }

        return pContorno;
    }

    bool esVerticeSuperiorDerecho(Punto p)
    {
        Punto pcF = Punto(puntoOpuesto->getX(),puntoCritico->getY());
        return p == pcF;
    }

    bool esVerticeSuperiorIzquierdo(Punto p)
    {
        return p == *puntoCritico;
    }

    bool esVerticeSuperior(Punto p)
    {
        return esVerticeSuperiorDerecho(p) || esVerticeSuperiorIzquierdo(p);
    }

    bool esVerticeInferiorDerecho (Punto p)
    {
        return p == *puntoOpuesto;
    }

    bool esVerticeInferiorIzquierdo (Punto p)
    {
        Punto poF = Punto(puntoCritico->getX(),puntoOpuesto->getY());
        return p == poF;
    }

    bool esVerticeInferior (Punto p)
    {
        return esVerticeInferiorDerecho(p) || esVerticeInferiorIzquierdo(p);
    }

    bool esLadoHorizontalSuperior(Punto p)
    {
        return !esVerticeSuperior(p) && (p.getY() == puntoCritico->getY());
    }

    bool esLadoHorizontalInferior(Punto p)
    {
        return !esVerticeInferior(p) && (p.getY() == puntoOpuesto->getY());
    }

    bool esLadoVerticalDerecho(Punto p)
    {
        return !esVerticeInferior(p) && !esVerticeSuperior(p) && (p.getX() == puntoOpuesto->getX());
    }

    bool esLadoVerticalDerecho(int x)
    {
        return x >= puntoOpuesto->getX();
    }

    bool esLadoVerticalIzquierdo(Punto p)
    {
        return !esVerticeInferior(p) && !esVerticeSuperior(p) && (p.getX() == puntoCritico->getX());
    }

    bool esLadoVerticalIzquierdo(int x)
    {
        return x <= puntoCritico->getX();
    }

    bool esLadoVertical (Punto p)
    {
        return esLadoVerticalDerecho(p) || esLadoVerticalIzquierdo(p);
    }

    bool esLadoHorizontalSuperior(int y)
    {
        return y <= puntoCritico->getY();
    }

};

class Reposable
{
protected:
    const PuntoConstante * puntoReposo;
public:
    Reposable(int prX, int prY) : puntoReposo(new PuntoConstante(prX,prY)) {}
    virtual ~Reposable()
    {
        delete puntoReposo;
    }
    virtual void reposar()=0;
    const PuntoConstante* getPuntoReposo()
    {
        return puntoReposo;
    }
};



class Movible
{
protected:
    int velocidad;
    const int velocidadInicial;
    int dirX;
    int dirY;
    const int dirXInicial;
    const int dirYInicial;
    bool meMuevo;

public:
    enum Direccion
    {
        Arriba = -1,
        Abajo = 1,
        Derecha = 1,
        Izquierda = -1
    };
    Movible(int vel, Direccion dx, Direccion dy) : velocidadInicial(vel), dirXInicial(dx), dirYInicial(dy)
    {
        this->velocidad = vel;
        this->dirX = dx;
        this->dirY = dy;
        this->meMuevo = false;

    }
    virtual ~Movible() {}
    virtual void mover()=0;
    int getVelocidad()
    {
        return velocidad;
    }
    void setVelocidad(int valor)
    {
        velocidad = valor;
    }
    int getDirX()
    {
        return dirX;
    }
    int getDirY()
    {
        return dirY;
    }
    int getDirXInicial()
    {
        return dirXInicial;
    }
    int getDirYInicial()
    {
        return dirYInicial;
    }
    void setDirX(int valor)
    {
        dirX = valor;
    }
    void setDirY(int valor)
    {
        dirY = valor;
    }
    bool getMeMuevo()
    {
        return meMuevo;
    }
    void setMeMuevo(bool valor)
    {
        meMuevo = valor;
    }
    int getVelocidadInicial()
    {
        return velocidadInicial;
    }
    void invertirDirecciones()
    {
        dirX *= -1;
        dirY *= -1;
    }
    void invertirDirX()
    {
        dirX *= -1;
    }
    void invertirDirY()
    {
        dirY *= -1;
    }

};

class IEsImpactable
{
public:
    virtual bool hayImpacto(Movible*)=0;
    virtual void reciboImpacto(Movible*)=0;
    virtual void redirigirMovible(Movible* pMovible, Punto puntoContacto)=0;
};

class PiezaCircular: public Pieza, public IUsaAllegro
{
protected:
    int radio;
    bool esRelleno; /** Esta propiedad la tuvimos que agregar cuando incorporamos las burbujas, antes todas las figuras eran rellenas**/
    void activarAllegro() override
    {
        AllegroEjecuta::activarAllegro();
    }
public:
    PiezaCircular(int pcX, int pcY,int rad, bool relleno) : Pieza(pcX, pcY), radio(rad), esRelleno(relleno)
    {
        activarAllegro();
    }
    virtual ~PiezaCircular() {}
    int getRadio() const
    {
        return radio;
    }
    void setRadio(int valor)
    {
        radio=valor;
    }
    void dibujar(BITMAP* buffer) override
    {
        if(esRelleno)
            circlefill(buffer,puntoCritico->getX(),puntoCritico->getY(),radio,makecol(colorPieza->getR(),colorPieza->getG(),colorPieza->getB()));
        else
            circle(buffer,puntoCritico->getX(),puntoCritico->getY(),radio,makecol(colorPieza->getR(),colorPieza->getG(),colorPieza->getB()));
    }
    list<Punto> puntosEnContornoPieza() override  /** Funciona bien cuando se compara con los puntos de una Pieza rectangular (es decir, Rectángulo con rectángulo: PiezaRectangular::puntosEnContornoPieza vs. PiezaRectangular::puntosEnContornoPieza)
                                                    , o círculo con rectángulo (PiezaCircular::puntosEnContornoPieza vs. PiezaRectangular::puntosEnContornoPieza), pero no resulta con una figura circular (no sirve para círculo con círculo) **/
    {
        int centroX = puntoCritico->getX();
        int centroY = puntoCritico->getY();
        list<Punto> pContorno;

        for (int x = (centroX - radio); x <= (centroX + radio); x++)
        {
            for (int y = (centroY - radio); y <= (centroY + radio); y++)
            {
                double distancia = sqrt( pow((x - centroX),2) + pow((y - centroY),2) ); //Fórmula de distancia para verificar si un punto está dentro de un círculo. Ver (https://www.youtube.com/watch?v=JFcdKZNylII)

                if(distancia == radio)
                {
                    pContorno.push_back(Punto(x,y));
                }
            }
        }

        return pContorno;
    }

    bool seSuperponeConOtroCirculo(PiezaCircular* otroCir) /** Puede decir si dos círculos se tocan, pero no puede determinar cuál es el punto tocado **/
    {
        Punto* centroOtroCir = otroCir->getPuntoCritico();
        int radioOtroCir = otroCir->getRadio();
        Punto* centroMio = this->getPuntoCritico();
        int radioMio = this->getRadio();

        double distancia = sqrt( pow((centroMio->getX() - centroOtroCir->getX()),2) + pow((centroMio->getY() - centroOtroCir->getY()),2) ); //Fórmula de distancia

        return !(distancia > (radioMio+radioOtroCir));
    }
    /**  Estas funciones están para usar en conjunto con "seSuperponeConOtroCirculo".
         No pueden determinar si se superponen dos círculos, pero de hacerlo, dicen en qué
         cuadrante se daría el impacto (Los puntos de la cruz del círculo y su centro se subsumen a uno de los cuadrantes)

                * -> P sup
                *
    P izq-> * * C * * -> P der
                *
                * -> P inf
    **/
    bool esArcoSuperiorDerecho(Punto p) // p sería el centro de otra figura circular
    {
        return ((p.getY() < this->getPuntoCritico()->getY()) && (p.getX()> this->getPuntoCritico()->getX())) || ((p.getX() == this->getPuntoCritico()->getX()) && (p.getY() <= this->getPuntoCritico()->getY())); //... OR PuntoSup o centros superpuestos
    }

    bool esArcoInferiorDerecho(Punto p)
    {
        return ((p.getY() > this->getPuntoCritico()->getY()) && (p.getX()> this->getPuntoCritico()->getX())) || ((p.getX()>this->getPuntoCritico()->getX()) && (p.getY() == this->getPuntoCritico()->getY()));//... OR PuntoDerecho
    }

    bool esArcoSuperiorIzquierdo(Punto p)
    {
        return ((p.getY() < this->getPuntoCritico()->getY()) && (p.getX()< this->getPuntoCritico()->getX())) || ((p.getX()<this->getPuntoCritico()->getX()) && (p.getY() == this->getPuntoCritico()->getY()));//... OR PuntoIzquiedo
    }

    bool esArcoInferiorIzquierdo(Punto p)
    {
        return ((p.getY() > this->getPuntoCritico()->getY()) && (p.getX()< this->getPuntoCritico()->getX())) || ((p.getX() == this->getPuntoCritico()->getX()) && (p.getY()>this->getPuntoCritico()->getY()));//... OR PuntoInferior
    }
};

class Submarino; //Declaración incompleta de Submarino para poder crear la firma de Bola::desplazarJuntoConSubmarino
class Ring;

class Bola : public Movible, public Reposable, public PiezaCircular
{
protected:
    bool bolaPerdida;
    bool rompiLad;
    bool reboteRingLad;
    bool reboteSub;
    bool rompiBur;

    void activarAllegro() override
    {
        AllegroEjecuta::activarAllegro();
    }

public:
    Bola(): Movible(1,Derecha,Abajo), Reposable(303,60),PiezaCircular(303,60,10,true)
    {
        this-> bolaPerdida = false;
        this-> rompiLad = false;
        this->rompiBur = false;
        this-> reboteRingLad = false;
        this-> reboteSub = false;
        activarAllegro();
        setColorPieza(124,250,16);
    }

    virtual ~Bola() {}

    void mover() override
    {
        int centroX = puntoCritico->getX();
        int centroY = puntoCritico->getY();
        setPuntoCritico(centroX + (1 * dirX), centroY + (1 * dirY));
        setMeMuevo(true);
    }

    void desplazarJuntoConSubmarino(Submarino* submarino); //Tuve que crear el comportamiento de la función después de la declaración completa de Submarino

    void reposar() override
    {
        setPuntoCritico(getPuntoReposo()->getX(),getPuntoReposo()->getY());
        setMeMuevo(false);
    }

    bool getBolaPerdida() const
    {
        return bolaPerdida;
    }
    void setBolaPerdida(bool valor)
    {
        bolaPerdida = valor;
    }


    bool getRompiLad() const
    {
        return rompiLad;
    }
    void setRompiLad(bool valor)
    {
        rompiLad = valor;
    }
    bool getRompiBur() const
    {
        return rompiBur;
    }
    void setRompiBur(bool valor)
    {
        rompiBur = valor;
    }
    bool getReboteRingLad() const
    {
        return reboteRingLad;
    }
    void setReboteRingLad(bool valor)
    {
        reboteRingLad = valor;
    }
    bool getReboteSub() const
    {
        return reboteSub;
    }
    void setReboteSub(bool valor)
    {
        reboteSub = valor;
    }

    void restablecerFlagsImpacto()
    {
        bolaPerdida = false;
        rompiLad=false;
        rompiBur=false;
        reboteRingLad=false;
        reboteSub= false;
    }
    bool hayFlagsImpactoActivas()
    {
        return bolaPerdida || rompiLad || rompiBur || reboteRingLad || reboteSub;
    }


};

class TipoLadrillo
{
protected:
    int resistencia;
    ColorDTO* colorLadrillo;
    void setResistencia(int valor)
    {
        resistencia = valor;
    }
    void setColorLadrillo (int r, int g, int b)
    {
        colorLadrillo->setR(r);
        colorLadrillo->setG(g);
        colorLadrillo->setB(b);
    }
public:
    TipoLadrillo(int dureza, int r, int g, int b) : resistencia(dureza), colorLadrillo(new ColorDTO(r,g,b)) {}
    virtual ~TipoLadrillo()
    {
        delete colorLadrillo;
    }
    int getResistencia()
    {
        return resistencia;
    }
    ColorDTO* getColorLadrillo() const
    {
        return colorLadrillo;
    }
    virtual void impacto(Bola* bola)=0;
};

class LadUnSoloUso : public TipoLadrillo
{
public:
    LadUnSoloUso(int r, int g, int b) : TipoLadrillo(1,r,g,b) {}
    void impacto(Bola* bola) override
    {
        resistencia=0;
        bola->setRompiLad(true);
    }
};

class LadVariosUsos : public TipoLadrillo
{
private:
    static const int maxResistencia = 3;
    int validRes(int dureza)
    {
        if (dureza>maxResistencia || dureza<1)
            dureza =maxResistencia;
        return dureza;
    }
    int colDur(int resistencia, char canal)
    {

        const ColorDTO* c1 = new ColorDTO(224, 247, 230); //verdeSuave
        const ColorDTO* c2 = new ColorDTO(169, 232, 219); //verdeAgua
        const ColorDTO* c3 = new ColorDTO(1, 191, 196); //turquesa

        switch(resistencia)
        {
        case 1:
            try
            {
                if(canal == 'r')
                    return c1->getR();
                else if(canal == 'g')
                    return c1->getG();
                else if(canal == 'b')
                    return c1->getB();
                else
                    throw new CustomException("Error en LadVariosUsos->colorDur(int resistencia,char canal) con el canal");
            }
            catch(CustomException& e)
            {
                cout << e.what() << endl;
                return c1->getR();
            }

            break;
        case 2:
            try
            {
                if(canal == 'r')
                    return c2->getR();
                else if(canal == 'g')
                    return c2->getG();
                else if(canal == 'b')
                    return c2->getB();
                else
                    throw new CustomException("Error en LadVariosUsos->colorDur(int resistencia,char canal) con el canal");
            }
            catch(CustomException& e)
            {
                cout << e.what() << endl;
                return c2->getR();
            }

            break;
        case 3:
            try
            {
                if(canal == 'r')
                    return c3->getR();
                else if(canal == 'g')
                    return c3->getG();
                else if(canal == 'b')
                    return c3->getB();
                else
                    throw new CustomException("Error en LadVariosUsos->colorDur(int resistencia,char canal) con el canal");
            }
            catch(CustomException& e)
            {
                cout << e.what() << endl;
                return c3->getR();
            }

            break;
        default:
            try
            {
                throw new CustomException("Error en LadVariosUsos->colorDur(int resistencia,char canal) con la resistencia");
            }
            catch(CustomException& e)
            {
                cout << e.what() << endl;
                return c1->getR();
            }
        }

        delete c1;
        delete c2;
        delete c3;
    }
public:
    LadVariosUsos(int dureza): TipoLadrillo(validRes(dureza),colDur(validRes(dureza),'r'),colDur(validRes(dureza),'g'),colDur(validRes(dureza),'b')) {}
    void impacto(Bola* bola) override
    {
        if(resistencia>0)
            setResistencia(resistencia-1);
        if(resistencia>0)
        {
            setColorLadrillo(colDur(resistencia,'r'),colDur(resistencia,'g'),colDur(resistencia,'b'));
            bola->setReboteRingLad(true);
        }
        else
            bola->setRompiLad(true);
    }
};

class LadIndestructible : public TipoLadrillo
{
protected:
    int contadorImpactosLadIndestructible;
public:
    LadIndestructible() : TipoLadrillo(-1,185,185,185)
    {
        this->contadorImpactosLadIndestructible=0;
    }
    int getContadorImpactosLadIndestructible()const
    {
        return contadorImpactosLadIndestructible;
    }
    void setContadorImpactosLadIndestructible(int valor)
    {
        contadorImpactosLadIndestructible=valor;
    }
    void impacto(Bola* bola) override
    {
        contadorImpactosLadIndestructible++;
        bola->setReboteRingLad(true);
    }
};


class Ladrillo : public PiezaRectangular, public IEsImpactable
{
protected:
    TipoLadrillo* tipoLad;
public:
    static const int largo = 60;
    static const int alto = 25;
    enum Tipo
    {
        Marron = 1,
        Lila = 2,
        Naranja = 3,
        Piedra = 4,
        Turquesa = 5,
        VerdeAgua = 6,
        VerdeSuave = 7,
        Indestructible = 8
    };

    Ladrillo(int pcX, int pcY, Tipo tipo): PiezaRectangular(pcX, pcY, pcX+largo, pcY+alto)
    {
        activarAllegro();

        this->puntosEnContorno = this->puntosEnContornoPieza();

        switch(tipo)
        {
        case Marron:
            tipoLad = new LadUnSoloUso(95, 75, 62);
            break;
        case Lila:
            tipoLad = new LadUnSoloUso(205, 193, 203);
            break;
        case Naranja:
            tipoLad = new LadUnSoloUso(235, 165, 106);
            break;
        case Piedra:
            tipoLad = new LadUnSoloUso(219, 204, 193);
            break;
        case Turquesa:
            tipoLad = new LadVariosUsos(3);
            break;
        case VerdeAgua:
            tipoLad = new LadVariosUsos(2);
            break;
        case VerdeSuave:
            tipoLad = new LadVariosUsos(1);
            break;
        case Indestructible:
            tipoLad = new LadIndestructible();
            break;
        default:
            cout << "Que onda " << tipo << endl;
            tipoLad = new LadUnSoloUso(95, 75, 62);
            break;
        }

        ColorDTO* colorLad = this->tipoLad->getColorLadrillo();
        setColorPieza(colorLad->getR(),colorLad->getG(),colorLad->getB());
    }
    virtual ~Ladrillo()
    {
        delete tipoLad;
    }

    void setPuntoOpuesto(int poX, int poY)
    {
        puntoOpuesto = new Punto(poX,poY);
    }

    bool hayImpacto(Movible* pMovible) override
    {
        bool result = false;

        if(typeid(Bola)==typeid(*pMovible))
        {
            Bola* soyBola = static_cast<Bola*>(pMovible); /** static_cast es menos seguro que dynamic_cast, pero menos costoso. Por eso usamos typeid para constatar **/
            int radio = soyBola->getRadio();
            int izqBola = soyBola->getPuntoCritico()->getX() - radio;
            int derBola = soyBola->getPuntoCritico()->getX() + radio;
            int izqLad = this->puntoCritico->getX();
            int derLad = this->puntoOpuesto->getX();
            int abajoBola = soyBola->getPuntoCritico()->getY() + radio;
            int arribaBola = soyBola->getPuntoCritico()->getY() - radio;
            int abajoLad = this->getPuntoOpuesto()->getY();
            int arribaLad = this->getPuntoCritico()->getY();

            if(((izqBola <= derLad) && (izqBola >= izqLad)) || ((derBola >= izqLad) && (derBola <= derLad)))
            {
                if(((abajoBola >= arribaLad) && (abajoBola <= abajoLad)) || ((arribaBola <= abajoLad) && (arribaBola >= arribaLad)))
                {
                    Punto* puntoContacto = Pieza::getPuntoSiHaySuperposicion(soyBola);
                    if(puntoContacto!=0)
                    {
                        result =true;
                        redirigirMovible(pMovible,*puntoContacto);
                    }
                    delete puntoContacto;
                }
            }
        }
        return result;
    }

    void reciboImpacto (Movible* pMovible) override
    {
        if(typeid(Bola)==typeid(*pMovible))
        {
            Bola* soyBola = static_cast<Bola*>(pMovible);
            tipoLad->impacto(soyBola);
        }
    }

    TipoLadrillo* getTipoLadrillo() const
    {
        return tipoLad;
    }

    void dibujar (BITMAP* buffer) override
    {
        /** HERENCIA - POLIMORFISMO: Primero llamamos al método de la clase padre, después agregamos comportamiento*/

        PiezaRectangular::dibujar(buffer);

        if(typeid(LadIndestructible)==typeid(*(this->tipoLad)))
            for(int grosor = 0; grosor<2; grosor++)
                rect(buffer,puntoCritico->getX()+grosor,puntoCritico->getY()+grosor,puntoOpuesto->getX()-grosor,puntoOpuesto->getY()-grosor,makecol(255,255,255));
    }

private:
    void redirigirMovible(Movible* pMovible, Punto p) override
    {
        if(typeid(Bola)==typeid(*pMovible))
        {
            if(Ladrillo::esVerticeSuperiorIzquierdo(p))
            {
                pMovible->setDirX(pMovible->Izquierda);
                pMovible->setDirY(pMovible->Arriba);
            }
            else if (Ladrillo::esVerticeSuperiorDerecho(p))
            {
                pMovible->setDirX(pMovible->Derecha);
                pMovible->setDirY(pMovible->Arriba);
            }
            else if (Ladrillo::esVerticeInferiorIzquierdo(p))
            {
                pMovible->setDirX(pMovible->Izquierda);
                pMovible->setDirY(pMovible->Abajo);
            }
            else if (Ladrillo::esVerticeInferiorDerecho(p))
            {
                pMovible->setDirX(pMovible->Derecha);
                pMovible->setDirY(pMovible->Abajo);
            }
            else if (Ladrillo::esLadoHorizontalSuperior(p))
                pMovible->setDirY(pMovible->Arriba);
            else if (Ladrillo::esLadoHorizontalInferior(p))
                pMovible->setDirY(pMovible->Abajo);
            else if (Ladrillo::esLadoVerticalIzquierdo(p))
                pMovible->setDirX(pMovible->Izquierda);
            else if (Ladrillo::esLadoVerticalDerecho(p))
                pMovible->setDirX(pMovible->Derecha);
        }
    }

};


class Muro : public IEsImpactable
{
public:
    static const int F = 7;//Filas 7
    static const int C = 9;//Columnas 9
    static const int InicioPCXMuro = 13;
    static const int InicioPCYMuro = 500;
    static const int Interespaciado = 5;
    static const int FinPCYMuro = InicioPCYMuro + ((F-1)* (Ladrillo::alto+Interespaciado)) + Ladrillo::alto;
    static const int FinPCXMuro = InicioPCXMuro +((C-1) * (Ladrillo::largo+Interespaciado)) + Ladrillo::largo;
protected:
    int ladrillosDestruidosEnRonda;
    int pant1[F][C] =
    {
        {0,0,3,0,1,0,3,0,2},
        {3,0,2,1,0,1,2,0,1},
        {0,0,1,0,3,0,0,0,0},
        {8,0,0,0,6,6,0,1,0},
        {0,2,7,6,0,6,7,0,2},
        {0,0,0,0,7,7,0,0,0},
        {8,0,1,7,1,0,0,0,7}
    };

    int pant2[F][C] =
    {
        {1,1,0,1,1,1,0,1,0},
        {2,0,2,0,2,2,2,2,2},
        {3,8,0,0,0,8,3,4,3},
        {8,8,4,0,0,8,4,4,4},
        {8,8,0,0,8,8,5,5,5},
        {6,0,0,0,8,6,6,6,6},
        {7,8,4,8,4,7,7,7,7}
    };

    int muroActual[F][C] =
    {
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0}
    };

    list<Ladrillo*> muroActualList;
    static Muro* unicaInstancia;
    Muro()
    {
        this->ladrillosDestruidosEnRonda=0;
    }

private:
    void insertarEnMuroActualList (int col, int fil, int tipoLad);
    void redirigirMovible(Movible* pMovible, Punto p)
    {
        /*Sin implementar, se ocupa Ladrillo de esta función*/
    }
public:
    virtual ~Muro()
    {
        muroActualList.clear();
        delete[] pant1;
        delete[] pant2;
        delete[] muroActual;
        delete unicaInstancia;
    }
    static Muro* getInstancia();
    Muro(Muro &otroMuro) = delete;
    void operator = (const Muro&) = delete;

    int* getMuroActual()
    {
        return &muroActual[0][0];
    }
    int* getPant1()
    {
        return &pant1[0][0];
    }
    int* getPant2()
    {
        return &pant2[0][0];
    }
    void setMuroActual(int fila, int columna, int valor)
    {
        getMuroActual()[fila * C + columna] = valor;
    }
    void configurarMuroActualMultiMap(int);
    void dibujarMuroActualList(BITMAP* buffer);
    bool hayImpacto(Movible* pMovible) override
    {
        bool result = false;

        if(typeid(Bola)==typeid(*pMovible))
        {
            Bola* soyBola = static_cast<Bola*>(pMovible);
            if (soyBola->getPuntoCritico()->getY() + soyBola->getRadio() >= InicioPCYMuro) // Es decir, si el punto más meridional de la bola iguala o supera el inicio del Muro
            {
                for (list<Ladrillo*>::iterator it=muroActualList.begin(); it != muroActualList.end(); ++it)
                {
                    Ladrillo* lad = *it;
                    if (lad->hayImpacto(soyBola))
                    {
                        lad->reciboImpacto(soyBola);
                        if(lad->getTipoLadrillo()->getResistencia()>0)
                        {
                            ColorDTO* colorLad = lad->getTipoLadrillo()->getColorLadrillo();
                            lad->setColorPieza(colorLad->getR(),colorLad->getG(),colorLad->getB());
                        }
                        result = true;
                    }
                }
            }
        }
        return result;
    }

    void reciboImpacto (Movible* pMovible) override
    {
        if(typeid(Bola)==typeid(*pMovible))
        {
            for (list<Ladrillo*>::iterator it=muroActualList.begin(); it != muroActualList.end(); ++it)
            {
                Ladrillo* lad = *it;
                if(lad->getTipoLadrillo()->getResistencia() == 0)
                {
                    muroActualList.erase(it);
                    ladrillosDestruidosEnRonda++;
                }
            }
        }
    }

    bool seEliminaronLadrillos()
    {
        bool result = true;
        for (list<Ladrillo*>::iterator it=muroActualList.begin(); it!=muroActualList.end(); ++it)
        {
            Ladrillo* lad = *it;

            TipoLadrillo* tipoLadrillo = lad->getTipoLadrillo();

            if(typeid(LadIndestructible)!=typeid(*tipoLadrillo))
            {
                result = false;
                break;
            }
        }

        return result;
    }

    int getLadrillosDestruidosEnRonda()
    {
        return ladrillosDestruidosEnRonda;
    }
    void resetLadrillosDestruidosEnRonda()
    {
        ladrillosDestruidosEnRonda=0;
    }


};
Muro* Muro::unicaInstancia = 0;

Muro* Muro::getInstancia()
{
    if(unicaInstancia == 0)
        unicaInstancia = new Muro();
    return unicaInstancia;
}

void Muro::insertarEnMuroActualList(int col,int fil, int tipoLad)
{
    int ladX = InicioPCXMuro + (col * (Ladrillo::largo + Interespaciado));
    int ladY= InicioPCYMuro + (fil * (Ladrillo::alto + Interespaciado));

    Ladrillo* lad = new Ladrillo(ladX,ladY,(Ladrillo::Tipo)tipoLad);
    muroActualList.push_back(lad);
}

void Muro::configurarMuroActualMultiMap(int nivel)
{
    muroActualList.clear();
    for(int fil = 0; fil< F; fil++)
    {
        for (int col= 0; col< C; col++)
        {
            switch(nivel)
            {
            case 1:
                if (pant1[fil][col]>0)
                {
                    insertarEnMuroActualList(col,fil,pant1[fil][col]);
                }
                break;
            case 2:
                if (pant2[fil][col]>0)
                {
                    insertarEnMuroActualList(col,fil,pant2[fil][col]);
                }
                break;
            default:
                int azar = rand() % 8;//nunca saldría un 8 (indestructible) // a propósito, no sea que ladrillos indestructibles encierren a ladrillos comunes y no se pueda ganar.
                if(azar != 0) //no existe el tipo "0". Cuando azar es cero, se saltea insertarEnMuroActualList, lo que equivale a que no exista un Lad en esa posición.
                {
                    insertarEnMuroActualList(col,fil,azar);
                }
            }
        }
    }
}

void Muro::dibujarMuroActualList(BITMAP* buffer)
{
    for (list<Ladrillo*>::iterator it=muroActualList.begin(); it!=muroActualList.end(); ++it)
    {
        Ladrillo* lad = *it;
        lad->dibujar(buffer);
    }
}

class Burbuja : public Movible, public PiezaCircular, public IEsImpactable
{
protected:
    int resistencia;
    unsigned int rondaBurbuja; //Sirve para limitar/reducir la velocidad en burbuja
    bool meImpactoSubmarinoOBola = false;

public:
    Burbuja(int vel, int pcX, int pcY, int rad): Movible(vel,Derecha,Arriba), PiezaCircular(pcX,pcY,rad,false)
    {
        activarAllegro();
        setColorPieza(254,0,204);
        this->resistencia=1;
        this->rondaBurbuja = 0;
    }
    virtual ~Burbuja() {}
    int getResistencia() const
    {
        return resistencia;
    }
    void setResistencia(int valor)
    {
        resistencia=valor;
    }

    void setMeImpactoSubmarinoOBola (bool valor)
    {
        meImpactoSubmarinoOBola = valor;
    }

    bool getMeImpactoSubmarinoOBola ()
    {
        return meImpactoSubmarinoOBola;
    }
    void mover() override
    {
        if(rondaBurbuja!=0 && rondaBurbuja%100==0)
            Burbuja::invertirDirX();

        int centroX = puntoCritico->getX();
        int centroY = puntoCritico->getY();
        setPuntoCritico(centroX + (1 * dirX), centroY + (1 * dirY));
        setMeMuevo(true);
        rondaBurbuja++;
    }

    bool hayImpacto(Movible* pMovible) override
    {
        bool result = false;

        if(typeid(Bola)==typeid(*pMovible) || typeid(Burbuja)==typeid(*pMovible) )
        {
            Bola* soyBola = 0;
            Burbuja* soyBur = 0;
            PiezaCircular* circ2;

            if (typeid(Bola)==typeid(*pMovible))
            {
                soyBola = static_cast<Bola*>(pMovible);
                circ2 = soyBola;
            }
            else
            {
                soyBur = static_cast<Burbuja*>(pMovible);
                circ2 = soyBur;
            }

            if(seSuperponeConOtroCirculo(circ2))
            {
                // cout << "ESTOY DENTRO FUNCA SE SUPERPONE!!!!" << endl;
                // Sleep(500);
                result =true;
                redirigirMovible(pMovible,*(circ2->getPuntoCritico()));
                if(soyBola!=0)
                {
                    soyBola->setRompiBur(result);
                    setMeImpactoSubmarinoOBola(result);
                }

            }

        }
        return result;
    }

    void reciboImpacto(Movible* pMovible) override
    {
        if(typeid(Bola)==typeid(*pMovible))
        {
            this->resistencia = 0;
            //cout << "BOLA COLISIONA BURBUJA: " << endl;
            // Sleep(3000);
        }

        if(typeid(Burbuja)==typeid(*pMovible))
        {
            //Sin acción por ahora
            // cout << "SE CHOCARON LAS BURBUJAS" << endl;
            //Sleep(1000);
        }
    }


private:
    void redirigirMovible(Movible* pMovible, Punto otroCentro) override
    {
        if(typeid(Bola)==typeid(*pMovible) || typeid(Burbuja)==typeid(*pMovible))
        {
            if(PiezaCircular::esArcoSuperiorDerecho(otroCentro))
            {
                pMovible->setDirX(pMovible->Derecha);
                pMovible->setDirY(pMovible->Arriba);
            }
            else if(PiezaCircular::esArcoInferiorDerecho(otroCentro))
            {
                pMovible->setDirX(pMovible->Derecha);
                pMovible->setDirY(pMovible->Abajo);
            }
            else if(PiezaCircular::esArcoInferiorIzquierdo(otroCentro))
            {
                pMovible->setDirX(pMovible->Izquierda);
                pMovible->setDirY(pMovible->Abajo);
            }
            else if(PiezaCircular::esArcoSuperiorIzquierdo(otroCentro))
            {
                pMovible->setDirX(pMovible->Izquierda);
                pMovible->setDirY(pMovible->Arriba);
            }
        }
    }



};

class Burbujero : public IEsImpactable
{
protected:
    list<Burbuja*> burbujeroList;
    int burbujasDestruidasEnRonda;
    int burbujasDestruidasParaVida;
    static Burbujero* unicaInstancia;
    vector<int> radiosPosibles;
    bool burbujeroOn;
    time_t tempInicial;
    double plazoAprox;
    Burbujero()
    {
        this->radiosPosibles.push_back(10);
        this->radiosPosibles.push_back(13);
        this->burbujeroOn = false;
        this->plazoAprox = 1.5; //segundos
        this->burbujasDestruidasEnRonda = 0;
        this->burbujasDestruidasParaVida = 0;
        void reiniciarTempInicial();
    }
public:
    static const int pisoYBurbujero = Muro::FinPCYMuro;
    static const int paredIzqBurbujero = Muro::InicioPCXMuro;
    static const int paredDerBurbujero = Muro::FinPCXMuro;
    virtual ~Burbujero()
    {
        burbujeroList.clear();
        delete unicaInstancia;
        radiosPosibles.clear();
    }
    static Burbujero* getInstancia();
    Burbujero(Burbujero &otroBurbujero) = delete;
    void operator = (const Burbujero&) = delete;

    bool getBurbujeroOn() const
    {
        return burbujeroOn;
    }
    int getBurbujasDestruidasEnRonda ()
    {
        return burbujasDestruidasEnRonda;
    }

    void resetBurbujasDestruidasEnRonda ()
    {
        burbujasDestruidasEnRonda = 0;
    }

    int getBurbujasDestruidasParaVida ()
    {
        return burbujasDestruidasParaVida;
    }

    void resetBurbujasDestruidasParaVida ()
    {
        burbujasDestruidasParaVida = 0;
    }
    void setBurbujeroOn(bool flag)
    {
        burbujeroOn=flag;
    }

    list<Burbuja*>* getBurbujeroList()
    {
        return &burbujeroList;
    }
    void limpiarBurbujeroList()
    {
        burbujeroList.clear();
    }

    void reiniciarTempInicial()
    {
        time(&tempInicial);   //setea el tiempo actual al momento de la llamda en el parámetro tempInicial
    }
    void siCrearBurbujasPorPlazo()
    {
        if(burbujeroOn)
        {
            time_t ahora = time(NULL);
            srand(ahora);

            if(difftime(ahora,tempInicial)>= plazoAprox)
            {
                int cant = 0;
                while(cant<3 and burbujeroList.size() <9)
                {
                    int radio = radiosPosibles[rand()%radiosPosibles.size()];
                    int pcY =pisoYBurbujero-radio;
                    int pcX = (rand()% ((paredDerBurbujero-radio)-(paredIzqBurbujero+radio)+1)) + (paredIzqBurbujero+radio);
                    burbujeroList.push_back(new Burbuja(1,pcX,pcY,radio));
                    cant++;
                }
                reiniciarTempInicial();
            }
        }
    }

    void dibujarBurbujeroList(BITMAP* buffer)
    {
        for (list<Burbuja*>::iterator it=burbujeroList.begin(); it!=burbujeroList.end(); ++it)
        {
            Burbuja* bur = *it;
            bur->dibujar(buffer);
        }
    }

    void redirigirMovible(Movible* pMovible, Punto p) override
    {
        /*Se encarga class Burbuja*/
    }

    bool hayImpacto(Movible* pMovible) override
    {
        bool result = false;
        if(typeid(Bola)==typeid(*pMovible))
        {
            for (list<Burbuja*>::iterator it=burbujeroList.begin(); it != burbujeroList.end(); ++it)
            {
                Burbuja* bur = *it;
                if(bur->hayImpacto(pMovible))
                {
                    bur->reciboImpacto(pMovible);
                    result=true;
                }
            }
        }
        return result;
    }

    void reciboImpacto (Movible* pMovible) override
    {
        for (list<Burbuja*>::iterator it=burbujeroList.begin(); it != burbujeroList.end(); ++it)
        {
            if((*it)->getResistencia() == 0)
            {

                if((*it)->getMeImpactoSubmarinoOBola())
                {
                    burbujasDestruidasEnRonda++;
                    burbujasDestruidasParaVida++;
                }
                burbujeroList.erase(it);
            }
        }

    }

    void eliminarBurbuja(Burbuja* bur)
    {
        burbujeroList.remove(bur);
    }

};

Burbujero* Burbujero::unicaInstancia = 0;
Burbujero* Burbujero::getInstancia()
{
    if(unicaInstancia == 0)
        unicaInstancia = new Burbujero();
    return unicaInstancia;
}

class Submarino : public Movible, public Reposable, public PiezaRectangular, public IEsImpactable
{
public:
    static const int largo = 120;
    static const int alto = 29;

protected:
    int vidas;
    static Submarino* unicaInstancia;
    ColorDTO* colorPrimero;
    map<int, BITMAP*> secuenciaDestruccionDerecha;
    map<int, BITMAP*> secuenciaDestruccionIzquierda;
    BITMAP* imagenSubmarinoDerecha;
    BITMAP* imagenSubmarinoIzquierda;
    BITMAP* imagenSubmarino;
    Submarino(int pcX, int pcY): Movible(1,Derecha,Abajo), Reposable(pcX, pcY), PiezaRectangular(pcX,pcY,pcX+largo,pcY+alto)
    {
        try
        {
            activarAllegro();
            colorPrimero = new ColorDTO(248,254,121);
            setColorPieza(colorPrimero->getR(), colorPrimero->getG(), colorPrimero->getB()); //amarillito

            this->vidas = 3;


            //ColorDTO* naranjita = new ColorDTO(250, 209, 3);
            //ColorDTO* fanta = new ColorDTO(250, 67, 3);
            //ColorDTO* rojo = new ColorDTO(255, 0, 0);

//        this->secuenciaDestruccion.insert(pair<int, ColorDTO*>(0,naranjita));
//        this->secuenciaDestruccion.insert(pair<int, ColorDTO*>(1,fanta));
//        this->secuenciaDestruccion.insert(pair<int, ColorDTO*>(2,rojo));

            imagenSubmarinoDerecha = load_bitmap("img/submarinoDerecha.bmp",NULL);
            imagenSubmarinoIzquierda = load_bitmap("img/submarinoIzquierda.bmp",NULL);
            if(rand()%2 == 0 )
            {
                imagenSubmarino = imagenSubmarinoDerecha;
                dirX = Derecha;
            }
            else
            {
                imagenSubmarino = imagenSubmarinoIzquierda;
                dirX = Izquierda;
            }


            this->secuenciaDestruccionDerecha.insert(pair<int, BITMAP*>(0,load_bitmap("img/submarinoDerechaMuerte1.bmp",NULL)));
            this->secuenciaDestruccionDerecha.insert(pair<int, BITMAP*>(1,load_bitmap("img/submarinoDerechaMuerte2.bmp",NULL)));
            this->secuenciaDestruccionDerecha.insert(pair<int, BITMAP*>(2,load_bitmap("img/submarinoDerechaMuerte3.bmp",NULL)));
            this->secuenciaDestruccionIzquierda.insert(pair<int, BITMAP*>(0,load_bitmap("img/submarinoIzquierdaMuerte1.bmp",NULL)));
            this->secuenciaDestruccionIzquierda.insert(pair<int, BITMAP*>(1,load_bitmap("img/submarinoIzquierdaMuerte2.bmp",NULL)));
            this->secuenciaDestruccionIzquierda.insert(pair<int, BITMAP*>(2,load_bitmap("img/submarinoIzquierdaMuerte3.bmp",NULL)));

        }
        catch(AllegroNoFuncionaException& e)
        {
            cout << e.what() << endl;
        }
        catch(ImagenArchivoNoEncontradoException& e)
        {
            cout << e.what() << endl;
        }
        catch(exception& e)
        {
            cout << "Standard exception: " << e.what() << endl;
        }
    }


public:
    virtual ~Submarino()
    {
        secuenciaDestruccionDerecha.clear();
        secuenciaDestruccionIzquierda.clear();

        delete imagenSubmarinoDerecha;
        delete imagenSubmarinoIzquierda;
        delete imagenSubmarino;
        delete colorPrimero;
        delete unicaInstancia;
    }
    static Submarino* getInstancia();
    Submarino(Submarino &otroSubmarino) = delete;
    void operator = (const Submarino &) = delete;

    Punto* getPuntoOpuesto()
    {
        return puntoOpuesto;
    }
    void setPuntoOpuesto(int poX, int poY) override
    {
        puntoOpuesto = new Punto(poX,poY);
    }
    void setVidas(int valor)
    {
        vidas = valor;
    }
    int getVidas() const
    {
        return vidas;
    }
    map<int, BITMAP*> getSecuenciaDestruccion() const
    {
        map<int, BITMAP*> secuenciaDestruccion;
        if(dirX == Derecha)
            secuenciaDestruccion = secuenciaDestruccionDerecha;
        else
            secuenciaDestruccion = secuenciaDestruccionIzquierda;

        return secuenciaDestruccion;
    }

    void mover()

    {
        if(key[KEY_RIGHT])
        {
            puntoCritico->setX(puntoCritico->getX() + getVelocidad());
            puntoOpuesto->setX(puntoOpuesto->getX() + getVelocidad());
            dirX = Derecha;
        }
        if(key[KEY_DOWN])
        {
            puntoCritico->setY(puntoCritico->getY() + getVelocidad());
            puntoOpuesto->setY(puntoOpuesto->getY() + getVelocidad());
        }
        if(key[KEY_UP])
        {
            puntoCritico->setY(puntoCritico->getY() - getVelocidad());
            puntoOpuesto->setY(puntoOpuesto->getY() - getVelocidad());
        }
        if(key[KEY_LEFT])
        {
            puntoCritico->setX(puntoCritico->getX() - getVelocidad());
            puntoOpuesto->setX(puntoOpuesto->getX() - getVelocidad());
            dirX = Izquierda;
        }
    }

    void reposar()
    {
        const PuntoConstante* pReposo = getPuntoReposo();
        setPuntoCritico(pReposo->getX(),pReposo->getY());
        setPuntoOpuesto(pReposo->getX()+ largo, pReposo->getY() + alto);
        setMeMuevo(false);
    }

    void dibujarDestruccion(unsigned int secuencia, BITMAP* buffer)
    {
        if(secuencia>=0 && secuencia <getSecuenciaDestruccion().size())
        {
            //dibujar(buffer);
            draw_sprite(buffer,getSecuenciaDestruccion()[secuencia],this->getPuntoCritico()->getX(),this->getPuntoCritico()->getY()-10); /**El -10 es una corrección de la imagen sobre la pieza rectangular*/

        }
        if(dirX == Derecha)
            imagenSubmarino = imagenSubmarinoDerecha;
        else
            imagenSubmarino = imagenSubmarinoIzquierda;
    }

    bool hayImpacto(Movible* pMovible) override
    {
        bool result = false;
        if(typeid(Bola)==typeid(*pMovible))
        {
            Bola* soyBola = static_cast<Bola*>(pMovible);
            int radio = soyBola->getRadio();
            int izqBola = soyBola->getPuntoCritico()->getX() - radio;
            int derBola = soyBola->getPuntoCritico()->getX() + radio;
            int izqSub = this->puntoCritico->getX();
            int derSub = this->puntoOpuesto->getX();
            int abajoBola = soyBola->getPuntoCritico()->getY() + radio;
            int arribaBola = soyBola->getPuntoCritico()->getY() - radio;
            int abajoSub = this->getPuntoOpuesto()->getY();
            int arribaSub = this->getPuntoCritico()->getY();

            if(((izqBola <= derSub) && (izqBola >= izqSub)) || ((derBola >= izqSub) && (derBola <= derSub)))
            {
                if(((abajoBola >= arribaSub) && (abajoBola <= abajoSub)) || ((arribaBola <= abajoSub) && (arribaBola >= arribaSub)))
                {
                    result=true;
                    Punto* puntoContacto = Pieza::getPuntoSiHaySuperposicion(soyBola);
                    if( puntoContacto != 0)
                    {
                        redirigirMovible(pMovible,*puntoContacto);
                    }
                    else
                    {
                        if(key[KEY_DOWN])
                        {
                            redirigirMovible(pMovible,Punto(izqSub+2,abajoSub)); /** El 2 es para evitar las esquinas*/
                        }
                        else if(key[KEY_UP])
                        {
                            redirigirMovible(pMovible,Punto(izqSub+2,arribaSub));
                        }
                        else if(key[KEY_LEFT])
                        {
                            redirigirMovible(pMovible,Punto(izqSub,arribaSub-2));
                        }
                        else if(key[KEY_RIGHT])
                        {
                            redirigirMovible(pMovible,Punto(derSub,arribaSub-2));
                        }

                    }
                    soyBola->setReboteSub(result);
                    delete puntoContacto;
                }
            }

        }
        if(typeid(Burbuja)==typeid(*pMovible))
        {
            Burbuja* soyBur = static_cast<Burbuja*>(pMovible);
            int burIzq = soyBur->getPuntoCritico()->getX() - soyBur->getRadio();
            int burDer = soyBur->getPuntoCritico()->getX() + soyBur->getRadio();
            int burSup = soyBur->getPuntoCritico()->getY() - soyBur->getRadio();
            int burInf = soyBur->getPuntoCritico()->getY() + soyBur->getRadio();

            int subIzq = this->getPuntoCritico()->getX();
            int subDer = this->getPuntoOpuesto()->getX();
            int subSup = this->getPuntoCritico()->getY();
            int subInf = this->getPuntoOpuesto()->getY();

            if(((burIzq <= subDer) && (burDer >= subIzq)) || ((burDer >= subIzq) && (burDer <= subDer)))
            {
                if(((burInf >= subSup) && (burInf <= subInf)) || ((burSup <= subInf) && (burSup >= subSup)))
                {
                    Punto* puntoContacto = Pieza::getPuntoSiHaySuperposicion(soyBur);
                    if( puntoContacto != 0)
                    {
                        result=true;
                        soyBur->setMeImpactoSubmarinoOBola(true);
                        soyBur->setResistencia(0);
                    }
                }
            }

        }
        return result;
    }

    void reciboImpacto(Movible* pMovible) override
    {
        //
    }

    void redirigirMovible(Movible* pMovible, Punto p) override
    {
        if(typeid(Bola)==typeid(*pMovible))
        {

            int punta = 20;

            if((esVerticeInferiorIzquierdo(p) || esLadoHorizontalInferior(p)) && p.getX()<= (puntoCritico->getX()+ punta) )
            {
                pMovible->setDirX(pMovible->Izquierda);
                pMovible->setDirY(pMovible->Abajo);
            }
            else if((esVerticeInferiorDerecho(p) || esLadoHorizontalInferior(p)) && p.getX()>= (puntoOpuesto->getX()- punta))
            {
                pMovible->setDirX(pMovible->Derecha);
                pMovible->setDirY(pMovible->Abajo);
            }
            else if (esLadoHorizontalInferior(p))
                pMovible->setDirY(pMovible->Abajo);
            else if (esLadoVertical(p))
            {
                pMovible->invertirDirX();
                pMovible->setDirY(pMovible->Abajo);
            }
            else if ((esVerticeSuperiorIzquierdo(p) || esLadoHorizontalSuperior(p)) && p.getX()<= (puntoCritico->getX()+ punta) )
            {
                pMovible->setDirX(pMovible->Izquierda);
                pMovible->setDirY(pMovible->Arriba);
            }
            else if((esVerticeSuperiorDerecho(p) || esLadoHorizontalSuperior(p)) && p.getX()>= (puntoOpuesto->getX()- punta))
            {
                pMovible->setDirX(pMovible->Derecha);
                pMovible->setDirY(pMovible->Arriba);
            }
            else if (esLadoHorizontalSuperior(p))
                pMovible->setDirY(pMovible->Arriba);
        }
    }
    void dibujar (BITMAP* buffer) override
    {
        //PiezaRectangular::dibujar(buffer);
        if(dirX == Derecha)
        {
            imagenSubmarino = imagenSubmarinoDerecha;
        }
        else
        {
            imagenSubmarino = imagenSubmarinoIzquierda;
        }
        draw_sprite(buffer,imagenSubmarino,this->getPuntoCritico()->getX(),this->getPuntoCritico()->getY()-10); /**El -10 es una corrección de la imagen sobre la pieza rectangular*/
    }

};
Submarino* Submarino::unicaInstancia = 0;
Submarino* Submarino::getInstancia()
{
    if(unicaInstancia == 0)
    {
        unicaInstancia = new Submarino(243,20);
    }
    return unicaInstancia;
}


class DadorDeVidas : public Movible, public PiezaRectangular, public IEsImpactable
{
protected:
    static const int largo = 27;
    static const int alto = 20;
    static const int duracionEnSegundos = 5;
    ColorDTO* colorPrimero;
    time_t tempInicial;
    BITMAP* imagenVida;

public:

    DadorDeVidas(int pcX, int pcY): Movible(1,Derecha,Abajo), PiezaRectangular(pcX,pcY,pcX+largo,pcY+alto)
    {
        try
        {
            activarAllegro();
            colorPrimero = new ColorDTO(0, 0, 0 );
            setColorPieza(colorPrimero->getR(), colorPrimero->getG(), colorPrimero->getB());
            time(&tempInicial);

            imagenVida = load_bitmap("img/vida.bmp",NULL);

        }
        catch(AllegroNoFuncionaException& e)
        {
            cout << e.what() << endl;
        }
        catch(ImagenArchivoNoEncontradoException& e)
        {
            cout << e.what() << endl;
        }
        catch(exception& e)
        {
            cout << "Standard exception: " << e.what() << endl;
        }

    }

    virtual ~DadorDeVidas()
    {
        delete colorPrimero;
        delete imagenVida;
    }
    static DadorDeVidas* getInstancia();
    DadorDeVidas(DadorDeVidas &otroDadorDeVidas) = delete;
    void operator = (const DadorDeVidas &) = delete;

    Punto* getPuntoOpuesto()
    {
        return puntoOpuesto;
    }

    void setPuntoOpuesto(int poX, int poY) override
    {
        puntoOpuesto = new Punto(poX,poY);
    }


    bool hayImpacto(Movible* pMovible) override
    {
        bool result = false;
        int vidaDer = getPuntoOpuesto()->getX();
        int vidaIzq = getPuntoCritico()->getX();
        int vidaSup = getPuntoCritico()->getY();
        int vidaInf = getPuntoOpuesto()->getY();

        if(typeid(Submarino)==typeid(*pMovible))
        {
            Submarino* soySubmarino = static_cast<Submarino*>(pMovible);
            int derSub = soySubmarino->getPuntoOpuesto()->getX();
            int izqSub = soySubmarino->getPuntoCritico()->getX();
            int supSub = soySubmarino->getPuntoCritico()->getY();
            int infSub = soySubmarino->getPuntoOpuesto()->getY();

        if((vidaIzq <= derSub && vidaDer >= izqSub) || (izqSub <= vidaDer && derSub >= vidaIzq))
         {
         if((vidaSup <= infSub && vidaInf >= supSub) || (supSub <= vidaInf && infSub >= vidaSup))
              {
                   result = true;
               }
          }
        }
        return result;
    }

    void reciboImpacto(Movible* pMovible) override
    {
        //
    }

    void redirigirMovible(Movible* pMovible, Punto p) override
    {
//        if(typeid(Bola)==typeid(*pMovible))
//        {
//
//            if((esVerticeInferiorIzquierdo(p) || esLadoHorizontalInferior(p)) && p.getX()<= (puntoCritico->getX()+20) )
//            {
//                pMovible->setDirX(pMovible->Izquierda);
//                pMovible->setDirY(pMovible->Abajo);
//            }
//            else if((esVerticeInferiorDerecho(p) || esLadoHorizontalInferior(p)) && p.getX()>= (puntoOpuesto->getX()-20))
//            {
//                pMovible->setDirX(pMovible->Derecha);
//                pMovible->setDirY(pMovible->Abajo);
//            }
//            else if (esLadoHorizontalInferior(p))
//                pMovible->setDirY(pMovible->Abajo);
//            else if (esLadoVertical(p))
//            {
//                pMovible->invertirDirX();
//                pMovible->setDirY(pMovible->Abajo);
//            }
//            else if ((esVerticeSuperiorIzquierdo(p) || esLadoHorizontalSuperior(p)) && p.getX()<= (puntoCritico->getX()+20) )
//            {
//                pMovible->setDirX(pMovible->Izquierda);
//                pMovible->setDirY(pMovible->Arriba);
//            }
//            else if((esVerticeSuperiorDerecho(p) || esLadoHorizontalSuperior(p)) && p.getX()>= (puntoOpuesto->getX()-20))
//            {
//                pMovible->setDirX(pMovible->Derecha);
//                pMovible->setDirY(pMovible->Arriba);
//            }
//            else if (esLadoHorizontalSuperior(p))
//                pMovible->setDirY(pMovible->Arriba);
//        }
    }
    void mover() override
    {
        int puntoCriticoX = puntoCritico->getX();
        int puntoOpuestoX = puntoOpuesto->getX();
        setPuntoCritico(puntoCriticoX + (1 * dirX),puntoCritico->getY());
        setPuntoOpuesto(puntoOpuestoX + (1 * dirX),puntoOpuesto->getY());
        setMeMuevo(true);
    }

    bool siPasoTiempoDeVidaDelDador ()
    {
        time_t ahora = time(NULL);
        bool result = false;
        if(difftime(ahora,tempInicial)>= duracionEnSegundos)
        {

            result = true;
        }
        return result;
    }

    void dibujar (BITMAP* buffer) override
    {
        //PiezaRectangular::dibujar(buffer);
        draw_sprite(buffer,imagenVida,this->getPuntoCritico()->getX()-6,this->getPuntoCritico()->getY());
        //PiezaRectangular::dibujar(buffer);
    }

};




class ContenedorDeDadoresDeVida : public IEsImpactable
{
protected:
    list<DadorDeVidas*> dadorDeVidasList;
    static ContenedorDeDadoresDeVida* unicaInstancia;
    int burbujasADestruirParaGenerarDadorDeVida;
    bool ganeUnaVida;

    ContenedorDeDadoresDeVida()
    {
        burbujasADestruirParaGenerarDadorDeVida = 10 ;
        ganeUnaVida = false;
    }
public:
    virtual ~ContenedorDeDadoresDeVida()
    {
        dadorDeVidasList.clear();
        delete unicaInstancia;
    }
    static ContenedorDeDadoresDeVida* getInstancia();
    ContenedorDeDadoresDeVida(ContenedorDeDadoresDeVida &otroContenedorDeDadoresDeVida) = delete;
    void operator = (const ContenedorDeDadoresDeVida&) = delete;

    list<DadorDeVidas*>* getDadorDeVidasList()
    {
        return &dadorDeVidasList;
    }

    bool getGaneUnaVida (){
        return ganeUnaVida;
    }

    void setGaneUnaVida(bool valor){
        ganeUnaVida = valor;
    }
    void limpiarDadorDeVidasList()
    {
        dadorDeVidasList.clear();
    }

    bool siCrearDadorDeVida(int burbujasDestruidas)
    {
        bool result = false;
        int limiteIzquierdoRing = 5 + 15; /* sumo un margen de 15 */
        int limiteDerechoRing = 601 - 40 - 15; /* resto 40, que es el largo de la vida 40 y resto un margen de 15*/
        int limiteSuperiorRing = 10 + 29 + 15 ; /* resto 29 que es el alto del submarino para que no se superpongan al crearse además sumo una distancia de 15 más*/
        int limiteInferiorRing = ((717-10)/2) -50 - 15 ;  /* la distancia hasta la que puede bajar el submarino menos la altura de la vida.*/
        int puntoCriticoX;
        int puntoCriticoY;
        DadorDeVidas* nuevoDadorDeVidas;
        if(burbujasDestruidas >= burbujasADestruirParaGenerarDadorDeVida)
        {
            if(dadorDeVidasList.size() < 1)
            {
                puntoCriticoX =(rand()% (limiteDerechoRing)) + limiteIzquierdoRing;
                puntoCriticoY = (rand()% (limiteInferiorRing)) + limiteSuperiorRing;
                nuevoDadorDeVidas = new DadorDeVidas(puntoCriticoX,puntoCriticoY);
                if(rand()%2 == 0 )
                    nuevoDadorDeVidas -> invertirDirX(); /* para que aleatoriamente inicie moviendo hacia la izquierda o la derecha*/
                dadorDeVidasList.push_back(nuevoDadorDeVidas);
                result = true;
            }

        }
        return result;
    }

    void dibujarContenedorDeDadoresDeVidaList(BITMAP* buffer)
    {

        for (list<DadorDeVidas*>::iterator it=dadorDeVidasList.begin(); it!=dadorDeVidasList.end(); ++it)
        {
            DadorDeVidas* dadVidas = *it;
            dadVidas->dibujar(buffer);
        }
    }

    void redirigirMovible(Movible* pMovible, Punto p) override
    {
        /*No tiene efecto*/
    }

    bool hayImpacto(Movible* pMovible) override
    {
        bool result = false;
        if(typeid(Submarino)==typeid(*pMovible))
        {
            for (list<DadorDeVidas*>::iterator it=dadorDeVidasList.begin(); it != dadorDeVidasList.end(); ++it)
            {
                DadorDeVidas* dadVida = *it;
                if(dadVida->hayImpacto(pMovible))
                {
                    dadVida->reciboImpacto(pMovible);
                    result=true;
                }
            }
        }
        return result;
    }

    void reciboImpacto (Movible* pMovible) override
    {
        for (list<DadorDeVidas*>::iterator it=dadorDeVidasList.begin(); it != dadorDeVidasList.end(); ++it)
        {
            dadorDeVidasList.erase(it);
        }

    }

    void eliminarDadorDeVida(DadorDeVidas* dadVidas)
    {
        dadorDeVidasList.remove(dadVidas);
    }

    void limpiarContenedorDeDadoresDeVida()
    {
        dadorDeVidasList.clear();
    }
    bool analizoTiempoDeVidaContenedorDeDadores()
    {
        bool result = false;
        for (list<DadorDeVidas*>::iterator it=dadorDeVidasList.begin(); it!=dadorDeVidasList.end(); ++it)
        {
            DadorDeVidas* dadVidas = *it;
            if(dadVidas -> siPasoTiempoDeVidaDelDador ())
            {
                eliminarDadorDeVida(dadVidas);
                result = true;
            }
        }
        return result;
    }

};

ContenedorDeDadoresDeVida* ContenedorDeDadoresDeVida::unicaInstancia = 0;
ContenedorDeDadoresDeVida* ContenedorDeDadoresDeVida::getInstancia()
{
    if(unicaInstancia == 0)
        unicaInstancia = new ContenedorDeDadoresDeVida();
    return unicaInstancia;
}


void Bola::desplazarJuntoConSubmarino(Submarino* submarino)
{
    int puntoDesplazamientoX = submarino->getPuntoCritico()->getX() + ((submarino->getPuntoOpuesto()->getX() - submarino->getPuntoCritico()->getX())/2 );
    int puntoDesplazamientoY = submarino->getPuntoOpuesto()->getY()+this -> getRadio() +1;
    setPuntoCritico(puntoDesplazamientoX, puntoDesplazamientoY);
}



class Ring : public PiezaRectangular, public IEsImpactable
{
protected:
    int nivel;
    map<string,BITMAP*> misFondos;
    BITMAP* gameover;
    static const int grosor = 6;
    const Punto* pcPrimero;
    const Punto* poPrimero;
    int contadorImpactosBola; //data inútil? Es para hacer algo con reciboImpacto
    int contadorImpactosSubmarino; //data inútil?
    static Ring* unicaInstancia;
    Ring(): PiezaRectangular(5,10,601,717), pcPrimero(new Punto(5,10)), poPrimero(new Punto(601,717))
    {

        try
        {
            activarAllegro();

            this->misFondos.insert(pair<string,BITMAP*>("1",load_bitmap("img/fondo1.bmp",NULL)));
            this->misFondos.insert(pair<string,BITMAP*>("2",load_bitmap("img/fondo2.bmp",NULL)));
            this->misFondos.insert(pair<string,BITMAP*>("3",load_bitmap("img/fondo3.bmp",NULL)));
            this->misFondos.insert(pair<string,BITMAP*>("4",load_bitmap("img/fondo4.bmp",NULL)));
            this->misFondos.insert(pair<string,BITMAP*>("5",load_bitmap("img/fondo5.bmp",NULL)));

            for (map<string,BITMAP*>::iterator it=misFondos.begin(); it!=misFondos.end(); ++it)
            {
                BITMAP* fondo = it->second;
                if(!fondo)
                    throw new ImagenArchivoNoEncontradoException("Falta archivo de fondo");
            }

            this->gameover = load_bitmap("img/gameover.bmp",NULL);
            if(!gameover)
                throw new ImagenArchivoNoEncontradoException("Falta archivo de GameOver");

            setColorPieza(255,255,255);

            this->contadorImpactosBola = 0;
            this->contadorImpactosSubmarino = 0;
            this->nivel = 1;
            //Ring no puede realizar this->puntosEnContorno = this->puntosEnContornoPieza();
            // porque cuando se dibuja, su PuntoCritico y PuntoOpuesto cambian.
        }
        catch(AllegroNoFuncionaException& e)
        {
            cout << e.what() << endl;
        }
        catch(ImagenArchivoNoEncontradoException& e)
        {
            cout << e.what() << endl;
        }
        catch(exception& e)
        {
            cout << "Standard exception: " << e.what() << endl;
        }


    }
private:
    void validarFondos(string index)
    {
        map<string,BITMAP*>::iterator it;
        it = misFondos.find(index);
        if(it == misFondos.end())
            throw ImagenNoEncontradaException(index);
    }

    void dibujarFondo(string index, BITMAP* buffer)
    {
        try
        {
            validarFondos(index);
            draw_sprite(buffer,misFondos[index],11,16);
        }
        catch(exception& e)
        {
            cout << e.what() << endl;
        }
    }

    void DibujarFondoPorNivel(int nivel, BITMAP* buffer)
    {
        try
        {
            switch (nivel)
            {
            case 1:
                dibujarFondo("1",buffer);
                break;
            case 2:
                dibujarFondo("2",buffer);
                break;
            case 3:
                dibujarFondo("3",buffer);
                break;
            case 4:
                dibujarFondo("4",buffer);
                break;
            case 5:
                dibujarFondo("5",buffer);
                break;
            default:
                int valor = nivel%5;
                DibujarFondoPorNivel(valor == 0? 5 : valor, buffer); /** Recursividad **/
                break;
            }
        }
        catch(exception& e)
        {
            cout << e.what() << endl;
        }
    }

    void redirigirMovible(Movible* pMovible, Punto p = Punto(0,0)) override
    {
        if(typeid(Bola)==typeid(*pMovible))
        {
            if (p == Punto(0,0))
                throw new CustomException("ring->redirigirMovible no admite Punto(0,0) cuando es Bola.");

            Bola* soyBola = static_cast<Bola*>(pMovible);
            if(esVerticeSuperior(p) || esLadoHorizontalSuperior(p))
                soyBola->setBolaPerdida(true);
            else if (esVerticeInferiorIzquierdo(p))
            {
                soyBola->setDirY(soyBola->Arriba);
                soyBola->setDirX(soyBola->Derecha);
            }
            else if (esVerticeInferiorDerecho(p))
            {
                soyBola->setDirY(soyBola->Arriba);
                soyBola->setDirX(soyBola->Izquierda);
            }
            else if (esLadoHorizontalInferior(p))
                soyBola->setDirY(soyBola->Arriba);
            else if (esLadoVerticalIzquierdo(p))
                soyBola->setDirX(soyBola->Derecha);
            else if (esLadoVerticalDerecho(p))
                soyBola->setDirX(soyBola->Izquierda);
        }

        if(typeid(Burbuja)==typeid(*pMovible))
        {
            Burbuja* soyBur = static_cast<Burbuja*>(pMovible);

            if(esLadoVerticalDerecho(soyBur->getPuntoCritico()->getX()+soyBur->getRadio()))
                soyBur->setDirX(soyBur->Izquierda);
            else if (esLadoVerticalIzquierdo(soyBur->getPuntoCritico()->getX()-soyBur->getRadio()))
                soyBur->setDirX(soyBur->Derecha);
        }

        if(typeid(Submarino)==typeid(*pMovible))
        {
            Submarino* soySubmarino = static_cast<Submarino*>(pMovible);
            int ladoDer = this->getPuntoOpuesto()->getX();
            int ladoIzq = this->getPuntoCritico()->getX();
            int ladoSup = this->getPuntoCritico()->getY();
            int ladoInf = (717-10)/2;


//            cout << "lado derX   "  << this->getPuntoOpuesto()->getX() << "\n";
//            cout << "lado izqx   "  << this->getPuntoCritico()->getX() << "\n";
//            cout << "lado supy   "  << this->getPuntoCritico()->getY() << "\n";
//            cout << "lado infy   "  << this->getPuntoOpuesto()->getY() << "\n";

            if(esLadoVerticalDerecho(soySubmarino->getPuntoOpuesto()->getX()))
            {
                soySubmarino->getPuntoOpuesto()->setX(ladoDer - 1);
                soySubmarino->getPuntoCritico()->setX(ladoDer - 1 - Submarino::largo);
            }
            else if(esLadoVerticalIzquierdo(soySubmarino->getPuntoCritico()->getX()))
            {

                soySubmarino->getPuntoCritico()->setX(ladoIzq + 1);
                soySubmarino->getPuntoOpuesto()->setX(ladoIzq + 1 + Submarino::largo);

            }
            else if(esLadoHorizontalSuperior(soySubmarino->getPuntoCritico()->getY()))
            {
                soySubmarino->getPuntoCritico()->setY(ladoSup + 1);
                soySubmarino->getPuntoOpuesto()->setY(ladoSup + 1 + Submarino::alto);
            }
            else if(esLimiteHorizontalIntefior(soySubmarino->getPuntoOpuesto()->getY()))
            {
                soySubmarino->getPuntoCritico()->setY(ladoInf - 1- Submarino::alto);
                soySubmarino->getPuntoOpuesto()->setY(ladoInf - 1);
            }
        }
        if(typeid(DadorDeVidas)==typeid(*pMovible))
        {
            DadorDeVidas* soyDadorDeVidas = static_cast<DadorDeVidas*>(pMovible);

            if(esLadoVerticalDerecho(soyDadorDeVidas->getPuntoOpuesto()->getX()))
            {
                soyDadorDeVidas->setDirX(soyDadorDeVidas->Izquierda);
            }
            else if(esLadoVerticalIzquierdo(soyDadorDeVidas->getPuntoCritico()->getX()))
            {

                soyDadorDeVidas->setDirX(soyDadorDeVidas->Derecha);

            }

        }


    }

    bool esLimiteHorizontalIntefior(int y)
    {
        int ladoInf = (717-10)/2;
        return y >= ladoInf;
    }

public:
    virtual ~Ring()
    {
        misFondos.clear();
        delete gameover;
        delete pcPrimero;
        delete poPrimero;
        delete unicaInstancia;
    }
    static Ring *getInstancia();
    Ring(Ring &otroRing) = delete;
    void operator = (const Ring &) = delete;
    Punto* getPuntoOpuesto()
    {
        return puntoOpuesto;
    }
    void setPuntoOpuesto (int poX, int poY) override
    {
        puntoOpuesto = new Punto(poX,poY);
    }
    const Punto* getPcPrimero() const
    {
        return pcPrimero;
    }
    const Punto* getPoPrimero() const
    {
        return poPrimero;
    }
    int getNivel() const
    {
        return nivel;
    }
    void setNivel(int valor)
    {
        nivel=valor;
    }

    void dibujar (BITMAP* buffer) override
    {
        DibujarFondoPorNivel(nivel, buffer);

        getPuntoCritico()->setX(getPcPrimero()->getX());
        getPuntoCritico()->setY(getPcPrimero()->getY());
        getPuntoOpuesto()->setX(getPoPrimero()->getX());
        getPuntoOpuesto()->setY(getPoPrimero()->getY());
        ColorDTO* color = getColorPieza();

        for (int i=0; i<grosor; i++)
        {
            rect(buffer,getPuntoCritico()->getX(),getPuntoCritico()->getY(),getPuntoOpuesto()->getX(),getPuntoOpuesto()->getY(),makecol(color->getR(),color->getG(),color->getB()));
            getPuntoCritico()->setX(getPuntoCritico()->getX()+1);
            getPuntoCritico()->setY(getPuntoCritico()->getY()+1);
            getPuntoOpuesto()->setX(getPuntoOpuesto()->getX()-1);
            getPuntoOpuesto()->setY(getPuntoOpuesto()->getY()-1);
        }
    }

    void dibujarGameOver(BITMAP* buffer)
    {
        draw_sprite(buffer, gameover,150,300);
    }

    bool hayImpacto(Movible* pMovible) override
    {
        bool result = false;

        int ladoDer = getPuntoOpuesto()->getX();
        int ladoIzq = getPuntoCritico()->getX();
        int ladoSup = getPuntoCritico()->getY();
        int ladoInf = getPuntoOpuesto()->getY();

        if(typeid(Bola)==typeid(*pMovible))
        {
            Bola* soyBola = static_cast<Bola*>(pMovible);
            Punto* puntoContacto = Pieza::getPuntoSiHaySuperposicion(soyBola);
            if( puntoContacto != 0)
            {
                result=true;
                redirigirMovible(pMovible,*puntoContacto);
                soyBola->setReboteRingLad(result);
            }
            delete puntoContacto;
        }
        else if(typeid(Burbuja)==typeid(*pMovible))
        {
            Burbuja* soyBur = static_cast<Burbuja*>(pMovible);
            int radioBur = soyBur->getRadio();
            int burIzq = soyBur->getPuntoCritico()->getX()-radioBur;
            int burDer = soyBur->getPuntoCritico()->getX()+radioBur;
            int burSup = soyBur->getPuntoCritico()->getY() - radioBur;

            if(burDer >= ladoDer || burIzq <= ladoIzq || burSup <= ladoSup)
            {
                result = true;
                if (burDer >= ladoDer || burIzq <= ladoIzq)
                    redirigirMovible(soyBur);
                else
                    soyBur->setResistencia(0);
            }
        }
        else if(typeid(Submarino)==typeid(*pMovible))
        {

            Submarino* soySubmarino = static_cast<Submarino*>(pMovible);
            //int supSub = soySubmarino->getPuntoCritico()->getY(); //Por ahora no me meto con la posibilidad de arriba abajo del submarino
            //int infSub = soySubmarino->getPuntoOpuesto()->getY();
            int derSub = soySubmarino->getPuntoOpuesto()->getX();
            int izqSub = soySubmarino->getPuntoCritico()->getX();
            int supSub = soySubmarino->getPuntoCritico()->getY();
            int infSub = soySubmarino->getPuntoOpuesto()->getY();

            if(derSub>= ladoDer || izqSub <= ladoIzq || supSub >= ladoSup || infSub <= ladoInf )
            {
                redirigirMovible(soySubmarino);
                result = true;
            }
        }
        else if(typeid(DadorDeVidas)==typeid(*pMovible))
        {
            DadorDeVidas* soyDadorDeVidas = static_cast<DadorDeVidas*>(pMovible);

            int puntoCriticoX = soyDadorDeVidas->getPuntoCritico()->getX();
            int puntoOpuestoX = soyDadorDeVidas->getPuntoOpuesto()->getX();

            if(puntoOpuestoX>= ladoDer || puntoCriticoX  <= ladoIzq)
            {
                redirigirMovible(soyDadorDeVidas);
                result = true;
            }
        }
        return result;
    }

    void reciboImpacto(Movible* pMovible) override
    {
        if(typeid(Bola)==typeid(*pMovible))
            ++contadorImpactosBola;

        if(typeid(Submarino)==typeid(*pMovible))
            ++contadorImpactosSubmarino;
    }

    void moverEnMi(Movible* pMovible)
    {
        pMovible->mover();
        if(hayImpacto(pMovible))
            reciboImpacto(pMovible);
    }

    void moverEnMi(ContenedorDeDadoresDeVida* pDadorDeVidas)
    {
        list<DadorDeVidas*>* listDadorDeVidas = pDadorDeVidas ->getDadorDeVidasList();
        for (list<DadorDeVidas*>::iterator it=listDadorDeVidas->begin(); it!=listDadorDeVidas->end(); ++it)
        {
            DadorDeVidas* dadVidas = *it;
            dadVidas->mover();

            if(hayImpacto(dadVidas))
                reciboImpacto(dadVidas);
        }
    }
};

Ring* Ring::unicaInstancia = 0;
Ring* Ring::getInstancia()
{
    if(unicaInstancia == 0)
    {
        unicaInstancia = new Ring();
    }
    return unicaInstancia;
}




class Cartelera : public IUsaAllegro
{
protected:
    Puntaje* puntaje;
    FONT* arialB;
    FONT* arial20;
    BITMAP* logo;
    BITMAP* panel;
    static Cartelera* unicaInstancia;
    void activarAllegro() override
    {
        AllegroEjecuta::activarAllegro();
    }
    Cartelera(string archivoHighScore)
    {
        try
        {
            activarAllegro();
            DATAFILE* datafile = load_datafile("recursos.dat");
            if(!datafile)
                throw new FunteArchivoNoEncontradoException("Falta archivo recursos.dat");

            this->arial20 = (FONT*)datafile[0].dat;
            this->arialB = (FONT*)datafile[1].dat;
            this->logo = load_bitmap("img/logo.bmp",NULL);
            if(!logo)
                throw new ImagenArchivoNoEncontradoException("Falta archivo logo.bmp");
            this->panel = load_bitmap("img/panel.bmp",NULL);
            if(!panel)
                throw new ImagenArchivoNoEncontradoException("Falta archivo panel.bmp");
            this->puntaje = Puntaje::getInstancia(archivoHighScore);

            delete datafile;
        }
        catch(AllegroNoFuncionaException& e)
        {
            cout << e.what() << endl;
        }
        catch(FunteArchivoNoEncontradoException& e)
        {
            cout << e.what() << endl;
        }
        catch(ImagenArchivoNoEncontradoException& e)
        {
            cout << e.what() << endl;
        }
        catch(exception& e)
        {
            cout << "Standard exception: " << e.what() << endl;
        }
    }
public:
    virtual ~Cartelera()
    {
        delete puntaje;
        delete arialB;
        delete arial20;
        delete logo;
        delete panel;
        delete unicaInstancia;
    }
    static Cartelera *getInstancia();
    static Cartelera *getInstancia(string);
    Cartelera(Cartelera &otraCartelera) = delete;
    void operator = (const Cartelera &) = delete;
    Puntaje* getPuntaje()
    {
        return puntaje;
    }
    void dibujarCartelera(BITMAP* buffer, int vidas, int nivel)
    {
        draw_sprite(buffer, logo, 615,5); //x y Dibuja el logo
        draw_sprite(buffer, panel, 620,140); //x y Dibuja el panel

        textprintf_ex(panel, arialB, 130, 3, makecol(0,0,0), makecol(0,96,64),"            ");
        textprintf_ex(panel, arialB, 130, 3, makecol(255,255,0), makecol(0,96,64),"%d",nivel);

        textprintf_ex(panel, arialB, 160, 65, makecol(0,0,0), makecol(0,96,64),"            ");
        textprintf_ex(panel, arialB, 160, 65, makecol(255,255,0), makecol(0,96,64),"%d",puntaje->getPuntajeActual());

        textprintf_ex(panel, arialB, 130, 130, makecol(0,0,0), makecol(0,96,64),"            ");
        textprintf_ex(panel, arialB, 130, 130, makecol(255,255,0), makecol(0,96,64),"%d",vidas);

        textprintf_ex(buffer,arial20,700,100,makecol(255,255,255),makecol(0,96,64),"Puntaje Max:%u",puntaje->getHighScore());

        textprintf_ex(panel, arialB, 160, 65, makecol(0,0,0), makecol(0,96,64),"            ");
        textprintf_ex(panel, arialB, 160, 65, makecol(255,255,0), makecol(0,96,64),"%d",puntaje->getPuntajeActual());

    }

};

Cartelera* Cartelera::unicaInstancia = 0;
Cartelera* Cartelera::getInstancia()
{
    if(unicaInstancia == 0)
    {
        unicaInstancia = new Cartelera("high.dat");
    }
    return unicaInstancia;
}

Cartelera* Cartelera::getInstancia(string archivoHighScore)
{
    if(unicaInstancia == 0)
    {
        unicaInstancia = new Cartelera(archivoHighScore);
    }
    return unicaInstancia;
}

class Pantalla : public IUsaAllegro
{
protected:

    int ancho_;
    int alto_;
    Muro* muro;
    Bola* bola;
    Ring* ring;
    Submarino* submarino;
    Burbujero* burbujero;
    Cartelera* cartelera;
    ContenedorDeDadoresDeVida* contenedorDeDadoresDeVida;

    BITMAP* buffer;
    list<BITMAP*> secuenciasBuffer;

    static Pantalla* unicaInstancia;
    void activarAllegro() override
    {
        AllegroEjecuta::activarAllegro();
    }

    Pantalla(int ancho, int alto)
    {
        try
        {
            activarAllegro();
            this->ancho_ = ancho;
            this->alto_ = alto;
            set_color_depth(32);
            if (set_gfx_mode(GFX_AUTODETECT, ancho_, alto_, 0, 0) != 0) //Autodetecta el entorno gráfico, se dipone el tamaño de pantalla (ancho,alto) y se setea cuantos pixels extras ocultos queremos (0,0)
                throw AllegroNoFuncionaException("No se pudo setear la Resolucion de color a 32 bit. " + string(allegro_error));
            this->buffer = create_bitmap(ancho_,alto_);
            limpiarBuffer();
            /**IMPORTANTÍSIMO: Se debe ejecutar primero set_color_depth(32) y luego el set_gfx_mode antes que cualquier carga de BITMAP o instanciación de un objeto que use BITMAP **/

            this->muro = Muro::getInstancia();
            this->bola = new Bola();
            this->ring = Ring::getInstancia();
            this->cartelera = Cartelera::getInstancia();

            this->submarino = Submarino::getInstancia();
            this->burbujero = Burbujero::getInstancia();
            this->contenedorDeDadoresDeVida = ContenedorDeDadoresDeVida::getInstancia();

        }
        catch(AllegroNoFuncionaException& e)
        {
            cout << e.what() << endl;
        }
        catch(exception& e)
        {
            cout << "Standard exception: " << e.what() << endl;
        }
    }

public:
    virtual ~Pantalla()
    {
        delete muro;
        delete bola;
        delete ring;
        delete cartelera;
        delete submarino;
        delete burbujero;
        delete buffer;
        delete contenedorDeDadoresDeVida;
        secuenciasBuffer.clear();
        delete unicaInstancia;
    }

    static Pantalla *getInstancia(int ancho, int alto);
    Pantalla(Pantalla &otroJuego) = delete;
    void operator = (const Pantalla &) = delete;

    void dibujarRecuadro(Punto,Punto,int,int);


    FONT* getArial20();
    FONT* getArialB();

    void mostrarLadrillos();
    void setMuroActual(int fila, int columna, int valor)
    {
        muro->setMuroActual(fila,columna,valor);
    };
    Muro* getMuro()
    {
        return muro;
    }
    Burbujero* getBurbujero()
    {
        return burbujero;
    }
    Bola* getBola()
    {
        return bola;
    }
    Ring* getRing()
    {
        return ring;
    }
    Cartelera* getCartelera()
    {
        return cartelera;
    }
    Submarino* getSubmarino()
    {
        return submarino;
    }
    ContenedorDeDadoresDeVida* getContenedorDeDadoresDeVida()
    {
        return contenedorDeDadoresDeVida;
    }

    BITMAP* getBuffer()
    {
        return buffer;
    }
    void dibujar(Pieza* pieza)
    {
        pieza->dibujar(buffer);
    }
    void reposar(Reposable* reposable)
    {
        reposable->reposar();
    }
    void lanzarFotograma()
    {
        decorarBuffer();
        transmitirBufferOSecuencia();
    }
    void inaugurarNivel();
    void siSubmarinoNoLanza();

    void moverBurbujasEnPantalla(int rondas, bool puedoMover)
    {
        bool hayImpactoDestructivo = false;
        list<Burbuja*>* burList = burbujero->getBurbujeroList();
        //list<Burbuja*> burListCopy = *(burbujero->getBurbujeroList()); //desreferencio el puntero y coloco su valor en una variable que no está en memoria;
        for (list<Burbuja*>::iterator it=burList->begin(); it!=burList->end(); ++it)
        {

            Burbuja* bur = *it;

            if(rondas%3==0 && puedoMover)
            {
                bur->mover();
            }

            if(ring->hayImpacto(bur))
            {
                ring->reciboImpacto(bur);
                hayImpactoDestructivo = true; //En realidad solo si toca el techo hay impactoDestructivo
            }
            if(submarino->hayImpacto(bur))
            {
                submarino->reciboImpacto(bur);
                hayImpactoDestructivo = true;
            }
            if(bur->hayImpacto(bola))
            {
                bur->reciboImpacto(bola);
                hayImpactoDestructivo = true;
            }

            //En caso de querer que DadorDeVidas haga rebotar las burbujas, programar aquí



            /** Esto comentado sería parte del desarrollo para que las burbujas reboten entre sí.
            El efecto visual es más lindo si dejamos que se superpongan. En caso de habilitarlo, hay que programar en Ring::hayImpacto que el suelo explote las burbujas que reboten al fondo **/

//            for(list<Burbuja*>::iterator itc=burListCopy.begin(); itc!=burListCopy.end(); ++itc)
//            {
//                Burbuja* bur2 = *itc;
//                Punto p1 = *(bur->getPuntoCritico());
//                Punto p2 = *(bur2->getPuntoCritico());
//
////                 if((p1==p2))
////                    cout << "SOY LA MISMA: " << endl;
//
//                if(!(p1==p2))
//                {
//                    if(bur->hayImpacto(bur2))
//                    {
//                        bur->reciboImpacto(bur2);
//                    }
//                }
//
//            }
        }

        if(hayImpactoDestructivo)
            burbujero->reciboImpacto(bola);

    }

    void setDirXBolaQuieta()
    {
        Punto puntoInicial = *(ring->getPuntoCritico());
        Punto puntoFinal = Punto(ring->getPuntoOpuesto()->getX(),puntoInicial.getY());
        Punto centroSub = Punto(submarino->getPuntoCritico()->getX()+(Submarino::largo/2),puntoInicial.getY());

        double distanciaInicio = sqrt(pow((centroSub.getX() - puntoInicial.getX()),2) + pow((centroSub.getY() - puntoInicial.getY()),2));
        double distanciaFinal = sqrt(pow((centroSub.getX() - puntoFinal.getX()),2) + pow((centroSub.getY() - puntoFinal.getY()),2));
        if(distanciaInicio <= distanciaFinal)//el centro del sub más cercano a la pared izquierda del Ring o en el medio
            bola->setDirX(bola->Derecha);
        else
            bola->setDirX(bola->Izquierda);
    }

private:
    void limpiarBuffer()
    {
        clear_to_color(buffer, makecol(0, 96, 64));
        //clear_bitmap(buffer); //limpia el buffer y reimprime el color(negro)
    }
    void transmitirBuffer(BITMAP* unBuffer)
    {
        blit(unBuffer, screen, 0,0,0,0,ancho_,alto_);
    }
    void decorarBuffer();
    void transmitirBufferOSecuencia();

    BITMAP* crearNuevaCopiaBuffer()
    {
        BITMAP* aux = create_bitmap(ancho_,alto_);
        draw_sprite(aux, buffer, 0, 0);
        return aux;
    }

    void sobreescibirBuffer(BITMAP* bmp)
    {
        draw_sprite(buffer, bmp, 0, 0);
    }
};

Pantalla* Pantalla::unicaInstancia = 0;
Pantalla* Pantalla::getInstancia(int ancho, int alto)
{
    if(unicaInstancia == 0)
    {
        unicaInstancia = new Pantalla(ancho,alto);
    }
    return unicaInstancia;
}

void Pantalla::dibujarRecuadro(Punto vertice, Punto verticeOpuesto,int grosor, int color)
{
    try
    {
        for (int i=0; i<grosor; i++)
        {
            rect(buffer,vertice.getX(),vertice.getY(),verticeOpuesto.getX(),verticeOpuesto.getY(),color);
            vertice.setX(vertice.getX()+1);
            vertice.setY(vertice.getY()+1);
            verticeOpuesto.setX(verticeOpuesto.getX()-1);
            verticeOpuesto.setY(verticeOpuesto.getY()-1);
        }
    }
    catch(exception& e)
    {
        cout << "Standard exception en dibujarRecuadro: " << e.what() << endl;
    }


}


void Pantalla::decorarBuffer() /** Este es el método más importante, porque es el que plasma lo visual. Es un símil patrón Decorator (el decorator usa el objeto completo, nosotros sólo decoramos la propiedad buffer, que si bien es un objeto, lo usamos como cuasi primitivo tipo string)**/
{
    int vidas = submarino->getVidas();

    limpiarBuffer();

    cartelera->dibujarCartelera(buffer,vidas,ring->getNivel());

    dibujar(ring); /** Uso de Polimorfismo, el método Pantalla::dibujar recibe como parámetro (Pieza* pieza). Luego, cada pieza llama a su propio método dibujar especializado**/

    muro->dibujarMuroActualList(buffer);

    burbujero->dibujarBurbujeroList(buffer);

    contenedorDeDadoresDeVida -> dibujarContenedorDeDadoresDeVidaList(buffer);

    if(!bola->getBolaPerdida())
    {
        //cout << "Velocidad: " << bola->getVelocidad() << endl;
        dibujar(bola);
        dibujar(submarino);
    }
    else
    {
        burbujero->limpiarBurbujeroList(); /** Si se pierde la bola, vaciamos las burbujas. Se hace acá para que no se borren las burbujas antes de que se termine la secuencia de destrucción **/
        contenedorDeDadoresDeVida->limpiarContenedorDeDadoresDeVida();
        burbujero->resetBurbujasDestruidasParaVida(); /** para que comience a contar de nuevo en la siguiente ronda**/
        BITMAP* bmpAuxAntesSecuencia = crearNuevaCopiaBuffer(); // crearNuevaCopiaBuffer toma la propiedad buffer de Pantalla y retorna una copia idéntica (pero de memoria distinta, es decir, otro objeto)
        secuenciasBuffer.clear();
        for(unsigned int i = 0; i <submarino->getSecuenciaDestruccion().size(); i++)
        {
            submarino->dibujarDestruccion(i,buffer); //se dibuja la secuencia en el buffer principal
            secuenciasBuffer.push_back(crearNuevaCopiaBuffer()); //guardo el puntero en la lista
            sobreescibirBuffer(bmpAuxAntesSecuencia);
        }

        delete bmpAuxAntesSecuencia;

        if(vidas<=0)
        {
            limpiarBuffer();
            cartelera->dibujarCartelera(buffer,vidas,ring->getNivel());
            ring->dibujar(buffer);
            ring->dibujarGameOver(buffer);
            secuenciasBuffer.push_back(crearNuevaCopiaBuffer());
        }

    }

}

void Pantalla::transmitirBufferOSecuencia()
{
    if (secuenciasBuffer.empty())
        transmitirBuffer(buffer);
    else
    {
        for(list<BITMAP*>::iterator it=secuenciasBuffer.begin(); it != secuenciasBuffer.end(); ++it)
        {
            transmitirBuffer(*it);
            Sleep(300);
        }
        Sleep(300);
        secuenciasBuffer.clear();
    }
}


void Pantalla::inaugurarNivel()
{
    muro->configurarMuroActualMultiMap(ring->getNivel());
    /** si se van a usar los contadore para algo, aquí habría que reiniciarlos*/
    burbujero->resetBurbujasDestruidasParaVida();
    //reiniciar contadores /
    reposar(bola);
    reposar(submarino);
    lanzarFotograma();
    int vel = bola->getVelocidadInicial()+ ((int)ring->getNivel()/3);
    bola->setVelocidad(vel);
    submarino->setVelocidad(vel);
    burbujero->setBurbujeroOn(false);
    burbujero->getBurbujeroList()->clear();
    contenedorDeDadoresDeVida->limpiarDadorDeVidasList();
}

void Pantalla::siSubmarinoNoLanza()
{
    if(!bola->getMeMuevo() && !key[KEY_SPACE])
    {
        ring->moverEnMi(submarino);
        bola->desplazarJuntoConSubmarino(submarino);
        lanzarFotograma();
    }

}



class JuegoSubmarino : public IUsaAllegro
{
protected:
    bool quieroMusica;
    bool quieroSonido;
    bool nuevoNivel;
    Audio* audio;
    Pantalla* pantalla;
    int rondas;
    static JuegoSubmarino * unicaInstancia;
    void activarAllegro() override
    {
        AllegroEjecuta::activarAllegro();
    }

    JuegoSubmarino(int anchoPantalla, int altoPantalla)
    {
        activarAllegro();

        this->audio = Audio::getInstancia();
        this->pantalla = Pantalla::getInstancia(anchoPantalla,altoPantalla);
        this->quieroMusica = true;
        this->quieroSonido = true;
        this->nuevoNivel = true;
        this->rondas = 0;
    }

public:
    virtual ~JuegoSubmarino()
    {
        delete audio;
        delete pantalla;
        delete unicaInstancia;
    }
    static JuegoSubmarino* getInstancia(int, int);
    static JuegoSubmarino* getInstancia();
    JuegoSubmarino(JuegoSubmarino &otroJuego) = delete;
    void operator = (const JuegoSubmarino &) = delete;
    void setearVolumen(unsigned int, unsigned int);
    void ejecutarMusica(string, int);
    string getTeclaEncendido();
    void lanzarFotograma();
    void pausarMusica();
    void ejecutarSonido(string);
    void setPuntajeActual(unsigned int);
    void setHighScore(unsigned int);
    bool getQuieroMusica();
    void setQuieroMusica(bool);
    bool getQuieroSonido();
    void setQuieroSonido(bool);
    void audioOnOff();
    int getVidas();
    void setVidas(int);
    int getNivel();
    void setNivel(int);
    int getRondas() const;
    void setRondas(int valor);

    void correrPartida();
    Pantalla* getPantalla();
    void inaugurarNivel();
    void jugar(); //Meter acá el main cuando todo esté arreglado
    void ejecutarRonda();
    void siHabilitoLanzamiento();
    void siEjecutarSonidosRonda();

private:
    void reiniciarValores()
    {
        setVidas(3);
        setNivel(1);
        nuevoNivel=true;
        pantalla->getBola()->setVelocidad(pantalla->getBola()->getVelocidadInicial());
        pantalla->getCartelera()->getPuntaje()->setPuntajeActual(0);
    }
};

JuegoSubmarino* JuegoSubmarino::unicaInstancia = 0;
JuegoSubmarino* JuegoSubmarino::getInstancia(int anchoPantalla, int altoPantalla)
{
    if(unicaInstancia == 0)
    {
        unicaInstancia = new JuegoSubmarino(anchoPantalla,altoPantalla);
    }
    return unicaInstancia;
}

JuegoSubmarino* JuegoSubmarino::getInstancia()
{
    if(unicaInstancia == 0)
    {
        unicaInstancia = new JuegoSubmarino(1024,740);
    }
    return unicaInstancia;
}

void JuegoSubmarino::setearVolumen(unsigned int sample_volume, unsigned int midi_volume)
{
    audio->setearVolumen(sample_volume, midi_volume);
}

void JuegoSubmarino::ejecutarMusica(string index, int num)
{
    audio->ejecutarMusica(index,num);
}


void JuegoSubmarino::lanzarFotograma()
{
    pantalla->lanzarFotograma();
}

void JuegoSubmarino::pausarMusica()
{
    audio->pausarMusica();
}

void JuegoSubmarino::ejecutarSonido(string sonido)
{
    audio->ejecutarSonido(sonido); //play_sample(sonido_gameover,200,150,1000,0);
}


bool JuegoSubmarino::getQuieroMusica()
{
    return quieroMusica;
}

void JuegoSubmarino::setQuieroMusica(bool flag)
{
    quieroMusica = flag;
}

bool JuegoSubmarino::getQuieroSonido()
{
    return quieroSonido;
}

void JuegoSubmarino::setQuieroSonido(bool flag)
{
    quieroSonido = flag;
}

void JuegoSubmarino::audioOnOff()
{
    if(key[KEY_DEL])
    {
        setQuieroMusica(!getQuieroMusica());
        if (getQuieroMusica())
            midi_resume();
        else
            pausarMusica();
    }

    if(key[KEY_TAB])
        setQuieroSonido(!getQuieroSonido());
}

void JuegoSubmarino::siEjecutarSonidosRonda()
{
    if(getQuieroSonido())
    {
        Bola* _bola = pantalla->getBola();
        ContenedorDeDadoresDeVida* _contenedor = pantalla->getContenedorDeDadoresDeVida();
        if(_bola->getBolaPerdida())
            ejecutarSonido("vidaPerdida");
        if(_bola->getReboteSub())
            ejecutarSonido("rebotaBase");
        if(_bola->getRompiLad())
            ejecutarSonido("ladrilloRoto");
        if(_bola->getRompiBur())
            ejecutarSonido("ladrilloRoto"); //hasta conseguir sonido mejor
        if(_bola->getReboteRingLad())
            ejecutarSonido("rebotePelota");
        if(_contenedor->getGaneUnaVida())
            ejecutarSonido("vidaExtra");
    }
}

int JuegoSubmarino::getVidas()
{
    return pantalla->getSubmarino()->getVidas();
}

void JuegoSubmarino::setVidas(int vida)
{
    pantalla->getSubmarino()->setVidas(vida);
}

int JuegoSubmarino::getNivel()
{
    return pantalla->getRing()->getNivel();
}

void JuegoSubmarino::setNivel(int niv)
{
    pantalla->getRing()->setNivel(niv);
}

int JuegoSubmarino::getRondas() const
{
    return rondas;
}

void JuegoSubmarino::setRondas(int valor)
{
    rondas=valor;
}

void JuegoSubmarino::inaugurarNivel()
{
    pantalla->inaugurarNivel();
    if(quieroSonido)
        ejecutarSonido("inicioNivel");
    nuevoNivel = false;
    rondas=0;
}


Pantalla* JuegoSubmarino::getPantalla()
{
    return pantalla;
}

void JuegoSubmarino::ejecutarRonda()
{
    Bola* _bola = pantalla->getBola();
    Submarino* _sub = pantalla->getSubmarino();
    Ring* _ring = pantalla->getRing();
    Muro* _muro = pantalla->getMuro();
    Burbujero* _burbujero = pantalla->getBurbujero();
    Puntaje* _puntaje = pantalla->getCartelera()->getPuntaje();
    ContenedorDeDadoresDeVida* _contenedorDeDadoresDeVida = pantalla->getContenedorDeDadoresDeVida();

    pantalla->siSubmarinoNoLanza();
    siHabilitoLanzamiento();
    if(_bola ->getMeMuevo())
    {

        _burbujero->siCrearBurbujasPorPlazo();
        if(_contenedorDeDadoresDeVida->siCrearDadorDeVida(_burbujero->getBurbujasDestruidasParaVida()))
        {
            _burbujero->resetBurbujasDestruidasParaVida();
        }

        bool flagInterno = true;
        for(int i = 0; i<_bola->getVelocidad(); i++) /** Bola::mover() siempre mueve 1; usamos velocidad de bola dentro del
                                                        bucle for. Mientras no choque con nada, se le permite volver a mover
                                                        hasta que se le termine la velocidad*/
        {
            pantalla->moverBurbujasEnPantalla(rondas,flagInterno); /** El ciclo de vida de las burbujas es tan complejo, que directamente se encarga pantalla en un método aparte*/

            flagInterno=false;/** me aseguro de que las burbujas muevan una sola vez por ciclo de bola*/
            _ring->moverEnMi(_bola);

            if(_muro->hayImpacto(_bola))
            {
                _muro->reciboImpacto(_bola);
                _puntaje->setPuntajeActual(_puntaje->getPuntajeActual()+ (_muro->getLadrillosDestruidosEnRonda() * 10));
                _muro->resetLadrillosDestruidosEnRonda();

            }
            _puntaje->setPuntajeActual(_puntaje->getPuntajeActual()+ (_burbujero->getBurbujasDestruidasEnRonda() * 1));
            _burbujero->resetBurbujasDestruidasEnRonda();

            if (_sub->hayImpacto(_bola))
                _sub->reciboImpacto(_bola);
            if(_bola->hayFlagsImpactoActivas())
                break;
        }

        _ring->moverEnMi(_sub);

        if (_contenedorDeDadoresDeVida->hayImpacto(_sub))
        {
            _contenedorDeDadoresDeVida->reciboImpacto(_sub);
            _contenedorDeDadoresDeVida->setGaneUnaVida(true);
            _burbujero->resetBurbujasDestruidasParaVida();
            this->setVidas(this->getVidas()+1);
        }
        if(_contenedorDeDadoresDeVida->analizoTiempoDeVidaContenedorDeDadores())
        {
            _burbujero->resetBurbujasDestruidasParaVida();
        }

        _ring ->moverEnMi(_contenedorDeDadoresDeVida);

         if (_contenedorDeDadoresDeVida->hayImpacto(_sub))
        {
            _contenedorDeDadoresDeVida->reciboImpacto(_sub);
            _burbujero->resetBurbujasDestruidasParaVida();
            this->setVidas(this->getVidas()+1);
        }
        if(_contenedorDeDadoresDeVida->analizoTiempoDeVidaContenedorDeDadores())
        {
            _burbujero->resetBurbujasDestruidasParaVida();
        }

        rondas++;

        if(_bola->getBolaPerdida())
        {
            _sub->setVidas(_sub->getVidas()-1);
            setRondas(0);
        }

        else if(_muro->seEliminaronLadrillos())
        {
            nuevoNivel=true;
            setNivel(getNivel()+1);
            setRondas(0);
        }

    }
}

void JuegoSubmarino::siHabilitoLanzamiento()
{
    Bola* _bola = pantalla->getBola();
    if(key[KEY_SPACE] && !_bola->getMeMuevo())
    {
        if(quieroSonido)
            audio->detenerSonido("inicioNivel");  //stop_sample(sonido_InicioNivel);

        Burbujero* _burbujero = pantalla->getBurbujero();

        _bola->setMeMuevo(true);
        pantalla->setDirXBolaQuieta();

        _burbujero->reiniciarTempInicial();//reinicia el tiempo internopara la creación de burbujas
        _burbujero->setBurbujeroOn(true);// habilita la creación de burbujas
    }
}

void JuegoSubmarino::jugar()
{

    setearVolumen(230,209);
    ejecutarMusica("inicio",0);//play_midi(musicaInicio,0);


    while(!key[KEY_ESC])
    {
        lanzarFotograma();

        if(key[KEY_ENTER])
        {

            while(!key[KEY_ESC] && pantalla->getSubmarino()->getVidas() > 0)
            {
                if(nuevoNivel)
                {
                    pausarMusica();
                    inaugurarNivel();
                }
                ejecutarRonda();
                audioOnOff(); /** detecta si se presionan las teclas para habilitar sonido. El problema está en la velocidad del bucle. No le puedo poner sleep porque paraliza el juego*/
                siEjecutarSonidosRonda();
                lanzarFotograma();

                if(pantalla->getBola()->getBolaPerdida() && getVidas()>0)
                {
                    pantalla->reposar(pantalla->getBola());
                    pantalla->reposar(pantalla->getSubmarino());
                }

                pantalla->getBola()->restablecerFlagsImpacto();
                pantalla->getContenedorDeDadoresDeVida()->setGaneUnaVida(false);
                if (key[KEY_0] || key[KEY_0_PAD]) /** El bucle es tan rápido, que, hasta que se deja de presionar la tecla se cambia varias veces de nivel. Por eso el Sleep **/
                {
                    Sleep(300);
                    nuevoNivel=true;
                    setNivel(getNivel()+1);
                    clear_keybuf(); /** limpia el buffer del teclado **/
                }
            }

            pausarMusica();
            if(getQuieroSonido())
                ejecutarSonido("gameOver"); //play_sample(sonido_gameover,200,150,1000,0);


            Puntaje* objPuntaje = pantalla->getCartelera()->getPuntaje();
            unsigned int puntajeActual = objPuntaje->getPuntajeActual();
            if(puntajeActual > objPuntaje->getHighScore())
            {
                objPuntaje->setHighScore(puntajeActual);
            }

            do /** Este bucle está a propósito, para que el bucle principal no arranque nuevamente hasta que no se presione Enter o Esc */
            {
                reiniciarValores();
            }
            while(!key[KEY_ESC] && !key[KEY_ENTER]);

        }



    }

}




int main()
{
    try
    {
        JuegoSubmarino* juegoSubmarino = JuegoSubmarino::getInstancia(1024,740);
        juegoSubmarino->jugar();
        return 0;
    }
    catch(exception& e)
    {
        cout << "Standard exception: " << e.what() << endl;
        return 1;
    }

}
END_OF_MAIN(); /** es un método de Allegro para poner fin a su librería */
