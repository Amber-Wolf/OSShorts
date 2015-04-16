#include "wolfercm_hw8.h"
#include <iostream>
#include <iterator>
#include <string>
#include <algorithm>    // std::copy
#include <thread>
using namespace std;

//Drives the MinScalar object
wolfercm_hw8::wolfercm_hw8(){
  currentPermutationCount = 0;
  getParams();  //gets parameters from the user.
  findMinScalarProduct();  //Finds the minimum scalar product
}

wolfercm_hw8::~wolfercm_hw8(){
  
}

//returns n! where the given integer is n.
int wolfercm_hw8::factorial(int n){
  int result = 1;
  for(; n>1; n--){
    result = result * n;
  }
  return result;
}

//Instantiates a MSPThr with the correct data.
//Runs it to find the Min Scalar Product as a seperate thread.
//Returns the thread.
thread  wolfercm_hw8::operator()(MSPThr& mspthr){
  int addPerm = totalPermutations / numThreads;
  mspthr.permutationNumber = currentPermutationCount;
  currentPermutationCount += addPerm;
  if((currentPermutationCount + addPerm)>totalPermutations){
    currentPermutationCount = totalPermutations;
  }
  mspthr.endPermutationNumber = currentPermutationCount;
  mspthr.constant = base2;
  mspthr.change = base1;
  return thread([&mspthr](){
      mspthr.findScalarProduct();
    });
}

void wolfercm_hw8::findMinScalarProduct() {
    // Create helper classes that run on separate threads to compute
    // subset of permutations (using values already read from user)
    vector<MSPThr>      mspList(numThreads);
    vector<thread> thrList(numThreads);
    // Generate threads using functor API of MinScalarProd
    transform(mspList.begin(), mspList.end(), thrList.begin(), *this);
    // Wait for threads to finish
    for_each(thrList.begin(), thrList.end(), std::mem_fn(&std::thread::join));
    //Compute minimum scalar product from all threads using operator< defined on MSPThr
    MSPThr msp = *std::min_element(mspList.begin(), mspList.end());
    //Print final results
    msp.print(base2);
}

//sorts all the given vector arrays.
void wolfercm_hw8::sortAll(){
  sort(base1.begin(),base1.end());
  sort(base2.begin(),base2.end());
}

//Unused code that calculates the inner product incorrectly.
int wolfercm_hw8::calcScalar(vector<int>& comparator){
  int init = 0;
  inner_product(comparator.begin(), comparator.end(),
		base1.begin(), init);
  return init;
}

//Gets the parameters from the user.
void wolfercm_hw8::getParams(){
  cout << "Enter vector size" << endl;
  cin >> vecSize;  //Gets the vector size
  base1 = vector<int>();
  base2 = vector<int>();
  cout << "Enter data for vector1:" << endl;
  for(int x =0; x<vecSize;x++){  //Gets the input for the first vector
    int place=0;
    cin >> place;
    base1.push_back(place);
  } 
  cout << "Enter data for vector2:" << endl;  //Gets the input for the second vector
  for(int x =0; x<vecSize;x++){
    int place=0;
    cin >> place;
    base2.push_back(place);
  } 
  cout<< "Enter thread count." << endl; //Gets the number of threads wishe to be utlized.
  //cin.clear();
  cin >> numThreads;
  totalPermutations = factorial(vecSize);
  if(numThreads > totalPermutations){
    numThreads = totalPermutations;
  }
}

MSPThr::MSPThr(){

}

MSPThr::~MSPThr(){

}

//Prints both vectors, takes the otherVector that was considered constant
void MSPThr::print(vector<int> otherVec){
  cout << "vec1: [";
  for_each(otherVec.begin(),otherVec.end()-1,[](int i){ cout  << i << ", ";});
  cout << otherVec[otherVec.size()-1] <<"]"<< endl;
  cout << "vec2: [";
  for_each(best.begin(),best.end()-1,[](int i){ cout << i<< ", ";});
  cout << best[best.size()-1]<< "]" << endl;
  cout << "Scalar Product : " << bestVal << endl;
}

//Finds the minimum scalar product in the range
void MSPThr::findScalarProduct(){
  for(int x=0; x<permutationNumber; x++){
    next_permutation(change.begin(), change.end());  //gets the permutations to the range that should be checked
  } 
  bestVal = inner_product(change.begin(), change.end(),
		constant.begin(), 0);  //Setd the initial best value;
  best = change;
  for(; permutationNumber<endPermutationNumber; permutationNumber++){
    next_permutation(change.begin(), change.end());  //Loops through the permutations and finds the best.
    int calcVal =  inner_product(change.begin(), change.end(),
		constant.begin(), 0);
    if(bestVal > calcVal){
      bestVal = calcVal;
      best = change;
    }
  }
}

bool MSPThr::operator<(MSPThr& other){
  return (bestVal < other.bestVal);
}

bool MSPThr::operator>(MSPThr& other){
  return (bestVal > other.bestVal);
}

int main(){  //runs the driver
  wolfercm_hw8 driver;
  return 0;
}


