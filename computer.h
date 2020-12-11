#ifndef COMPUTER_H
#define COMPUTER_H

#include <string>
#include <stdexcept>
#include <array>

namespace {
    using id_type = uint_fast64_t; // Typ zwracany przez Id(str), reprezentuje kod reprezentujący dane id.
    constexpr id_type id_code_base = 38; // Liczba różnych znaków w łańcuchach znaków reprezentujących id + 2.
    constexpr size_t id_size_min = 1;
    constexpr size_t id_size_max = 6;

    /*
     * Zwraca kod znaku.
     * Znaki reprezentujące ID są cyframi lub literami.
     * Na początku jeśli 'c' jest małą literą, zamieniane jest na dużą literę.
     * Następnie zwracany jest numer znaku 'c' w ciągu '0', '1', ..., '9', 'A', 'B', ..., 'Z'
     * (gdzie numer znaku '0' jest równy 1, itd).
     * Jeżeli 'c' nie jest ani cyfrą ani literą rzucany jest wyjątek std::logic_error.
     */
    constexpr id_type get_char_id(char c) {
        if (c >= 'a' && c <= 'z')
            c = c + 'A' - 'a';
        if (c >= '0' && c <= '9')
            return c - '0' + 1;
        else if (c >= 'A' && c <= 'Z')
            return c - 'A' + '9' - '0' + 2;
        else {
            throw std::logic_error("NOT VALID ID SIGN!");
        }
    }

    struct Instr {};
}

/*
 * Zwraca zakodowane różnowartościowo id reprezentowane przez łańcuch 'id_str'.
 * Jeżeli długość 'id_str' nie jest z zakresu {1, ..., 6} rzuca std::logic_error.
 */
constexpr id_type Id(const char *id_str) {
    std::basic_string_view<char> s(id_str);
    if (id_size_min <= s.size() && s.size() <= id_size_max) {
        id_type p = id_code_base;
        id_type res = 0;
        for (char i : s) {
            if (i != '\0') {
                id_type c = get_char_id(i);
                res = res * p + c;
            }
        }
        return res;
    } else {
        throw std::logic_error("NOT VALID ID LENGHT!");
    }
}

template<auto T>
struct Num;

template<typename T>
struct Mem;

template<id_type T>
struct Lea;

template<typename ...T>
struct Program;

template<id_type id, typename Value>
struct D : Instr {
};

template<typename Dst, typename Src>
struct Mov : Instr {
};

template<typename Arg1, typename Arg2>
struct Add : Instr {
};;

template<typename Arg1, typename Arg2>
struct Sub : Instr {
};;

template<typename Arg1>
struct Inc : Instr {
};;

template<typename Arg1>
struct Dec : Instr {
};;

template<typename Arg1, typename Arg2>
struct And : Instr {
};;

template<typename Arg1, typename Arg2>
struct Or : Instr {
};;

template<typename Arg>
struct Not : Instr {
};;

template<typename Arg1, typename Arg2>
struct Cmp : Instr {
};;

template<id_type id>
struct Label : Instr {
};;

template<id_type label>
struct Jmp : Instr {
};;

template<id_type label>
struct Jz : Instr {
};;

template<id_type label>
struct Js : Instr {
};;

template<size_t size, typename word_t>
struct Computer {
private:
    using memory_t = std::array<word_t, size>; // Typ reprezentujacy tablicę przechowującą efektywne wartości
    // kolejnych adresów pamięci komputera.

    using ids_t = std::array<id_type, size>; // Typ reprezentujący tablicę która dla komórki pamięci o adresie 'i'
    // przechowuje identyfikator zmiennej w niej
    // przechowywanej.

    // Struktura reprezentująca aktualny stan komputera.
    struct hardware {
        memory_t mem;
        ids_t ids;
        bool ZF;
        bool SF;
        size_t ind; // Reprezentuje adres ostatniej zadeklarowanej zmiennej.
    };

public:
    template<typename Prog>
    static constexpr std::array<word_t, size> boot() {
        if (!ComputerProgram<Prog>::check_corectness())
            throw std::logic_error("INVALID INSTRUCTION!");
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
        else h.ZF = 0;

        if (result < 0)
            h.SF = 1;
        else h.SF = 0;
    }

    // Dla danego wyniku operacji logicznej ustawia odpowiednie flagi w strukturze
    // reprezentującej stan komputera, zgodnie z treścią polecenia.
    static constexpr void set_flags_logical(hardware &h, word_t result) {
        if (result == 0)
            h.ZF = 1;
        else h.ZF = 0;
    }

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

        constexpr static bool check_corectness() {
            return CorrectnessChecker<Instructions...>::check();
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

    static constexpr bool array_has(const ids_t &ids, id_type id) {
        for (const auto id_it : ids) {
            if (id_it == id) return true;
        }
        return false;
    }

    template<id_type id>
    struct Evaluator<Lea<id>> {
        static constexpr auto rvalue(hardware &h) {
            size_t ret = h.ind;
            for (size_t i = 0; i < h.ind; i++) {
                if (id == h.ids[i]) {
                    ret = i;
                    return ret;
                }
            }
            throw std::logic_error("NO ID!");
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

    // Struktura udostępniająca metodę sprawdzającą, czy instrukcje w programie są rzeczywiście instrukcjami.
    template<typename... Instr>
    struct CorrectnessChecker;

    template<>
    struct CorrectnessChecker<> {
        // Jeżeli nie mamy już żadnej instrukcji do sprawdzenia, wszystkie były poprawne.
        constexpr static bool check() {
            return true;
        }
    };

    template<typename T, typename... Instructions>
    struct CorrectnessChecker<T, Instructions...> {
        constexpr static bool check() {
            if (std::is_base_of_v<Instr, T>) {
                return CorrectnessChecker<Instructions...>::check();
            } else return false;
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
    template<id_type id, typename Value, typename... Instructions>
    struct DeclarationParser<D<id, Value>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            if (h.ind < h.mem.size()) {
                h.ids[h.ind] = id;
                h.mem[h.ind] = Evaluator<Value>::rvalue(h);
                h.ind++;
            } else {
                throw std::logic_error("NOT ENOUGH MEMORY TO DECLARE!");
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

    template<typename... Instructions>
    struct InstructionsParser;

    // Koniec programu.
    template<typename ...OrginalInstructions>
    struct InstructionsParser<Program<OrginalInstructions...>> {
        constexpr static void evaluate([[maybe_unused]] hardware &h) {

        }
    };

    // w celach ignorowania labela pojawiajacego się przed jumpem
    template<typename ...OrginalInstructions, id_type ignore, typename... Instructions>
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

    template<typename ...OrginalInstructions, id_type label_id, typename Value, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, D<label_id, Value>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, id_type label_id, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Jmp<label_id>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            LabelParser<Program<OrginalInstructions...>, label_id, OrginalInstructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, id_type label_id, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Jz<label_id>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            if (h.ZF)
                InstructionsParser<Program<OrginalInstructions...>, Jmp<label_id>, Instructions...>::evaluate(h);
            else InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, id_type label_id, typename... Instructions>
    struct InstructionsParser<Program<OrginalInstructions...>, Js<label_id>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            if (h.SF)
                InstructionsParser<Program<OrginalInstructions...>, Jmp<label_id>, Instructions...>::evaluate(h);
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

    template<typename Program, id_type label_to_find, typename... Instructions>
    struct LabelParser;

    template<typename ...OrginalInstructions, id_type label_to_find, id_type id, typename... Instructions>
    struct LabelParser<Program<OrginalInstructions...>, label_to_find, Label<id>, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            if (label_to_find == id)
                InstructionsParser<Program<OrginalInstructions...>, Instructions...>::evaluate(h);
            else LabelParser<Program<OrginalInstructions...>, label_to_find, Instructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, id_type label_to_find, typename... Instructions>
    struct LabelParser<Program<OrginalInstructions...>, label_to_find, Instr, Instructions...> {
        constexpr static void evaluate(hardware &h) {
            LabelParser<Program<OrginalInstructions...>, label_to_find, Instructions...>::evaluate(h);
        }
    };

    template<typename ...OrginalInstructions, id_type label_to_find>
    struct LabelParser<Program<OrginalInstructions...>, label_to_find> {
        constexpr static void evaluate([[maybe_unused]] hardware &h) {
            throw std::logic_error("LABEL NOT FOUND");
        }
    };
};

#endif //COMPUTER_H