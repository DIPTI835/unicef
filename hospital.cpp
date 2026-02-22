#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
using namespace std;

class record {
private:
    int sno;
    string hname;
    string cname;
    int cage;
    long mobile_no;
    string vaccinated;

public:
    void setSerialNo(int s) { sno = s; }
    void setHospital(const string& h) { hname = h; }
    void setCandidate(const string& c) { cname = c; }
    void setAge(int a) { cage = a; }
    void setMobile(long m) { mobile_no = m; }
    void setVaccinated(const string& v) { vaccinated = v; }

    void excelhos(const string &filename){
        ofstream file(filename, ios::app);
        if(file.is_open()){
            file << sno << "," << hname << "," << cname << ","
                 << cage << "," << mobile_no << "," << vaccinated << "\n";
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

// function to edit age in CSV
void editAge(const string& filename, const string& name, int newAge) {
    ifstream infile(filename);
    vector<string> lines;
    string line;

    while(getline(infile, line)) {
        vector<string> fields;
        size_t start = 0, pos;
        while((pos = line.find(",", start)) != string::npos) {
            fields.push_back(line.substr(start, pos - start));
            start = pos + 1;
        }
        fields.push_back(line.substr(start));

        if(fields.size() >= 6 && fields[2] == name) {
            fields[3] = to_string(newAge); // update age
            line = fields[0] + "," + fields[1] + "," + fields[2] + "," +
                   fields[3] + "," + fields[4] + "," + fields[5];
        }
        lines.push_back(line);
    }
    infile.close();

    ofstream outfile(filename, ios::trunc);
    for(const auto& l : lines) {
        outfile << l << "\n";
    }
    outfile.close();
}

int main() {
    cout << "Content-type: text/html\n\n";   // CGI header

    char* lenStr = getenv("CONTENT_LENGTH");
    int len = lenStr ? atoi(lenStr) : 0;

    string postData;
    if(len > 0) {
        postData.resize(len);
        cin.read(&postData[0], len);

        // parse values forr adding new recordsssssssss
        string snoStr   = getValue(postData, "sno");
        string hname    = getValue(postData, "hname");
        string cname    = getValue(postData, "cname");
        string ageStr   = getValue(postData, "cage");
        string mobileStr= getValue(postData, "mobile_no");
        string vaccinated = getValue(postData, "vaccinated");

        int sno   = snoStr.empty() ? 0 : stoi(snoStr);
        int cage  = ageStr.empty() ? 0 : stoi(ageStr);
        long mobile_no = mobileStr.empty() ? 0 : stol(mobileStr);

        if(!cname.empty()) {
            record r;
            r.setSerialNo(sno);
            r.setHospital(hname);
            r.setCandidate(cname);
            r.setAge(cage);
            r.setMobile(mobile_no);
            r.setVaccinated(vaccinated);
            r.excelhos("hospital.csv");

            cout << "<p>Saved: " << sno << " " << hname << " " << cname
                 << " age=" << cage << " mobile=" << mobile_no
                 << " vaccinated=" << vaccinated << "</p>";
        }

        // parse values for******* editing age
        string editName = getValue(postData, "edit_name");
        string newAgeStr = getValue(postData, "new_age");
        if(!editName.empty() && !newAgeStr.empty()) {
            int newAge = stoi(newAgeStr);
            editAge("hospital.csv", editName, newAge);
            cout << "<p>Updated age for " << editName << " to " << newAge << "</p>";
        }
    }

    //html forms
    cout << "<html><head><title>Hospital Records</title></head><body>";
    cout << "<h1>Hospital Candidate Entry</h1>";
    cout << "<form method='post' action='/cgi-bin/hospital.cgi'>";
    cout << "Serial No: <input type='number' name='sno'><br>";
    cout << "Hospital Name: <input type='text' name='hname'><br>";
    cout << "Candidate Name: <input type='text' name='cname'><br>";
    cout << "Candidate Age: <input type='number' name='cage'><br>";
    cout << "Mobile No: <input type='number' name='mobile_no'><br>";
    cout << "Vaccinated (yes/no): <input type='text' name='vaccinated'><br>";
    cout << "<input type='submit' value='Save'>";
    cout << "</form>";

    cout << "<h2>Edit Candidate Age</h2>";
    cout << "<form method='post' action='/cgi-bin/hospital.cgi'>";
    cout << "Candidate Name: <input type='text' name='edit_name'><br>";
    cout << "New Age: <input type='number' name='new_age'><br>";
    cout << "<input type='submit' value='Update Age'>";
    cout << "</form>";

    // table of all records
    ifstream infile("hospital.csv");
    if(infile.is_open()) {
        cout << "<h2>All Records</h2>";
        cout << "<table border='1'>";
        cout << "<tr><th>Serial No</th><th>Hospital</th><th>Candidate</th><th>Age</th><th>Mobile</th><th>Vaccinated</th></tr>";
        string line;
        while(getline(infile, line)) {
            vector<string> fields;
            size_t start = 0, pos;
            while((pos = line.find(",", start)) != string::npos) {
                fields.push_back(line.substr(start, pos - start));
                start = pos + 1;
            }
            fields.push_back(line.substr(start));

            if(fields.size() >= 6) {
                cout << "<tr>";
                for(const auto& f : fields) {
                    cout << "<td>" << f << "</td>";
                }
                cout << "</tr>";
            }
        }
        cout << "</table>";
        infile.close();
    }

    cout << "</body></html>";
    return 0;
}