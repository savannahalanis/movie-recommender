#ifndef MOVIE_INCLUDED
#define MOVIE_INCLUDED

#include <string>
#include <vector>

class Movie {
  public:
    Movie(const std::string& id, const std::string& title,
          const std::string& release_year,
          const std::vector<std::string>& directors,
          const std::vector<std::string>& actors,
          const std::vector<std::string>& genres, float rating);
    std::string get_id() const {return m_id;}
    std::string get_title() const {return m_title;}
    std::string get_release_year() const {return m_releaseYear;}
    float get_rating() const {return m_rating;}
    std::vector<std::string> get_directors() const {return m_directors;}
    std::vector<std::string> get_actors() const {return m_actors;}
    std::vector<std::string> get_genres() const {return m_genres;}

  private:
    std::string m_id;
    std::string m_title;
    std::string m_releaseYear;
    float m_rating;
    std::vector<std::string> m_directors;
    std::vector<std::string> m_actors;
    std::vector<std::string> m_genres;
};

#endif // MOVIE_INCLUDED
