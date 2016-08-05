#include "JackSum.h"
#include <iomanip>
#include <set>

string int2str(int i) {
	if (i<0) {
		return "-" + int2str(-i);
	} else if (i<10) {
		return i2s[i];
	} else {
		return int2str(i/10)+i2s[i];
	}
}

void removeSpaces(string& s) {
	// remove all spaces inside the string
	int i,k;
	k=0;
	for (i=0; i<s.length(); i++) {
		if (s[i]!=' ') {
			if (k<i)
				s[k]=s[i];
			k++;
		}
	}
	if (k==0)
		s = "";
	else if (k<s.length())
		s.resize(k);
}

double getLastDouble(string& str, int endPos, int& startPos) {
    // get the last double before the position endPos
    // return the double and the starting position of the double
    
    int j = endPos;
    while (j>0 && (!isdigit(str[j])))
        j--;
    int i = j;
    while (i>0 && (isdigit(str[i-1]) || str[i-1]=='.' || str[i-1]=='-' || str[i-1]=='e'))
        i--;
    if (j>=i) {
        startPos = i;
        return atof(str.substr(i,j-i+1).c_str());
    } else {
        cerr << "Error! Double does not exist! " << str.substr(0,endPos+1) << endl;
        exit(1);
    }
}

bool isEdgeLen(string& str, int endPos) {
    // check whether the last double is an edge length
    while (endPos>0) {
        // stop if the character is ':' ',' ')' or '('
        if (str[endPos]==':' || str[endPos]==',' || str[endPos]==')' || str[endPos]=='(')
            break;
        endPos--;
    }
    return (str[endPos]==':');
}

Node::Node(string name) {
	n[0]=n[1]=n[2]=NULL;
	this->name = name;
	// set subtreeText[i] to empty string
	subtreeText[0]=subtreeText[1]=subtreeText[2]="";
	isSelected = 0;
}

Node::Node(string name, Node* p) {
	n[0]=p;
	n[1]=n[2]=NULL;
	this->name = name;
	// set subtreeText[i] to empty string
	subtreeText[0]=subtreeText[1]=subtreeText[2]="";
	isSelected = 0;
}

bool Node::isLeaf() {
	return (n[2]==NULL);
}

void Node::addChild(Node* child) {
	// add a child
	int i;
	for (i=0; i<3; i++) {
		if (n[i]==NULL) {
			n[i]=child;
			break;
		}
	}
}

string Node::toText(Node* parent) {
	// build the text of the subtree rooted at each child
	int i,j,p;
	int np[3]; // the children which are not same as parent

	if (isLeaf()) {
		return name;
	}
	i=0;
	string outstr = "";
	for (j=0; j<3; j++) {
		if (n[j]!=parent) {
			if (n[j]!=NULL) {
				if (subtreeText[j]=="") {
					subtreeText[j] = n[j]->toText(this);
				}
				np[i++]=j;
			}
		}
	}
	if (i==2) {
		if (subtreeText[np[0]] < subtreeText[np[1]])
			outstr = "("+subtreeText[np[0]]+","+subtreeText[np[1]]+")";
		else
			outstr = "("+subtreeText[np[1]]+","+subtreeText[np[0]]+")";
	} else if (i==3) {
		if (subtreeText[0] < subtreeText[1]) {
			if (subtreeText[1] < subtreeText[2])
				outstr = "("+subtreeText[0]+","+subtreeText[1]+","+subtreeText[2]+")";
			else if (subtreeText[0] < subtreeText[2])
				outstr = "("+subtreeText[0]+","+subtreeText[2]+","+subtreeText[1]+")";
			else
				outstr = "("+subtreeText[2]+","+subtreeText[0]+","+subtreeText[1]+")";
		} else {
			if (subtreeText[1] > subtreeText[2])
				outstr = "("+subtreeText[2]+","+subtreeText[1]+","+subtreeText[0]+")";
			else if (subtreeText[0] > subtreeText[2])
				outstr = "("+subtreeText[1]+","+subtreeText[2]+","+subtreeText[0]+")";
			else
				outstr = "("+subtreeText[1]+","+subtreeText[0]+","+subtreeText[2]+")";
		}
	}
	return outstr;
}

int Node::calNumTips(Node* parent, int totNumTips) {
	// compute the number of tips can be accessed when going through each child
	// this function is only valid for internal node only
	// return the sum of number of tips of the children (excepting the one same as the parent)
	int i,k;
	int sum = 0;
	k=-1;
	for (i=0; i<3; i++) {
		if (n[i] != parent) {
			if (n[i]->isLeaf()) {
				numtips[i] = 1;
			} else {
				numtips[i] = n[i]->calNumTips(this, totNumTips);
			}
			sum += numtips[i];
		} else {
			k = i;
		}
	}
	if (k>-1)
		numtips[k] = totNumTips - sum;
	return sum;
}

int Node::calNumTipsSelect(Node* parent,int totSelectTips) {
	// compute the number of selected tips can be accessed when going through each child
	// this function is only valid for internal node only
	// return the sum of number of tips of the children (excepting the one same as the parent)
	int i,k;
	int sum = 0;
	k=-1;
	for (i=0; i<3; i++) {
		if (n[i] != parent) {
			if (n[i]->isLeaf()) {
				if (n[i]->isSelected)
					numtipsSelected[i] = 1;
				else
					numtipsSelected[i] = 0;
			} else {
				numtipsSelected[i] = n[i]->calNumTipsSelect(this, totSelectTips);
			}
			sum += numtipsSelected[i];
		} else {
			k = i;
		}
	}
	if (k>-1)
		numtipsSelected[k] = totSelectTips - sum;
	return sum;
}

UnrootTree::UnrootTree() {
	// default constructor
	
	weight = 0.0;
	text = "";
	totNumTips = 0;
	totNumTipsSelected = 0;
}

UnrootTree::~UnrootTree() {
	// destructor
	int i;
	for (i=0; i<numNodes(); i++) {		
		delete allNodes[i];
	}
	allNodes.clear();
}

int UnrootTree::numNodes() {
	// return the number of nodes
	return allNodes.size();
}

void UnrootTree::makeText() {
	// show the formatted texts of this unrooted tree
	int i;
	string s,t;
	s="";
	if (nodes2Leaves.size()==0)
		buildNodes2Leaves();
	for (i=0; i<nodes2Leaves.size(); i++) {
		t = nodes2Leaves[i]->toText(NULL);
		if (s == "" || t < s)
			s = t;
	}
	text = s;
}

void UnrootTree::buildNodes2Leaves() {
        // build the array of nodes2Leaves
        int i,j;
        int numLeaves;
        nodes2Leaves.clear();
        for (i=0; i<allNodes.size(); i++) {
                numLeaves=0;
                if (!allNodes[i]->isLeaf()) {
                        for (j=0; j<3; j++) {
                                if (allNodes[i]->n[j]!=NULL && allNodes[i]->n[j]->isLeaf())
                                        numLeaves++;
                        }
                }
                if (numLeaves==2) {
                        nodes2Leaves.push_back(allNodes[i]);
                }
        }
}

void UnrootTree::parseNewickStr(string& str) {
	int startPos;
	parseNewickSubStr(str, str.length()-1, startPos, NULL);
	rooted2unrooted(); // change to unrooted tree if it is a rooted tree
}

void UnrootTree::rooted2unrooted() {
	// change a rooted tree to unrooted tree
	int i;
	Node *c, *d;
	int rootID;
	// identify the root, which has exactly two children
	for (i=0; i<allNodes.size(); i++) {
		c = allNodes[i];
		if (c->n[1]!=NULL && c->n[2]==NULL) {
			// this is the root
			// remove the root node from the array
			allNodes.erase(allNodes.begin()+i);
			break;
		}
		c = NULL;
	}
	if (c!=NULL) {
		// the root has been identified
		// move c->n[0]'s pointer from parent to c->n[1]
		d = c->n[0];
		for (i=0; i<3; i++) {
			if (d->n[i] == c) {
				d->n[i] = c->n[1];
				break;
			}
		}
		// move c->n[1]'s pointer from parent to c->n[0]
		d = c->n[1];
		for (i=0; i<3; i++) {
			if (d->n[i] == c) {
				d->n[i] = c->n[0];
				break;
			}
		}
		// delete the root node from the memory
		delete c;		
	}
}

// helping function
Node* UnrootTree::parseNewickSubStr(string& str, int endPos, int& startPos, Node* parent) {
    // input: 1. string; 2. end position of the string; 3. parent node (if available)
    // output: 1. the root of the subtree generated ending at str[endPos];
    //         2. the startPos of the subtree on the string
    //

	removeSpaces(str); // remove all spaces inside the string
    
    if (str.size() == 0)
        return NULL;
    
    // There are three types: starting node, internal node or leaf
    
    int pos;
    double tNumber;
    Node* startNode = NULL;
    Node* interNode = NULL;
    Node* leaf = NULL;
    Node* node1 = NULL;
    Node* node2 = NULL;
    Node* node3 = NULL;
    
    if (str[endPos]==';') {
        // TYPE I : starting node
		// if there are three subnodes, then it is a unrooted tree
        
        // first check whether the tree has weighting
        if (str[endPos-1]==']') {
            // the tree has weighting
            tNumber = getLastDouble(str, endPos-2, pos); // get the weighting
            endPos = pos-2;
        } else {
            endPos--;
        }
		
        // get dist1 if available
        if (isEdgeLen(str, endPos)) {
            tNumber = getLastDouble(str, endPos, pos); // get the edge length
            endPos = pos-2;
        }
		
        if (str[endPos]==')')
            endPos--;
        
		node3 = parseNewickSubStr(str, endPos, pos, startNode);
		
        if (pos>2) {
            // more than one node
			startNode = new Node("");
			allNodes.push_back(startNode);
			startNode->addChild(node3);
			node3->addChild(startNode);
            
            node2 = parseNewickSubStr(str, pos-2, pos, startNode);
            startNode->addChild(node2);
            node2->addChild(startNode);
            
            if (pos>2) {
                // unrooted tree
                node1 = parseNewickSubStr(str, pos-2, pos, startNode);
				startNode->addChild(node1);
            }
        } else {
            // only one node
            startNode = node3;
        }
        
        
        // for starting node, pos should be 1
        if (pos > 1) {
            cerr << "Non-binary tree : " << str << endl << flush;
            exit(1);
        }
        if (pos > 0)
            startPos = pos-1;
        else
            startPos = pos;

        return startNode;
    } else {
        
        // get dist1 if available
        if (isEdgeLen(str, endPos)) {
            tNumber = getLastDouble(str, endPos, pos);
            endPos = pos-2;
        }
        
        // skip the bootstrap value if any
        pos = str.find_last_of("(,)", endPos);
        if (str[pos]==')')
            endPos = pos;
        
        if (str[endPos] == ')') {
            
            // TYPE II : internal node
            
            interNode = new Node("");
            allNodes.push_back(interNode);
            
            node3 = parseNewickSubStr(str, endPos-1, pos, interNode);
            interNode->addChild(node3);
            node3->addChild(interNode);

            node2 = parseNewickSubStr(str, pos-2, pos, interNode);
            interNode->addChild(node2);
            node2->addChild(interNode);

            startPos = pos-1;
            return interNode;
            
        } else {
            
            // TYPE III : leaf
            
            // get the starting position of the leaf
            startPos = str.find_last_of("(,)", endPos);
            if (startPos == string::npos) {
                startPos = 0;
            } else {
                startPos = startPos+1;
            }
            
            // get the leaf name
            string leafName = "";
            if (endPos-startPos+1 > 0)
                leafName = str.substr(startPos, endPos-startPos+1);
            
            Node* leaf = new Node(leafName);
            allNodes.push_back(leaf);
            name2Node.insert(pair<string, Node*>(leafName, leaf));
            totNumTips++;
            
            return leaf;
            
        }
    }
}

void UnrootTree::resetSelect() {
	// reset all "isSelected" to 0
	int i;
	for (i=0; i<allNodes.size(); i++)
		allNodes[i]->isSelected = 0;
}

int UnrootTree::setSelect(string name) {
	// set the leaf with that name to 1, return 1 if the leaf exists, 0 otherwise.
	map<string,Node*>::iterator itr;
	itr = name2Node.find(name);
	if (itr == name2Node.end())
		return 0;
	if (itr->second->isSelected == 0) {
		itr->second->isSelected = 1;
		totNumTipsSelected++;
	}
	return 1;
}

void UnrootTree::computeAllTipsSelected() {
	// compute the allTipsSelected[] for all the nodes
	
	// pick the first internal node
	int i=0;
	while (i<numNodes() && allNodes[i]->isLeaf()) {
		i++;
	}
	if (i >= allNodes.size())
		return;
		
	// compute the number of tips which can be accessed for their each child
	allNodes[i]->calNumTips(NULL, totNumTips);

	// compute the number of selected tips which can be accessed for their each child
	allNodes[i]->calNumTipsSelect(NULL, totNumTipsSelected);
}

bool UnrootTree::checkWhetherSister() {
	// check whether all the selected leaves are in the sister group
	int i, j;
	for (i=0; i<allNodes.size(); i++) {
		if (!allNodes[i]->isLeaf()) {
			bool t = true;
			for (j=0; j<3; j++) {
				if (allNodes[i]->numtipsSelected[j] > 0 && allNodes[i]->numtipsSelected[j] <  allNodes[i]->numtips[j])
					t = false;
			}
			if (t)
				return true;
		}
	}
}

int main(int argc, char** argv) {
	
	if (argc < 4) {
		cout << "JackSum version " << VERSION << endl << endl;
		cout << "By given a set of trees, to check how many of them in which the input species are forming sister groups." << endl; 
		cout << "    Syntax: " << argv[0] << " [tree file] [species 1] [species 2] <species 3> ... <species k>" << endl;
		cout << endl;
		cout << "Note:" << endl; 
		cout << "    - The tree file should contain at least two trees and each tree is with at least 4 tips." << endl;
		cout << "    - This program is designed for unrooted tree. If the input tree is rooted, then the program will" << endl;
		cout << "      convert it into unrooted before analysis." << endl;
		cout << "    - The user has to input at least 2 species." << endl << endl; 

		cout << "Example:" << endl;
		cout << "    To check whether the species: Scer and Smik, form a clade in the trees inside the file 'input.tree.txt'" << endl;
		cout << "         $" << argv[0] << " input.trees.txt Scer Smik" << endl << endl;

		cout << "Contact: Lars Jermiin <Lars.Jermiin@csiro.au>" << endl;
		cout << "         Thomas Wong <Thomas.Wong@anu.edu.au>" << endl;
		exit(1);
	}
	
	// By given a set of trees, to check how many of them in which the input species are forming sister groups
	

	cout << "=======================================" << endl;
	cout << "　　Welcome to JackSum version " << VERSION << endl;
	cout << "=======================================" << endl;
		
		
	// get the set of species
	vector<string> species;
	int i;
	for (i=2; i<argc; i++)
		species.push_back(argv[i]);

	// output the selected species
	cout << "Selected species: ";
	for (i=0; i<species.size(); i++) {
		if (i > 0)
			cout << ", ";
		cout << species[i];
	}
	cout << endl << endl;

	// output the heading
	cout << "Tree \t # of selected species inside \t Whether they are in sister group" << endl;

	// for summary purpose
	int numTreeWithAllSelectedSpecies = 0;
	int numTreeWithAllSpecInSisterGrp = 0;

	int numSelectedTips;
	int isSister;
	int numTrees = 0;

		
	UnrootTree* tree;
	ifstream fin;
	fin.open(argv[1]);
	string aline;
	while (getline(fin, aline)) {
		if (aline.length() > 0) {
			tree = new UnrootTree();
			tree->parseNewickStr(aline);
			numTrees++;

			// for each species, set the select flag for the tree
			for (i=0; i<species.size(); i++) {
				tree->setSelect(species[i]);
			}
			
			// do computation
			tree->computeAllTipsSelected();
			tree->makeText();
			cout << tree->text << "\t";
			numSelectedTips = tree->totNumTipsSelected;
			cout << numSelectedTips << "\t";
			if (numSelectedTips == species.size())
				numTreeWithAllSelectedSpecies++;
			if (numSelectedTips > 1) {
				isSister = tree->checkWhetherSister();
				cout << isSister << endl;
				if (numSelectedTips == species.size() && isSister) {
					numTreeWithAllSpecInSisterGrp++;
				}
			} else {
				cout << 0 << endl;
			}
			
			// delete tree
			delete(tree);

		}
	}
	fin.close();
	
	// show the summary
	cout << endl;
	cout << "===========================" << endl;
	cout << "Summary" << endl;
	cout << "===========================" << endl;
	cout << "Trees in input file                         :\t" << numTrees << endl;
	cout << "Trees with selected species                 :\t" << numTreeWithAllSelectedSpecies << endl;
	cout << "Trees with selected species forming a clade :\t" << numTreeWithAllSpecInSisterGrp << endl;
	cout << "Names of selected species                   :\t";

        // output the selected species
        for (i=0; i<species.size(); i++) {
                if (i > 0)
                        cout << ", ";
                cout << species[i];
        }
        cout << endl << endl;

	return 0;
}
