#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <ctime>

using namespace std;

/* Contar o numero de iteracoes por cada um dos algoritmos */
int counterBruteForce=0, counterDevide = 0;

struct Point
{
    double x, y;
};

int pNum;
vector<Point> p;

/* Gerar um valor aleatorio para o intervalo de valores especificado */
double randomNumber(double min, double max)
{
    return ((double)rand() / (double)RAND_MAX) * (max - min) + min;
}


/* Gerar os pontos para um ficheiro txt e depois ler esse mesmo ficheiro */
void GenerateRandPoints() {
    srand(time(0));
    ofstream outputfile;
    outputfile.open("Pontos.txt");
    if (outputfile.is_open()) {
        cout << "Especificar o numero de pontos a gerar: ";
        int pairNumber = 0;
        cin >> pairNumber;
        double xMin, xMax, yMin, yMax;
        cout << "Intervalo de valores para -x,+x coordenadas" << endl;
        cin >> xMin >> xMax;
        cout << "Intervalo de valores para -y,+y coordenadas" << endl;
        cin >> yMin >> yMax;

        outputfile << pairNumber << endl;
        for (int i = 0; i < pairNumber; i++)
            outputfile << randomNumber(xMin, xMax) << " " << randomNumber(yMin, yMax) << endl;
        outputfile.close();
    }
    else{
        cout << "Erro ao gerar o ficheiro de dados" << endl;
    }
}

/* Distancia euclideana entre dois pontos */
double EuclideanDistance(Point p1, Point p2)
{
    return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}

/* Ordenar os valores de cada strip */
vector<Point> merge(vector<Point> left, vector<Point> right, string coordinate_dimension) {
    int i = 0, j = 0, k = 0; //i:return vect j:left k:right counters
    int lsize = left.size();
    int rsize = right.size();
    int size = lsize + rsize;
    vector<Point> sortedVect(size);
    for (i = 0; i < size; i++) {
        counterDevide++;
        if (j < lsize && k < rsize) {
            if (coordinate_dimension == "x") { // Ordenar por X-coordenada
                if (left[j].x <= right[k].x) {
                    sortedVect[i] = left[j];
                    j++;
                }
                else {
                    sortedVect[i] = right[k];
                    k++;
                }
            }
            else { // Ordenar por Y-coordenada
                if (left[j].y <= right[k].y) {
                    sortedVect[i] = left[j];
                    j++;
                }
                else {
                    sortedVect[i] = right[k];
                    k++;
                }
            }

        }
        else if (j < lsize) {
            sortedVect[i] = left[j];
            j++;
        }
        else if (k < rsize) {
            sortedVect[i] = right[k];
            k++;
        }
    }
    return sortedVect;
}

/* Ordenar os valores dos diferentes pontos */
vector<Point> mergeSort(vector<Point> vect, string coord) {
    int size = vect.size();
    int mid = size / 2;
    vector<Point> left;
    vector<Point> right;
    if (size <= 1)//base case
        return vect;
    for (int i = 0; i < mid; i++)
    {
        left.push_back(vect[i]);
        right.push_back(vect[i + mid]);
    }
    if (size % 2 != 0)
        right.push_back(vect.back());//Push last element of vector to the right vector in case size of vect is odd

    left = mergeSort(left, coord);
    right = mergeSort(right, coord);

    return merge(left, right, coord);
}

// Brute Force version
vector<Point> bruteForce(vector<Point> p) {
    int psize = p.size();
    if (psize < 2)
        return p;
    else
    {
        double min = EuclideanDistance(p[0], p[1]);
        Point p1 = p[0], p2 = p[1];
        for (int i = 0; i < psize; i++)
            for (int j = i + 1; j < psize; j++)
            {
                double DistBetweenPoints = EuclideanDistance(p[i], p[j]);
                counterBruteForce++;
                if (DistBetweenPoints < min) {
                    min = DistBetweenPoints;
                    //Par correspondente a menor distância:
                    p1 = p[i];
                    p2 = p[j];
                }
            }

        //Variavel com o resultado final
        vector<Point> minPoints;
        minPoints.push_back(p1);
        minPoints.push_back(p2);
        return minPoints;
    }
}

// Divide & Conquer version
vector<Point> closestPair(vector<Point> points) {
    if (points.size() <= 3) {
        // Para tres pontos efetuar o algoritmo de forca bruta
        return bruteForce(points);
    }


    int NumberofPoints = points.size();
    int ClusterSize = NumberofPoints / 2;
    vector<Point> leftPairs;
    vector<Point> rightPairs;
    vector<Point> leftResult, rightResult;

    //1-Divide all points in two halves.
    if (NumberofPoints % 2 == 0) {
        for (int i = 0; i < ClusterSize; i++)
        {
            leftPairs.push_back(points[i]);
            rightPairs.push_back(points[i + ClusterSize]);
        }
    }
    else
    {
        for (int i = 0; i <= ClusterSize; i++) //A esquerda tera mais um ponto para o caso dos impares
        {
            leftPairs.push_back(points[i]);
        }
        for (int i = ClusterSize + 1; i < NumberofPoints; i++)
        {
            rightPairs.push_back(points[i]);
        }
    }

    //2-Recursivamente encontrar a distancia minim nos subarrays
    leftResult = closestPair(leftPairs);
    rightResult = closestPair(rightPairs);
    double minLeft = EuclideanDistance(leftResult[0], leftResult[1]);
    double minRight = EuclideanDistance(rightResult[0], rightResult[1]);
    double minLR = min(minLeft, minRight);
    vector<Point> stripPoints;


    //3-Verificar quais os pontos admitidos na strip
    for (int i = ClusterSize-1; i > 0; i--)
    {
        if (points[ClusterSize].x - points[i].x < minLR)
            stripPoints.push_back(points[i]);
        else
            break; // In case px>min
    }
    for (int i = ClusterSize + 1; i < NumberofPoints; i++)
    {
        if (points[i].x- points[ClusterSize].x < minLR)
            stripPoints.push_back(points[i]);
        else
            break; // In case px>min
    }

    //4-Ordenar os pontos na strip pelo y
    stripPoints.push_back(points[ClusterSize]);
    vector<Point> sortedStripPointsy;
    vector<Point> stripClosestPair;
    if (stripPoints.size() > 1) // At least one 2 points be in the strip area
    {
        // If there is a point in Strip Area
        double min = minLR;
        sortedStripPointsy = mergeSort(stripPoints, "y");
        int sizePointStrip = sortedStripPointsy.size();
        for (int i = 0; i < sizePointStrip - 1; i++)
            for (int j = i + 1; j < sizePointStrip && (sortedStripPointsy[j].y - sortedStripPointsy[i].y < min); j++)
            {
                counterDevide++;
                double pairDistance = EuclideanDistance(sortedStripPointsy[i], sortedStripPointsy[j]);
                if (pairDistance < min)
                {
                    min = pairDistance;
                    stripClosestPair.clear();
                    stripClosestPair.push_back(sortedStripPointsy[i]);
                    stripClosestPair.push_back(sortedStripPointsy[j]);
                }
            }

        if (stripClosestPair.size()>0) // If closest pair is in the strip line
            return stripClosestPair;
        else
            return (minLeft < minRight) ? leftResult : rightResult;
    }
    else
        return (minLeft < minRight) ? leftResult : rightResult;
}