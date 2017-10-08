/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ExtFunctions.h
 * Author: praveen
 *
 * Created on September 6, 2017, 11:47 PM
 */

#ifndef EXTFUNCTIONS_H
#define EXTFUNCTIONS_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <algorithm>
#include <unistd.h>

std::string &ltrim(std::string &s);

std::string &rtrim(std::string &s);

std::string trim(const std::string &s);

void error(const char *msg);

#endif /* EXTFUNCTIONS_H */

