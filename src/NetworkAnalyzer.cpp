#include "../include/NetworkAnalyzer.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

NetworkAnalyzer::NetworkAnalyzer(const vector<User>& users)
        : users(users) {
    calculateFollowing();
}

const User* NetworkAnalyzer::getUserById(int id) const {
    for (const auto& user : users)
        if (user.id == id)
            return &user;
    return nullptr;
}

// =======Count how many users each user follows======
void NetworkAnalyzer::calculateFollowing() {
    for (const auto& user : users) {
        for (const auto& follower : user.followers) {
            followingCount[follower.getId()]++;
        }
    }
}

vector<int> NetworkAnalyzer::intersectVectors(const vector<int>& a,
                                              const vector<int>& b) {
    vector<int> result;
    vector<int> aCopy = a;
    vector<int> bCopy = b;

    sort(aCopy.begin(), aCopy.end());
    sort(bCopy.begin(), bCopy.end());

    ranges::set_intersection(aCopy.begin(), aCopy.end(),
                             bCopy.begin(), bCopy.end(),
                             back_inserter(result));
    return result;
}

// =================Most Influencer=====================
string NetworkAnalyzer::MostInfluencerUser() const {
    stringstream out;
    size_t maxFollowers = 0;

    for (const auto& user : users) {
        maxFollowers = max(maxFollowers, user.followers.size());
    }

    out << "\n========= MOST INFLUENCER USERS =========\n";
    for (const auto& user : users) {
        if (user.followers.size() == maxFollowers) {
            out << "ID: " << user.id
                 << "\nName: " << user.name
                 << "\nNumber of Followers: " << user.followers.size()
                 << "\n-------------------------\n";
        }
    }
    return out.str();
}
//====================Most Active=================
string NetworkAnalyzer::MostActiveUser() const {
    stringstream out;
    int maxActivity = -1;
    for (const auto& user : users) {
        int following = followingCount.count(user.id) ? followingCount.at(user.id) : 0;
        maxActivity = max(maxActivity, following);
    }


    out << "\n========= MOST ACTIVE USER(S) =========\n";

    for (const auto& user : users) {
        int following = followingCount.count(user.id) ? followingCount.at(user.id) : 0;
        if (following == maxActivity) {
            out << "\nID: " << user.id
                 << "\nName: " << user.name
                 << "\nNumber of Following: " << following
                 << "\n------------------------------------";
        }
    }
    return out.str();
}

//====================Mutual followers between N users====================
string NetworkAnalyzer::mutualFollowers(const vector<int>& userIds) {
    stringstream out;

    if (userIds.size() < 2) {
        cout << "Please enter 2 or more users.\n";
        return {};
    }

    vector<vector<int>> followersLists;

    // collect followers of each user
    for (int id : userIds) {
        for (const auto& user : users) {
            if (user.id == id) {
                vector<int> f;
                for (const auto& fol : user.followers)
                    f.push_back(fol.getId());

                followersLists.push_back(f);
                break;
            }
        }
    }

    if (followersLists.empty()) return {};

    // intersection
    vector<int> result = followersLists[0];
    for (size_t i = 1; i < followersLists.size(); i++) {
        result = intersectVectors(result, followersLists[i]);
    }

    if (result.empty()) {
        return "\nThere are no mutual followers.\n";

    }

    for (int id : result) {
        for (const auto& user : users) {
            if (user.id == id)
                out << "\nID: " << user.id
                    << "\nName: " << user.name << "\n";
        }
    }
    return out.str();
}



//===================Suggest users to follow (followers of followers)=====================

string NetworkAnalyzer::suggestUsersToFollow(int userId) {
    vector<int> directFollowers;
    vector<int> suggestions;

    // find target user
    for (const auto& user : users) {
        if (user.id == userId) {
            for (const auto& f : user.followers)
                directFollowers.push_back(f.getId());
            break;
        }
    }

    // followers of followers
    for (int followerId : directFollowers) {
        for (const auto& user : users) {
            if (user.id == followerId) {
                for (const auto& f : user.followers) {
                    // avoid duplicates & avoid suggesting himself
                    if (f.getId() != userId &&
                        find(directFollowers.begin(),
                             directFollowers.end(),
                             f.getId()) == directFollowers.end()) {
                        suggestions.push_back(f.getId());
                    }
                }
            }
        }
    }

    // unique
    sort(suggestions.begin(), suggestions.end());
    suggestions.erase(unique(suggestions.begin(), suggestions.end()),
                      suggestions.end());

    stringstream out;
    out << "SUGGESTED USERS TO FOLLOW\n";

    if (suggestions.empty())
        return out.str() + "None\n";

    for (int id : suggestions) {
        for (const auto& user : users) {
            if (user.id == id)
                out << "\nID: " << user.id
                    << "\nName: " << user.name << "\n";
        }
    }
    return out.str();
}