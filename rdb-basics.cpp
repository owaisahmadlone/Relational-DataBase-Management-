#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <list>
#include "rdb-attr.cpp"

using namespace std;

class IntAttr : public Attr {
private:
    int value;
public:
    explicit IntAttr(int value=0) : value(value) {}

	Type getType() const override { return Type::Int; }

    string getStr() const override {
		return to_string(value);
	}

	float getValue() const override{
		return value;
	}

	void setValue(int v){
		value=v;
	}

	bool operator==(const Attr& other) const override{
        const auto& otherInt = dynamic_cast<const IntAttr&>(other);
        return value == otherInt.value;
    }

    bool operator<(const Attr& other) const override{
        const auto& otherInt = dynamic_cast<const IntAttr&>(other);
        return value < otherInt.value;
    }

    bool operator<=(const Attr& other) const override{
        const auto& otherInt = dynamic_cast<const IntAttr&>(other);
        return value <= otherInt.value;
    }

    bool operator>(const Attr& other) const override{
        const auto& otherInt = dynamic_cast<const IntAttr&>(other);
        return value > otherInt.value;
    }

    bool operator>=(const Attr& other) const override{
        const auto& otherInt = dynamic_cast<const IntAttr&>(other);
        return value >= otherInt.value;
    }

    bool operator!=(const Attr& other) const override{
        const auto& otherInt = dynamic_cast<const IntAttr&>(other);
        return value != otherInt.value;
    }
};

class FloatAttr : public Attr {
private:
    float value;

public:
    explicit FloatAttr(float value=0.0f): value(value) {}

	Type getType() const override { return Type::Float; }

    string getStr() const override{
        return to_string(value);
    }
	float getValue() const override{
		return value;
	}
	void setValue(float v){
		value = v;
	}

	bool operator==(const Attr& other) const override{
        const auto& otherfloat= dynamic_cast<const FloatAttr&>(other);
        return value == otherfloat.value;
    }


	bool operator<(const Attr& other) const override{
        const auto& otherfloat= dynamic_cast<const FloatAttr&>(other);
        return value < otherfloat.value;
    }

    bool operator<=(const Attr& other) const override{
        const auto& otherfloat = dynamic_cast<const FloatAttr&>(other);
        return value <= otherfloat.value;
    }

    bool operator>(const Attr& other) const override{
        const auto& otherfloat = dynamic_cast<const FloatAttr&>(other);
        return value > otherfloat.value;
    }

    bool operator>=(const Attr& other) const override{
        const auto& otherfloat = dynamic_cast<const FloatAttr&>(other);
        return value >= otherfloat.value;
    }

    bool operator!=(const Attr& other) const override {
        const auto& otherfloat = dynamic_cast<const FloatAttr&>(other);
        return value != otherfloat.value;
    }
};

class StringAttr : public Attr {
private:
    string value;
public:
     explicit StringAttr(string input="") : value(std::move(input)) {}

	Type getType() const override { return Type::String;}

    string getStr() const override{
		return value;
	}
	float getValue() const override{
		 return 0.0f;
	 };

	 void setValue(string in){
		value = std::move(in);
	}

	bool operator==(const Attr& other) const override {
        const auto& otherstr= dynamic_cast<const StringAttr&>(other);
         return (value == other.getStr());
    }

    bool operator<(const Attr& other) const override {
        const auto& otherstr= dynamic_cast<const StringAttr&>(other);
         return (value < other.getStr());
    }

    bool operator<=(const Attr& other)const override{
        const auto& otherstr = dynamic_cast<const StringAttr&>(other);
         return (value <= other.getStr());
    }

    bool operator>(const Attr& other) const override {
        const auto& otherstr = dynamic_cast<const StringAttr&>(other);
        return !(value <= otherstr.value);
    }

    bool operator>=(const Attr& other) const override{
        const auto& otherstr = dynamic_cast<const StringAttr&>(other);
        return !(value < otherstr.value);
    }

    bool operator!=(const Attr& other) const override{
        const auto& otherstr = dynamic_cast<const StringAttr&>(other);
        return value != other.getStr();
    }
};

struct DNFformula { list <list <tuple <string, char, Attr *> > > ops; };

bool Attr::compare(char op, Attr* other) const {
    if (getType() != other->getType()) {
        throw runtime_error("Attribute types do not match");
    }

    switch (op) {
        case '=':
            return *this == *other;
        case '<':
            return *this < *other;
        case '>':
            return *this > *other;
        default:
            throw runtime_error("Invalid operator: " + string(1, op));
    }
}

class Record {
private:
    vector<Attr *> attributes;
	int nattr;
public:
	explicit Record(const vector<Attr *>& attributes = {}): attributes(attributes), nattr(attributes.size()){}

	Attr* getAttr(int index) {
    if (index < nattr) {
        return attributes[index];
    }
	return nullptr;
	}

	Record* clone(){
		return  new Record(attributes);
	}
	bool operator==(Record& other){
		if(nattr==other.nattr){
			for(int i=0;i<nattr;i++){
				if(attributes[i]->getType()==other.attributes[i]->getType() and attributes[i]->getStr()==other.attributes[i]->getStr()) continue;
				else return false;
			}
			return true;
		}
		return false;
	}
	int getNumAttrs() const{
		return nattr;
	}

	vector<Attr *> getAttrs(){
		return attributes;
	}

 	void addIntAttr(int value) {
        attributes.push_back(new IntAttr(value));
		nattr++;
    }
    void addFloatAttr(float value) {
        attributes.push_back(new FloatAttr(value));
		nattr++;
    }
    void addStringAttr(basic_string<char, char_traits<char>, allocator<char>> value) {
		auto* st = new StringAttr(value);
    	attributes.push_back(st);
		nattr++;
    }

	void addAttr(Attr& attr){
		if(attr.getType()==Attr::Type::Int) addIntAttr(attr.getValue());
		if(attr.getType()==Attr::Type::Float) addFloatAttr(attr.getValue());
		if(attr.getType()==Attr::Type::String) addStringAttr(attr.getStr());
	}

	string getIntAttrValue(int index) const {
        if (auto intAttr = dynamic_cast<IntAttr*>(attributes[index])) {
            return intAttr->getStr();
        }
		return "NaN";
    }


	string getFloatAttrValue(int index) const {
        if (auto floatAttr = dynamic_cast<FloatAttr*>(attributes[index])) {
            return floatAttr->getStr();
        }
		return "NaN";
    }

	string getStringAttrValue(int index) const {
        if (auto stringAttr = dynamic_cast<StringAttr*>(attributes[index])) {
            return stringAttr->getStr();
        }
        return "";
    }
    void print() const {
        for (const auto& attr : attributes) {
            cout << attr->getStr() << " ";
        }
        cout << endl;
    }
};

class Relation {
	int nattr = 0, nrecs = 0;
	vector<string> attrnames;
	vector<int> attrinds;
	list<Record *> recs;

public:

	explicit Relation(int nattr = 0, int nrecs = 0, vector<string> attrnames = {}, vector<int> attrinds = {},
					  list<Record *> recs = {}) :
			nattr(nattr), nrecs(nrecs), attrnames(std::move(attrnames)), attrinds(std::move(attrinds)),
			recs(std::move(recs)) {}

	int getnattr() const {
		return nattr;
	}

	Attr *getAttr(const string& name) {
		int i = 0;
		for (const auto& iter: attrnames) {
			if (name == iter) return (recs.front())->getAttr(i);
			i++;
		}
		return nullptr;
	}

	int getnrecs() const {
		return nrecs;
	}

	int getIndex(string atname) {
		int i=0;
		for (const auto& iter: attrnames) {
			if (iter == atname) return i;
			i++;
		}
		return -1;
	}

	vector<string> getattrnames() const {
		return attrnames;
	};

	vector<int> getattrinds() const {
		return attrinds;
	}

	list<Record *> getRecords() const {
		return recs;
	}

	void removeRec(Record& rec){
	auto iterartor = recs.begin();
		for(auto iter:recs){
			if(*iter == rec){
				recs.erase(iterartor);
			}
			iterartor++;
		}
	}

	void addIntColumn(const string &attrname) {
		attrnames.push_back(attrname);
		attrinds.push_back(0);
		nattr++;
	}

	void addFloatColumn(const string &attrname) {
		attrnames.push_back(attrname);
		attrinds.push_back(1);
		nattr++;
	}

	void addStringColumn(const string &attrname) {
		attrnames.push_back(attrname);
		attrinds.push_back(2);
		nattr++;
	}

	void addRecord(Record *rec) {
		if (rec->getNumAttrs() != nattr) {
			throw runtime_error("Record has different number of attributes than relation");
		}
		recs.push_back(rec);
		nrecs++;
	}

	void Delete() {
		for (auto rec: recs) {
			delete rec;
		}
		recs.clear();
		nrecs = 0;
	}

	void print() {
		//print attributes
		for (const auto& name: attrnames) {
			cout << name << "\t";
		}
		cout << endl;
		// Print records
		for (auto rec: recs) {
			for (int i = 0; i < nattr; i++) {
				Attr *attr = rec->getAttr(i);
				if (attr->getType() == Attr::Type::Int) {
					cout << dynamic_cast<IntAttr *>(attr)->getStr() << "\t";
				} else if (attr->getType() == Attr::Type::Float) {
					cout << dynamic_cast<FloatAttr *>(attr)->getStr() << "\t";
				} else if (attr->getType() == Attr::Type::String) {
					cout << dynamic_cast<StringAttr *>(attr)->getStr() << "\t";
				}
			}
			cout << endl;
		}
	}
};






