#include <iostream>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>
#include <iomanip>

using namespace std;

struct Routing {      //Structure which will hold individual routing table information for each node
	vector<int> nodeA; //node we are trying to reach
	vector<int> nodeB; //next hop node
	vector<int> price; //price for the path
};

int main(int argc, char** argv) {

	string round = argv[2]; //takes the second argument to be later converted to an int to count rounds

    int count = 0,
		from,
		to,
		value,
		max = 0,
		lastNode,
		loopCount = 0,
		dvPacket = 0,
        roundCount = 0,
        rounds = round[0] - 48, //calculation to convert string read from command line to int for later loop
        startNode = 0, //to test path from starting node to ending node
        endNode = 23;

	vector<int> node1;  //these are used tto hold the information read in from the file before analysis
	vector<int> node2;
	vector<int> cost;

    ofstream fout;
    fout.open("routingTable.txt"); //file used in creating results.txt

	ifstream fin;
	fin.open(argv[1]); //command line argument containing input file

	if (!fin) {
		cout << "File did not open please try a different file";

		return - 1;
	}

	while (!fin.eof()) {  //reading data in from the input file and filling temporary arrays
		fin >> from >> to >> value;  

		node1.push_back(from);
		node2.push_back(to);
		cost.push_back(value);

		++count;  //couting the number of line in the input file
	}
    
    for(int i = 0; i < count; i++){ //these are used to determine max node by checking the temporary arrays
        if(node1.at(i) > max){
            max = node1.at(i);
        }                          
    }
    for(int i = 0; i < count; i++){
        if(node2.at(i) > max){
            max = node2.at(i);
        }
    }

	Routing routing[max + 1]; //this creates annd array of routing table structures the size of the amout of nodes

	for (int i = 0; i <= max; ++i) {  //initializes routing tables with 0 for path to itself and 99999 to any other node
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

	for (int i = 0; i <= count; ++i) { //initializes the routing tables with the information from the file
		routing[node1[i]].price.at(node2[i]) = cost[i];
		routing[node2[i]].price.at(node1[i]) = cost[i];
	}

	vector<int> neighbors[max + 1]; //creates an array to store node neighbors

	for (int i = 0; i <= max; ++i) { //checks to see if nodes are directly connected and fill neighbors[]
		for (int j = 0; j <= max; ++j) {
			if (routing[i].price.at(j) > 0 && routing[i].price.at(j) < 99999)
				neighbors[i].push_back(j);
		}
	}

	bool flag = true; //flag to determine convergence
	while (flag) {
		flag = false;
		for (int i = 0; i <= max; ++i) { //iterating through each node
			for (int j = 0; j < neighbors[i].size(); ++j) { //iterate through each nodes number of neighbors
				dvPacket++; 
				for (int k = 0; k <= max; ++k) { //checking each nodes connection with every other node
					if(routing[i].price.at(k) > routing[neighbors[i].at(j)].price.at(k) + routing[neighbors[i].at(j)].price.at(i) 
                    && routing[neighbors[i].at(j)].nodeB.at(i) == i){ //check both if a faster route has been found as well as check if the proper next hop is being taken
                      routing[i].price.at(k) = routing[neighbors[i].at(j)].price.at(k) + routing[i].price.at(neighbors[i].at(j));
                      routing[i].nodeB.at(k) = neighbors[i].at(j);
                      lastNode = i; //saves last node edited
                      flag = true; //cause loop to run again
					}
				}
			}
		}
		roundCount++;
		if (roundCount == rounds) { //when roundCount equals rounds we print to console based on command line argument 
			cout << "The routing tables after " << rounds << "round(s) are as follows" << endl
                 << "--------------------------------------------------" << endl;
            for (int i = 0; i <= max; ++i) { //iterates through nodes and prints to console
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

    if (loopCount < rounds){ //checks if routing table completed prior to amount requested through command line
        cout << "***The nodes converged prior to the amount of rounds entered***" << endl << endl;
        for(int i = 0; i <= max; ++i){
                cout << "  Routing table for node " << i << endl
				     << "  dest     cost         next hop" << endl;           
				for (int j = 0; j < max + 1; j++) {
					cout << setw(6) <<  routing[i].nodeA.at(j) << " \t " << setw(6) << routing[i].price.at(j) 
                         << " \t " << setw(4) << routing[i].nodeB.at(j) << endl;
				}
            cout << endl << endl;
        }
    }

    for(int i = 0; i <= max; ++i){  //outputs the routing tables after full convergence
                fout << "  Routing table for node " << i <<  endl
				     << "  dest     cost      next hop" << endl;           
				for (int j = 0; j < max + 1; j++) {
					fout << setw(6) <<  routing[i].nodeA.at(j) << " \t " << setw(6) << routing[i].price.at(j) 
                         << " \t " << setw(4) << routing[i].nodeB.at(j) << endl;
				}
            fout << endl << endl;
        }

    fout << endl << "Number of rounds to converge: " << loopCount-1 << endl //outputs the requested counts
         << "Last node to converge completely: " << lastNode  << endl 
         << "Number of DV messages sent: " << dvPacket << endl << endl;

    while(startNode != endNode){ //displays path from starting node to end node with hopes
      fout << startNode << " ---> ";
      startNode = routing[startNode].nodeB.at(endNode);
    }

    fout << endNode << endl;
		
    fout.close();
    fin.close();
    return 0;
}
