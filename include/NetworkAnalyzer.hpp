//
// Created by Admin on 12/21/2025.
//

#ifndef DSA_PROJECT_FALL25_NETWORKANALYZER_H
#define DSA_PROJECT_FALL25_NETWORKANALYZER_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "XMLParsser.hpp"

using namespace std;

class NetworkAnalyzer {

private:
    vector<User> users;

    // userId -> following count
    unordered_map<int, int> followingCount;

    void calculateFollowing();
    vector<int> intersectVectors(const vector<int> &a, const vector<int> &b);
    const User* getUserById(int id) const;

public:
    explicit NetworkAnalyzer(const vector<User>& users);

    void MostInfluencerUser() const;
    void MostActiveUser() const;

    vector<User> mutualFollowers(const vector<int> &userIds);
    vector<User> suggestUsersToFollow(int userId);

};


#endif //DSA_PROJECT_FALL25_NETWORKANALYZER_H