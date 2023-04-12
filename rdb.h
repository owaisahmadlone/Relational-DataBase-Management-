#ifndef RELATIONLIB_H
#define RELATIONLIB_H
using namespace std;

#include <vector>
#include <string>
#include <list>

class Attr{
public:
	enum Type {
        Int,
        Float,
        String
    };
	virtual Type getType() const = 0;
	virtual string getStr() const = 0;
	virtual float getValue()const =0;

	bool compare(char i, Attr *pAttr) const;
};

class IntAttr : public Attr{
	int value;
public:
	explicit IntAttr(int value =0);
	void setValue(int);
};

class FloatAttr : public Attr{
	float value;
public:
	explicit FloatAttr(float value=0.0f);
	void setValue(float);
};

class StringAttr: public Attr{
	string value;
public:
	explicit StringAttr(string input="");
	void setValue(string);
};

class Record {
	vector<Attr *> attributes;
	int nattr;
public:
	explicit Record(const vector<Attr *>& attributes = {});
	Attr* getAttr(int);
	void addAttr(Attr& attr);
	bool operator==(Record& other);
	void addIntAttr(int value);
    void addFloatAttr(float value);
    //void addStringAttr(const string& value);
	Record* clone();
};
class Relation{
	int nattr=0, nrecs=0;
	vector<string> attrnames;
	vector<int>attrinds;
	list <Record*> recs;
public:
	explicit Relation(int nattr=0, int nrecs=0, vector<string> attrnames={}, vector<int> attrinds={}, list<Record*> recs={});
	Attr * getAttr(string);
	int getnattr() const;
	int getnrecs() const;
	vector<string> getattrnames()const;
	int getIndex(string& );
	vector<int> getattrinds()const;
	list<Record*> getRecords() const;
	void addRecord(Record *rec);
	void addIntColumn(const string&);
	void addFloatColumn(const string&);
	void addStringColumn(const string&);
	void Delete();
	void print();
};

#endif
