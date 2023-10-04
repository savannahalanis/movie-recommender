#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

MovieDatabase::MovieDatabase() {
    // need to use load() to populate database
}

MovieDatabase::~MovieDatabase() {
    // deletes all dynamically allocated objects
    vector<string>::iterator it = m_allMovies.begin();
    while(it != m_allMovies.end()) {
        Movie* ptr = get_movie_from_id(*it);
        delete ptr;
        it = m_allMovies.erase(it);
    }
}

bool MovieDatabase::load(const string& filename) {
       ifstream infile(filename);
    if (!infile) 
        return false;

    // keeps track of what element the program is on while reading in the movie list txt file
    bool isID = true;
    bool isTitle = false, isYear = false, isDirector = true, isActor = false, isGenre = false, isRating = false, isBlank = false;

    string id, title, year;
    float rating;
    vector<string> directors, lowerDirectors;
    vector<string> actors, lowerActors;
    vector<string> genres, lowerGenres;

    string s;

    while(getline(infile, s)) {
        // inserts lines from file into corresponding elements in accordance with the proper format
        if (isID) {
            for(int i = 0; i < s.size(); i++)
                id += tolower(s.at(i));
            isID = false;
            isTitle = true;
        } else if (isTitle) {
            title = s;
            isTitle = false;
            isYear = true;
        } else if (isYear) {
            year = s;
            isYear = false;
            isDirector = true;
        } else if (isDirector) {
            stringToVector(s, directors, lowerDirectors);
            isDirector = false;
            isActor = true;
        } else if (isActor) {
            stringToVector(s, actors, lowerActors);
            isActor = false;
            isGenre = true;
        } else if (isGenre) {
            stringToVector(s, genres, lowerGenres);
            isGenre = false;
            isRating = true;
        } else if (isRating) {
            rating = stof(s);

            // inserts movies into 4 binary search trees based on id, directors, actors, and genres
            Movie* moviePtr = new Movie(id, title, year, directors, actors, genres, rating);
            m_IdDB.insert(id, moviePtr);

            for (vector<string>::iterator it = lowerDirectors.begin(); it != lowerDirectors.end(); it++) {
                m_DirectorDB.insert(*it, moviePtr);
            }
            for (vector<string>::iterator it = lowerActors.begin(); it != lowerActors.end(); it++) {
                m_ActorDB.insert(*it, moviePtr);
            }
            for (vector<string>::iterator it = lowerGenres.begin(); it != lowerGenres.end(); it++) {
                m_GenreDB.insert(*it, moviePtr);
            }

            isRating = false;
            isBlank = true;
        } else if (isBlank) {
            // clearing the vectors for next use
            vector<string>::iterator it = directors.begin();
            while(it != directors.end())
                it = directors.erase(it);
            it = lowerDirectors.begin();
            while(it != lowerDirectors.end())
                it = lowerDirectors.erase(it);

            it = actors.begin();
            while(it != actors.end())
                it = actors.erase(it);
            it = lowerActors.begin();
            while(it != lowerActors.end())
                it = lowerActors.erase(it);

            it = genres.begin();
            while(it != genres.end())
                it = genres.erase(it);
            it = lowerGenres.begin();
            while(it != lowerGenres.end())
                it = lowerGenres.erase(it);
            
            // cleanup
            id = "";
            isBlank = false;
            isID = true;
        }
    }
    return true;
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const {
    // looks through the movie id binary search tree to see the movie associated with the ID
    string lowerId;
    for(int i = 0; i < id.size(); i++)
        lowerId += tolower(id.at(i));
    TreeMultimap<string, Movie*>::Iterator it = m_IdDB.find(lowerId);
    if (!it.is_valid())
        return nullptr;
    return it.get_value();
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const {
    // looks through the director binary search tree to return a pointer vector of movies associated with a director
    string lowerDirector;
    for (int i = 0; i < director.size(); i++)
        lowerDirector += tolower(director.at(i));

    TreeMultimap<string, Movie*>::Iterator it = m_DirectorDB.find(lowerDirector);
    vector<Movie*> movies;
    while(it.is_valid()) {
        movies.push_back(it.get_value());
        it.advance();
    }
    return movies;
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const {
    // looks through the actor binary search tree to return a pointer vector of movies associated with a actor
    string lowerActor;
    for (int i = 0; i < actor.size(); i++)
        lowerActor += tolower(actor.at(i));
    
    TreeMultimap<string, Movie*>::Iterator it = m_ActorDB.find(lowerActor);
    vector<Movie*> movies;
    while(it.is_valid()) {
        movies.push_back(it.get_value());
        it.advance();
    }
    return movies;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const {
    // looks through the genre binary search tree to return a pointer vector of movies associated with a genre
    string lowerGenre;
    for (int i = 0; i < genre.size(); i++)
        lowerGenre += tolower(genre.at(i));

    TreeMultimap<string, Movie*>::Iterator it = m_GenreDB.find(lowerGenre);
    vector<Movie*> movies;
    while(it.is_valid()) {
        //cerr << "movie in movies vector - " << it.get_value() << endl;
        movies.push_back(it.get_value());
        it.advance();
    }
    return movies;
}

void MovieDatabase::stringToVector(string s, vector<string>& upper, vector<string>& lower) {
    string tempUpper, tempLower;
    int counter = 0;
    for(int i = 0; i < s.size(); i++, counter++) {
        if (s.at(i) == ',') { 
            upper.push_back(tempUpper);
            lower.push_back(tempLower);
            tempUpper = "";
            tempLower = "";
        } else if (i == s.size()-1) {
            tempUpper += (s.at(i));
            tempLower += (tolower(s.at(i)));
            upper.push_back(tempUpper);
            lower.push_back(tempLower);
            tempUpper = "";
            tempLower = "";
        } else {
            tempUpper += (s.at(i)); 
            tempLower += (tolower(s.at(i)));
        }
    }
}