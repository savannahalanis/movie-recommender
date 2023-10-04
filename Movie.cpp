#include "Movie.h"

#include <string>
#include <vector>
using namespace std;

Movie::Movie(const string& id, const string& title, const string& release_year, const vector<string>& directors, const vector<string>& actors, const vector<string>& genres, float rating) {
    m_id = id;
    m_title = title;
    m_releaseYear = release_year;
    m_rating = rating;
    m_directors = directors;
    m_actors = actors;
    m_genres = genres;
}