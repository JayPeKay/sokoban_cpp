#include "String.h"

String::String() {
	this->value = (char *)malloc(1);
	this->length = 0;
}

String::String(char *value) {
	this->value = (char *)malloc(strlen(value));
	this->value = value;
	this->length = strlen(value);
}

String::String(char value) {
	this->value = (char *)malloc(2);
	this->value[0] = value;
	this->value[1] = '\0';
	this->length = 1;
}

String::String(int value) {
	char buffer[30];
	sprintf(buffer, "%d", value);
	this->length = strlen(buffer);
	this->value = (char *)malloc(length);
	strcpy(this->value, buffer);
}

String::String(double value) {
	char buffer[30];
	sprintf(buffer, "%.3f", value);
	this->length = strlen(buffer);
	this->value = (char *)malloc(length);
	strcpy(this->value, buffer);
}

String::String(String &s) {
	this->value = (char *)malloc(strlen(s.value) + 1);
	strcpy(this->value, s.value);
	this->length = s.length;
}

String::~String() {
	free(this->value);
}

String String::operator+(String s) {
	char *temp = (char *)malloc(length + s.length + 1);
	strcpy(temp, value);
	strcat(temp, s.value);
	String r = temp;
	free(temp);

	return r;
}

String String::operator+(int value) {
	String t(value);
	return *this + t;
}

String String::operator+(double value) {
	String t(value);
	return *this + t;
}

String String::operator+(char value) {
	String t(value);
	return *this + t;
}

String String::operator+=(String s) {
	char *temp = value;
	length = length + s.length;
	value = (char *)malloc(length + s.length + 1);
	strcpy(value, temp);
	strcat(value, s.value);
	free(temp);

	return *this;
}

String String::operator+=(int value) {
	String t(value);
	return *this += t;
}

String String::operator+=(double value) {
	String t(value);
	return *this += t;
}

String String::operator+=(char value) {
	String t(value);
	return *this += t;
}

String String::operator--() {
	if(length > 0) {
		this->length--;
		this->value[length] = '\0';
	}

	return *this;
}

String String::operator--(int) {
	String temp = String(*this);
	operator--();

	return temp;
}

String::operator char *() {
	return this->value;
}