#include "../include/NetworkAnalyzer.hpp"

#include <algorithm>
#include <iostream>

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
void NetworkAnalyzer::MostInfluencerUser() const {
    size_t maxFollowers = 0;

    for (const auto& user : users) {
        maxFollowers = max(maxFollowers, user.followers.size());
    }

    cout << "\n========= MOST INFLUENCER USERS =========\n";
    for (const auto& user : users) {
        if (user.followers.size() == maxFollowers) {
            cout << "ID: " << user.id
                 << "\nName: " << user.name
                 << "\nNumber of Followers: " << user.followers.size()
                 << "\n-------------------------\n";
        }
    }
}
//====================Most Active=================
void NetworkAnalyzer::MostActiveUser() const {
    int maxActivity = -1;
    for (const auto& user : users) {
        int following = followingCount.count(user.id) ? followingCount.at(user.id) : 0;
        int activity = user.followers.size() + following;
        maxActivity = max(maxActivity, activity);
    }


    cout << "\n========= MOST ACTIVE USER(S) =========\n";

    for (const auto& user : users) {
        int following = followingCount.count(user.id) ? followingCount.at(user.id) : 0;
        int activity = user.followers.size() + following;

        if (activity == maxActivity) {
            cout << "\nID: " << user.id
                 << "\nName: " << user.name
                 << "\nNumber of Followers: " << user.followers.size()
                 << "\nNumber of Following: " << following
                 << "\nActivity (Followers + Following): " << activity
                 << "\n------------------------------------";
        }
    }
}

//====================Mutual followers between N users====================
vector<User> NetworkAnalyzer::mutualFollowers(const vector<int>& userIds) {

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
        cout << "\nThere are no mutual followers.\n";
        return {};
    }

    // convert IDs → User objects
    vector<User> mutualUsers;
    for (int id : result) {
        for (const auto& user : users) {
            if (user.id == id) {
                mutualUsers.push_back(user);
                break;
            }
        }
    }
    cout<<"\n========= Mutual Users =========\n";
    return mutualUsers;
}



//===================Suggest users to follow (followers of followers)=====================

vector<User> NetworkAnalyzer::suggestUsersToFollow(int userId) {
    cout <<"\n========= Suggested Users =========\n";
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

    // convert IDs → Users
    vector<User> result;
    for (int id : suggestions) {
        for (const auto& user : users) {
            if (user.id == id) {
                result.push_back(user);
                break;
            }
        }
    }
    // If no suggestions, add "none" to the list
    if (result.empty()) {
        cout<<"None";
    }

    return result;
}