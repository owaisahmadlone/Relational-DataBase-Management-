#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "rdb-basics.cpp"

using namespace std;



Relation* Union(Relation* R1, Relation * R2) {
    // Check that the two relations have the same number of attributes
    if (R1->getnattr() != R2->getnattr()) {
        throw runtime_error("Cannot compute union of relations with different number of attributes");
    }

    // Check that the two relations have the same attribute names
    if (R1->getattrnames() != R2->getattrnames()) {
        throw runtime_error("Cannot compute union of relations with different attribute names");
    }

    // Create the union relation
    auto*  rUnion = new Relation(R1->getnattr(), 0, R1->getattrnames(), R1->getattrinds(), {});

    // Add all records from R1 to the union relation
    for (auto rec : R1->getRecords()) {
        rUnion->addRecord(new Record(*rec));
    }

    // Add only the records from R2 that are not already in the union relation
    for (auto rec : R2->getRecords()) {
        bool isDuplicate = false;
        for (auto recUnion : rUnion->getRecords()) {
			if(*rec == *recUnion){
				isDuplicate=true;
				break;
			}
        }
        if (!isDuplicate) {
            rUnion->addRecord(new Record(*rec));
        }
    }

    return rUnion;
}

Relation* Difference(Relation* R1, Relation* R2) {
    // check if the two relations have the same number of attributes
    if (R1->getnattr() != R2->getnattr()) {
        throw runtime_error("Relations have different number of attributes");
    }

    // create a new relation with the same attribute names as R1
    auto* diff = new Relation(R1->getnattr(), 0, R1->getattrnames(), R1->getattrinds(), list<Record*>());

    // iterate through all records in R1
    for (auto r1 : R1->getRecords()) {
        bool found = false;
        // check if r1 is present in R2
        for (auto r2 : R2->getRecords()) {
            if (*r1==*r2) {
                found = true;
                break;
            }
        }
        // add r1 to diff if it is not present in R2
        if (!found) {
            auto* newrec = new Record();
            for (int i = 0; i < R1->getnattr(); i++) {
                Attr* r1attr = r1->getAttr(R1->getattrinds()[i]);
                if(r1attr->getType() == Attr::Type::Int) newrec->addIntAttr(r1attr->getValue()) ;
				if(r1attr->getType() == Attr::Type::Float) newrec->addFloatAttr(r1attr->getValue()) ;
				if(r1attr->getType() == Attr::Type::String) newrec->addStringAttr(r1attr->getStr()) ;
            }
            diff->addRecord(newrec);
        }
    }
    return diff;
}

Relation * cartesianproduct(Relation * R1, Relation * R2) {
    // Create a new relation with attribute names and indices from both R1 and R2
    vector<string> attrNames;
    vector<int> attrInds;

    // Add attribute names and indices from R1
    for (int i = 0; i < R1->getnattr(); i++) {
        attrNames.push_back(R1->getattrnames()[i]);
        attrInds.push_back(i);
    }

    // Add attribute names and indices from R2
    for (int i = 0; i < R2->getnattr(); i++) {
        attrNames.push_back(R2->getattrnames()[i]);
        attrInds.push_back(i + R1->getnattr());
    }

    // Create a new relation with the combined attribute names and indices
    auto * result = new Relation(R1->getnattr() + R2->getnattr(), 0, attrNames, attrInds, {});

    // Iterate through each record in R1 and R2
    for (auto r1rec : R1->getRecords()) {
        for (auto r2rec : R2->getRecords()) {
            // Create a new record with the combined attributes
            auto * newrec = new Record();

            // Add attributes from R1 record
            for (int i = 0; i < R1->getnattr(); i++) {
                Attr * r1attr = r1rec->getAttr(i);
                Attr * newattr = r1attr;
                newrec->addAttr(*newattr);
            }

            // Add attributes from R2 record
            for (int i = 0; i < R2->getnattr(); i++) {
                Attr * r2attr = r2rec->getAttr(i);
                Attr * newattr = r2attr;
                newrec->addAttr(*newattr);
            }

            // Add the new record to the result relation
            result->addRecord(newrec);
        }
    }

    return result;
}

Relation* Projection(Relation* R1, list<string> projectattrs) {
    // create a new relation with the projected attributes
    list<int> projectinds;
    list<string> attrnames;
    for (string attr : projectattrs) {
        int attrind = R1->getIndex(attr);
        if (attrind == -1) {
            throw runtime_error("Attribute " + attr + " not found in relation");
        }
        projectinds.push_back(attrind);
        attrnames.push_back(attr);
    }
	vector<int>projInds(projectinds.begin(),projectinds.end());
	vector<string>atNames(attrnames.begin(),attrnames.end());
    auto* proj = new Relation(projectinds.size(), R1->getnrecs(), atNames, projInds, list<Record*>());

    // iterate through all records in R1
    for (auto rec : R1->getRecords()) {
        Record* newrec = new Record();
        // add attributes to new record corresponding to the projected attributes
        for (int i : projectinds) {
            Attr* attr = rec->getAttr(i);
            newrec->addAttr(*attr);
        }
        proj->addRecord(newrec);
    }
    return proj;
}

Relation * selection(Relation * R1, DNFformula * f) {
    // create a new relation with the same attribute names as R1
    auto * result = new Relation(R1->getnattr(), 0, R1->getattrnames(), R1->getattrinds(), list<Record*>());

    // iterate through all records in R1
    for (auto rec : R1->getRecords()) {
        bool found = false;
        // check if rec satisfies any of the disjunctions in f
        for (const auto& disj : f->ops) {
            bool match = true;
            // check if rec satisfies all the conjunctions in the disjunction
            for (auto conj : disj) {
                string attrname = get<0>(conj);
                char op = get<1>(conj);
                Attr * val = get<2>(conj);
				int ind = R1->getIndex(attrname);
                Attr * recattr = rec->getAttr(ind);
                if (recattr == nullptr || !(recattr->compare(op, val))) {
                    match = false;
                    break;
                }
            }
            if (match) {
                found = true;
                break;
            }
        }
        // add rec to result if it satisfies at least one disjunction in f
        if (found) {
            result->addRecord(rec->clone());
        }
    }
    return result;
}

Relation * rename(Relation * R1, string s1, string s2) {
	// Check if attribute with name s1 exists in R1
	int s1Index = R1->getIndex(s1);
	if (s1Index == -1) {
		throw runtime_error("Attribute with name " + s1 + " does not exist in relation");
	}
	// Check if attribute with name s2 already exists in R1
	if (R1->getIndex(s2) != -1) {
		throw runtime_error("Attribute with name " + s2 + " already exists in relation");
	}

	vector<string> newAttrNames = R1->getattrnames();
    newAttrNames[s1Index] = s2;

	Relation * newR1 = new Relation(R1->getnattr(), R1->getnrecs(), newAttrNames, R1->getattrinds(),list<Record *> (R1->getRecords()));
	return newR1;
}


Relation* naturaljoin(Relation * R1, Relation * R2, list<string> joinattr) {
	for (auto iter: R2->getattrnames()) {
		for (auto iter2: joinattr) {
			if (iter == iter2) {
				R2 = rename(R2, iter, "joinAttr2");
			}
		}
		for (auto iter: R1->getattrnames()) {
			for (auto iter2: joinattr) {
				if (iter == iter2) {
					R1 = rename(R1, iter, "joinAttr1");
				}
			}
		}


		Relation *Cp = cartesianproduct(R1, R2);
		Relation *result = new Relation(Cp->getnattr(), 0, Cp->getattrnames(), Cp->getattrinds(), {});
		for (auto r1: Cp->getRecords()) {
			int ind1 = Cp->getIndex("joinAttr1");
			int ind2 = Cp->getIndex( "joinAttr2");
			if (r1->getAttr(ind1)->getStr() == r1->getAttr(ind2)->getStr()) {
				result->addRecord(r1);
			}
		}
		for (auto iter: result->getattrnames()) {
			auto iterator = joinattr.begin();
			if (iter == "joinAttr1") {
				result = rename(result, "joinAttr1", *iterator);
				iterator++;
			}
		}
		list<string> projectattrs;
		for (auto iter: result->getattrnames()) {
			if (iter != "joinAttr2") {
				projectattrs.push_back(iter);
			}
		}
		Relation *final = Projection(result, projectattrs);
		return final;
	}
}











