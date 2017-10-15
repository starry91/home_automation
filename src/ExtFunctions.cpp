/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ExtFunctions.cpp
 * Author: praveen
 * 
 * Created on September 6, 2017, 11:47 PM
 */

#include "ExtFunctions.h" 
std::string &ltrim(std::string &s) {
    s.erase(s.begin(),
            find_if_not(s.begin(), s.end(), [](int c) {
                return isspace(c);
            }));
    return s;
}

std::string &rtrim(std::string &s) {
    s.erase(
            find_if_not(s.rbegin(), s.rend(), [](int c) {
                return isspace(c);
            }).base(),
            s.end());
    return s;
}

std::string trim(const std::string &s) {
    std::string t = s;
    return ltrim(rtrim(t));
}

void error(const char *msg) {
    perror(msg);
    exit(1);
}