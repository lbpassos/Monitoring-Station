#ifndef IOMANIP_H
#define IOMANIP_H

#include <iostream.h>

template <class T> class smanip {			
	ios & (* f)(ios &, T);
	T a;
public:
	smanip(ios & (* ff)(ios &, T), T aa)
		: f(ff), a(aa) { }
	friend istream & operator>>(istream & is, const smanip<T> & m)
		{ m.f(is, m.a); return is; }
	friend ostream & operator<<(ostream & os, const smanip<T> & m)
		{ m.f(os, m.a); return os; }
};

/*
template<class T>
ostream & operator<<(ostream & os, const smanip<T> & m)
		{ m.f(os, m.a); return os; }
*/

template <class T> class imanip {			
	istream & (*f)(istream &, T);	   
	T a;
public:
	imanip(ios & (* ff)(istream &, T), T aa)
		: f(ff), a(aa) { }
	friend istream & operator>>(istream & is, const imanip<T> & m)
		{ return m.f(is, m.a); }
};

template <class T> class omanip {			
	ostream & (* f)(ostream &, T);
	T a;
public:
	omanip(ostream & (* ff)(ostream &, T), T aa)
		: f(ff), a(aa) { }
	friend ostream & operator<<(ostream & os, const omanip<T> & m)
		  { return m.f(os, m.a); }
};
/*
template <class T>
ostream & operator<<(ostream & os, const omanip<T> & m)
	{ return m.f(os, m.a); }
*/

template <class T> class iomanip {			
	ios & (* f)(ios &, T);
	T a;
public:
	iomanip(ios & (* ff)(ios &, T), T aa)
		: f(ff), a(aa) { }
	friend istream & operator>>(iostream & s, const iomanip<T> & m)
		{ return m.f(s, m.a); }
	friend ostream & operator<<(iostream & s, const iomanip<T> & m)
		{ return m.f(s, m.a); }
};

// set the conversion base to 0, 8, 10, or 16
smanip<int>		 setbase(int _b);

// clear the flags bitvector according to the bits set in b
smanip<long>	 resetiosflags(long _b);

// set the flags bitvector according to the bits set in b
smanip<long>	 setiosflags(long _b);

// set fill character for padding a field
smanip<int>		 setfill(int _f);

// set the floating-point precision to n digits
smanip<int>		 setprecision(int _n);

// set the field width to n
smanip<int>		 setw(int _n);

#endif
