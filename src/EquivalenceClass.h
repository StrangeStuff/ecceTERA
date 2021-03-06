#ifndef ADJ_CLASS_H_
#define ADJ_CLASS_H_


/*

This file contains a class for adjacency classes

Created the: 24-11-2015
by: Wandrille Duchemin

Last modified the: 13-06-2016
by: Wandrille Duchemin

*/

#include <string>
#include <vector>

#include <Bpp/Exceptions.h>

#include "ReconciledTree.h"
#include "AdjMatrix.h"
#include "AdjTree.h"


using namespace bpp;
using namespace std;

class EquivalenceClass
{
protected:
	int Gfamily1; // id of the first gene family
	int Gfamily2; // id of the second gene family

	vector <string> Lnames1;//list of leaf names of the first gene family
	vector <string> Lnames2;//list of leaf names of the second gene family

	pair <int,int> ancestors; //Id of the ancestors of the EquivalenceClass in the trees of respectively Gfamily1 and Gfamily2


	bool SetAdjMatrix; //false if the AdjMatrix of the EqClass is not set yet
	bool SetAdjForest; //false if the Adj Trees of the EqClass is not set yet

	AdjMatrix * Amat;
	vector <AdjTree * > * AdjForest;



	//protected adders. These are protected because they do not make any check on gfamily name
	void addLname1(string name); // adds a Leaf name to Lnames1
	void addLname2(string name); // adds a Leaf name to Lnames2
	void addAdj(string name1, string name2); // adds a pair of Leaf names
	void addAdj(pair <string, string> names); // adds a pair of Leaf names
	void addAdjList(vector <pair <string,string> > nameList); // adds several pair of leaf names

	//protected removers
	bool removeLname1(int index); // removes an element from Lnames1 at a given index. Returns false if the index is not valid
	bool removeLname1(string name); // removes an element from Lnames1 with a given value. Returns false if the value is not valid
	bool removeLname2(int index); // removes an element from Lnames2 at a given index. Returns false if the index is not valid
	bool removeLname2(string name);  // removes an element from Lnames2 with a given value. Returns false if the value is not valid

	pair <int, int> getHighestCompatibleNodeIds(int NodeId1, int NodeId2, ReconciledTree * Rtree1, ReconciledTree * Rtree2, int Ancestor1, int Ancestor2);

	pair <int, int> getHighestCompatibleNodeIdsStoppingReception(int NodeId1, int NodeId2, ReconciledTree * Rtree1, ReconciledTree * Rtree2, int Ancestor1, int Ancestor2);

public:

	//EquivalenceClass(){}

	EquivalenceClass(int fam1, int fam2);

	~EquivalenceClass()
	{
		//cout << "plop EC" << endl;
		if(SetAdjMatrix)
			delete Amat;
		if(SetAdjForest)
		{
			for ( vector< AdjTree * >::iterator it = AdjForest->begin() ; it != AdjForest->end(); ++it)
		   	{
		    	delete (*it);
		   	}
		   	AdjForest->clear();
		   	delete AdjForest;

		}
			
	}




	//getters
	int getGfamily1() const;
	int getGfamily2() const;
	string getLname1(int index) const;
	string getLname2(int index) const;
	pair <string,string> getAdj(int index) const;
	int getNbAdj() const;

	pair <int,int> getAncestors();
	int getAncestor( int i);


	vector <pair <string, string> > getAdjs() const;

	AdjMatrix * getAmat() const
	{
		return 	Amat;
	}

	vector <AdjTree * > * getAdjForest() const
	{
		return 	AdjForest;
	}

	bool isetAdjMatrix()const{return SetAdjMatrix;}
	bool isetAdjForest()const{return SetAdjForest;}


	//setters
	void setGfamily1( int fam1);
	void setGfamily2( int fam2);
	void setAncestors(pair <int,int> pa);
	void setAncestor( int i, int a);

	void setAmat(AdjMatrix * amat)
	{
		if(Amat!=NULL)
			delete Amat;
		Amat = amat;
		SetAdjMatrix = true;
	}
	
	void setAdjForest(vector <AdjTree * > * Aforest)
	{
		if(SetAdjForest)
		{
			for ( vector< AdjTree * >::iterator it = AdjForest->begin() ; it != AdjForest->end(); ++it)
	   		{
	    		delete (*it);
	   		}
	   		delete AdjForest;
		}
	   	
		AdjForest = Aforest;
		SetAdjForest = true;
	}



	//hasers and finders
	bool hasLname1(string name);
	bool hasLname2(string name);
	int findLname1(string name);// returns -1 if the name is absent
	int findLname2(string name);// returns -1 if the name is absent

	bool hasAdj(string name1, string name2); //checks if the adj exists. works symetrically -> search for name1 in both Lnames1 and Lnames2
	bool hasAdj(pair <string , string> names); //checks if the adj exists. works symetrically -> search for name1 in both Lnames1 and Lnames2
	int findAdj(string name1, string name2); // returns -1 if the adj is absent
	int findAdj(pair <string , string> names); // returns -1 if the adj is absent

	//adders with checks on gfamily names
	bool CheckAddAdj(string name1, string name2, int fam1, int fam2); // adds a pair of Leaf names if fam1 and fam2 correspond to valid Gfamily names 
	bool CheckAddAdj(pair <string, string> names, pair <int, int> fams); // adds a pair of Leaf names if fams correspond to valid Gfamily names 
	bool CheckAddAdjList(vector <string> nameList1, vector<string> nameList2, pair <int, int> fams);
	bool CheckAddAdjList(vector <pair <string,string> > nameList, pair <int, int> fams); // adds several pair of leaf names

	//removers
	bool removeAdj(int index);// removes an element from Lnames1 and Lnames2 at a given index. Returns false if the index is not valid
	bool removeAdj(string name1, string name2);// removes an element from Lnames1 and Lnames2 with given values. Returns false if the values are not valid
	bool removeAdj(pair<string, string> names);// removes an element from Lnames1 and Lnames2 with given values. Returns false if the values are not valid


	//merging and refining
	bool merge( const EquivalenceClass * aClass); // adds the adjs of aClass to self. returns false if the GeneFamily index are differents. Does not change aClass
	
	vector<EquivalenceClass *> refineEqClass(ReconciledTree * Rtree1, ReconciledTree * Rtree2, bool forceLTrefining, bool verbose);
	vector<EquivalenceClass *> refineEqClassWhole(ReconciledTree * Rtree1, ReconciledTree * Rtree2, bool verbose);

	void printMe(bool verbose);

	

	void createAdjMatrix(map<int,vector<float> > speciesC0C1, map<int, map<string,int> > speGeneAdjNb,
							double Gcost, double Bcost, 
							ReconciledTree * rtree1, ReconciledTree * rtree2,
							bool VERBOSE, bool boltzmann , double temp , double absencePenalty);
	void computeAdjMatrix();
	void computeAdjMatrix(double WDupCost, double WLossCost, double WHgtCost);

	bool iscomputedAdjMatrix();
	void backtrackAdjMatrix(ReconciledTree * rtree1, ReconciledTree * rtree2, vector< AdjTree *> * AdjacencyTrees, bool stochastic, bool alwaysGainAtTop = true, double c1proba = 0.5); // creates the adj forest

	void backtrackAdjMatrixForSelf(ReconciledTree * rtree1, ReconciledTree * rtree2, bool stochastic, bool alwaysGainAtTop = true, double c1proba = 0.5, double verbose=false); // creates the adj forest
	int getNbAdjTrees();
	int getNbAdjGain();
	int getNbAdjBreak();


	void clone(const EquivalenceClass * EC);


	void dumpStuff()
	{
		if(SetAdjMatrix)
		{
			delete Amat;
			//cout << "Amat deleted" << endl;
		}
	};
};



#endif