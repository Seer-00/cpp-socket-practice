#pragma once

#include <string>
#include <qstring>
#include <qdebug.h>
#include <qmessagebox.h>
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")

#define SCLOSED		"Server CLOSED."
#define LOCAL_IP	"127.0.0.1"
#define LOCAL_PORT	1234
#define BUF_LEN		1024 * 8
#define SIZET		sizeof(size_t)

using namespace std;

/* ------------- Supplement Functions ------------- */

QString str2qstr(const string str);
string qstr2str(const QString qstr);
void str_to_arr(string in, char buf[]);


