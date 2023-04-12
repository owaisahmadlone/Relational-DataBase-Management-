#include<iostream>
using namespace std;

class Attr {
public:
	enum Type {
        Int,
        Float,
        String
    };
    virtual Type getType() const = 0;

	virtual string getStr() const = 0;

	virtual float getValue() const =0;

	bool compare(char op, Attr* other) const;

	virtual bool operator==(const Attr& other) const =0;

    virtual bool operator<(const Attr& other) const = 0;

    virtual bool operator<=(const Attr& other) const = 0;

    virtual bool operator>(const Attr& other) const = 0;

    virtual bool operator>=(const Attr& other) const = 0;

    virtual bool operator!=(const Attr& other) const = 0;

    virtual ~Attr() = default;
};
