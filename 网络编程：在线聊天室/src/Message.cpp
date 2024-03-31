#include "Message.h"

Message::Message(int type, string time, string from, string to, string content) : type(type), time(time), from(from), to(to), content(content) {
}

string Message::serialize() const {
    stringstream ss;
    ss << type << "|" << time << "|" << from << "|" << to << "|" << content << "|";
    return ss.str();
}

void Message::deserialize(const string &data) {
    stringstream ss(data);
    string item;
    vector<string> tokens;
    while (getline(ss, item, '|')) {
        tokens.push_back(item);
    }
    // std::cout << tokens[0] << tokens[1] << tokens[2] << tokens[3] << tokens[4] << endl;
    int type = stoi(tokens[0]);
    *this = Message(type, tokens[1], tokens[2], tokens[3], tokens[4]);
}