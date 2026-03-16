#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>
#include <set>
#include <iomanip>

using namespace std;

vector<vector<double>> dataset;



bool loadData(string filename)
{
    dataset.clear();

    ifstream file(filename);

    if(!file.is_open())
    {
        cout << "Error opening file\n";
        return false;
    }

    string line;

    while(getline(file,line))
    {
        if(line.empty()) continue;

        stringstream ss(line);
        vector<double> row;
        double value;

        while(ss >> value)
        {
            row.push_back(value);
        }

        dataset.push_back(row);
    }

    file.close();
    return true;
}


double distanceCalc(const vector<double>& a,
                    const vector<double>& b,
                    const set<int>& features)
{
    double dist = 0;

    for(int feature : features)
    {
        double diff = a[feature] - b[feature];
        dist += diff * diff;
    }

    return dist;
}



double evaluateAccuracy(const set<int>& features)
{
    int correct = 0;
    int n = dataset.size();

    for(int i = 0; i < n; i++)
    {
        double bestDist = numeric_limits<double>::max();
        int nearest = -1;

        for(int j = 0; j < n; j++)
        {
            if(i == j) continue;

            double dist = distanceCalc(dataset[i],
                                       dataset[j],
                                       features);

            if(dist < bestDist)
            {
                bestDist = dist;
                nearest = j;
            }
        }

        if(dataset[i][0] == dataset[nearest][0])
            correct++;
    }

    return (double)correct / n * 100;
}



void printSet(const set<int>& s)
{
    cout << "{";

    bool first = true;

    for(int f : s)
    {
        if(!first) cout << ",";
        cout << f;
        first = false;
    }

    cout << "}";
}



