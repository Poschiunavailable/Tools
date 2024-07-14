// RandomPasswordGenerator.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//
#include <random> 
#include <iostream>
#include <string>
#include <algorithm>
#include <ranges>


class PasswordGenerator
{
private:
    std::random_device randomDevice;
    double entropyValue;
    bool hasEntropy;

public:
    PasswordGenerator()
    {
        entropyValue = randomDevice.entropy();
        hasEntropy = entropyValue > 0;
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

        std::string password(length, '\0'); // Preallocate for efficiency
        std::ranges::generate(password, [&]() {
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
    std::cout << "Enter how many passwords should be generated: ";
    std::cin >> passwordAmount;

    for (int i = 0; i < passwordAmount; ++i) 
    {
        std::string password = passwordGenerator.generatePassword(passwordLength);
        std::cout << password << "\n";
    }

    std::cout << std::endl << "Press any key to exit...";
    std::cin.get(); // Waits for the user to press Enter (or any other key)

    return 0;
}
