#ifndef COMPUTER_H
#define COMPUTER_H

#include <cstdio>
#include <array>
#include <cstring>
#include <cassert>
#include <string>
#include <cstdlib>

using code_t = uint_fast64_t;
constexpr code_t id_code_base = 38;
constexpr size_t id_size_min = 1;
constexpr size_t id_size_max = 6;

//Poprawna lewa wartość (l-wartość) w TMPAsm to Mem.
//Poprawne prawe wartości (p-wartość) w TMPAsm to Mem, Num, Lea.

//Podstawowe elementy języka TMPAsm:

//Identyfikatory zmiennej lub etykiety Id Id(const char*) – identyfikator; identyfikator tworzony jest na
//podstawie ciągu znaków o długości od 1 do 6 włącznie, zawierającego małe i wielkie litery alfabetu angielskiego
// (a-zA-Z) oraz cyfry (0-9); małe i wielkie litery nie są rozróżniane. Przykłady poprawnych identyfikatorów:
// Id("A"), Id("01234"), Id("Cdefg").

/*template <char c>
static constexpr bool check_if_sign_valid() {
    return (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z');
}*/

static constexpr code_t get_id(const char &c) {
    /*static_assert((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z'),
                  "ID not valid: wrong sign!");*/
    // TODO this assert doesnt work :(
    char cc = c;
    if (cc >= 'a' && cc <= 'z')
        cc = cc + 'A' - 'a';
    if (cc >= '0' && cc <= '9')
        return cc - '0' + 1;
    else if (cc >= 'A' && cc <= 'Z')
        return cc - 'A' + '9' - '0' + 2;
    else {
        //TODO NOT VALID SIGNS
    }
}

static constexpr code_t Id(const char *id_str) {
    std::basic_string_view<char> s(id_str);
    if (id_size_min <= s.size() && s.size() <= id_size_max) {
        code_t p = id_code_base;
        code_t res = 0;
        bool czy = true;
        for (char i : s) {
            if (i == '\0')
                czy = false;
            code_t c = 0;
            if (czy)
                c = get_id(i);
            res = res * p + c;
        }
        return res;
    } else {
        //TODO NOT VALID ID LENGHT
    }
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
template<code_t T>
struct Lea;

//Program w języku TMPAsm składa się z ciągu instrukcji. Podczas ładowania programu pamięć komputera jest inicjowana
// zerami. Następnie wszystkie zmienne są kopiowane do pamięci komputera zgodnie z kolejnością deklaracji,
//a później wykonywane są pozostałe instrukcje.
template<typename ...T>
struct Program;

//TMPAsm wspiera następujące instrukcje:

//Deklaracja zmiennej D D<Id, Value> – deklaruje zmienną o identyfikatorze Id oraz wartości numerycznej Value.
//Przykład poprawnej deklaracji zmiennej: D<Id("A"), Num<5>>.
template<code_t id_code, typename Value>
struct D;

//Operacja kopiowania Mov Mov<Dst, Src> – kopiuje wartość Src do Dst; Dst musi być poprawną l-wartością,
// natomiast Src musi być poprawną p-wartością.
// Przykłady poprawnych instrukcji: Mov<Mem<Num<0>>, Num<13>>, Mov<Mem<Lea<Id("abc")>>, Mem<Num<0>>>.
template<typename Dst, typename Src>
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

template<typename Arg1, typename Arg2>
struct And;

template<typename Arg1, typename Arg2>
struct Or;

template<typename Arg>
struct Not;

template<typename Arg1, typename Arg2>
struct Cmp;

template<code_t id_code>
struct Label;

template<code_t label_code>
struct Jmp;

template<code_t label_code>
struct Jz;

template<code_t label_code>
struct Js;

//Szablon klasy Computer powinien mieć następujące parametry: wielkość pamięci – dodatnia wartość określająca liczbę
//komórek pamięci w słowach; typ słowa – typ całkowitoliczbowy reprezentujący podstawową jednostkę pamięci.
template<size_t size, typename word_t>
struct Computer {
private:
    using memory_t = std::array<word_t, size>;
    using id_codes_t = std::array<code_t, size>;
    struct hardware {
        memory_t mem;
        id_codes_t id_codes;
        bool ZF;
        bool SF;
        size_t ind;
    };

public:
    template<typename Prog>
    static constexpr std::array<word_t, size> boot() {
        hardware h = {memory_t(), id_codes_t(), false, false, 0};
        ComputerProgram<Prog>::declare_variables(h);
        ComputerProgram<Prog>::run(h);
        return h.mem;
    };

private:
    static constexpr void set_flags_arthmetic(hardware &h, word_t result) {
        if (result == 0)
            h.ZF = 1;
        else h.ZF = 0;

        if (result < 0)
            h.SF = 1;
        else h.SF = 0;
    }

    static constexpr void set_flags_logical(hardware &h, word_t result) {
        if (result == 0)
            h.ZF = 1;
        else h.ZF = 0;
    }

    //DECLARATION PARSER
    template<typename... Instr>
    struct DeclarationParser;

    template<>
    struct DeclarationParser<> {
        constexpr static void evaluate([[maybe_unused]] hardware &h) {
        }
    };

    template<code_t id_code, typename Value, typename... Instructions>
    struct DeclarationParser<D<id_code, Value>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            if (h.ind < h.mem.size()) {
                h.id_codes[h.ind] = id_code;
                h.mem[h.ind] = Evaluator<Value>::rvalue(h);
                h.ind++;
            } else {
                // TODO
            }
            DeclarationParser<Instructions...>::evaluate(h);
        }
    };

    template<typename Skip, typename... Instructions>
    struct DeclarationParser<Skip, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            DeclarationParser<Instructions...>::evaluate(h);
        }
    };

    //EVALUATOR
    template<typename V>
    struct Evaluator;

    template<auto val>
    struct Evaluator<Num<val>> {
        static constexpr auto rvalue([[maybe_unused]] hardware &h) {
            return val;
        }
    };

    static constexpr bool array_has(const id_codes_t &id_codes, code_t code) {
        for (const auto code_it : id_codes) {
            if (code_it == code) return true;
        }
        return false;
    }

    template<code_t id_code>
    struct Evaluator<Lea<id_code>> {
        static constexpr auto rvalue(hardware &h) {
            /*static_assert(array_has(h.id_codes, id_code), "No ID in memory!");*/
            size_t ret = h.ind;
            for (size_t i = 0; i < h.ind; i++) {
                if (id_code == h.id_codes[i]) {
                    ret = i;
                    return ret;
                }
            }
        }
    };

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
            InstructionsParser<Program<Instructions...>, Instructions...>::evaluate(h);
        }

        constexpr static void declare_variables(hardware &h) {
            DeclarationParser<Instructions...>::evaluate(h);
        }
    };

    //INSTRUCTIONS PARSER
    // TODO błędna instrukcja nie jest tu wykrywana (ta struktura nie powinna mieć implementacji)
    template<typename... Instructions>
    struct InstructionsParser;

    // TODO błąd kompilacji?
    template<typename ...OrginalInstructions>
    struct InstructionsParser<Program<OrginalInstructions...>> {
        constexpr static void evaluate([[maybe_unused]] hardware &h) {
        }
    };

    // w celach ignorowania labela pojawiajacego się przed jumpem
    template<typename ...OrginalInstructions, code_t ignore, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Label<ignore>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, typename Dst, typename Src, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Mov<Dst, Src>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            Evaluator<Dst>::lvalue(h) = Evaluator<Src>::rvalue(h);
            InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, typename Arg1, typename Arg2, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Add<Arg1, Arg2>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            auto result = Evaluator<Arg1>::rvalue(h) + Evaluator<Arg2>::rvalue(h);
            Evaluator<Arg1>::lvalue(h) = result;
            set_flags_arthmetic(h, result);
            InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, typename Arg1, typename Arg2, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Sub<Arg1, Arg2>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            auto result = Evaluator<Arg1>::rvalue(h) - Evaluator<Arg2>::rvalue(h);
            Evaluator<Arg1>::lvalue(h) = result;
            set_flags_arthmetic(h, result);
            InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, typename Arg1, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Inc<Arg1>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            auto result = Evaluator<Arg1>::lvalue(h)++;
            set_flags_arthmetic(h, result);
            InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, typename Arg1, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Dec<Arg1>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            auto result = Evaluator<Arg1>::lvalue(h)--;
            set_flags_arthmetic(h, result);
            InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, code_t label_code, typename Value, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, D<label_code, Value>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, code_t label_code, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Jmp<label_code>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            LabelParser<Program<OrginalInstructions...>, label_code, OrginalInstructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, code_t label_code, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Jz<label_code>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            if (h.ZF)
                InstructionsParser<Program<OrginalInstructions...>, Jmp<label_code>, Instructions...>::evaluate(h);
            else InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, code_t label_code, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Js<label_code>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            if (h.SF)
                InstructionsParser<Program<OrginalInstructions...>, Jmp<label_code>, Instructions...>::evaluate(h);
            else InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, typename Arg1, typename Arg2, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, And<Arg1, Arg2>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            auto result = Evaluator<Arg1>::rvalue(h) & Evaluator<Arg2>::rvalue(h);
            Evaluator<Arg1>::lvalue(h) = result;
            set_flags_logical(h, result);
            InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, typename Arg1, typename Arg2, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Or<Arg1, Arg2>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            auto result = Evaluator<Arg1>::rvalue(h) | Evaluator<Arg2>::rvalue(h);
            Evaluator<Arg1>::lvalue(h) = result;
            set_flags_logical(h, result);
            InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, typename Arg, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Not<Arg>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            auto result = ~Evaluator<Arg>::rvalue(h);
            Evaluator<Arg>::lvalue(h) = result;
            set_flags_logical(h, result);
            InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, typename Arg1, typename Arg2, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Cmp<Arg1, Arg2>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            auto result = Evaluator<Arg1>::rvalue(h) - Evaluator<Arg2>::rvalue(h);
            set_flags_arthmetic(h, result);
            InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }
    };

    //LABEL PARSER
    template<typename Program, code_t label_to_find, typename... Instr>
    struct LabelParser;

    template<typename ...OrginalInstructions, code_t label_to_find, code_t id_code, typename... Instr>
    struct LabelParser<Program<OrginalInstructions...>, label_to_find, Label<id_code>, Instr...> {
        constexpr static void evaluate(hardware &h) {
            if (label_to_find == id_code)
                InstructionsParser<Program<OrginalInstructions...>, Instr...>::evaluate(h);
            else LabelParser<Program<OrginalInstructions...>, label_to_find, Instr...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, code_t label_to_find, typename A, typename... Instr>
    struct LabelParser<Program<OrginalInstructions...>, label_to_find, A, Instr...> {
        constexpr static void evaluate(hardware &h) {
            LabelParser<Program<OrginalInstructions...>, label_to_find, Instr...>::evaluate(h);
        }
    };


    template<typename ...OrginalInstructions, code_t label_to_find>
    struct LabelParser<Program<OrginalInstructions...>, label_to_find> {
        constexpr static void evaluate([[maybe_unused]] hardware &h) {
            //TODO LABEL NOT FOUND albo zostawić skomentowane
        }
    };
};

#endif //COMPUTER_H