#include "Supplement.h"

/* ------------- Supplement Functions ------------- */

QString str2qstr(const string str)
{
	return QString::fromLocal8Bit(str.data());
}

string qstr2str(const QString qstr)
{
	QByteArray cdata = qstr.toLocal8Bit();
	return string(cdata);
}

void str_to_arr(string in, char buf[])
{
	u_short offset = sizeof(size_t);		// offset when copying
	size_t in_len = in.length() + 1;		// +1 for '/0'
	memcpy(buf, &in_len, sizeof(in_len));	// add data_len at first sizeof(size_t) byte 
	for (size_t i = 0; i < in_len; ++i) {
		buf[i + offset] = in[i];			// copy chars from string to buf[]
	}
}


