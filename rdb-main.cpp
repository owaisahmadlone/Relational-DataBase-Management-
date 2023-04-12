#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "rdb-join.cpp"

using namespace std;

Relation * MakeTable(){
	cout<<"Making a new table\n";
	auto* Table = new Relation;
	int c,r;
	cout<<"Enter rows and columns: \n";
	cin>>r>>c;
	cout<<"I. 0 for Int Attribute\n"
			  "II. 1 for Float Attribute\n"
			  "III. 2 for String Attribute\n";
	for(int i=0;i<c;i++){
		int x;
		cin >> x;
		if(x == 0){
			cout<<"Name of attribute: ";
			string name;
			cin>>name;
			Table->addIntColumn(name);
		}
		if(x == 1){
			cout<<"Name of attribute: ";
			string name;
			cin>>name;
			Table->addFloatColumn(name);
		}
		if(x==2){
			cout<<"Name of attribute: ";
			string name;
			cin>>name;
			Table->addStringColumn(name);
		}
	}
	for(int i=0;i<r;i++) {
		cout << "\nMaking record #" << i+1 <<endl;
		auto *r = new Record;
		for (int j = 0; j < c; j++) {
			if (Table->getattrinds()[j] == 0) {
				int v;
				cout << "Entry value for "<<Table->getattrnames()[j]<<" Attribute"<<endl;
				cin >> v;
				r->addIntAttr(v);
			}
			if (Table->getattrinds()[j] == 1) {
				float v;
				cout << "Enter value for "<<Table->getattrnames()[j]<<" Attribute"<<endl;
				cin >> v;
				r->addFloatAttr(v);
			}

			if(Table->getattrinds()[j]==2){
				string name;
				cout << "Enter value for "<<Table->getattrnames()[j]<<" Attribute"<<endl;
				cin >>name;
				r->addStringAttr(name);
			}
		}
		Table->addRecord(r);
	}
		return Table;
}


int main(){
	while(true) {
		int c;
		cout << "MENU:\n"
				"1. c=0 to exit the program\n"
				"2. c=1 to make/add a new table(s)\n"
				"3. c=2 to add new record(s) to a table\n"
				"4. c=3 to print table(s)\n"
				"5. c=4 to do a unary or binary operation on relation(s)\n"
				"6. c=5 to delete Table(s)\n";
		cout << "ENTER c: ";
		cin >> c;
		static vector<Relation *> Tables;
		if(c==0) break;
		if(c==1){
			int n;
			cout<<"Enter the number of tables you want to make/add: ";
			cin>>n;
			for(int i=0;i<n;i++){
				Relation * r = MakeTable();
				Tables.push_back(r);
				cout<<"\nTable #"<<Tables.size()<<" created!\n";
				cout<<"The number of Tables is :"<<Tables.size()<<endl;
			}
		}
		if(c==2){
			int t,n;
			cout<<"The number of Tables is :"<<Tables.size()<<endl;
			if(Tables.empty()){
				cout<<"ERROR: No Tables made\n";
				continue;
			}
			cout<<"\n Enter the Table no. you want to add records to(1, 2..so on) and the no. of records: ";
			cin>>t>>n;
			for(int i=0;i<n;i++) {
				cout << "\nMaking #" << i+1 << " record\n";
				auto *r = new Record;
				for (int j = 0; j < Tables[t - 1]->getnattr(); j++) {
					if (Tables[t-1]->getattrinds()[j] == 0) {
						int v;
						cout << "Enter value of integer attribute: "<<Tables[t-1]->getattrnames()[j]<<endl;
						cin >> v;
						r->addIntAttr(v);
					}
					if (Tables[t-1]->getattrinds()[j] == 1) {
						float v;
						cout << "Enter value of float attribute: "<<Tables[t-1]->getattrnames()[j]<<endl;
						cin >> v;
						r->addFloatAttr(v);
					}
				}
			Tables[t-1]->addRecord(r);
			}
		}
		if(c==3){
			if(Tables.empty()){
				cout<<"ERROR: No Tables made\n";
				continue;
			}
			while(true){
				cout<<"Enter the Table Number you want to print(1,2..so on); Enter 0 to stop:\n";
				int e;
				cin>>e;
				if (e==0) break;
				if(e>Tables.size()){
					cout<<"\nERROR: No Table with this number\n";
					continue;
				}
				else{
					cout<<"\n******Printing Table #"<<e<<"******"<<endl;
					Tables[e-1]->print();
				}
			}
		}

		if(c==4){
			int e;
			if(Tables.empty()){
				cout<<"ERROR: No Tables made\n";
				continue;
			}
			cout<<"Number of Tables = "<<Tables.size()<<endl;
			cout<<"MENU:\n"
				  "1. e=1 to find the Union of two relations.\n"
				  "2. e=2 to find the difference of two relations.\n"
				  "3. e=3 to find the cartesian product of two relations.\n"
				  "4. e=4 to project any given relation.\n"
				  "5. e=5 to find a selection of any given relation.\n"
				  "6. e=6 to rename an attribute in any of the schema.\n"
				  "7. e=7 to find the natural join of two relations.\n";
			cout<<"ENTER e: ";
			cin>>e;
			if(e==1){
				int r1,r2;
				cout<<"Enter the Table Numbers you want to combine: ";
				cin>>r1>>r2;
				Relation* Unionr = Union(Tables[r1-1],Tables[r2-1]);
				cout<<"The combined Table is :\n";
				Unionr->print();
			}
			if(e==2){
				int r1,r2;
				cout<<"Enter the Table numbers for difference operation: ";
				cin>>r1>>r2;
				Relation* diff = Difference(Tables[r1-1],Tables[r2-1]);
				cout<<"\nThe resultant Table is:\n";
				diff->print();
			}

			if(e==3){
				int r1,r2;
				cout<<"Enter the Table number you want to find cartesian product of: ";
				cin>>r1>>r2;
				Relation * CartProd = cartesianproduct(Tables[r1-1],Tables[r2-1]);
				cout<<"The resultant table is :\n";
				CartProd->print();
			}

			if(e==4){
				int r;
				cout<<"Enter the Relation/Table number you want to project: ";
				cin>>r;
				list<string> projectatrs;
				string input;
				cout<<"Enter the list of attributes you want to keep, enter 'end' to stop entering[case sensitive]:\n";
				while(true){
					cin>>input;
					if(input != "end") projectatrs.push_back(input);
					else break;
				}
				Relation* project = Projection(Tables[r-1],projectatrs);
				cout<<"\nThe resultant table is:\n";
				project->print();
			}
			if(e==5){
				int t;
				cout<<"Enter the table number you want to apply the selection on: ";
				cin>>t;
				DNFformula f;
				// create the first disjunction: age > 20
				list<tuple<string, char, Attr*>> disj1;
				disj1.emplace_back("Age", '>', new IntAttr(20));
				f.ops.push_back(disj1);

				Relation* selected_students = selection(Tables[t-1], &f);
				cout<<"The resultant Table is:\n";
				selected_students->print();
			}

			if(e==6){
				int t;
				cout<<"Enter the Table number you want to rename an attribute in: ";
				cin>>t;
				cout<<"The number of attributes in this Table is: "<<Tables[t-1]->getnattr()<<endl;
				string oldatt,newatt;
				cout<<"Enter the attribute you want to rename and the new attribute name: ";
				cin>>oldatt>>newatt;
				Tables[t-1] = rename(Tables[t-1],oldatt,newatt);
				cout<<"\nThe resultant table is:\n";
				Tables[t-1]->print();
			}
			if(e==7){
				int r1,r2;
				cout<<"Enter the Table numbers you want to apply natural join on: ";
				cin>>r1>>r2;
				list<string> joinattr;
				cout<<"\nEnter the list of common attributes: enter 'end' to stop entering:\n";
				while(true){
					string s;
					cin>>s;
					if(s=="end") break;
					else joinattr.push_back(s);
				}
				Relation* njoin = naturaljoin(Tables[r1-1],Tables[r2-1],joinattr);
				cout<<"The resultant table is:\n";
				njoin->print();
			}
		}
		if(c==5){
			cout<<"Enter the Table number(s) you  want to delete(1,2..): enter 0 to stop:\n";
			while(true){
				int c;
				cin>>c;
				if(c==0)break;
				else Tables.erase(Tables.begin()+c-1);
				cout<<"****Table #"<<c<<" deleted succesfully!****\n";
			}
		}
	}
	cout<<"***PROGRAM EXITED!***";
	return 0;
}