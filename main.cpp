#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <set>
#include <iomanip>

using namespace std;

vector<vector<double>> dataset;

bool loadData(const string& filename)
{
    dataset.clear();
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Error opening file\n";
        return false;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty()) continue;

        stringstream ss(line);
        vector<double> row;
        double value;

        while (ss >> value)
            row.push_back(value);

        if (!row.empty())
            dataset.push_back(row);
    }

    file.close();
    return !dataset.empty();
}

double distanceCalc(const vector<double>& a,
                    const vector<double>& b,
                    const set<int>& features)
{
    double dist = 0.0;

    for (int feature : features)
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

    for (int i = 0; i < n; i++)
    {
        double bestDist = numeric_limits<double>::max();
        int nearest = -1;

        for (int j = 0; j < n; j++)
        {
            if (i == j) continue;

            double dist = distanceCalc(dataset[i], dataset[j], features);

            if (dist < bestDist)
            {
                bestDist = dist;
                nearest = j;
            }
        }

        if (nearest != -1 && dataset[i][0] == dataset[nearest][0])
            correct++;
    }

    return (double)correct / n * 100.0;
}

void printSet(const set<int>& s)
{
    cout << "{";
    bool first = true;

    for (int f : s)
    {
        if (!first) cout << ",";
        cout << f;
        first = false;
    }

    cout << "}";
}

void forwardSelection(int totalFeatures)
{
    set<int> currentSet;
    set<int> bestOverallSet;
    double bestOverall = 0.0;

    cout << "\nBeginning search.\n\n";

    for (int level = 1; level <= totalFeatures; level++)
    {
        int bestFeature = -1;
        double bestAcc = 0.0;

        for (int feature = 1; feature <= totalFeatures; feature++)
        {
            if (currentSet.count(feature)) continue;

            set<int> temp = currentSet;
            temp.insert(feature);

            double acc = evaluateAccuracy(temp);

            cout << "Using feature(s) ";
            printSet(temp);
            cout << " accuracy is " << fixed << setprecision(1) << acc << "%\n";

            if (acc > bestAcc)
            {
                bestAcc = acc;
                bestFeature = feature;
            }
        }

        if (bestFeature != -1)
        {
            currentSet.insert(bestFeature);

            cout << "\nFeature set ";
            printSet(currentSet);
            cout << " was best, accuracy is " << bestAcc << "%\n";

            if (bestAcc > bestOverall)
            {
                bestOverall = bestAcc;
                bestOverallSet = currentSet;
            }
            else
            {
                cout << "(Warning, Accuracy has decreased! Continuing search in case of local maxima)\n";
            }

            cout << "\n";
        }
    }

    cout << "Finished search!! The best feature subset is ";
    printSet(bestOverallSet);
    cout << ", which has an accuracy of " << bestOverall << "%\n";
}

void backwardElimination(int totalFeatures)
{
    set<int> currentSet;
    for (int i = 1; i <= totalFeatures; i++)
        currentSet.insert(i);

    set<int> bestOverallSet = currentSet;
    double bestOverall = evaluateAccuracy(currentSet);

    cout << "\nBeginning search.\n\n";

    for (int level = totalFeatures; level > 1; level--)
    {
        int removeFeature = -1;
        double bestAcc = 0.0;

        for (int feature : currentSet)
        {
            set<int> temp = currentSet;
            temp.erase(feature);

            double acc = evaluateAccuracy(temp);

            cout << "Using feature(s) ";
            printSet(temp);
            cout << " accuracy is " << fixed << setprecision(1) << acc << "%\n";

            if (acc > bestAcc)
            {
                bestAcc = acc;
                removeFeature = feature;
            }
        }

        if (removeFeature != -1)
        {
            currentSet.erase(removeFeature);

            cout << "\nFeature set ";
            printSet(currentSet);
            cout << " was best, accuracy is " << bestAcc << "%\n";

            if (bestAcc > bestOverall)
            {
                bestOverall = bestAcc;
                bestOverallSet = currentSet;
            }
            else
            {
                cout << "(Warning, Accuracy has decreased! Continuing search in case of local maxima)\n";
            }

            cout << "\n";
        }
    }

    cout << "Finished search!! The best feature subset is ";
    printSet(bestOverallSet);
    cout << ", which has an accuracy of " << bestOverall << "%\n";
}

