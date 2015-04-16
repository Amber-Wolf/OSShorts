#include "GeneticData.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

int main(int argc, char *argv[]) {
    std::string refSeq(argv[1]), fileName(argv[2]);
    GeneticData ref = GeneticData(refSeq,"Reference");
    std::filebuf fb;
    fb.open (fileName,std::ios::in);
    std::istream is (&fb);
    std::cout << "Reading data from the input file: " << fileName <<std::endl;
    std::istream_iterator<GeneticData> wordReader(is);
    std::istream_iterator<GeneticData> eof;
    std::vector<GeneticData> genData;
    std::copy(wordReader,eof,std::back_inserter(genData));
    std::cout << "Loaded " << genData.size() << " entries from the input file." << std::endl;
    std::cout << "Finding top-10 best matches with: " << refSeq <<std::endl;
    std::sort(genData.begin(),genData.end(),[&ref](GeneticData a,GeneticData b){return SWComparator(a).getSmithWatermanScore(ref)> SWComparator(b).getSmithWatermanScore(ref);});
    std::for_each(genData.begin(), genData.begin()+10, [](GeneticData a){std::cout<<a<<std::endl;});
}
