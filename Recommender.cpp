#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "Movie.h"
#include "User.h"
#include "utility.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iostream>

using namespace std;

Recommender::Recommender(const UserDatabase& user_database, const MovieDatabase& movie_database) {
    m_UDB = &user_database;
    m_MDB = &movie_database;
    DIRECTORSCORE = 20;
    ACTORSCORE = 30;
    GENRESCORE = 1;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const {
    User* user = m_UDB->get_user_from_email(user_email);
    if(user == nullptr) {
        vector<MovieAndRank> empty;
        return empty;
    }

    vector<string> w_vectorHistory = user->get_watch_history();
    unordered_map<string, int> w_directors;
    unordered_map<string, int> w_actors;
    unordered_map<string, int> w_genres;
    unordered_map<string, int> recommendedMovies;
    vector<MovieAndRank> vector_rMovies;

    // gather the directors, actors, and genres a user has watched
    createUnorderedMapOfWatchedElements(w_vectorHistory, recommendedMovies, w_directors, w_actors, w_genres);

    // gather movies that share directors, actors, and genres from the watch history
    // uses unordered map to ensure recommendedMovies has no duplicates
    gatherRecommendedMovies(w_directors, recommendedMovies, w_directors, w_actors, w_genres, &MovieDatabase::get_movies_with_director);
    gatherRecommendedMovies(w_actors, recommendedMovies, w_directors, w_actors, w_genres, &MovieDatabase::get_movies_with_actor);
    gatherRecommendedMovies(w_genres, recommendedMovies, w_directors, w_actors, w_genres, &MovieDatabase::get_movies_with_genre);

    // puts movies map of recommended movies into vector so they can be sorted by compatibility score later
    for(unordered_map<string, int>::iterator it = recommendedMovies.begin(); it != recommendedMovies.end(); it++) {
        if (it->second != -1) {
            MovieAndRank mar(it->first, it->second);
            vector_rMovies.push_back(mar);
        }
    }

    // calculates order of recommendation, based on compatibility, ratings, and alphabetical order
    sort(vector_rMovies.begin(), vector_rMovies.end(), [&](const MovieAndRank& a, const MovieAndRank& b)->bool {
        // returns comparision based on compatibility score, if different
        if(a.compatibility_score > b.compatibility_score) {
            return true;
        } else if (a.compatibility_score < b.compatibility_score) {
            return false;
        }

        // returns comparision based on rating, if different
        float a_rating = m_MDB->get_movie_from_id(a.movie_id)->get_rating();
        float b_rating = m_MDB->get_movie_from_id(b.movie_id)->get_rating();
        if(a_rating > b_rating) {
            return true;
        } else if(a_rating < b_rating) {
            return false;
        }

        // returns comparision based on alphabetical order
        string a_title = m_MDB->get_movie_from_id(a.movie_id)->get_title();string b_title = m_MDB->get_movie_from_id(b.movie_id)->get_title();
        if(a_title < b_title)
            return true;
        return false;
    });

    /*
    for(vector<MovieAndRank>::iterator iteratorr = vector_rMovies.begin(); iteratorr != vector_rMovies.end(); iteratorr++) {
    }
    */

    // makes a vector with the desired amount of movie recommendations
    vector<MovieAndRank> recommendations;
    vector<MovieAndRank>::iterator it = vector_rMovies.begin();
    for(int i = 0; i < movie_count && it != vector_rMovies.end(); i++, it++)
        // checks to make sure it doesn't return already watched movies
        if(it->compatibility_score != -1)
            recommendations.push_back(*it);

    return recommendations; 
}

void Recommender::createUnorderedMapOfWatchedElements(vector<string> watch_history, unordered_map<string, int>& movies, unordered_map<string, int>& watched_d, unordered_map<string, int>& watched_a, unordered_map<string, int>& watched_g) const {
    for(vector<string>::iterator it = watch_history.begin(); it != watch_history.end(); it++) {
        Movie* movie = m_MDB->get_movie_from_id(*it);
        movies[movie->get_id()] = -1;

        // puts watched directors into director set
        vector<string> directors = movie->get_directors();
        for(vector<string>::iterator d_it = directors.begin(); d_it != directors.end(); d_it++) {
            if(watched_d.find(*d_it) == watched_d.end())
                watched_d[*d_it] = 1;
            else
                watched_d[*d_it]++;
        }

        // puts watched actors into actor set
        vector<string> actors = movie->get_actors();
        for(vector<string>::iterator a_it = actors.begin(); a_it != actors.end(); a_it++) {
            if(watched_a.find(*a_it) == watched_a.end())
                watched_a[*a_it] = 1;
            else
                watched_a[*a_it]++;
        }

        // puts watched genres into genre set
        vector<string> genres = movie->get_genres();
        for(vector<string>::iterator g_it = genres.begin(); g_it != genres.end(); g_it++) {
            if(watched_g.find(*g_it) == watched_g.end())
                watched_g[*g_it] = 1;
            else
                watched_g[*g_it]++;
        }
    }
}


void Recommender::gatherRecommendedMovies(std::unordered_map<std::string, int>& search, unordered_map<string, int>& movies, unordered_map<string, int>& watched_d, unordered_map<string, int>& watched_a, unordered_map<string, int>& watched_g, vector<Movie*> (MovieDatabase::*gatherMovies)(const string&) const) const {
    // function gathers recommened movies and calculates recommendation score
    
    // for all the directors/actors/genres
    for (unordered_map<string, int>::iterator searchIt = search.begin(); searchIt != search.end(); searchIt++) {

        // gathers all other movies by the directors/actors/genres
        vector<Movie*> searchedMovies = (m_MDB->*gatherMovies)(searchIt->first);

        // for all the movies by the directors/actors/genres
        for (vector<Movie*>::iterator smIt = searchedMovies.begin(); smIt != searchedMovies.end(); smIt++) {
            int score = 0;

            if (movies.find((*smIt)->get_id()) == movies.end()) {
                // for all the directors of that movie
                vector<string> movieDirectors = (*smIt)->get_directors();
                for(vector<string>::iterator d_smIt = movieDirectors.begin(); d_smIt != movieDirectors.end(); d_smIt++) {
                    // for all the directors in the watched director set
                    for(unordered_map<string, int>::iterator d_it = watched_d.begin(); d_it != watched_d.end(); d_it++) {
                        // if any two are the same, add DIRECTORSCORE to the score total
                        if(d_it->first == *d_smIt)
                            score += watched_d[d_it->first] * DIRECTORSCORE;
                    }
                }

                // for all the actors of that movie
                vector<string> movieActors = (*smIt)->get_actors();
                for(vector<string>::iterator a_smIt = movieActors.begin(); a_smIt != movieActors.end(); a_smIt++) {
                    // for all the actors in the watched actor set
                    for(unordered_map<string, int>::iterator a_it = watched_a.begin(); a_it != watched_a.end(); a_it++) {
                    // if any two are the same, add ACTORSCORE to the score total
                        if(a_it->first == *a_smIt)
                            score += watched_a[a_it->first] * ACTORSCORE;
                    }
                }

                // for all the genres of that movie
                vector<string> movieGenres = (*smIt)->get_genres();
                for(vector<string>::iterator g_smIt = movieGenres.begin(); g_smIt != movieGenres.end(); g_smIt++) {
                   // for all the genres in the watched genre set
                   for(unordered_map<string, int>::iterator g_it = watched_g.begin(); g_it != watched_g.end(); g_it++) {
                        // if any two are the same, add GENRESCORE to the score total
                        if(g_it->first == *g_smIt)
                            score += watched_g[g_it->first] * GENRESCORE;
                   }
               }
            
                // adds the movie to the recommended movie vector
                movies[(*smIt)->get_id()] = score;
            }
        }
    }
}

