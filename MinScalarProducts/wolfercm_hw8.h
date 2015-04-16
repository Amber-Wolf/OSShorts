#ifndef WOLFERCM_HW8_8_
#define WOLFERCM_HW8_8_
#include <vector>
#include <thread>

using namespace std;

class MSPThr{

 public:

  MSPThr();
  ~MSPThr();
  void print(vector<int>);
  void findScalarProduct();
  int calcScalar(vector<int>&);


  vector<int> constant;  //Vector that does not change
  vector<int> change;  //Vector whose permutations are looped through
  int permutationNumber;  // The curent permutation the change vector is on.
  int endPermutationNumber;  //The last permutation number that needs to be checked.
  vector<int> best;  //The best version of the change vector.
		//This change vecotr produces the lowest scalar product with the constant vector.
  int bestVal;  //the lowest scalar product for the checked values so far.
  
  bool operator<(MSPThr& other);
  bool operator>(MSPThr& other);

 protected:

  

};


class wolfercm_hw8{

 public:

  wolfercm_hw8();
  ~wolfercm_hw8();
  thread operator()(MSPThr& mspthr);

 protected:

  thread makeMSPThr(MSPThr&);
  void sortAll();
  void getParams();
  int calcScalar(vector<int>&); 
  int numThreads;  //Number of threads to be used.
  int vecSize;   //size of the vector
  void findMinScalarProduct();   
  int totalPermutations;        //Total number of permutations possible.
  int currentPermutationCount;  //The current permutation mumber
			   //Used to assign threads a number of permutations
  int factorial(int n);

  vector<int> base1;
  vector<int> base2;
};


#endif
