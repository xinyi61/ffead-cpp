/*
	Copyright 2009-2020, Sumeet Chhetri

    Licensed under the Apache License, Version 2.0 (const the& "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
/*
 * CastUtil.h
 *
 *  Created on: Aug 5, 2012
 *      Author: Sumeet
 */

#ifndef CASTUTIL_H_
#define CASTUTIL_H_
#include "sstream"
#include <stdlib.h>
#include "StringUtil.h"
#include <cxxabi.h>
#include "cstring"
#include <stdio.h>
#include <assert.h>
#include "ThreadLocal.h"
#include "map"

typedef std::basic_ostringstream<char> fcpstream;
typedef std::map<std::string, void*> Context;
typedef void (*TemplatePtr) (Context*, std::string&);
typedef void (*TemplatePtrStream) (Context*, fcpstream&);

class CastUtil {
	static ThreadLocal _mcMap;
	template <typename T> static void primitive(const T& val, const char* fmt, std::string* d)
	{
		int n = snprintf(NULL, 0, fmt, val);
		char* ty = (char*)malloc((n + 5) * sizeof(char));
		/*int c = */snprintf(ty, n+1, fmt, val);
		//assert(strlen(ty)==n);
		//assert(c == n);
		d->append(ty, strlen(ty));
		free(ty);
	}

	static void numToStr(unsigned long long val, std::string* d) {
	    char ty[30];
		sprintf(ty, "%llu", val);
		d->append(ty, strlen(ty));
	}
public:
	static const std::string STD_STRING;
	static const std::string BOOL_TRUE;
	static const std::string BOOL_FALSE;

	static void clear();

	template <typename T> static std::string getClassName(T& t)
	{
		const char *mangled = typeid(t).name();
		std::string sm(mangled);
		if(_mcMap.get()==NULL) {
			_mcMap.set(new std::map<std::string, std::string>);
		}
		std::map<std::string, std::string>* _mangledClassNameMap = (std::map<std::string, std::string>*)_mcMap.get();
		std::map<std::string, std::string>::iterator it = _mangledClassNameMap->find(sm);
		if(it!=_mangledClassNameMap->end()) {
			std::string tn1 = it->second;
			if(tn1[tn1.length()-1]=='*')
				tn1 = tn1.substr(0,tn1.length()-1);
			return tn1;
		}
		int status;
		char *demangled = abi::__cxa_demangle(typeid(t).name(), 0, 0, &status);
		std::string tn(demangled);
		free(demangled);
		if(tn.find("std::__1::")!=std::string::npos) {
			StringUtil::replaceAll(tn, "std::__1::", "std::");
			if(tn.find("std::basic_string<char, std::char_traits<char>, std::allocator<char> >")!=std::string::npos) {
				StringUtil::replaceAll(tn, "std::basic_string<char, std::char_traits<char>, std::allocator<char> >", "std::string");
			}
		} else if(tn.find("std::__cxx11::")!=std::string::npos) {
			StringUtil::replaceAll(tn, "std::__cxx11::", "std::");
			if(tn.find("std::basic_string<char, std::char_traits<char>, std::allocator<char> >")!=std::string::npos) {
				StringUtil::replaceAll(tn, "std::basic_string<char, std::char_traits<char>, std::allocator<char> >", "std::string");
			}
		}
		_mangledClassNameMap->insert({sm, tn});
		if(tn[tn.length()-1]=='*')
			tn = tn.substr(0,tn.length()-1);
		return tn;
	}

	template <typename T, typename R> static R cast(const T& val)
	{
		return lexical_cast<R>(val);
	}

	static std::string fromNumber(unsigned long long val) {
		std::string d;
		numToStr(val, &d);
		return d;
	}
	static void fromNumber(unsigned long long val, std::string* d) {
		numToStr(val, d);
	}
	static std::string fromFloat(float val) {
		std::string d;
		int n = snprintf(NULL, 0, "%f", val);
		char* ty = (char*)malloc((n + 5) * sizeof(char));
		snprintf(ty, n+1, "%f", val);
		d.append(ty, strlen(ty));
		free(ty);
		return d;
	}
	static void fromFloat(float val, std::string* d) {
		int n = snprintf(NULL, 0, "%f", val);
		char* ty = (char*)malloc((n + 5) * sizeof(char));
		snprintf(ty, n+1, "%f", val);
		d->append(ty, strlen(ty));
		free(ty);
	}
	static std::string fromDouble(double val) {
		std::string d;
		int n = snprintf(NULL, 0, "%f", val);
		char* ty = (char*)malloc((n + 5) * sizeof(char));
		snprintf(ty, n+1, "%f", val);
		d.append(ty, strlen(ty));
		free(ty);
		return d;
	}
	static void fromDouble(double val, std::string* d) {
		int n = snprintf(NULL, 0, "%f", val);
		char* ty = (char*)malloc((n + 5) * sizeof(char));
		snprintf(ty, n+1, "%f", val);
		d->append(ty, strlen(ty));
		free(ty);
	}
	static std::string fromLongdouble(long double val) {
		std::string d;
		int n = snprintf(NULL, 0, "%Lf", val);
		char* ty = (char*)malloc((n + 5) * sizeof(char));
		snprintf(ty, n+1, "%Lf", val);
		d.append(ty, strlen(ty));
		free(ty);
		return d;
	}
	static void fromLongdouble(long double val, std::string* d) {
		int n = snprintf(NULL, 0, "%Lf", val);
		char* ty = (char*)malloc((n + 5) * sizeof(char));
		snprintf(ty, n+1, "%Lf", val);
		d->append(ty, strlen(ty));
		free(ty);
	}
	static std::string fromBool(bool val) {
		if(val)
			return "true";
		else
			return "false";
	}
	static void fromBool(bool val, std::string* d) {
		if(val)
			d->append("true");
		else
			d->append("false");
	}
	template <typename T> static T lexical_cast(const short& val)
	{
		T t;
		std::string tn = getClassName(t);
		if(tn==STD_STRING)
		{
			std::string _t;
			numToStr(val, &_t);
			t = *(T*)(&_t);
			return t;
		}
		std::stringstream ss;
		ss << val;
		ss >> t;
		if(ss)
		{
			return t;
		}
		else
		{
			throw ("Conversion exception - short to " + tn);
		}
	}
	template <typename T> static T lexical_cast(const unsigned short& val)
	{
		T t;
		std::string tn = getClassName(t);
		if(tn==STD_STRING)
		{
			std::string _t;
			numToStr(val, &_t);
			t = *(T*)(&_t);
			return t;
		}
		std::stringstream ss;
		ss << val;
		ss >> t;
		if(ss)
		{
			return t;
		}
		else
		{
			throw ("Conversion exception - unsigned short to " + tn);
		}
	}
	template <typename T> static T lexical_cast(const int& val)
	{
		T t;
		std::string tn = getClassName(t);
		if(tn==STD_STRING)
		{
			std::string _t;
			numToStr(val, &_t);
			t = *(T*)(&_t);
			return t;
		}
		std::stringstream ss;
		ss << val;
		ss >> t;
		if(ss)
		{
			return t;
		}
		else
		{
			throw ("Conversion exception - int to " + tn);
		}
	}
	template <typename T> static T lexical_cast(const unsigned int& val)
	{
		T t;
		std::string tn = getClassName(t);
		if(tn==STD_STRING)
		{
			std::string _t;
			numToStr(val, &_t);
			t = *(T*)(&_t);
			return t;
		}
		std::stringstream ss;
		ss << val;
		ss >> t;
		if(ss)
		{
			return t;
		}
		else
		{
			throw ("Conversion exception - unsigned int to " + tn);
		}
	}
	template <typename T> static T lexical_cast(const long& val)
	{
		T t;
		std::string tn = getClassName(t);
		if(tn==STD_STRING)
		{
			std::string _t;
			numToStr(val, &_t);
			t = *(T*)(&_t);
			return t;
		}
		std::stringstream ss;
		ss << val;
		ss >> t;
		if(ss)
		{
			return t;
		}
		else
		{
			throw ("Conversion exception - long to " + tn);
		}
	}
	template <typename T> static T lexical_cast(const unsigned long& val)
	{
		T t;
		std::string tn = getClassName(t);
		if(tn==STD_STRING)
		{
			std::string _t;
			numToStr(val, &_t);
			t = *(T*)(&_t);
			return t;
		}
		std::stringstream ss;
		ss << val;
		ss >> t;
		if(ss)
		{
			return t;
		}
		else
		{
			throw ("Conversion exception - unsigned long to " + tn);
		}
	}
	template <typename T> static T lexical_cast(const long long& val)
	{
		T t;
		std::string tn = getClassName(t);
		if(tn==STD_STRING)
		{
			std::string _t;
			numToStr(val, &_t);
			t = *(T*)(&_t);
			return t;
		}
		std::stringstream ss;
		ss << val;
		ss >> t;
		if(ss)
		{
			return t;
		}
		else
		{
			throw ("Conversion exception - long long to " + tn);
		}
	}
	template <typename T> static T lexical_cast(const unsigned long long& val)
	{
		T t;
		std::string tn = getClassName(t);
		if(tn==STD_STRING)
		{
			std::string _t;
			numToStr(val, &_t);
			t = *(T*)(&_t);
			return t;
		}
		std::stringstream ss;
		ss << val;
		ss >> t;
		if(ss)
		{
			return t;
		}
		else
		{
			throw ("Conversion exception - unsigned long long to " + tn);
		}
	}
	template <typename T> static T lexical_cast(const double& val)
	{
		T t;
		std::string tn = getClassName(t);
		if(tn==STD_STRING)
		{
			std::string _t;
			primitive(val, "%f", &_t);
			t = *(T*)(&_t);
			return t;
		}
		std::stringstream ss;
		ss << val;
		ss >> t;
		if(ss)
		{
			return t;
		}
		else
		{
			throw ("Conversion exception - double to " + tn);
		}
	}
	template <typename T> static T lexical_cast(const long double& val)
	{
		T t;
		std::string tn = getClassName(t);
		if(tn==STD_STRING)
		{
			std::string _t;
			primitive(val, "%Lf", &_t);
			t = *(T*)(&_t);
			return t;
		}
		std::stringstream ss;
		ss << val;
		ss >> t;
		if(ss)
		{
			return t;
		}
		else
		{
			throw ("Conversion exception - double to " + tn);
		}
	}
	template <typename T> static T lexical_cast(const float& val)
	{
		T t;
		std::string tn = getClassName(t);
		if(tn==STD_STRING)
		{
			std::string _t;
			primitive(val, "%f", &_t);
			t = *(T*)(&_t);
			return t;
		}
		std::stringstream ss;
		ss << val;
		ss >> t;
		if(ss)
		{
			return t;
		}
		else
		{
			throw ("Conversion exception - float to " + tn);
		}
	}
	template <typename T> static T lexical_cast(const bool& val)
	{
		T t;
		std::string tn = getClassName(t);
		if(tn==STD_STRING)
		{
			std::string* d = NULL;
			if(val)
				d = new std::string("true");
			else
				d = new std::string("false");
			t = static_cast<T>(*d);
			delete d;
			return t;
		}
		std::stringstream ss;
		ss << val;
		ss >> t;
		if(ss)
		{
			return t;
		}
		else
		{
			throw ("Conversion exception - bool to " + tn);
		}
	}
	template <typename T> static T lexical_cast(const std::string& vval)
	{
		T t;
		void* vt;
		std::string tn = getClassName(t);
		char* endptr;
		float f = 0;
		double d = 0;
		long double ld = 0;
		int i = 0;
		short s = 0;
		long l = 0;
		unsigned short us = 0;
		unsigned int ui = 0;
		unsigned long ul = 0;
		long long ll = -1;
		unsigned long long ull = -1;
		bool b = false;
		if(tn=="float")
		{
			if(vval.at(0)=='0' || vval.find("+0")==0 || vval.find("-0")==0)
			{
				int dots = StringUtil::countOccurrences(vval, ".");
				if(dots>1) {
					throw std::runtime_error("Conversion exception - std::string to float");
				}
				if(vval.at(0)=='0')
				{
					if(vval.find_first_not_of(".0")==std::string::npos) {
					} else {
						f = strtof(vval.c_str(), &endptr);
						bool invalid = (f==0);
						if(invalid)
						{
							throw std::runtime_error("Conversion exception - std::string to float");
						}
					}
				}
				else
				{
					if(vval.find_first_not_of(".0", 1)==std::string::npos) {
					} else {
						f = strtof(vval.substr(1).c_str(), &endptr);
						bool invalid = (f==0);
						if(invalid)
						{
							throw std::runtime_error("Conversion exception - std::string to float");
						}
					}
				}
			}
			else
			{
				f = strtof(vval.c_str(), &endptr);
				bool invalid = (f==0);
				if(invalid)
				{
					throw std::runtime_error("Conversion exception - std::string to float");
				}
			}
			vt = &f;
			t = *(T*)vt;
		}
		else if(tn=="double")
		{
			if(vval.at(0)=='0' || vval.find("+0")==0 || vval.find("-0")==0)
			{
				int dots = StringUtil::countOccurrences(vval, ".");
				if(dots>1) {
					throw std::runtime_error("Conversion exception - std::string to double");
				}
				if(vval.at(0)=='0')
				{
					if(vval.find_first_not_of(".0")==std::string::npos) {
					} else {
						d = strtod(vval.c_str(), &endptr);
						bool invalid = (d==0);
						if(invalid)
						{
							throw std::runtime_error("Conversion exception - std::string to double");
						}
					}
				}
				else
				{
					if(vval.find_first_not_of(".0", 1)==std::string::npos) {
					} else {
						d = strtod(vval.substr(1).c_str(), &endptr);
						bool invalid = (d==0);
						if(invalid)
						{
							throw std::runtime_error("Conversion exception - std::string to double");
						}
					}
				}
			}
			else
			{
				d = strtod(vval.c_str(), &endptr);
				bool invalid = (d==0);
				if(invalid)
				{
					throw std::runtime_error("Conversion exception - std::string to double");
				}
			}
			vt = &d;
			t = *(T*)vt;
		}
		else if(tn=="long double")
		{
			if(sscanf(vval.c_str(), "%Lf", &ld)==1)
			{
			}
			else
			{
				throw std::runtime_error("Conversion exception - std::string to long double");
			}
			vt = &ld;
			t = *(T*)vt;
		}
		else if(tn=="int")
		{
			if(vval=="0" || (vval.length()>0 && vval.at(0)=='0' && vval.find_first_not_of("0")==std::string::npos)
					|| (vval.find("+0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)
					|| (vval.find("-0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)) {
			} else {
				bool invalid = false;
				if(vval!="0")
				{
					i = strtol(vval.c_str(), &endptr, 10);
					invalid = (i==0);
				}
				if(invalid)
				{
					throw std::runtime_error("Conversion exception - std::string to int");
				}
			}
			vt = &i;
			t = *(T*)vt;
		}
		else if(tn=="char")
		{
			char tc = vval.at(0);
			vt = &tc;
			t = *(T*)vt;
		}
		else if(tn=="unsigned char")
		{
			unsigned char tc = vval.at(0);
			vt = &tc;
			t = *(T*)vt;
		}
		else if(tn=="short")
		{
			if(vval=="0" || (vval.length()>0 && vval.at(0)=='0' && vval.find_first_not_of("0")==std::string::npos)
					|| (vval.find("+0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)
					|| (vval.find("-0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)) {
			} else {
				bool invalid = false;
				if(vval!="0")
				{
					s = strtol(vval.c_str(), &endptr, 10);
					invalid = (s==0);
				}
				if(invalid)
				{
					throw std::runtime_error("Conversion exception - std::string to short");
				}
			}
			vt = &s;
			t = *(T*)vt;
		}
		else if(tn=="long")
		{
			if(vval=="0" || (vval.length()>0 && vval.at(0)=='0' && vval.find_first_not_of("0")==std::string::npos)
					|| (vval.find("+0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)
					|| (vval.find("-0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)) {
			} else {
				bool invalid = false;
				if(vval!="0")
				{
					l = strtol(vval.c_str(), &endptr, 10);
					invalid = (l==0);
				}
				if(invalid)
				{
					throw std::runtime_error("Conversion exception - std::string to long");
				}
			}
			vt = &l;
			t = *(T*)vt;
		}
		else if(tn=="unsigned short")
		{
			if(vval=="0" || (vval.length()>0 && vval.at(0)=='0' && vval.find_first_not_of("0")==std::string::npos)
					|| (vval.find("+0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)
					|| (vval.find("-0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)) {
			} else {
				bool invalid = false;
				if(vval!="0")
				{
					us = strtol(vval.c_str(), &endptr, 10);
					invalid = (us==0);
				}
				if(invalid)
				{
					throw std::runtime_error("Conversion exception - std::string to unsigned short");
				}
			}
			vt = &us;
			t = *(T*)vt;
		}
		else if(tn=="unsigned int")
		{
			if(vval=="0" || (vval.length()>0 && vval.at(0)=='0' && vval.find_first_not_of("0")==std::string::npos)
					|| (vval.find("+0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)
					|| (vval.find("-0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)) {
			} else {
				bool invalid = false;
				if(vval!="0")
				{
					ui = strtol(vval.c_str(), &endptr, 10);
					invalid = (ui==0);
				}
				if(invalid)
				{
					throw std::runtime_error("Conversion exception - std::string to unsigned int");
				}
			}
			vt = &ui;
			t = *(T*)vt;
		}
		else if(tn=="unsigned long")
		{
			if(vval=="0" || (vval.length()>0 && vval.at(0)=='0' && vval.find_first_not_of("0")==std::string::npos)
					|| (vval.find("+0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)
					|| (vval.find("-0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)) {
			} else {
				bool invalid = false;
				if(vval!="0")
				{
					ul = strtol(vval.c_str(), &endptr, 10);
					invalid = (ul==0);
				}
				if(invalid)
				{
					throw std::runtime_error("Conversion exception - std::string to unsigned long");
				}
			}
			vt = &ul;
			t = *(T*)vt;
		}
		else if(tn=="long long")
		{
			if(sscanf(vval.c_str(), "%lld", &ll)==1)
			{
			}
			else
			{
				throw std::runtime_error("Conversion exception - std::string to long long");
			}
			vt = &ll;
			t = *(T*)vt;
		}
		else if(tn=="unsigned long long")
		{
			if(sscanf(vval.c_str(), "%llu", &ull)==1)
			{
			}
			else
			{
				throw std::runtime_error("Conversion exception - std::string to unsigned long long");
			}
			vt = &ull;
			t = *(T*)vt;
		}
		else if(tn=="bool")
		{
			if(StringUtil::toLowerCopy(vval)=="true" || StringUtil::toLowerCopy(vval)=="1")
				b = true;
			else if(StringUtil::toLowerCopy(vval)=="false" || StringUtil::toLowerCopy(vval)=="0")
				b = false;
			else
			{
				throw std::runtime_error("Conversion exception - std::string to bool");
			}
			vt = &b;
			t = *(T*)vt;
		}
		else if(tn==STD_STRING || tn=="string")
		{
			std::string str = vval;
			if(vval.length()==0)return t;
			std::string* d = new std::string(vval);
			t = *(T*)d; 
			delete d;
		}
		else
		{
			throw std::runtime_error("Generic Conversion exception");
		}
		return t;
	}
	static float toFloat(const std::string& vval) {
		char* endptr;
		float f = 0;
		if(vval.at(0)=='0' || vval.find("+0")==0 || vval.find("-0")==0)
		{
			int dots = StringUtil::countOccurrences(vval, ".");
			if(dots>1) {
				throw std::runtime_error("Conversion exception - std::string to float");
			}
			if(vval.at(0)=='0')
			{
				if(vval.find_first_not_of(".0")==std::string::npos) {
				} else {
					f = strtof(vval.c_str(), &endptr);
					bool invalid = (f==0);
					if(invalid)
					{
						throw std::runtime_error("Conversion exception - std::string to float");
					}
				}
			}
			else
			{
				if(vval.find_first_not_of(".0", 1)==std::string::npos) {
				} else {
					f = strtof(vval.substr(1).c_str(), &endptr);
					bool invalid = (f==0);
					if(invalid)
					{
						throw std::runtime_error("Conversion exception - std::string to float");
					}
				}
			}
		}
		else
		{
			f = strtof(vval.c_str(), &endptr);
			bool invalid = (f==0);
			if(invalid)
			{
				throw std::runtime_error("Conversion exception - std::string to float");
			}
		}
		return f;
	}
	static double toDouble(const std::string& vval) {
		char* endptr;
		double d = 0;
		if(vval.at(0)=='0' || vval.find("+0")==0 || vval.find("-0")==0)
		{
			int dots = StringUtil::countOccurrences(vval, ".");
			if(dots>1) {
				throw std::runtime_error("Conversion exception - std::string to double");
			}
			if(vval.at(0)=='0')
			{
				if(vval.find_first_not_of(".0")==std::string::npos) {
				} else {
					d = strtod(vval.c_str(), &endptr);
					bool invalid = (d==0);
					if(invalid)
					{
						throw std::runtime_error("Conversion exception - std::string to double");
					}
				}
			}
			else
			{
				if(vval.find_first_not_of(".0", 1)==std::string::npos) {
				} else {
					d = strtod(vval.substr(1).c_str(), &endptr);
					bool invalid = (d==0);
					if(invalid)
					{
						throw std::runtime_error("Conversion exception - std::string to double");
					}
				}
			}
		}
		else
		{
			d = strtod(vval.c_str(), &endptr);
			bool invalid = (d==0);
			if(invalid)
			{
				throw std::runtime_error("Conversion exception - std::string to double");
			}
		}
		return d;
	}
	static long double toLongdouble(const std::string& vval) {
		long double ld = 0;
		if(sscanf(vval.c_str(), "%Lf", &ld)==1)
		{
		}
		else
		{
			throw std::runtime_error("Conversion exception - std::string to long double");
		}
		return ld;
	}
	static short toShort(const std::string& vval) {
		short s = 0;
		char* endptr;
		/*if(vval=="0" || (vval.length()>0 && vval.at(0)=='0' && vval.find_first_not_of("0")==std::string::npos)
				|| (vval.find("+0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)
				|| (vval.find("-0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)) {
		} else {*/
			bool invalid = false;
			if(vval!="0")
			{
				s = strtol(vval.c_str(), &endptr, 10);
				invalid = (s==0);
			}
			if(invalid)
			{
				throw std::runtime_error("Conversion exception - std::string to short");
			}
		//}
		return s;
	}
	static int toInt(const std::string& vval) {
		int i = 0;
		char* endptr;
		/*if(vval=="0" || (vval.length()>0 && vval.at(0)=='0' && vval.find_first_not_of("0")==std::string::npos)
				|| (vval.find("+0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)
				|| (vval.find("-0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)) {
		} else {*/
			bool invalid = false;
			if(vval!="0")
			{
				i = strtol(vval.c_str(), &endptr, 10);
				invalid = (i==0);
			}
			if(invalid)
			{
				throw std::runtime_error("Conversion exception - std::string to int");
			}
		//}
		return i;
	}
	static long toLong(const std::string& vval) {
		long l = 0;
		char* endptr;
		/*if(vval=="0" || (vval.length()>0 && vval.at(0)=='0' && vval.find_first_not_of("0")==std::string::npos)
				|| (vval.find("+0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)
				|| (vval.find("-0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)) {
		} else {*/
			bool invalid = false;
			if(vval!="0")
			{
				l = strtol(vval.c_str(), &endptr, 10);
				invalid = (l==0);
			}
			if(invalid)
			{
				throw std::runtime_error("Conversion exception - std::string to long");
			}
		//}
		return l;
	}
	static long long toLonglong(const std::string& vval) {
		long long ll = -1;
		if(sscanf(vval.c_str(), "%lld", &ll)==1)
		{
		}
		else
		{
			throw std::runtime_error("Conversion exception - std::string to long long");
		}
		return ll;
	}
	static unsigned short toUShort(const std::string& vval) {
		unsigned short s = 0;
		char* endptr;
		/*if(vval=="0" || (vval.length()>0 && vval.at(0)=='0' && vval.find_first_not_of("0")==std::string::npos)
				|| (vval.find("+0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)
				|| (vval.find("-0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)) {
		} else {*/
			bool invalid = false;
			if(vval!="0")
			{
				s = strtol(vval.c_str(), &endptr, 10);
				invalid = (s==0);
			}
			if(invalid)
			{
				throw std::runtime_error("Conversion exception - std::string to unsigned short");
			}
		//}
		return s;
	}
	static unsigned int toUInt(const std::string& vval) {
		unsigned int i = 0;
		char* endptr;
		/*if(vval=="0" || (vval.length()>0 && vval.at(0)=='0' && vval.find_first_not_of("0")==std::string::npos)
				|| (vval.find("+0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)
				|| (vval.find("-0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)) {
		} else {*/
			bool invalid = false;
			if(vval!="0")
			{
				i = strtol(vval.c_str(), &endptr, 10);
				invalid = (i==0);
			}
			if(invalid)
			{
				throw std::runtime_error("Conversion exception - std::string to unsigned int");
			}
		//}
		return i;
	}
	static unsigned long toULong(const std::string& vval) {
		unsigned long l = 0;
		char* endptr;
		/*if(vval=="0" || (vval.length()>0 && vval.at(0)=='0' && vval.find_first_not_of("0")==std::string::npos)
				|| (vval.find("+0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)
				|| (vval.find("-0")==0 && vval.find_first_not_of("0", 1)==std::string::npos)) {
		} else {*/
			bool invalid = false;
			if(vval!="0")
			{
				l = strtol(vval.c_str(), &endptr, 10);
				invalid = (l==0);
			}
			if(invalid)
			{
				throw std::runtime_error("Conversion exception - std::string to unsigned long");
			}
		//}
		return l;
	}
	static unsigned long long toULonglong(const std::string& vval) {
		unsigned long long ull = -1;
		if(sscanf(vval.c_str(), "%llu", &ull)==1)
		{
		}
		else
		{
			throw std::runtime_error("Conversion exception - std::string to unsigned long long");
		}
		return ull;
	}
	static bool toBool(const std::string& vval) {
		bool b = false;
		if(StringUtil::toLowerCopy(vval)=="true" || StringUtil::toLowerCopy(vval)=="1")
			b = true;
		else if(StringUtil::toLowerCopy(vval)=="false" || StringUtil::toLowerCopy(vval)=="0")
			b = false;
		else
		{
			throw std::runtime_error("Conversion exception - std::string to bool");
		}
		return b;
	}
	static char toChar(const std::string& vval) {
		return vval.at(0);
	}
	static unsigned char toUChar(const std::string& vval) {
		unsigned char tc = vval.at(0);
		return tc;
	}
	template <typename T> static bool isPrimitiveDataType()
	{
		T t;
		std::string type = getClassName(t);
		if(type[type.length()-1]=='*')
			type = type.substr(0,type.length()-1);

		StringUtil::trim(type);
		if(type=="short" || type=="short int" || type=="signed short" || type=="signed short int"
				|| type=="unsigned short" || type=="unsigned short int"
				|| type=="signed" || type=="int" || type=="signed int"
				|| type=="unsigned" || type=="unsigned int" || type=="long"
				|| type=="long int" || type=="signed long" || type=="signed long int"
				|| type=="unsigned long" || type=="unsigned long int"
				|| type=="long long" || type=="long long int" || type=="signed long long"
				|| type=="signed long long int" || type=="unsigned long long"
				|| type=="unsigned long long int" || type=="long double" || type=="bool"
				|| type=="float" || type=="double" || type=="string" || type==STD_STRING
				|| type=="char" || type=="signed char" || type=="unsigned char"
				|| type=="wchar_t" ||  type=="char16_t" ||type=="char32_t")
		{
			return true;
		}
		return false;
	}
	template <typename T> static std::string getTypeName()
	{
		T t;
		std::string type = getClassName(t);
		if(type[type.length()-1]=='*')
			type = type.substr(0,type.length()-1);

		StringUtil::trim(type);
		if(type=="short" || type=="short int" || type=="signed short" || type=="signed short int")
		{
			return "short";
		}
		else if(type=="unsigned short" || type=="unsigned short int")
		{
			return "unsigned short";
		}
		else if(type=="signed" || type=="int" || type=="signed int")
		{
			return "int";
		}
		else if(type=="unsigned" || type=="unsigned int")
		{
			return "unsigned int";
		}
		else if(type=="long" || type=="long int" || type=="signed long" || type=="signed long int")
		{
			return "long";
		}
		else if(type=="unsigned long" || type=="unsigned long int")
		{
			return "unsigned long";
		}
		else if(type=="long long" || type=="long long int" || type=="signed long long" || type=="signed long long int")
		{
			return "long long";
		}
		else if(type=="unsigned long long" || type=="unsigned long long int")
		{
			return "unsigned long long";
		}
		else if(type=="long double")
		{
			return "long double";
		}
		return type;
	}
};

#endif /* CASTUTIL_H_ */
