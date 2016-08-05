#include <vector>
#include <map>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>

#define VERSION 1.1

using namespace std;

static string i2s[10] = {"0","1","2","3","4","5","6","7","8","9"};

class Node {

public:

	// variables
	
	Node* n[3];
	
	int numtips[3]; // store the number of tips can be accessed when going through each child
	
	string subtreeText[3]; // the text of the subtree rooted at each child

	string name;
	
	int isSelected; // 1 : this node is selected; 0 : otherwise
	
	int numtipsSelected[3]; // store the number of selected tips can be accessed when going through each child

	// functions
	
	Node(string name); // default constructor
	
	Node(string name, Node* p); // constructor with adding one child
	
	bool isLeaf(); // return whether this node is a leaf

	string toText(Node* parent); // reporting the text format for the current node

	void addChild(Node* child); // add a child

	int calNumTips(Node* parent, int totNumTips); // compute the number of tips can be accessed when going through each child
	
	int calNumTipsSelect(Node* parent,int totSelectTips); // compute the number of selected tips can be accessed when going through each child
	
};

class UnrootTree {

public:

	// variables
	
	double weight;
	
	string text;
	
	int totNumTips;
	
	int totNumTipsSelected;
	
	map<string, Node*> name2Node; // mapping: name -> node

	vector<Node*> allNodes; // all nodes of this unrooted tree
	
	vector<Node*> nodes2Leaves; // a set of nodes with 2 leaves
	
	// functions
	
	UnrootTree(); // default constructor
	
	~UnrootTree(); // destructor
	
	int numNodes(); // return the number of nodes
	
	void makeText(); // make the formatted texts of this unrooted tree
	
	void buildNodes2Leaves(); // build the array of nodes2Leaves

	void parseNewickStr(string& str); // parse a newick string
	
	void rooted2unrooted(); // change a rooted tree to unrooted tree
	
	// functions for checking whether the selected leaves are in sister group
	
	void resetSelect(); // reset all "isSelected" to 0
	
	int setSelect(string name); // set the leaf with that name to 1, return 1 if the leaf exists, 0 otherwise.
	
	void computeAllTipsSelected(); // compute the allTipsSelected[] for all the nodes
	
	bool checkWhetherSister(); // check whether all the selected leaves are in the sister group
	
private:
	
	Node* parseNewickSubStr(string& str, int endPos, int& startPos, Node* parent); // helping function for parsing a newick string
};


// other functions

string int2str(int i);

void removeSpaces(string& s); // remove all the spaces inside the space
