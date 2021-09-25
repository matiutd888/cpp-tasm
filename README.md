# Assembly simulation using Metaprogramming in C++

This project was created as an assignment for [Languages and tools for programming I](https://usosweb.mimuw.edu.pl/kontroler.php?_action=katalog2%2Fprzedmioty%2FpokazPrzedmiot&prz_kod=1000-223bJNP1&callback=g_65a6c0ee&lang=en)

The goal of this project was to create simulate simple Computer, with its own memory, running assembly-like language.
Simulation had to be implemented using metaprogramming and C++ templates.

Computer is parameterized by
* size of memory
* type of basic word (had to be integer type)

Computer class has core method ```boot```, which loads and executes programme written in TMPAsm (Template Metha programming Assembly) in compile time.

Programme written in TMPAsm language is effectively a sequence of instructions.

### Instruction types
#### Fundamental instructions
* ```Id(const char*)``` - creates a label or variable identificator.
* ```Num<word_type>``` - pvalue, represents numerical value <br>
    Examples: ```Num<10>```, ```Num<-1>```
* ```Mem<Addr>``` - lvalue (and also pvalue), provides access to memory address ```Addr```, which has to be valid pvalue. <br>
    Examples of correct memory references: ```Mem<Num<0>>```, ```Mem<Lea<Id("a")>>```
* ```Lea<Id>``` - pvalue, loads effective address of variable labaled with ```Id``` label. <br>
    Example: ```Lea<Id("A")>```
* ```D<Id, Value>``` - declares variable with id ```Id``` and numberical value ```Value```. <br>
    Example: ```D<Id("A"), Num<5>>``` 
* ```Mov<Dst, Src>``` - copies value of Src into Dst, Dst must be a valid l-value, and Src must be a valid r-value<br>
    Examples: ```Mov<Mem<Num<0>>```, ```Num<13>>```, ```Mov<Mem<Lea<Id("abc")>>```, ```Mem<Num<0>>>```
#### Arithmetic operations
* ```Add<Arg1, Arg2>``` - adds ```Arg2``` to ```Arg1``` and saves result to ```Arg1``` (```Arg1``` has to be lvalue, ```Arg2``` has to be pvalue). <br>
* ```Sub<Arg1, Arg2>``` - substracts ```Arg2``` from ```Arg1``` and saves result to ```Arg1``` (```Arg1``` has to be lvalue, ```Arg2``` has to be pvalue).<br>
* ```Inc<Arg>``` - Increments ```Arg``` (it has to be lvalue).<br>
* ```Dec<Arg>``` - Decrements ```Arg``` (it has to be lvalue).<br>
Examples: ```Add<Mem<Num<0>>```, ```Num<1>>, Inc<Mem<Lea<Id("a")>>>``` <br><br>

Arithmetic operations set
* ZF flag (zero flag) in processor to ```1``` if result is equal to ```0```, flag is set to ```0``` otherwise.
* SF flag (sign flag) in processor to ```1``` if result is negative, to ```0``` otherwise.
#### Additional instructions
* Logical operators:<br>
  ```And<Arg1, Arg2>``` <br>
  ```Or<Arg1, Arg2>``` <br>
Result is stored in ```Arg1``` (it has to be lvalue).<br>
  ```Not<Arg>``` (result is stored in ```Arg```, it has to be lvalue) <br><br>
These operations can change zero flag. <br>
Examples: ```And<Mem<Num<0>>```, ```Num<1>>```, ```Not<Mem<Lea<Id("a")>>>```. <br><br>
* ```Cmp<Arg1, Arg2>``` works like ```Sub<Arg1,Arg2>``` but doesn't change Arg1, only sets flags.<br>
  Example: ```Cmp<Mem<Num<0>>```, ```Num<1>>```.
* ```Label<Id("label")>``` - creates a Label of certain Id, that can be accesed later (like in assembly).
* Conditional jumps<br>
  ```Jmp<Id>``` - jumps to a label.<br>
  ```Jz<Id>```  - jumps if Label ZF is set to ```1```.<br>
  ```Js<Label>```  - jumps if Label SF is set to ```1```.<br>
  Examples: ```Jmp<Id("label")>```, ```Jz<Id("stop")>```
  
<br> You can find example programs in [computer_example.cc](computer_example.cc) file or [tests](tests) directory. Also, task description in Polish can be found in [desc](desc) directory.
