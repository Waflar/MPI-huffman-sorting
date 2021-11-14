#include<stdio.h>
#include<conio.h>
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<mpi.h>

using namespace std;

struct SYMB_HAFF
{
	char c;
	int count;
	stringstr;
} symb_send;

typedefmap<char, pair<int, string>>SYMB_TYPE;

void sort(SYMB_HAFF* mas, intfirst, intlast)
{
	int mid;
	SYMB_HAFFtmp;
	int f = first, l = last;
	mid = mas[(f + l) / 2].count;
	do
	{
		while (mas[f].count > mid) f++;
		while (mas[l].count < mid) l--;
		if (f <= l)
		{
			tmp = mas[f];
			mas[f] = mas[l];
			mas[l] = tmp;
			f++;
			l--;
		}
	} while (f < l);
	if (first < l) sort(mas, first, l);
	if (f < last) sort(mas, f, last);
}

int main(intargc, char* argv)
{
	setlocale(LC_ALL, "Russian");
	char c;
	intalph = 0, e;
	SYMB_TYPEsymb;
	string text, compressedText;
	structSYMB_HAFF* al, * al1;
	doublevCompressed = 0, vOrigin, koeff;
	ifstreaminp;
	ofstream out;

	int size, rank;
	MPI_STATUS st;
	intminRest, k, extraRest, * masCount, * masInd;
	string subtext, subCompressedText;
	SYMB_TYPEsubsymb;
	SYMB_HAFF* send, * subsend;
	int* sendSize;

	MPI_Init(&argc, &argv);
	MPI_Comm_Size(MPI_Comm_World, &size);
	MPI_Comm_Rank(MPI_Comm_World, &rank);

	if (size < 2) {
		fprintf(stderr, "Requires at least two processes.\n");
		exit(-1);
	}

	constintnitems = 3;
	intblocklengths[3] = { 1,1,1 };
	MPI_Datatypetypes[3] = { MPI_CHAR, MPI_INT, *MPI_CHAR };
	MPI_Datatype MPI_SYMB_HAFF;
	MPI_Aintoffsets[3];

	offsets[0] = offsetof(symb_send, c);
	offsets[1] = offsetof(symb_send, count);
	offsets[1] = offsetof(symb_send, str);

	MPI_Type_create_struct(nitems, blocklengths, offsets, types, &MPI_SYMB_HAFF);
	MPI_Type_commit(&MPI_SYMB_HAFF);

	if (rank == 0)
	{
		inp.open("input.txt");
		while (inp >> c)
		{
			text += c;
		}
		inp.close();

		send = newSYMB_HAFF[text.length];
	}

	masCount = newint[size];
	masInd = newint[size];

	minRest = text.size() / size;
	extraRest = text.size() % size;
	k = 0;
	for (inti = 0; i < size - 1; i++)
	{
		if (i < extraRest)
			masCount[i] = minRest + 1;
		else
			masCount[i] = minRest;
		masInd[i] = k;
		k = k + masCount[i];
	}
	MPI_Scatterv(text.c_str(), masCount, masInd, MPI_CHAR, &subtext, text.size(), MPI_CHAR, 0, MPI_Comm_World);

	for (inti = 0; i < subtext.size; i++)
	{
		SYMB_TYPE::iterator it = subsymb.find(subtext[i]);
		if (it != subsymb.end())
		{
			it->second.first++;
		}
		else
		{
			subsymb.insert(pair<char, pair<int, string>>{subtext[i], pair<int, string> {1, ""}});
		}
	}

	subsend = newSYMB_HAFF[subsymb.size()];
	int k = 0;
	for (SYMB_TYPE::iterator it = subsymb.begin(); it != subsymb.end(); it++)
	{
		subsend[k] = { it->first, it->second.first, "" };
		k++;
	}

	MPI_Gather(&subsend, subsymb.size(), MPI_SYMB_HAFF, &send, text.length, MPI_SYMB_HAFF, 0, MPI_Comm_World);

	delete[]subsend;

	if (rank == 0)
	{
		for (inti = 0; i < text.length; i++)
		{
			SYMB_TYPE::iterator it = symb.find(send[i].c);
			if (it != symb.end())
			{
				it->second.first += send[i].count;
			}
			else
			{
				symb.insert(pair<char, pair<int, string>>{subtext[i], pair<int, string> {send[i].count, ""}});
			}
		}

		delete[] send;

		alph = symb.size();
		al = newSYMB_HAFF[alph];
		al1 = newSYMB_HAFF[alph];
		k = 0;
		for (SYMB_TYPE::iterator it = symb.begin(); it != symb.end(); it++)
		{
			al[k].c = it->first;
			al[k].count = it->second.first;
			al[k].str = it->second.second;
			al1[k] = al[k];

			k++;
		}

		sort(al, 0, alph - 1);
		sort(al1, 0, alph - 1);
		for (inti = 0; i < alph; i++)
		{
			al1[i].str += al1[i].c;
		}
		e = alph - 1;
		al[e].str += '1';
		al[e - 1].str += '0';

		while ((al1[e].count + al1[e - 1].count) != text.size())
		{
			al1[e - 1].str += al1[e].str;
			al1[e - 1].count += al1[e].count;
			al1[e].count = 0;
			e--;
			sort(al1, 0, alph - 1);

			for (int j = 0; j < alph; j++)
				for (int l = 0; l < al1[e].str.size(); l++)
					if (al[j].c == al1[e].str[l])
						al[j].str += '1';

			for (int j = 0; j < alph; j++)
				for (int l = 0; l < al1[e - 1].str.size(); l++)
					if (al[j].c == al1[e - 1].str[l])
						al[j].str += '0';
		}

		for (inti = 0; i < alph; i++)
		{
			reverse(al[i].str.begin(), al[i].str.end());
			symb.at(al[i].c) = pair<int, string>{ al[i].count, al[i].str };
		}

		delete[]al;
		delete[]al1;
	}

	MPI_Bcast(&symb, symb.size, MPI_SYMB_HAFF, 0, MPI_Comm_World);

	for (inti = 0; i < subtext.size(); i++)
	{
		subCompressedText += symb.at(subtext[i]).second;
	}

	MPI_Gatherv(subCompressedText.c_str(), masCount, masInd, MPI_CHAR, &compressedText, compressedText.size(), MPI_CHAR, 0, MPI_Comm_World);

	if (rank == 0)
	{
		out.open("output.txt");
		out << "Таблица символов Хаффмана:" << endl;
		out << "Символ" << "\t" << "Частота" << "\t" << "Код" << endl;
		for (SYMB_TYPE::iterator it = symb.begin(); it != symb.end(); it++)
		{
			out << it->first << "\t" << it->second.first << "\t" << it->second.second << endl;
		}

		out << "Исходный текст: " << text << endl;
		out << "Сжатыйтекст: " << compressedText << endl;

		for (inti = 0; i < alph; i++)
			vCompressed += al[i].count * al[i].str.size();
		vOrigin = text.size() * 8;
		koeff = (abs(vOrigin - vCompressed) / vOrigin) * 100;
		out << "Относительный коэффициент сжатия: " << koeff << "%";

		out.close();
	}

	MPI_Type_free(&MPI_SYMB_HAFF);
	MPI_Finalize();

	return 0;
}
