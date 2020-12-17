#ifndef COMPUTER_H
#define COMPUTER_H

#include <string>
#include <stdexcept>
#include <array>

namespace priv_ns {
    using id_type = uint_fast64_t; // Typ zwracany przez Id(str), reprezentuje kod reprezentujący dane id.
    constexpr id_type id_code_base = 36; // Liczba różnych znaków w łańcuchach znaków reprezentujących.
    constexpr size_t id_size_min = 1;
    constexpr size_t id_size_max = 6;

    // Zwraca kod znaku.
    // Znaki reprezentujące ID są cyframi lub literami.
    // Na początku jeśli 'c' jest małą literą, zamieniane jest na dużą literę.
    // Następnie zwracany jest numer znaku 'c' w ciągu '0', '1', ..., '9', 'A', 'B', ..., 'Z'
    // (gdzie numer znaku '0' jest równy 1, itd).
    // Jeżeli 'c' nie jest ani cyfrą ani literą rzucany jest wyjątek std::invalid_argument.
    constexpr id_type get_char_id(char c) {
        if (c >= 'a' && c <= 'z')
            c = c + 'A' - 'a';
        if (c >= '0' && c <= '9')
            return c - '0' + 1;
        else if (c >= 'A' && c <= 'Z')
            return c - 'A' + '9' - '0' + 2;
        else {
            throw std::invalid_argument("NOT VALID ID SIGN!");
        }
    }

    struct Instr {
    };
}

// Zwraca zakodowane różnowartościowo id reprezentowane przez łańcuch 'id_str'.
// Jeżeli długość 'id_str' nie jest z zakresu {1, ..., 6} rzuca std::invalid_argument.
constexpr priv_ns::id_type Id(const char *id_str) {
    std::basic_string_view<char> s(id_str);
    if (priv_ns::id_size_min <= s.size() && s.size() <= priv_ns::id_size_max) {
        priv_ns::id_type p = priv_ns::id_code_base;
        priv_ns::id_type res = 0;
        for (char i : s) {
            if (i != '\0') {
                priv_ns::id_type c = priv_ns::get_char_id(i);
                res = res * p + c;
            }
        }
        return res;
    } else {
        throw std::invalid_argument("NOT VALID ID LENGHT!");
    }
}

template<auto T>
struct Num;

template<typename T>
struct Mem;

template<priv_ns::id_type T>
struct Lea;

template<typename ...T>
struct Program;

template<priv_ns::id_type id, typename Value>
struct D : priv_ns::Instr {
};

template<typename Dst, typename Src>
struct Mov : priv_ns::Instr {
};

template<typename Arg1, typename Arg2>
struct Add : priv_ns::Instr {
};

template<typename Arg1, typename Arg2>
struct Sub : priv_ns::Instr {
};

template<typename Arg1>
struct Inc : priv_ns::Instr {
};

template<typename Arg1>
struct Dec : priv_ns::Instr {
};

template<typename Arg1, typename Arg2>
struct And : priv_ns::Instr {
};

template<typename Arg1, typename Arg2>
struct Or : priv_ns::Instr {
};

template<typename Arg>
struct Not : priv_ns::Instr {
};

template<typename Arg1, typename Arg2>
struct Cmp : priv_ns::Instr {
};

template<priv_ns::id_type id>
struct Label : priv_ns::Instr {
};

template<priv_ns::id_type label>
struct Jmp : priv_ns::Instr {
};

template<priv_ns::id_type label>
struct Jz : priv_ns::Instr {
};

template<priv_ns::id_type label>
struct Js : priv_ns::Instr {
};

template<size_t size, typename word_t>
struct Computer {
private:
    // Typ reprezentujacy tablicę przechowującą efektywne wartości
    // kolejnych adresów pamięci komputera.
    using memory_t = std::array<word_t, size>;

    // Typ reprezentujący tablicę która dla komórki pamięci o adresie 'i'
    // przechowuje identyfikator zmiennej w niej
    // przechowywanej.
    using ids_t = std::array<priv_ns::id_type, size>;

    // Struktura reprezentująca aktualny stan komputera.
    struct hardware {
        memory_t mem;
        ids_t ids;
        bool ZF;
        bool SF;
        size_t ind; // Reprezentuje liczbę zadeklarowanych zmmiennych.
    };

public:
    template<typename Prog>
    static constexpr std::array<word_t, size> boot() {
        if (!ComputerProgram<Prog>::check_corectness())
            throw std::logic_error("INVALID INSTRUCTION FOUND!");
        hardware h = {memory_t(), ids_t(), false, false, 0};
        ComputerProgram<Prog>::declare_variables(h);
        ComputerProgram<Prog>::run(h);
        return h.mem;
    };

private:
    // Dla danego wyniku operacji arytmetycznej ustawia odpowiednie flagi w strukturze
    // reprezentującej stan komputera, zgodnie z treścią polecenia.
    static constexpr void set_flags_arthmetic(hardware &h, word_t result) {
        if (result == 0)
            h.ZF = 1;
        else
            h.ZF = 0;

        if (result < 0)
            h.SF = 1;
        else
            h.SF = 0;
    }

    // Dla danego wyniku operacji logicznej ustawia odpowiednie flagi w strukturze
    // reprezentującej stan komputera, zgodnie z treścią polecenia.
    static constexpr void set_flags_logical(hardware &h, word_t result) {
        if (result == 0)
            h.ZF = 1;
        else
            h.ZF = 0;
    }

    template<typename Prog>
    struct ComputerProgram;

    template<typename... Instructions>
    struct ComputerProgram<Program<Instructions...>> {
        // Wykonuje instrukcje zawarte w ...Instrukctions, omijając deklaracje.
        constexpr static void run(hardware &h) {
            InstructionsParser<Program<Instructions...>, Instructions...>::evaluate(h);
        }

        // Deklaruje zawarte w Programie zmienne.
        constexpr static void declare_variables(hardware &h) {
            DeclarationParser<Instructions...>::evaluate(h);
        }

        // Sprawdza, czy wszystkie 'instrukcje' w Programie są rzeczywiście instrukcjami.
        // Zwraca wartość 'true' jeśli tak.
        constexpr static bool check_corectness() {
            return CorrectnessChecker<Program<Instructions...>, Instructions...>::check();
        }
    };

    // Udostępnia metody ewaluujące struktury będące l-wartościami lub p-wartościami.
    template<typename T>
    struct Evaluator;

    template<auto val>
    struct Evaluator<Num<val>> {
        static constexpr auto rvalue([[maybe_unused]] hardware &h) {
            return val;
        }

        static constexpr bool check_rvalue() {
            return true;
        };
    };

    template<priv_ns::id_type id>
    struct Evaluator<Lea<id>> {
        // Zwraca adres w pamięci zmiennej o identyfikatorze 'id',
        // jeśli nie zostanie ona znaleziona rzucany jest wyjątek.
        static constexpr auto rvalue(hardware &h) {
            size_t ret = h.ind;
            for (size_t i = 0; i < h.ind; i++) {
                if (id == h.ids[i]) {
                    ret = i;
                    return ret;
                }
            }
            throw std::logic_error("NOT EXISTING ID!");
        }

        static constexpr bool check_rvalue() {
            return true;
        };
    };

    template<typename T>
    struct Evaluator<Mem<T>> {
        static constexpr auto rvalue(hardware &h) {
            return h.mem[Evaluator<T>::rvalue(h)];
        }

        static constexpr auto &lvalue(hardware &h) {
            return h.mem[Evaluator<T>::rvalue(h)];
        }

        static constexpr bool check_rvalue() {
            return Evaluator<T>::check_rvalue();
        };

        static constexpr bool check_lvalue() {
            return Evaluator<T>::check_rvalue();
        };
    };

    // Struktura udostępniająca metodę sprawdzającą,
    // czy instrukcje w programie są rzeczywiście instrukcjami.
    template<typename... Instr>
    struct CorrectnessChecker;

    template<typename... OrginalInstructions>
    struct CorrectnessChecker<Program<OrginalInstructions...>> {
        // Jeżeli nie mamy już żadnej instrukcji do sprawdzenia, wszystkie były poprawne.
        constexpr static bool check() {
            return true;
        }
    };

    template<typename... OrginalInstructions, typename T, typename... Instructions>
    struct CorrectnessChecker<Program<OrginalInstructions...>, T, Instructions...> {
        constexpr static bool check() {
            if (std::is_base_of_v<priv_ns::Instr, T>) {
                InstructionsParser<Program<OrginalInstructions...>, T>::check();
                return CorrectnessChecker<Program<OrginalInstructions...>, Instructions...>::check();
            } else {
                return false;
            }
        }
    };

    // Struktura udostępniająca metodę deklarującą wszystkie zmienne zgodnie z kolejnością deklaracji.
    template<typename... Instr>
    struct DeclarationParser;

    template<>
    struct DeclarationParser<> {
        // Jeżeli nie mamy już deklaracji do zadeklarowania, nic nie robimy.
        constexpr static void evaluate([[maybe_unused]] hardware &h) {
        }
    };

    // Jeżeli przechodząc przez instrukcje natrafimy na deklarację, deklarujemy zmienną w pamięci komputera.
    template<priv_ns::id_type id, auto val, typename... Instructions>
    struct DeclarationParser<D<id, Num<val>>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            if (h.ind < h.mem.size()) {
                h.ids[h.ind] = id;
                h.mem[h.ind] = Evaluator<Num<val>>::rvalue(h);
                h.ind++;
            } else {
                throw std::logic_error("TOO MANY DECLARATIONS!");
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

    // Jeżeli deklaracja nie została zmatchowana do D<id, Num<val>> to znaczy,
    // że Value nie jest Num, rzucamy więc wyjątek.
    template<priv_ns::id_type id, typename Value, typename... Instructions>
    struct DeclarationParser<D<id, Value>, Instructions...> {
        constexpr static void evaluate([[maybe_unused]] hardware &h) {
            throw std::invalid_argument("DECLARATION VALUE IS NOT NUM!");
        }
    };

    // Struktura zawiera metodę 'evaluate', która wykonuje kolejne instrukcje programu.
    // Zmiany przechowywane są w argumencie do metody typu 'hardware' przekazywanym
    // przez referencję.
    template<typename... Instructions>
    struct InstructionsParser;

    template<typename ...OrginalInstructions>
    struct InstructionsParser<Program<OrginalInstructions...>> {
        constexpr static void evaluate([[maybe_unused]] hardware &h) {
        }
    };

    // Jeżeli instrukcja nie została zmatchowana, skipujemy ją (nie rzucamy wyjątku, ponieważ poprawność
    // instrukcji sprawdzana jest w innym miejscu.
    template<typename ...OrginalInstructions, typename Skip, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Skip, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }

        constexpr static void check() {}
    };

    template<typename ...OrginalInstructions, typename Dst, typename Src, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Mov<Dst, Src>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            Evaluator<Dst>::lvalue(h) = Evaluator<Src>::rvalue(h);
            InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }

        constexpr static void check() {
            Evaluator<Dst>::check_lvalue();
            Evaluator<Src>::check_rvalue();
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

        constexpr static void check() {
            Evaluator<Arg1>::check_lvalue();
            Evaluator<Arg2>::check_rvalue();
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

        constexpr static void check() {
            Evaluator<Arg1>::check_lvalue();
            Evaluator<Arg2>::check_rvalue();
        }
    };

    template<typename ...OrginalInstructions, typename Arg, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Inc<Arg>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            word_t &result = Evaluator<Arg>::lvalue(h);
            result += 1;
            set_flags_arthmetic(h, result);
            InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }

        constexpr static void check() {
            Evaluator<Arg>::check_lvalue();
        }
    };

    template<typename ...OrginalInstructions, typename Arg, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Dec<Arg>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            word_t &result = Evaluator<Arg>::lvalue(h);
            result -= 1;
            set_flags_arthmetic(h, result);
            InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }

        constexpr static void check() {
            Evaluator<Arg>::check_lvalue();
        }
    };

    template<typename ...OrginalInstructions, priv_ns::id_type label_id, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Jmp<label_id>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            LabelParser<Program<OrginalInstructions...>, label_id, OrginalInstructions...>::evaluate(h);
        }

        constexpr static void check() {}
    };

    template<typename ...OrginalInstructions, priv_ns::id_type label_id, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Jz<label_id>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            if (h.ZF)
                InstructionsParser<Program<OrginalInstructions...>, Jmp<label_id>, Instructions...>::evaluate(h);
            else InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }

        constexpr static void check() {}
    };

    template<typename ...OrginalInstructions, priv_ns::id_type label_id, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Js<label_id>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            if (h.SF)
                InstructionsParser<Program<OrginalInstructions...>, Jmp<label_id>, Instructions...>::evaluate(h);
            else InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }

        constexpr static void check() {}
    };

    template<typename ...OrginalInstructions, typename Arg1, typename Arg2, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, And<Arg1, Arg2>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            auto result = Evaluator<Arg1>::rvalue(h) & Evaluator<Arg2>::rvalue(h);
            Evaluator<Arg1>::lvalue(h) = result;
            set_flags_logical(h, result);
            InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }

        constexpr static void check() {
            Evaluator<Arg1>::check_lvalue();
            Evaluator<Arg2>::check_rvalue();
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

        constexpr static void check() {
            Evaluator<Arg1>::check_lvalue();
            Evaluator<Arg2>::check_rvalue();
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

        constexpr static void check() {
            Evaluator<Arg>::check_lvalue();
        }
    };

    template<typename ...OrginalInstructions, typename Arg1, typename Arg2, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Cmp<Arg1, Arg2>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            auto result = Evaluator<Arg1>::rvalue(h) - Evaluator<Arg2>::rvalue(h);
            set_flags_arthmetic(h, result);
            InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }

        constexpr static void check() {
            Evaluator<Arg1>::check_rvalue();
            Evaluator<Arg2>::check_rvalue();
        }
    };

    // Zawiera metodę 'evaluate' znajdującą pierwsze wystąpienie Label<label_to_find>.
    // Po znalezieniu wywoływane jest analizowanie pozostałych instrukcji (InstructionsParser::evaluate).
    // Jeżeli label nie zostanie odnaleziony rzucany jest wyjątek.
    template<typename Program, priv_ns::id_type label_to_find, typename... Instructions>
    struct LabelParser;

    // Jeżeli trafiliśmy na jakiś Label, sprawdzamy czy to ten, którego szukamy.
    // Jeżeli tak, to kończymy szukanie i wywołujemy 'evaluate' z 'InstructionsParser' na pozostałych instrukcjach.
    // Jeżeli nie, kontynuujemy szukanie.
    template<typename ...OrginalInstructions, priv_ns::id_type label_to_find, priv_ns::id_type id, typename... Instructions>
    struct LabelParser<Program<OrginalInstructions...>, label_to_find, Label<id>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            if (label_to_find == id)
                InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
            else LabelParser<Program<OrginalInstructions...>, label_to_find, Instructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, priv_ns::id_type label_to_find, typename Skip, typename... Instructions>
    struct LabelParser<Program<OrginalInstructions...>, label_to_find, Skip, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            LabelParser<Program<OrginalInstructions...>, label_to_find, Instructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, priv_ns::id_type label_to_find>
    struct LabelParser<Program<OrginalInstructions...>, label_to_find> {
        constexpr static void evaluate([[maybe_unused]] hardware &h) {
            throw std::logic_error("LABEL NOT FOUND");
        }
    };
};

#endif //COMPUTER_H