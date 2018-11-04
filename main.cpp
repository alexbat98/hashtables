#include <iostream>

#include "hashtable.h"

int main() {

    hashtable<int, std::string> ht;

    std::string str = "User";
    std::string str2 = "Another user";

    ht.add(3, str);
    ht.add(34569, str2);

    std::string res = ht.get(3);

    std::cout << res << std::endl;

    ht.remove(3);

    res = ht.get(34569);

    std::cout << res << std::endl;

    bool has = ht.has_key(3);

    std::cout << has << std::endl;
    return 0;
}