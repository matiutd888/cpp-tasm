#ifndef COMPUTER_H
#define COMPUTER_H

#include <cstdio>
#include <array>
#include <cstring>
#include <cassert>
#include <string>

// Listy
/*
struct null_list {
    using _head = null_list;
    using _tail = null_list;
};

template <typename head, typename tail = null_list>
struct list {
    using _head = head;
    using _tail = tail;
};

template <typename ...args>
struct make_list;

template <typename head>
struct make_list<head> {
    using _list = list<head>;
};

template <typename head, typename ...tail>
struct make_list<head, tail...> {
    using _list = list<head, typename make_list<tail...>::_list >;
}; */

using code_type = uint_fast64_t;
constexpr code_type id_code_base = 64;

//Poprawna lewa wartość (l-wartość) w TMPAsm to Mem.
//Poprawne prawe wartości (p-wartość) w TMPAsm to Mem, Num, Lea.

//Podstawowe elementy języka TMPAsm:

//Identyfikatory zmiennej lub etykiety Id Id(const char*) – identyfikator; identyfikator tworzony jest na
//podstawie ciągu znaków o długości od 1 do 6 włącznie, zawierającego małe i wielkie litery alfabetu angielskiego
// (a-zA-Z) oraz cyfry (0-9); małe i wielkie litery nie są rozróżniane. Przykłady poprawnych identyfikatorów:
// Id("A"), Id("01234"), Id("Cdefg").

static constexpr code_type get_code(char c) {
    if (c >= 'a' && c <= 'z')
        c = c + 'A' - 'a';

    if (c >= '0' && c <= '9')
        return c - '0' + 1;
    else if (c >= 'A' && c <= 'Z')
        return c - 'A' + '9' - '0' + 2;
    //TODO exception zamiast return
    return 0;
}

static constexpr code_type Id(const char *id_str) {//TODO jeśli id_str jest nie poprawny to program nie może się skompilować
    std::basic_string_view<char> s(id_str);
    code_type p = id_code_base;
    code_type res = 0;
    bool czy = true;
    for (char i : s) {
        if (i == '\0')
            czy = false;
        code_type c = 0;
        if (czy)
            c = get_code(i);
        res = res * p + c;
    }
    return res;
}

//Literały numeryczne Num Literały całkowitoliczbowe. Przykłady poprawnych literałów: Num<13>, Num<0>, Num<-50>.
template<auto T>
struct Num;

//Pamięć Mem Mem<Addr> – dostęp do komórki pamięci pod adresem Addr; Addr musi być poprawną p-wartością.
// Odwołania poza dostępną pamięć są wykrywane i zgłaszane jako błąd. Zakres adresacji jest zgodny z wersją unsigned
//typu słowa zdefiniowanego dla danego komputera. Przykłady poprawnych odwołań do pamięci: Mem<Num<0>>, Mem<Lea<Id("a")>>.
template<typename T>
struct Mem;

//Pobranie efektywnego adresu zmiennej Lea, Lea<Id> – zwraca wartość efektywnego adresu zmiennej Id.
//Przykłady poprawnych pobrań adresu zmiennej: Lea<Id("A")>, Lea<Id("a")>.
template<code_type T>
struct Lea;

//Program w języku TMPAsm składa się z ciągu instrukcji. Podczas ładowania programu pamięć komputera jest inicjowana
// zerami. Następnie wszystkie zmienne są kopiowane do pamięci komputera zgodnie z kolejnością deklaracji,
//a później wykonywane są pozostałe instrukcje.
template<typename ...T>
struct Program;

//TMPAsm wspiera następujące instrukcje:

//Deklaracja zmiennej D D<Id, Value> – deklaruje zmienną o identyfikatorze Id oraz wartości numerycznej Value.
//Przykład poprawnej deklaracji zmiennej: D<Id("A"), Num<5>>.
template<code_type code, typename A>
struct D;


//Operacja kopiowania Mov Mov<Dst, Src> – kopiuje wartość Src do Dst; Dst musi być poprawną l-wartością,
// natomiast Src musi być poprawną p-wartością.
// Przykłady poprawnych instrukcji: Mov<Mem<Num<0>>, Num<13>>, Mov<Mem<Lea<Id("abc")>>, Mem<Num<0>>>.

template<typename A, typename B>
struct Mov;


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

template<typename Arg1, typename Arg2>
struct Add;

template<typename Arg1, typename Arg2>
struct Sub;

template<typename Arg1>
struct Inc;

template<typename Arg1>
struct Dec;

//Operacje logiczne And, Or, Not
//And<Arg1, Arg2> – bitowa operacja and
//Or<Arg1, Arg2> – bitowa operacja or
//Wynik powyższych operacji jest umieszczany w Arg1. Arg1 musi być poprawną l-wartością, natomiast Arg2 musi być
//poprawną p-wartością.
//Not<Arg> – bitowa negacja
//Arg musi być poprawną l-wartością. Wszystkie operacje logiczne ustawiają: - flagę ZF (zero flag) procesora na 1,
//jeśli wynik jest 0, a na 0 w przeciwnym przypadku.
//Przykłady poprawnych operacji: And<Mem<Num<0>>, Num<1>>, Not<Mem<Lea<Id("a")>>>.

template<typename Arg1, typename Arg2>
struct And;

template<typename Arg1, typename Arg2>
struct Or;

template<typename Arg>
struct Not;

//Operację porównania Cmp
//Cmp<Arg1, Arg2> – działa jak operacja odejmowania, ale nie zapisuje wyniku, a tylko ustawia flagi.
//Arg1 oraz Arg2 muszą być poprawnymi p-wartościami. Przykład poprawnej operacji: Cmp<Mem<Num<0>>, Num<1>>.


//Oznaczenie etykiety Label
//Label<Id> – ustawienie etykiety o identyfikatorze Id. Przykład poprawnej etykiety: Label<Id("label")>.

template<code_type code>
struct Label;
//Instrukcje skoków Jmp, Jz, Js
//Jmp<Label> – skok bezwarunkowy do etykiety o identyfikatorze Label //TODO liniowe wyszukanie w liście
//Jz<Label>  – skok warunkowy do Label w przypadku gdy flaga ZF jest ustawiona na 1
//Js<Label>  – skok warunkowy do Label w przypadku gdy flaga SF jest ustawiona na 1
//Przykłady poprawnych skoków:
//Jmp<Id("label")>, Jz<Id("stop")>.


template<code_type label_code>
struct Jmp;

template<code_type label_code>
struct Jz;

template<code_type label_code>
struct Js;


//Szablon klasy Computer powinien mieć następujące parametry: wielkość pamięci – dodatnia wartość określająca liczbę
//komórek pamięci w słowach; typ słowa – typ całkowitoliczbowy reprezentujący podstawową jednostkę pamięci.
template<size_t size, typename T>
struct Computer {
private:
    using memory_t = std::array<T, size>;
    using ids_t = std::array<code_type, size>;
    struct hardware {
        memory_t mem;
        ids_t ids;
        bool ZF;
        bool SF;
    };

public:
    template<typename Prog>
    static constexpr std::array<T, size> boot() {
        hardware h = {memory_t(), ids_t(), false, false};
        ComputerProgram<Prog>::run(h);
        return h.mem;
    };

private:
    // TODO jaki typ result
    // Czy unsigned - unsigned ma być ujemny????
    static constexpr void set_flags_arthmetic(hardware &h, T result) {
        if (result == 0)
            h.ZF = 1;
        else h.ZF = 0;

        if (result < 0)
            h.SF = 1;
        else h.SF = 0;
    }

    static constexpr void set_flags_logical(hardware &h, T result) {
        if (result == 0)
            h.ZF = 1;
        else h.ZF = 0;
    }

    template<typename V>
    struct Evaluator;

    template<auto val>
    struct Evaluator<Num<val>> {
        static constexpr auto rvalue(hardware &h) {
            return val;
        }
    };

    template<code_type code>
    struct Evaluator<Lea<code>> {
        static constexpr auto rvalue(hardware &h) {
            for (int i = 0; i < h.ids.size(); i++) {
                if (code == h.ids[i])
                    return i;
            }
        }
    };

    // TODO sprawdzanie zakresu
    template<typename B>
    struct Evaluator<Mem<B>> {
        static constexpr auto rvalue(hardware &h) {
            return h.mem[Evaluator<B>::rvalue(h)];
        }

        static constexpr auto &lvalue(hardware &h) {
            return h.mem[Evaluator<B>::rvalue(h)];
        }
    };

    template<typename P>
    struct ComputerProgram;

    template<typename... Instructions>
    struct ComputerProgram<Program<Instructions...>> {
        constexpr static void run(hardware &h) {
            InstructionsParser<Instructions...>::evaluate(h);
        }
    };

    template<typename... Instructions>
    struct InstructionsParser {
        constexpr static void evaluate(hardware &h) {

        }
    };

    /* // TODO błąd kompilacji?
     template<>
     struct InstructionsParser<> {
         constexpr static void evaluate(hardware &h) {
         }
     };*/

    template<typename Dst, typename Src, typename... Instructions>
    struct InstructionsParser<Mov<Dst, Src>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            Evaluator<Dst>::lvalue(h) = Evaluator<Src>::rvalue(h);
            InstructionsParser<Instructions...>::evaluate(h);
        }
    };

    template<typename Arg1, typename Arg2, typename... Instructions>
    struct InstructionsParser<Add<Arg1, Arg2>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            auto result = Evaluator<Arg1>::rvalue(h) + Evaluator<Arg2>::rvalue(h);
            Evaluator<Arg1>::lvalue(h) = result;
            set_flags_arthmetic(h, result);
            InstructionsParser<Instructions...>::evaluate(h);
        }
    };

    template<typename Arg1, typename Arg2, typename... Instructions>
    struct InstructionsParser<Sub<Arg1, Arg2>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            auto result = Evaluator<Arg1>::rvalue(h) - Evaluator<Arg2>::rvalue(h);
            Evaluator<Arg1>::lvalue(h) = result;
            set_flags_arthmetic(h, result);
            InstructionsParser<Instructions...>::evaluate(h);
        }
    };

    template<typename Arg1, typename... Instructions>
    struct InstructionsParser<Inc<Arg1>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            auto result = Evaluator<Arg1>::lvalue(h)++;
            set_flags_arthmetic(h, result);
            InstructionsParser<Instructions...>::evaluate(h);
        }
    };

    template<typename Arg1, typename... Instructions>
    struct InstructionsParser<Dec<Arg1>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            auto result = Evaluator<Arg1>::lvalue(h)--;
            set_flags_arthmetic(h, result);
            InstructionsParser<Instructions...>::evaluate(h);
        }
    };

    template<code_type label_code, typename... Instructions>
    struct InstructionsParser<Jmp<label_code>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            LabelParser<label_code, Instructions...>::evaluate(h);
        }
    };

    template<code_type label_code, typename... Instructions>
    struct InstructionsParser<Jz<label_code>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            if (h.ZF)
                InstructionsParser<Jmp<label_code>, Instructions...>::evaluate(h);
            else InstructionsParser<Instructions...>::evaluate(h);
        }
    };

    template<code_type label_code, typename... Instructions>
    struct InstructionsParser<Js<label_code>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            if (h.SF)
                InstructionsParser<Jmp<label_code>, Instructions...>::evaluate(h);
            else InstructionsParser<Instructions...>::evaluate(h);
        }
    };

    //--------------------LABEL PARSER
    template<const code_type label_to_find, typename... Instr>
    struct LabelParser;

    template<const code_type label_to_find, code_type code, typename... Instr>
    struct LabelParser<label_to_find, Label<code>, Instr...> {
        static constexpr void evaluate(hardware &h) {
            if (label_to_find == code)
                InstructionsParser<Instr...>::evaluate(h);
            else LabelParser<label_to_find, Instr...>::evaluate(h);
        }
    };

    template<const code_type label_to_find, typename A, typename... Instr>
    struct LabelParser<label_to_find, A, Instr...> {
        static constexpr void evaluate(hardware &h) {
            LabelParser<label_to_find, Instr...>::evaluate(h);
        }
    };

    template<const code_type label_to_find>
    struct LabelParser<label_to_find> {
        static constexpr void evaluate(hardware &h) {
            static_assert("No label!");
        }
    };

    template<typename Arg1, typename Arg2, typename... Instructions>
    struct InstructionsParser<And<Arg1, Arg2>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            auto result = Evaluator<Arg1>::rvalue(h) & Evaluator<Arg2>::rvalue(h);
            Evaluator<Arg1>::lvalue(h) = result;
            set_flags_logical(h, result);
            InstructionsParser<Instructions...>::evaluate(h);
        }
    };

    template<typename Arg1, typename Arg2, typename... Instructions>
    struct InstructionsParser<Or<Arg1, Arg2>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            auto result = Evaluator<Arg1>::rvalue(h) | Evaluator<Arg2>::rvalue(h);
            Evaluator<Arg1>::lvalue(h) = result;
            set_flags_logical(h, result);
            InstructionsParser<Instructions...>::evaluate(h);
        }
    };

    template<typename Arg, typename... Instructions>
    struct InstructionsParser<Not<Arg>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            auto result = ~Evaluator<Arg>::rvalue(h);
            Evaluator<Arg>::lvalue(h) = result;
            set_flags_logical(h, result);
            InstructionsParser<Instructions...>::evaluate(h);
        }
    };
};

// TODO declaration parser

#endif //COMPUTER_H