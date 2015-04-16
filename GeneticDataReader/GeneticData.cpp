#include "GeneticData.h"
#include <cstring>
#include <fstream>

GeneticData::GeneticData(const std::string& seq, const std::string& info) :
    info(info), seq(seq) {
}


/**
   Stream insertion operator to print GeneticData
*/
std::ostream& operator<<(std::ostream& os, const GeneticData& gd) {
    return (os << gd.info << ": " << gd.seq);
}

/**
   Stream extraction operator>> to read GeneticData from FASTA file
   format.
*/
std::istream& operator>>(std::istream& is, GeneticData& gd) {
    std::getline(is, gd.info);
    std::string temp;
    gd.seq.clear();
    while ((is.peek() != EOF) && (is.peek() != '>')) {
        std::getline(is, temp);
        gd.seq += temp;
    }
    return is;
}

SWComparator::SWComparator(const GeneticData& ref) :
    refData(ref), gapPenalty(-1) {
    computeScoreMatrix(1, -1);    
}

int
SWComparator::encodeBase(char c) const {
    switch (c) {
    case 'A' : return 0;
    case 'C' : return 1;
    case 'G' : return 2;
    case 'T' : return 3;
    case 'N' : return 4;
    case 'P' : return 5;
    case 'Y' : return 6;
    case 'R' : return 7;
    case 'W' : return 8;
    case 'S' : return 9;
    case 'K' : return 10;
    case 'M' : return 11;
    case 'D' : return 12;
    case 'V' : return 13;
    case 'H' : return 14;
    case 'B' : return 15;
    case 'X' : return 16;
    }
    return -1;
}

int
SWComparator::getSmithWatermanScore(const GeneticData& otherData) const {
    const std::string& refSeq = refData.getSeq();
    const std::string& other  = otherData.getSeq();
    int numOfRows = refSeq.length();
    int numOfCols = other.length();
    
    std::vector<std::vector<int>> alignMatrix(numOfRows + 1,
                                              std::vector<int>(numOfCols + 1));
    int maxScore = 0;
    // build the Smith-Waterman matrix row by row
    for (int i = 1; i <= numOfRows; i++) {
        for (int j = 1; j <= numOfCols; j++) {
            // Initialize max to the first of the three terms (NORTH).
            const int base1 = encodeBase(refSeq[i - 1]);
            const int base2 = encodeBase(other[j - 1]);
            int innerMax = std::max(0, alignMatrix[i - 1][j] + gapPenalty);
            
            // See if the second term is larger (WEST).
            int west = alignMatrix[i][j - 1] + gapPenalty;
            if (innerMax <= west) {
                innerMax = west;
            }
            
            // See if the third term is the largest (NORTHWEST)
            int northwest = alignMatrix[i - 1][j - 1]
                + scoreMatrix[base1][base2];
            if (innerMax <= northwest) {
                innerMax = northwest;
            }
            
            alignMatrix[i][j] = innerMax;
            maxScore = std::max(maxScore, innerMax);
        }
    }
    return maxScore;
}

void
SWComparator::computeScoreMatrix(const int match, const int mismatch) {
    // We create a 2-D matrix of scores to make alignment faster. The
    // matrix is initialized below in a two-step process to provide
    // maximum compatibility between C++ compilers.  First we
    // introduce m (match) and n (mismatch) homonyms to make the
    // matrix look nice in code.
    const int m = match;
    const int n = mismatch;
    
    const int tmpScoringMatrix[17][17] = {
        //A, C, G, T, N, P, Y, R, W, S, K, M, D, V, H, B, X        
        {m, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n},  //A
        {n, m, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n},  //C
        {n, n, m, n, n, n, n, n, n, n, n, n, n, n, n, n, n},  //G
        {n, n, n, m, n, n, n, n, n, n, n, n, n, n, n, n, n},  //T
        {n, n, n, n, m, n, n, n, n, n, n, n, n, n, n, n, n},  //N
        {n, n, n, n, n, m, n, n, n, n, n, n, n, n, n, n, n},  //P: gap
        {n, m, n, m, n, n, m, n, n, n, n, n, n, n, n, n, n},  //Y: C, T
        {m, n, m, n, n, n, n, m, n, n, n, n, n, n, n, n, n},  //R: A, G
        {m, n, n, m, n, n, n, n, m, n, n, n, n, n, n, n, n},  //W: A, T
        {n, m, m, n, n, n, n, n, n, m, n, n, n, n, n, n, n},  //S: G, C
        {n, n, m, m, n, n, n, n, n, n, m, n, n, n, n, n, n},  //K: T, G
        {m, m, n, n, n, n, n, n, n, n, n, m, n, n, n, n, n},  //M: C, A
        {m, n, m, m, n, n, n, n, n, n, n, n, m, n, n, n, n},  //D: not C
        {m, m, m, n, n, n, n, n, n, n, n, n, n, m, n, n, n},  //V: not T
        {m, m, n, m, n, n, n, n, n, n, n, n, n, n, m, n, n},  //H: not G
        {n, m, m, m, n, n, n, n, n, n, n, n, n, n, n, m, n},  //B: not A
        {n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, m}   //X: unknown
    };

    // Copy the data from local variables to our instance variables.
    std::memcpy(scoreMatrix, tmpScoringMatrix, sizeof(scoreMatrix));
}


