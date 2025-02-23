
//Licensed under MIT License - © Patrick Temborius 2024

#include <random> 
#include <iostream>
#include <string>
#include <algorithm>
#include <ranges>


class PasswordGenerator
{
private:
    std::random_device randomDevice;
    bool hasEntropy;

public:
    PasswordGenerator()
    {
        hasEntropy = randomDevice.entropy() > 0;
        if (hasEntropy)
        {
            std::cout << "Using real random number generator." << std::endl;
        }
        else
        {
            std::cout << "Using pseudo random number generator." << std::endl;
        }
    }

    std::string generatePassword(int length) {
        constexpr std::string_view characters = R"(abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{};':"\|,.<>/?)";

        std::uniform_int_distribution<std::string::size_type> dist(0, characters.size() - 1);

        std::mt19937 gen(randomDevice());

        // Preallocate for efficiency
        std::string password(length, '\0'); 
        std::ranges::generate(password, [&]() 
        {
            // Prefer the hardware random number generator, otherwise fallback to engine
            return characters[hasEntropy ? dist(randomDevice) : dist(gen)];
        });

        // Shuffle using ranges
        std::ranges::shuffle(password, std::mt19937(randomDevice()));

        return password;
    }
};


int main()
{
    auto passwordGenerator = PasswordGenerator();

    int passwordLength;
    std::cout << "Enter desired password length: ";
    std::cin >> passwordLength;

    int passwordAmount;
    std::cout << "Enter amount of passwords to be generated: ";
    std::cin >> passwordAmount;

    for (int i = 0; i < passwordAmount; ++i) 
    {
        std::string password = passwordGenerator.generatePassword(passwordLength);
        std::cout << password << "\n";
    }
    std::cin.get();

    std::cout << std::endl << "Press any key to exit...";
    std::cin.get(); // Waits for the user to press Enter (or any other key)

    return 0;
}
