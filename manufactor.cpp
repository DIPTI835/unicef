#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>   // for getenv
using namespace std;

class Manufacture {
private:
    int serial_no;
    string vaccine;
    int count;
    int need;

public:
    void setSerialNo(int sno) { serial_no = sno; }
    void setVaccine(const string& v) { vaccine = v; }
    void editCount(int newCount) { count = newCount; }
    void setNeed(int n) { need = n; }

    void excel(const string &filename){
        ofstream file(filename, ios::app);
        if(file.is_open()){
            file << serial_no << " " << vaccine << "," << count << "," << need << "\n";
            file.close();
        } else {
            cout << "error opening file\n";
        }
    }
};


string getValue(const string& data, const string& key) {
    size_t pos = data.find(key + "=");
    if(pos == string::npos) return "";
    size_t start = pos + key.size() + 1;
    size_t end = data.find("&", start);
    if(end == string::npos) end = data.size();
    return data.substr(start, end - start);
}

int main() {
    cout << "Content-type: text/html\n\n";   // CGI header

    
    char* lenStr = getenv("CONTENT_LENGTH");
    int len = lenStr ? atoi(lenStr) : 0;

    string postData;
    if(len > 0) {
        postData.resize(len);
        cin.read(&postData[0], len);

        
        string serialStr = getValue(postData, "serial_no");
        string vaccine   = getValue(postData, "vaccine");
        string countStr  = getValue(postData, "count");
        string needStr   = getValue(postData, "need");

        int serial_no = serialStr.empty() ? 0 : stoi(serialStr);
        int count     = countStr.empty() ? 0 : stoi(countStr);
        int need      = needStr.empty() ? 0 : stoi(needStr);

        if(!vaccine.empty()) {
            Manufacture m;
            m.setSerialNo(serial_no);
            m.setVaccine(vaccine);
            m.editCount(count);
            m.setNeed(need);
            m.excel("manufacture.csv");

            cout << "<p>Saved: " << serial_no << " " << vaccine
                 << " count=" << count << " need=" << need << "</p>";
        }
    }

    
    cout << "<html><head><title>Manufacture Portal</title></head><body>";
    cout << "<h1>Welcome to Manufacture Portal</h1>";
    cout << "<form method='post' action='/cgi-bin/manufacture.cgi'>";
    cout << "Serial No: <input type='number' name='serial_no' placeholder='serial_no'><br>";
    cout << "Vaccine Name: <input type='text' name='vaccine' placeholder='vaccine'><br>";
    cout << "Count: <input type='number' name='count' placeholder='count'><br>";
    cout << "Need: <input type='number' name='need' placeholder='need'><br>";
    cout << "<input type='submit' value='Save'>";
    cout << "</form>";
    cout << "</body></html>";

    return 0;
}