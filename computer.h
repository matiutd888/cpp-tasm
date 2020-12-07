#ifndef COMPUTER_H
#define COMPUTER_H

#include <vector>
#include <cstdint>

//TODO jeśli const expr funckja rzuci wyjątek lub asserta to bedzie błąd kompilacji
//TODO ogolnie mozna używać wielu rzeczy, które są constexpr
//Poprawna lewa wartość (l-wartość) w TMPAsm to Mem.
//Poprawne prawe wartości (p-wartość) w TMPAsm to Mem, Num, Lea.


//Podstawowe elementy języka TMPAsm:

//Identyfikatory zmiennej lub etykiety Id
//Id(const char*) – identyfikator; identyfikator tworzony jest na
//podstawie ciągu znaków o długości od 1 do 6 włącznie, zawierającego małe
//i wielkie litery alfabetu angielskiego (a-zA-Z) oraz cyfry (0-9);
//małe i wielkie litery nie są rozróżniane.
//Przykłady poprawnych identyfikatorów: Id("A"), Id("01234"), Id("Cdefg").
template <const char* str>
struct Id { //TODO -> funckja ale mozna zostawic tez struct
    //TODO jeśli value jest nie poprawny to program nie może się skompilować
    constexpr static const char* value = str;
};

//Literały numeryczne Num
//Literały całkowitoliczbowe.
//Przykłady poprawnych literałów:
template <int N>
struct Num {
    constexpr static int value = N;
};

//Pamięć Mem
//Mem<Addr> – dostęp do komórki pamięci pod adresem Addr; Addr musi być
//        poprawną p-wartością. Odwołania poza dostępną pamięć są wykrywane
//i zgłaszane jako błąd. Zakres adresacji jest zgodny z wersją unsigned
//        typu słowa zdefiniowanego dla danego komputera.
//Przykłady poprawnych odwołań do pamięci:
//Mem<Num<0>>, Mem<Lea<Id("a")>>.
template <typename T>
struct Mem {
};

//Pobranie efektywnego adresu zmiennej Lea
//        Lea<Id> – zwraca wartość efektywnego adresu zmiennej Id.
//Przykłady poprawnych pobrań adresu zmiennej:
//Lea<Id("A")>, Lea<Id("a")>.
template <typename T>
struct Lea {
};

//Program w języku TMPAsm składa się z ciągu instrukcji. Podczas ładowania
//programu pamięć komputera jest inicjowana zerami. Następnie wszystkie zmienne
//        są kopiowane do pamięci komputera zgodnie z kolejnością deklaracji,
//        a później wykonywane są pozostałe instrukcje.
template <typename... Operations>
struct Program {}; //TODO lista typów, pierwszy element i mamy ogon i z każdym z przeglądanych elementów można coś zroić

template <>
struct Program<> {
    constexpr static int value = 0;
};

template <typename Operation, typename... Rest>
struct Program<Operation, Rest...> {
    constexpr static int value = Operation::value + Program<Rest...>::value;
};

//TODO struktura ewaluator
//sparametryzowane przez instrukcje add, itd itp


//Szablon klasy Computer powinien mieć następujące parametry:
//wielkość pamięci – dodatnia wartość określająca liczbę komórek pamięci
//w słowach;
//typ słowa – typ całkowitoliczbowy reprezentujący podstawową jednostkę
//        pamięci.

template <size_t size, typename T>
class Computer {
private:
    static constexpr T memory [size];
public:
    //Dodatkowo klasa Computer powinna mieć metodę klasową boot, która załaduje
//oraz wykona przekazany program w języku TMPAsm (Template Metaprogramming
//Assembler) w czasie kompilowania programu.
    template <typename XD>
    constexpr static std::array<T, size> boot() {

        return std::array<T, size>();
    }
};

//TMPAsm wspiera następujące instrukcje:

//Deklaracja zmiennej D
//D<Id, Value> – deklaruje zmienną o identyfikatorze Id oraz wartości
//numerycznej Value.
//Przykład poprawnej deklaracji zmiennej:
//D<Id("A"), Num<5>>.


//Operacja kopiowania Mov
//        Mov<Dst, Src> – kopiuje wartość Src do Dst; Dst musi być poprawną
//l-wartością, natomiast Src musi być poprawną p-wartością.
//Przykłady poprawnych instrukcji:
//Mov<Mem<Num<0>>, Num<13>>, Mov<Mem<Lea<Id("abc")>>, Mem<Num<0>>>.


//Operacje arytmetyczne Add, Sub, Inc, Dec
//        Add<Arg1, Arg2> – dodawanie
//        Sub<Arg1, Arg2> – odejmowanie
//        Wynik powyższych operacji jest umieszczany w Arg1.
//Arg1 musi być poprawną l-wartością, natomiast Arg2 musi być poprawną
//        p-wartością.
//Inc<Arg> – specjalizacja dodawania, zwiększająca wartość Arg o 1
//Dec<Arg> – specjalizacja odejmowania, zmniejszająca wartość Arg o 1
//Arg musi być poprawną l-wartością.
//Wszystkie operacje arytmetyczne ustawiają:
//- flagę ZF (zero flag) procesora na 1, jeśli wynik jest 0,
//a na 0 w przeciwnym przypadku.
//- flagę SF (sign flag) procesora na 1, jeśli wynik jest ujemny,
//        a na 0 w przeciwnym przypadku.
//Operacje arytmetyczne są wykonywane zgodnie z typem słowa zdefiniowanym
//        dla danego komputera.
//Przykłady poprawnych operacji:
//Add<Mem<Num<0>>, Num<1>>, Inc<Mem<Lea<Id("a")>>>.


//Operacje logiczne And, Or, Not
//        And<Arg1, Arg2> – bitowa operacja and
//Or<Arg1, Arg2> – bitowa operacja or
//Wynik powyższych operacji jest umieszczany w Arg1.
//Arg1 musi być poprawną l-wartością, natomiast Arg2 musi być poprawną
//        p-wartością.
//Not<Arg> – bitowa negacja
//Arg musi być poprawną l-wartością.
//Wszystkie operacje logiczne ustawiają:
//- flagę ZF (zero flag) procesora na 1, jeśli wynik jest 0,
//a na 0 w przeciwnym przypadku.
//Przykłady poprawnych operacji:
//And<Mem<Num<0>>, Num<1>>, Not<Mem<Lea<Id("a")>>>.


//Operację porównania Cmp
//        Cmp<Arg1, Arg2> – działa jak operacja odejmowania, ale nie zapisuje wyniku,
//a tylko ustawia flagi.
//Arg1 oraz Arg2 muszą być poprawnymi p-wartościami.
//Przykład poprawnej operacji: Cmp<Mem<Num<0>>, Num<1>>.


//Oznaczenie etykiety Label
//        Label<Id> – ustawienie etykiety o identyfikatorze Id.
//Przykład poprawnej etykiety:
//Label<Id("label")>.


//Instrukcje skoków Jmp, Jz, Js
//        Jmp<Label> – skok bezwarunkowy do etykiety o identyfikatorze Label //TODO liniowe wyszukanie w liście
//Jz<Label>  – skok warunkowy do Label w przypadku gdy flaga ZF jest ustawiona na 1
//Js<Label>  – skok warunkowy do Label w przypadku gdy flaga SF jest ustawiona na 1
//Przykłady poprawnych skoków:
//Jmp<Id("label")>, Jz<Id("stop")>.


#endif //COMPUTER_H