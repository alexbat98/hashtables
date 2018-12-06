#include <iostream>

#include "hash_table.h"
#include "cuckoo_hash_table.h"
#include "open_hash_table.h"

#include <chrono>
#include <string>

int main() {

    hash_table<int, std::string> ht;

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


    long long seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 engine(static_cast<unsigned int>(seed));
    std::uniform_int_distribution<int > distribution(1, 50000);

    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < 1000000; i++) {
        int key = distribution(engine);
        std::string value = "value";

        ht.add(key, value);
    }
    auto end = std::chrono::system_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms" << std::endl;

    cuckoo_hash_table<int, std::string> cth;
    cth.add(0, str);
    cth.add(1, str);
    cth.add(3, str);
    cth.add(5, str);

    std::cout << cth.has_key(0) << std::endl;
    std::cout << cth.get(0) << std::endl;

    open_hash_table<int, std::string> oth;
    oth.add(0, str);
    oth.add(1, str);
    oth.add(3, str);
    oth.add(5, str);

    std::cout << oth.has_key(0) << std::endl;
    std::cout << oth.get(0) << std::endl;

#ifdef PAUSE_ON_EXIT
	std::cout << "Press any key to continue..." << std::endl;
	std::cin.get();
#endif

    return 0;
}