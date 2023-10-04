#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include "treemm.h"
#include <string>
#include <vector>

class Movie;

class MovieDatabase {
  public:
    MovieDatabase();
    ~MovieDatabase();
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;

  private:
    void stringToVector(std::string s, std::vector<std::string>& upper, std::vector<std::string>& lower);
    TreeMultimap<std::string, Movie*> m_IdDB;
    TreeMultimap<std::string, Movie*> m_DirectorDB;
    TreeMultimap<std::string, Movie*> m_ActorDB;
    TreeMultimap<std::string, Movie*> m_GenreDB;
    std::vector<std::string> m_allMovies;
};

#endif // MOVIEDATABASE_INCLUDED
