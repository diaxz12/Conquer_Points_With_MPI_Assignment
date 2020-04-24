//Luis Dias
//Com MPI

#include "Points.h"
#include <mpi.h>

int main()
{
    cout << "\t\t*******************************************" << endl;
    cout << "\t\t\tFinding Closest Pair" << endl;
    cout << "\t\t\tDeveloped By: Luis Dias" << endl;
    cout << "\t\t*******************************************" << endl << endl;

    //Funcao que permite criar o ficheiro com o numero de pontos especificados
    GenerateRandPoints();

    //Rotina para ler as coordenadas do ficheiro "Pontos.txt"
    string line;
    ifstream pointFile("Pontos.txt");
    if (pointFile.is_open())
    {
        getline(pointFile, line);
        pNum = stoi(line);
        int i = 0;

        while (getline(pointFile, line))
        {
            double x = stod(line.substr(0, line.find(" ")));
            double y = stod(line.substr(line.find(" ")));
            Point pTemp;
            pTemp.x = x;
            pTemp.y = y;
            p.push_back(pTemp);
            i++;
        }
        pointFile.close();
    }
    else{
        cout << "There was a problem opening or reading pair file" << endl;
    }

    // Brute-Force Strategy
    vector<Point> minPointsBruteForce;
    clock_t beginBruteForce = clock();
    minPointsBruteForce = bruteForce(p);
    clock_t endBruteForce = clock();
    double elapsedSecsBruteForce = double(endBruteForce - beginBruteForce) / CLOCKS_PER_SEC;
    double distMin = EuclideanDistance(minPointsBruteForce[0], minPointsBruteForce[1]);
    cout << "Brute-Force:" << endl;
    cout << "Min Dist: " << distMin << endl;
    cout << "Pairs are: (" << minPointsBruteForce[0].x << "," << minPointsBruteForce[0].y << ") (" << minPointsBruteForce[1].x << "," << minPointsBruteForce[1].y << ")" << endl;
    cout << "Total Running Time of Brute-Force: " << elapsedSecsBruteForce<< endl;
    cout << "Brute-Force Counter: " << counterBruteForce << endl;
    cout << "***********************************\n" << endl;
    // End of Brute-Force

    // Divide & Conquer Strategy
    clock_t beginDivide = clock();
    cout << "Divide & Conquer:" << endl;
    if (p.size() < 2) { // Para o caso de termos 2 pontos ou menos
        // retornar o resultado para o par
        cout << "p1: " << p[0].x << " " << p[0].y << " , p2: " << p[1].x << " " << p[1].y << EuclideanDistance(p[0], p[1]) << endl;
    }
    else {
        vector<Point> sortedPx;
        //ordenar a lista de pontos
        sortedPx = mergeSort(p, "x");
        vector<Point> closestP = closestPair(sortedPx);
        clock_t endDivide = clock();
        double elapsedSecsDevide = double(endDivide - beginDivide) / CLOCKS_PER_SEC;
        cout << "Min Dist: " << EuclideanDistance(closestP[0], closestP[1]) << endl;
        cout << "Closest Pair: (" << closestP[0].x << "," << closestP[0].y << ") (" << closestP[1].x << "," << closestP[1].y << ")" << endl;
        cout << "Total Running Time of Devide & Conquer: " << elapsedSecsDevide << endl;
        cout << "Devide & Conquer Counter: " << counterDevide << endl;
    }
    // End of Devide & Conquer Strategy

    return 0;
}