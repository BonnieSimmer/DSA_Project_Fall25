//
// Created by bonniesimmer on 12/22/25.
//

#ifndef DSA_PROJECT_FALL25_APPMODE_H
#define DSA_PROJECT_FALL25_APPMODE_H

enum class AppMode {
    GUI,
    CLI
};

extern AppMode currentMode;

void reportError(const std::string& title, const std::string& message);
void reportInfo(const std::string& title, const std::string& message);

#endif //DSA_PROJECT_FALL25_APPMODE_H