/*
 * Conversor de CSV file a Dat file
 * Mockaroo: Generador de datos de CSV (https://www.mockaroo.com/a4cb8490)
*/

#include <QCoreApplication>
#include <QFile>
#include <QMap>
#include <QVector>
#include <QString>
#include <fstream>
#include <cstring>
#include <iostream>

using namespace std;

typedef struct {
    uint idCurso;
    time_t fechaInscripcion;
} Inscripcion;

typedef struct {
    uint idCliente, cantInscriptos;
    Inscripcion* CursosInscriptos;
} Asistencia;

void add2map(QMap<uint, QVector<Inscripcion>>& Clientes, uint id, Inscripcion nodo);

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QMap<uint, QVector<Inscripcion>> AsistenciasMAPA;

    {
        QFile archicsv("./iriAsistenciaGYM.csv");
        if (!archicsv.open(QIODevice::ReadOnly | QIODevice::Text))
            return 1;

        QList<QString> data;
        QTextStream stream(&archicsv);
        QString separator(",");
        stream.readLine();
        while (stream.atEnd() == false) {
            QString line = stream.readLine();
            data << line.split(separator);

            uint ID = data[0].toUInt();
            Inscripcion Aux = { (data[1].toUInt()), (data[2].toLongLong()) };

            add2map(AsistenciasMAPA, ID, Aux);
            data.clear();
        }
        archicsv.close();

    }

    Asistencia* Asistencias = new Asistencia[AsistenciasMAPA.size()];
    uint i = 0;
    for (auto inicio = AsistenciasMAPA.cbegin(), final = AsistenciasMAPA.cend(); inicio != final; ++inicio) {
        Asistencias[i].idCliente = inicio.key();
        Asistencias[i].cantInscriptos = inicio.value().length();

        Asistencias[i].CursosInscriptos = new Inscripcion[Asistencias[i].cantInscriptos];
        for(uint j = 0; j < Asistencias[i].cantInscriptos; j++) {
            Asistencias[i].CursosInscriptos[j].idCurso = inicio.value()[j].idCurso;
            Asistencias[i].CursosInscriptos[j].fechaInscripcion = inicio.value()[j].fechaInscripcion;
        }
        i++;
    }

    /*for(uint i = 0; i < AsistenciasMAPA.size(); i++) {
        cout << "Id Cliente: " << Asistencias[i] .idCliente << " - Cantidad de Curso inscriptos: " << Asistencias[i].cantInscriptos << " >> ";
        for(uint j = 0; j < Asistencias[i].cantInscriptos; j++)
            cout << "Codigo Clase: " << Asistencias[i].CursosInscriptos[j].idCurso << " - ";

        cout << endl;
    }*/

    ofstream archibinwr("asistencias_1697673600000.dat", ios::binary);
    if (archibinwr.is_open()) {
        archibinwr.write((char*)Asistencias, sizeof(Asistencia) * AsistenciasMAPA.size());
    }
    archibinwr.close();

    delete[] Asistencias;
    Asistencias = nullptr;

    ifstream archibinrd("asistencias_1697673600000.dat", ios::binary | ios::ate);
    if(archibinrd.is_open()) {
        int length_ = archibinrd.tellg()/sizeof(Asistencia);
        archibinrd.seekg(ios::beg);
        Asistencias = new Asistencia[length_];
        archibinrd.read((char*)Asistencias, sizeof(Asistencia) * length_);
    }

    archibinrd.close();
    for(uint i = 0; i < AsistenciasMAPA.size(); i++) {
        cout << "Id Cliente: " << Asistencias[i] .idCliente << " - Cantidad de Curso inscriptos: " << Asistencias[i].cantInscriptos << " >> ";
        for(uint j = 0; j < Asistencias[i].cantInscriptos; j++)
            cout << "Codigo Clase: " << Asistencias[i].CursosInscriptos[j].idCurso << " - ";

        cout << endl;
    }

    delete[] Asistencias;
    return a.exec();
}

void add2map(QMap<uint, QVector<Inscripcion>>& Clientes, uint id, Inscripcion nodo) {
    if (Clientes.contains(id)) {
        Clientes[id].push_back(nodo);
        return;
    }

    QVector<Inscripcion> Clases;
    Clases.push_back(nodo);
    Clientes.insert(id, Clases);
}
