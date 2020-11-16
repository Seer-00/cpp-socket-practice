#pragma once

#include <list>
#include <string>
#include <qstring>
#include <qdebug.h>
#include <winsock.h>

using namespace std;

/* ------------- Supplement Functions ------------- */

QString str2qstr(const string str);
string qstr2str(const QString qstr);
void str_to_arr(string in, char buf[]);


