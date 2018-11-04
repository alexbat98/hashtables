#include <iostream>

#include "hashtable.h"

int main() {

    hashtable<int, std::string> ht;

    std::string str = "User";

    ht.add(3, str);

    std::string res = ht.get(3);

    std::cout << res << std::endl;

    ht.remove(3);

    bool has = ht.has_key(3);

    std::cout << has << std::endl;
    return 0;
}