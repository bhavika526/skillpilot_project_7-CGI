// greeting.cpp
// A C++ CGI program that reads a name from the query string
// and prints a personalized greeting as an HTML page.
//
// Example request:  /cgi-bin/greeting.exe?name=Bhavika
//
// Compile (Windows, MinGW):  g++ greeting.cpp -o greeting.exe -static
// Compile (Linux / macOS):   g++ greeting.cpp -o greeting.cgi

#include <cstdlib>   // getenv
#include <iostream>
#include <string>

using namespace std;

// Convert one hexadecimal character ('0'-'9', 'a'-'f', 'A'-'F') to its value.
// Returns -1 if the character is not a hex digit.
int hexValue(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

// Decode a URL-encoded string: "+" becomes a space and "%XX" becomes a character.
// Example: "Anna+Maria%21" -> "Anna Maria!"
string urlDecode(const string& text) {
    string result;
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == '+') {
            result += ' ';
        } else if (text[i] == '%' && i + 2 < text.size() &&
                   hexValue(text[i + 1]) != -1 && hexValue(text[i + 2]) != -1) {
            result += static_cast<char>(hexValue(text[i + 1]) * 16 + hexValue(text[i + 2]));
            i += 2;
        } else {
            result += text[i];
        }
    }
    return result;
}

// Find the value of a parameter in a query string such as "name=Bhavika&x=1".
// Returns an empty string if the parameter is not present.
string getParameter(const string& query, const string& key) {
    size_t start = 0;
    while (start <= query.size()) {
        size_t end = query.find('&', start);
        if (end == string::npos) end = query.size();

        string pair = query.substr(start, end - start);
        size_t equals = pair.find('=');
        if (equals != string::npos && urlDecode(pair.substr(0, equals)) == key) {
            return urlDecode(pair.substr(equals + 1));
        }
        start = end + 1;
    }
    return "";
}

// Remove spaces from the beginning and end of a string.
string trim(const string& text) {
    size_t first = text.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

// Make text safe to print inside HTML (prevents HTML/script injection).
string escapeHtml(const string& text) {
    string result;
    for (char c : text) {
        switch (c) {
            case '&':  result += "&amp;";  break;
            case '<':  result += "&lt;";   break;
            case '>':  result += "&gt;";   break;
            case '"':  result += "&quot;"; break;
            case '\'': result += "&#39;";  break;
            default:   result += c;
        }
    }
    return result;
}

int main() {
    // 1. Read the query string sent by the web server.
    const char* queryEnv = getenv("QUERY_STRING");
    string query = queryEnv ? queryEnv : "";

    // 2. Extract the "name" value, clean it, and limit its length.
    string name = trim(getParameter(query, "name"));
    if (name.size() > 50) {
        size_t cut = 50;
        // do not cut in the middle of a multi-byte (UTF-8) character
        while (cut > 0 && (static_cast<unsigned char>(name[cut]) & 0xC0) == 0x80) cut--;
        name = name.substr(0, cut);
    }

    // 3. Send the HTTP header. A blank line must follow it.
    cout << "Content-Type: text/html\n\n";

    // 4. Send the HTML page.
    cout << "<!DOCTYPE html>\n"
         << "<html lang=\"en\">\n"
         << "<head>\n"
         << "  <meta charset=\"UTF-8\">\n"
         << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
         << "  <title>Greeting</title>\n"
         << "  <style>\n"
         << "    body { font-family: Arial, sans-serif; background: #eef1f7; display: flex;\n"
         << "           justify-content: center; padding: 40px 15px; color: #1d2a44; }\n"
         << "    .box { background: #ffffff; max-width: 450px; width: 100%; padding: 30px;\n"
         << "           border-radius: 10px; text-align: center;\n"
         << "           box-shadow: 0 4px 15px rgba(0, 0, 0, 0.1); }\n"
         << "    a { color: #1d4ed8; font-weight: bold; }\n"
         << "    .error { color: #d93025; }\n"
         << "  </style>\n"
         << "</head>\n"
         << "<body>\n"
         << "  <div class=\"box\">\n";

    if (name.empty()) {
        cout << "    <h1 class=\"error\">No name received</h1>\n"
             << "    <p>Please go back and enter your name.</p>\n";
    } else {
        cout << "    <h1>Hello, " << escapeHtml(name) << "!</h1>\n"
             << "    <p>Welcome to my C++ CGI program.</p>\n";
    }

    cout << "    <p><a href=\"/index.html\">Back to the form</a></p>\n"
         << "  </div>\n"
         << "</body>\n"
         << "</html>\n";

    return 0;
}