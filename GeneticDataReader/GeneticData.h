#include <algorithm>
#include <unistd.h>
#include <iostream>
#include <iterator>

/**
   A simple class to uniquely identify and tag strings that contain
   genomic data consiting of the characters ATCG.
*/
class GeneticData {
    friend std::ostream& operator<<(std::ostream& os, const GeneticData& gd);
    friend std::istream& operator>>(std::istream& is, GeneticData& gd);
private:
    std::string info;  // FASTA header
    std::string seq;   // Actual sequence
public:
    GeneticData() {}
    GeneticData(const std::string& seq, const std::string& info = "");    
    ~GeneticData() {}

    const std::string& getSeq()  const { return seq; }
    const std::string& getInfo() const { return info; }    
};


/**
   A comparator class that can be used to compare a given genetic
   data against a reference sequence.  This comparator uses the
   Smith-Waterman algorithm to determine similarity between two
   genetic sequences.  See
   http://en.wikipedia.org/wiki/Smith%E2%80%93Waterman_algorithm for
   additional details.
*/
class SWComparator {
private:
    // The reference sequence against which comparisons must be
    // performed.
    const GeneticData& refData;
    // Scoring matrix to provide rapid look-up of scores
    int scoreMatrix[17][17];
    // The gap penalty
    const int gapPenalty;
public:
    // Simple constructor to initialize reference sequence
    SWComparator(const GeneticData& refSeq);
    ~SWComparator() {}

    /**
       Runs Smith-Waterman local alignment algorithms to measure
       similarity between the reference sequence (refSeq) and the
       other genetic string.
       
       \return This method returns a Smith-Waterman score.
    */
    int getSmithWatermanScore(const GeneticData& other) const;

    /**
       A simple functor that can be used to determine if gs1 is a
       better match to the reference sequence than gs2.

       \return This method returns true if gs1 has a better
       Smith-Waterman score than gs2.
    */
    bool operator()(const GeneticData& gs1,
                    const GeneticData& gs2) const {
        return getSmithWatermanScore(gs1) > getSmithWatermanScore(gs2);
    }

protected:
    int encodeBase(char c) const;
    void computeScoreMatrix(const int match, const int mismatch);
};
