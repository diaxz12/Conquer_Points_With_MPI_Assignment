//Luis Dias
//Com MPI

#include "Points.h"
#include <mpi.h>

int main(int argc, char *argv[])
{

    //MPI variables
    int NumberofProcessors, ProcessorID;
    double TimeStamp;
    vector<Point> ProcessorPoints;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &NumberofProcessors);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcessorID);

    if (ProcessorID == 0)    {
        TimeStamp = -MPI_Wtime(); // ProcessorID 0 starts measuring time
    }

    cout << "\t\t*******************************************" << endl;
    cout << "\t\t\tFinding Closest Pair MPI version | ProcessorID=" << ProcessorID << endl;
    cout << "\t\t\tDeveloped By: Luis Dias" << endl;
    cout << "\t\t*******************************************" << endl << endl;


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

    //Rotina para dividir o ficheiro conforme o processador em questao "Pontos.txt"
    int divisionsSize = p.size()/(NumberofProcessors+1);

    for(int i=ProcessorID*divisionsSize;i < (ProcessorID+2)*divisionsSize; i++){
        ProcessorPoints.push_back(p[i]);
    }

    // Brute-Force Strategy
    vector<Point> minPointsBruteForce;
    clock_t beginBruteForce = clock();
    minPointsBruteForce = bruteForce(ProcessorPoints);
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
    if (ProcessorPoints.size() < 2) { // Para o caso de termos 2 pontos ou menos
        // retornar o resultado para o par
        cout << "p1: " << ProcessorPoints[0].x << " " << ProcessorPoints[0].y << " , p2: " << ProcessorPoints[1].x << " " << ProcessorPoints[1].y << EuclideanDistance(ProcessorPoints[0], ProcessorPoints[1]) << endl;
    }
    else {
        vector<Point> sortedPx;
        //ordenar a lista de pontos
        sortedPx = mergeSort(ProcessorPoints, "x");
        vector<Point> closestP = closestPair(sortedPx);
        clock_t endDivide = clock();
        double elapsedSecsDevide = double(endDivide - beginDivide) / CLOCKS_PER_SEC;
        cout << "Min Dist: " << EuclideanDistance(closestP[0], closestP[1]) << endl;
        cout << "Closest Pair: (" << closestP[0].x << "," << closestP[0].y << ") (" << closestP[1].x << "," << closestP[1].y << ")" << endl;
        cout << "Total Running Time of Devide & Conquer: " << elapsedSecsDevide << endl;
        cout << "Devide & Conquer Counter: " << counterDevide << endl;
    }
    // End of Divide & Conquer Strategy


    if (ProcessorID == 0) {
        TimeStamp += MPI_Wtime();             // ProcessorID 0 measures time
        printf("MPI solution Time: %f s\n", TimeStamp);
    }
    fflush(stdout);
    MPI_Finalize();

    return 0;
}