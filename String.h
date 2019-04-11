#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class String {
public:
	char *value;
	int length;

	String();
	String(String &s);
	String(char *value);
	String(char value);
	String(int value);
	String(double value);
	~String();

	String operator+(String s);
	String operator+(int value);
	String operator+(double value);
	String operator+(char value);
	String operator+=(String s);
	String operator+=(int value);
	String operator+=(double value);
	String operator+=(char value);
	String operator--();
	String operator--(int);
	operator char*();
};

