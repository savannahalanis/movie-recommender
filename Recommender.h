#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

class UserDatabase;
class MovieDatabase;
class Movie;

struct MovieAndRank {
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score) { }

    std::string movie_id;
    int compatibility_score;
};

class Recommender {
  public:
    Recommender(const UserDatabase& user_database, const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email, int movie_count) const;

  private:
    void createUnorderedMapOfWatchedElements(std::vector<std::string> watch_history, std::unordered_map<std::string, int>& movies, std::unordered_map<std::string, int>& watched_d, std::unordered_map<std::string, int>& watched_a, std::unordered_map<std::string, int>& watched_g) const;

    void gatherRecommendedMovies(std::unordered_map<std::string, int>& search, std::unordered_map<std::string, int>& movies, std::unordered_map<std::string, int>& watched_d, std::unordered_map<std::string, int>& watched_a, std::unordered_map<std::string, int>& watched_g, std::vector<Movie*> (MovieDatabase::*gatherMovies)(const std::string&) const) const;

    int DIRECTORSCORE;
    int ACTORSCORE;
    int GENRESCORE;

    const UserDatabase* m_UDB;
    const MovieDatabase* m_MDB;
};

#endif // RECOMMENDER_INCLUDED
