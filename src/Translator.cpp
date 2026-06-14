#include "Translator.hpp"
#include "Settings.hpp"

#include <array>
#include <string_view>
#include <span>

#ifndef NDEBUG
#include <iostream>
#include <string>
#include <unordered_set>
#endif

namespace
{
    using namespace std::string_view_literals;

    struct Translation
    {
        std::string_view source;
        std::string_view translated;
    };

    constexpr std::array polishTranslations = {
        Translation{ "Start new game"sv, "Nowa gra"sv },
        Translation{ "Hall of fame"sv, "Hall of fame"sv },
        Translation{ "Settings"sv, "Ustawienia"sv },
        Translation{ "Quit the game"sv, "Zakończ grę"sv },
        Translation{ "Language has been changed"sv, "Język został zmieniony"sv },
        Translation{ "Choose an option"sv, "Wybierz odpowiednią opcję"sv },
        Translation{ "Change language"sv, "Zmień język"sv },
        Translation{ "Clear hall of fame"sv, "Wyczyść hall of fame"sv },
        Translation{ "Toggle challenge mode question"sv, "Włącz/wyłącz pytanie trybu zakładu"sv },
        Translation{ "Return to main menu"sv, "Powrót do menu"sv },
        Translation{ "Polish"sv, "Polski"sv },
        Translation{ "Current language: {}"sv, "Język: {}"sv },
        Translation{ "The game asks about the challenge mode"sv, "Gra pyta o tryb zakładu"sv },
        Translation{ "Main menu"sv, "Menu główne"sv },
        Translation{ "Difficulty selection"sv, "Wybór poziomu trudności"sv },
        Translation{ "Easy"sv, "Łatwy"sv },
        Translation{ "{} ({}-{})"sv, "{} ({}-{})"sv },
        Translation{ "Medium"sv, "Średni"sv },
        Translation{ "Hard"sv, "Trudny"sv },
        Translation{ "What difficulty would you like?"sv, "Jaki poziom trudności chcesz wybrać?"sv },
        Translation{ "Game mode selection"sv, "Wybór typu gry"sv },
        Translation{ "Standard mode"sv, "Gra standardowa"sv },
        Translation{ "New game plus"sv, "Nowa gra plus"sv },
        Translation{ "Which game mode do you want to play?"sv, "W jakim trybie gry chcesz grać?"sv },
        Translation{ "Enabling challenge mode"sv, "Włączenie trybu zakładu"sv },
        Translation{ "No"sv, "Nie"sv },
        Translation{ "Yes"sv, "Tak"sv },
        Translation{ "Would you like to enable challenge mode?"sv, "Czy chcesz włączyć tryb zakładu?"sv },
        Translation{ "Choose the difficulty to view TOP5"sv, "Wybierz poziom trudności do wyświetlenia TOP5"sv },
        Translation{ "TOP5 scores for {} difficulty"sv, "TOP5 wyników dla poziomu trudności: '{}'"sv },
        Translation{ "Enter any input to return to the main menu"sv, "Wpisz cokolwiek, aby wrócić do menu"sv },
        Translation{ "{} attempts"sv, "Liczba prób: {}"sv },
        Translation{ "{}: {} in {}"sv, "{}: {} za {}"sv },
        Translation{ "Language selection"sv, "Wybór języka"sv },
        Translation{ "English"sv, "Angielski"sv },
        Translation{ "Select the language"sv, "Wybierz język"sv },
        Translation{ "You picked the same language. Well done"sv, "Wybrano ten sam język. Genialnie"sv },
        Translation{ "Hall of fame clear confirmation"sv, "Potwierdzenie wyczyszczenia hall of fame"sv },
        Translation{ "Have you thought about it? Do you want to remove all the scores?"sv, "Czy na pewno chcesz usunąć wszystkie wyniki?"sv },
        Translation{ "Hall of fame was cleared"sv, "Wyczyszczono hall of fame"sv },
        Translation{ "Challenge mode asking was changed"sv, "Zmieniono ustawienie pytania o tryb zakładu"sv },
        Translation{ "The game doesn't ask about the challenge mode"sv, "Gra nie pyta o tryb zakładu"sv },
        Translation{ "Gameplay"sv, "Rozgrywka"sv },
        Translation{ "Try to guess the number"sv, "Spróbuj zgadnąć liczbę"sv },
        Translation{ "Difficulty: {}"sv, "Poziom trudności: {}"sv },
        Translation{ "Attempt #{}"sv, "Próba #{}"sv },
        Translation{ "Random number: {}"sv, "Losowa liczba: {}"sv },
        Translation{ "Not quite - the answer is higher"sv, "Nie do końca - odpowiedź brzmi wyżej"sv },
        Translation{ "The secret number lives upstairs"sv, "Tajemnicza liczba mieszka wyżej"sv },
        Translation{ "You're shooting below the mark"sv, "Celujesz za nisko"sv},
        Translation{ "The number is hiding above that"sv, "Liczba ukrywa się wyżej"sv },
        Translation{ "You're under the target"sv, "Jesteś poniżej celu"sv },
        Translation{ "Easy there, that's too much"sv, "Spokojnie, to za dużo"sv },
        Translation{ "Take that guess down a notch"sv, "Obniż trochę ten strzał"sv },
        Translation{ "You overshot it. Pull back"sv, "Przestrzelono. Zejdź niżej"sv },
        Translation{ "Too high - dial it down"sv, "Za wysoko - zmniejsz"sv },
        Translation{ "Bring it back down to earth"sv, "Sprowadź to z powrotem na ziemię"sv },
        Translation{ "Do you really want to enter a number outside the difficulty's range?"sv, "Czy poważnie chcesz wpisać liczbę spoza zakresu poziomu trudności?"sv },
        Translation{ "Please make a guess using a valid number"sv, "Wprowadź proszę poprawną liczbę"sv },
        Translation{ "You made {} mistakes in total"sv, "Liczba błędów: {}"sv },
        Translation{ "Enter your name. You will return to the main menu"sv, "Wprowadź swoje imię. Wrócisz do menu głównego"sv },
        Translation{ "The game took {}"sv, "Gra zajęła {}"sv },
        Translation{ "Reroll in {} attempts"sv, "Liczba prób do ponownego losowania ukrytej liczby: {}"sv },
        Translation{ "You have one last try before the reroll"sv, "Masz ostatnią szansę przed zmianą ukrytej liczby"sv },
        Translation{ "The secret number has been rerolled"sv, "Ukryta liczba została przelosowana"sv },
        Translation{ "{}: {} in {} {}"sv, "{}: {} za {} {}"sv },
        Translation{ "Wow! You guessed the number on your first try"sv, "Wow! Zgadłeś liczbę za pierwszym razem"sv },
        Translation{ "1 attempt"sv, "Liczba prób: 1" },
        Translation{ "Attempts limit selection"sv, "Wybór limitu prób"sv },
        Translation{ "Attempts limit:"sv, "Limit prób:"sv },
        Translation{ "How many tries do you think it will take you to guess?"sv, "Ile prób według ciebie wystarczy, aby zgadnąć liczbę?"sv },
        Translation{ "Attempt #{}/{}"sv, "Próba #{}/{}"sv },
        Translation{ "You ran out of attempts"sv, "Skończyły ci się próby"sv },
        Translation{ "Victory"sv, "Zwycięstwo"sv },
        Translation{ "Defeat"sv, "Porażka"sv },
        Translation{ "You made a mistake just once"sv, "Popełniono tylko jeden błąd"sv }
    };
        

#ifndef NDEBUG
    void logMissingTranslations(std::string_view text)
    {
        static std::unordered_set<std::string> alreadyLogged;

        std::string key;
        key.reserve(text.size());
        key.append(text);

        if (!alreadyLogged.insert(key).second)
            return;

        std::clog << "[i18n] Missing translation: \"" << text << "\"\n";
    }
#endif

    std::string_view findTranslation(std::string_view text, std::span<const Translation> translations)
    {
        for (const auto& translation : translations)
        {
            if (translation.source == text)
                return translation.translated;
        }

#ifndef NDEBUG
        logMissingTranslations(text);
#endif

        return text;
    }
}

std::string_view Translator::translateView(std::string_view text) const
{
    using enum Language;

    switch (this->settings.language)
    {
    case English:
        return text;

    case Polish:
        return findTranslation(text, polishTranslations);
    }

    return text;
}

std::string Translator::translateString(std::string_view text) const
{
    return std::string{ this->translateView(text) };
}
