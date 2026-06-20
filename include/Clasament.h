#pragma once
#include <algorithm>
#include <ostream>
#include <string>
#include <vector>

// clasa sablon (header-only) - top N intrari sortate descrescator.
// instantiata cu cel putin doua tipuri: Clasament<int> (scoruri) si
// Clasament<double> (timpi). Vezi MeniuRestart.
template <typename T>
class Clasament {
public:
    class Intrare {
    public:
        std::string nume;
        T valoare;
        bool operator>(const Intrare& alt) const { return valoare > alt.valoare; }
    };

private:
    std::vector<Intrare> intrari;
    int capacitate;

public:
    explicit Clasament(int cap = 5) : capacitate(cap) {}

    // adauga si sorteaza descrescator; pastreaza primele `capacitate` intrari
    void adauga(const std::string& nume, T valoare) {
        intrari.push_back({nume, valoare});
        std::sort(intrari.begin(), intrari.end(),
                  [](const Intrare& a, const Intrare& b) { return a > b; });
        if (static_cast<int>(intrari.size()) > capacitate)
            intrari.resize(capacitate);
    }

    // true daca valoarea ar intra in top
    [[nodiscard]] bool esteRecord(T valoare) const {
        if (static_cast<int>(intrari.size()) < capacitate)
            return true;
        return valoare > intrari.back().valoare;
    }

    // cppcheck-suppress unusedFunction
    [[nodiscard]] const std::vector<Intrare>& getIntrari() const { return intrari; }

    // cppcheck-suppress unusedFunction
    [[nodiscard]] int marime() const { return static_cast<int>(intrari.size()); }

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const Clasament<U>& c);
};

template <typename U>
std::ostream& operator<<(std::ostream& os, const Clasament<U>& c) {
    os << "=== Clasament (top " << c.capacitate << ") ===" << std::endl;
    int loc = 1;
    for (const auto& e : c.intrari)
        os << loc++ << ". " << e.nume << " - " << e.valoare << std::endl;
    return os;
}
