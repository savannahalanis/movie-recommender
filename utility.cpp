#include "utility.h"
#include "Recommender.h"

bool operator<(const MovieAndRank& m1, const MovieAndRank& m2){
    return m1.movie_id > m2.movie_id;
}