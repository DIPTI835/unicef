#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>   // for getenv
using namespace std;

class Unicef {


    int age;
    int arrived_or_not;
    string vname;
public:
    void setAge(int a) { age = a; }
    void setVname(const string& v) { vname = v; }
    void setArrived(int x) { arrived_or_not = x; }

    int getAge() const { return age; }
    string getVname() const { return vname; }
    int getArrived() const { return arrived_or_not; }

    void saveToCSV(const string& filename, int remaining=0) const {
        ofstream file(filename, ios::app);
        if(file.is_open()){
            file << age << "," << vname << "," << arrived_or_not << "," << remaining << "\n";
            file.close();
        }
    }
};

// Helper to extract values from POST data
string getValue(const string& data, const string& key) {
    size_t pos = data.find(key + "=");
    if(pos == string::npos) return "";
    size_t start = pos + key.size() + 1;
    size_t end = data.find("&", start);
    if(end == string::npos) end = data.size();
    return data.substr(start, end - start);
}

map<int,int> countUnvaccinatedByAge(const string& hospitalFile) {
    ifstream in(hospitalFile);
    string line;
    map<int,int> counts;
    while(getline(in,line)) {
        vector<string> fields;
        size_t start=0,pos;
        while((pos=line.find(",",start))!=string::npos){
            fields.push_back(line.substr(start,pos-start));
            start=pos+1;
        }
        fields.push_back(line.substr(start));
        if(fields.size()>=6){
            int age=stoi(fields[3]);
            string vaccinated=fields[5];
            if(vaccinated=="no"){
                counts[age]++;
            }
        }
    }
    return counts;
}

map<int,string> ageToVaccine(const string& unicefFile){
    ifstream in(unicefFile);
    string line;
    map<int,string> mapping;
    while(getline(in,line)){
        stringstream ss(line);
        string ageStr,vac;
        getline(ss,ageStr,','); // age
        getline(ss,vac,',');    // vaccine name
        string arrived;
        getline(ss,arrived,','); // arrived_or_not
        if(!ageStr.empty() && !vac.empty()){
            mapping[stoi(ageStr)] = vac;
        }
    }
    return mapping;
}

void updateManufactureNeeds(const string& hospitalFile,
                            const string& unicefFile,
                            const string& manufactureFile){
    auto counts = countUnvaccinatedByAge(hospitalFile);
    auto mapping = ageToVaccine(unicefFile);

    ifstream in(manufactureFile);
    vector<string> lines;
    string line;
    while(getline(in,line)){
        vector<string> fields;
        size_t start=0,pos;
        while((pos=line.find(",",start))!=string::npos){
            fields.push_back(line.substr(start,pos-start));
            start=pos+1;
        }
        fields.push_back(line.substr(start));
        if(fields.size()>=3){
            string vaccine=fields[1];
            // find age that maps to this vaccine
            for(auto &pair: mapping){
                int age=pair.first;
                string vac=pair.second;
                if(vac==vaccine){
                    int need=counts[age];
                    if(fields.size()==3){
                        line = fields[0]+","+fields[1]+","+fields[2]+","+to_string(need);
                    } else {
                        fields[3]=to_string(need);
                        line = fields[0]+","+fields[1]+","+fields[2]+","+fields[3];
                    }
                }
            }
        }
        lines.push_back(line);
    }
    in.close();

    ofstream out(manufactureFile, ios::trunc);
    for(auto &l: lines) out<<l<<"\n";
    out.close();
}

int main() {
    cout << "Content-type: text/html\n\n";

    // Read POST data
    char* lenStr = getenv("CONTENT_LENGTH");
    int len = lenStr ? atoi(lenStr) : 0;
    string postData;
    if(len > 0) {
        postData.resize(len);
        cin.read(&postData[0], len);

        string ageStr = getValue(postData, "age");
        string vname  = getValue(postData, "vname");
        string arrivedStr = getValue(postData, "arrived_or_not");

        if(!ageStr.empty() && !vname.empty() && !arrivedStr.empty()) {
            Unicef u;
            u.setAge(stoi(ageStr));
            u.setVname(vname);
            u.setArrived(stoi(arrivedStr));
            u.saveToCSV("unicef.csv");
            cout << "<p>Saved: Age=" << ageStr << " Vaccine=" << vname
                 << " Arrived=" << arrivedStr << "</p>";
        }
    }

    // Show HTML form
    cout << "<html><head><title>Unicef Records</title></head><body>";
    cout << "<h1>Unicef entry</h1>";
   cout << "<form method='post' action='/cgi-bin/unicef.exe'>";
    cout << "Age: <input type='number' name='age'><br>";
    cout << "Vaccine Name: <input type='text' name='vname'><br>";
    cout << "Arrived (1=yes,0=no): <input type='number' name='arrived_or_not'><br>";
    cout << "<input type='submit' value='Save'>";
    cout << "</form>";
    cout << "</body></html>";

    return 0;
}
