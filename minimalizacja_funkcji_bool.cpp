// quine_mccluskey.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>

using namespace std;

class QuineMcCluskey
{
public:

	int VAR;
	string dc;

	QuineMcCluskey(int a)
	{
		VAR = a;
		dc.append(a, '-');

	}

	vector<string> getVars()
	{
		vector<string> v;
		string letters[] = { "x1", "x2", "x3", "x4", "x5", "x6" };
		for (int i = 0; i < this->VAR; i++)
			v.push_back(letters[i]);

		return v;
	}

	string decToBin(int n)
	{
		if (n == 0 || n == 1)
			return n + "";

		if (n % 2 == 0)
			return decToBin(n / 2) + "0";
		else
			return decToBin(n / 2) + "1";
	}

	string pad(string bin)
	{
		int max = VAR - bin.length();
		for (int i = 0; i < max; i++)
			bin = "0" + bin;
		return bin;
	}

	bool isGreyCode(string a, string b)
	{
		int flag = 0;
		for (int i = 0; i < a.length(); i++)
		{
			if (a[i] != b[i])
				flag++;
		}
		return (flag == 1);
	}

	string replace_complements(string a, string b)
	{
		string temp = "";
		for (int i = 0; i < a.length(); i++)
		if (a[i] != b[i])
			temp = temp + "-";
		else
			temp = temp + a[i];

		return temp;
	}

	bool in_vector(vector<string> a, string b)
	{
		for (int i = 0; i < a.size(); i++)
		if (a[i].compare(b) == 0)
			return true;
		return false;
	}

	vector<string> reduce(vector<string> minterms)
	{
		vector<string> newminterms;

		int max = minterms.size();
		int checked[max];
		for (int i = 0; i < max; i++)
		{
			for (int j = i; j < max; j++)
			{
				if (isGreyCode(minterms[i], minterms[j]))
				{
					checked[i] = 1;
					checked[j] = 1;
					if (!in_vector(newminterms, replace_complements(minterms[i], minterms[j])))
						newminterms.push_back(replace_complements(minterms[i], minterms[j]));
				}
			}
		}

		for (int i = 0; i < max; i++)
		{
			if (checked[i] != 1 && !in_vector(newminterms, minterms[i]))
				newminterms.push_back(minterms[i]);
		}

		return newminterms;
	}

	string getValue(string a)
	{
		string temp = "";
		vector<string> vars = this->getVars();
		if (a == dc)
			return "1";

		for (int i = 0; i < a.length(); i++)
		{
			if (a[i] != '-')
			{
				if (a[i] == '0')
					temp = temp + vars[i] + "\'";
				else
					temp = temp + vars[i];
			}
		}
		return temp;
	}

	bool VectorsEqual(vector<string> a, vector<string> b)
	{
		if (a.size() != b.size())
			return false;

		sort(a.begin(), a.end());
		sort(b.begin(), b.end());
		for (int i = 0; i < a.size(); i++)
		{
			if (a[i] != b[i])
				return false;
		}
		return true;
	}

};

int main()
{
	int no;
	cout << endl << "Ilosc zmiennych: " << endl;
	cin >> no;
	if (no > 6 || no < 1)
	{
		cout << "ERROR - zla liczba zmiennych (1-6)" << endl;
		continue;
	}
	QuineMcCluskey qm(no);

	string temp = "";
	cout << "Wprowac wartosci funkcji (Zakres=0-" << pow(2, no) - 1 << ") po przecinku:" << endl;
	cin >> temp;

	vector<string> minterms;
	istringstream f(temp);
	string s;
	while (getline(f, s, ','))
	{
		int t = atoi(s.data());
		minterms.push_back(qm.pad(qm.decToBin(t)));
	}

	sort(minterms.begin(), minterms.end());

	do
	{
		minterms = qm.reduce(minterms);
		sort(minterms.begin(), minterms.end());
	} while (!qm.VectorsEqual(minterms, qm.reduce(minterms)));


	int i;
	cout << "Zminimalizowana funkcja: " << endl;
	for (i = 0; i < minterms.size() - 1; i++)
		cout << qm.getValue(minterms[i]) << "+";
	cout << qm.getValue(minterms[i]) << endl;

}
