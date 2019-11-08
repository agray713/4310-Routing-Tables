#include <iostream>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>
#include <iomanip>

using namespace std;

struct Routing {
	vector<int> nodeA;
	vector<int> nodeB;
	vector<int> price;
};

int main(int argc, char** argv) {

	string round = argv[2];

    int count = 0,
		from,
		to,
		value,
		max = 0,
		lastNode,
		loopCount = 0,
		dvPacket = 0,
        roundCount = 0,
        rounds = round[0] - 48;

	vector<int> node1;
	vector<int> node2;
	vector<int> cost;

    ofstream fout;
    fout.open("routingTable.txt");

	ifstream fin;
	fin.open(argv[1]);

	if (!fin) {
		cout << "File failed to open";

		return - 1;
	}

	while (!fin.eof()) {
		fin >> from >> to >> value;

		node1.push_back(from);
		node2.push_back(to);
		cost.push_back(value);

		++count;
	}

    for(int i = 0; i < count; i++){
        if(node1.at(i) > max){
            max = node1.at(i);
        }
    }
    for(int i = 0; i < count; i++){
        if(node2.at(i) > max){
            max = node2.at(i);
        }
    }

	Routing routing[max + 1];

	for (int i = 0; i <= max; ++i) {
		for (int j = 0; j <= max; ++j) {
			if (i == j) {
				routing[i].nodeA.push_back(j);
				routing[i].price.push_back(0);
			}
			else {
				routing[i].nodeA.push_back(j);
				routing[i].price.push_back(99999);
			}
			routing[i].nodeB.push_back(j);
		}
	}

	for (int i = 0; i <= count; ++i) {
		routing[node1[i]].price.at(node2[i]) = cost[i];
		routing[node2[i]].price.at(node1[i]) = cost[i];
	}

	vector<int> neighbors[max + 1];

	for (int i = 0; i <= max; ++i) {
		for (int j = 0; j <= max; ++j) {
			if (routing[i].price.at(j) > 0 && routing[i].price.at(j) < 99999)
				neighbors[i].push_back(j);
		}
	}

	bool flag = true;
	while (flag) {
		flag = false;
		for (int i = 0; i <= max; ++i) {
			for (int j = 0; j < neighbors[i].size(); ++j) {
				dvPacket++;
				for (int k = 0; k <= max; ++k) {
					if(routing[i].price.at(k) > routing[neighbors[i].at(j)].price.at(k) + routing[neighbors[i].at(j)].price.at(i) && routing[neighbors[i].at(j)].nodeB.at(i) == i){
                      routing[i].price.at(k) = routing[neighbors[i].at(j)].price.at(k) + routing[i].price.at(neighbors[i].at(j));
                      routing[i].nodeB.at(k) = neighbors[i].at(j);
                      lastNode = i;
                      flag = true;
					}
				}
			}
		}
		roundCount++;
		if (roundCount == rounds) {
			cout << "The routing tables after " << rounds << "round(s) are as follows" << endl
                 << "--------------------------------------------------" << endl;
            for (int i = 0; i <= max; ++i) {
				cout << "  Routing table for node " << i << endl;
				cout << "  dest     cost      next hop" << endl;           //prints table for each node
                    for (int j = 0; j < max + 1; j++) {
                        cout << setw(6) <<  routing[i].nodeA.at(j) << " \t " << setw(6) << routing[i].price.at(j)
                            << " \t " << setw(2) << routing[i].nodeB.at(j) << endl;
                    }
			}
		}
		++loopCount;
	}

    if (loopCount < rounds){
        cout << "***The nodes converged prior to the amount of rounds entered***" << endl << endl;
        for(int i = 0; i <= max; ++i){
                cout << "  Routing table for node " << i << endl
				     << "  dest     cost         next hop" << endl;           //prints table for each node
				for (int j = 0; j < max + 1; j++) {
					cout << setw(6) <<  routing[i].nodeA.at(j) << " \t " << setw(6) << routing[i].price.at(j)
                         << " \t " << setw(4) << routing[i].nodeB.at(j) << endl;
				}
            cout << endl << endl;
        }
    }

    for(int i = 0; i <= max; ++i){
                fout << "  Routing table for node " << i <<  endl
				     << "  dest     cost      next hop" << endl;           //prints table for each node
				for (int j = 0; j < max + 1; j++) {
					fout << setw(6) <<  routing[i].nodeA.at(j) << " \t " << setw(6) << routing[i].price.at(j)
                         << " \t " << setw(4) << routing[i].nodeB.at(j) << endl;
				}
            fout << endl << endl;
        }

    fout << endl << "Number of rounds to converge: " << loopCount-1 << endl
         << "Last node to converge completely: " << lastNode  << endl
         << "Number of DV messages sent: " << dvPacket << endl << endl;

	fout.close();
    fin.close();
	return 0;
}
