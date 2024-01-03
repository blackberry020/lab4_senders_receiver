// function for checking if a given string is number
bool isNumber(std::string str) {

    if (str.empty()) return false;

    for (int i = 0; i < str.size(); i++) {
        if (!(str[i] >= '0' && str[i] <= '9'))
            return false;
    }

    return true;
}