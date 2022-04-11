# vut-izp-proj2
Simple program that applies operations on sets and relations.

= Projekt 2 - Práce s datovými strukturami =

Na projektu lze pracovat v týmu 3-5 členů. V týdnu 1.-5.11. se seznamte se zadáním projektu a zformujte si tým, ve kterém chcete na projektu pracovat. Členové týmu musí patřit do stejné skupiny laboratorního cvičení.

== Motivace projektu ==

V prvním projektu jsme si vyzkoušeli jednoduché práce s textovými řetězci a základními cykly. V tomto projektu spojíme znalosti programování a diskrétní matematiky. Projekt zahrnuje zpracování vstupu, dynamickou alokaci a algoritmy pro průchody v datových strukturách.

== Popis projektu ==

Cílem projektu je vytvořit program, který bude implementovat základní matematické operace nad množinami a binárními relacemi. Vstupem programu budou textová data reprezentující množiny a relace a zadání operací. Výsledek zpracování bude program tisknout na standardní výstup.

== Detailní specifikace ==

Program implementujte ve zdrojovém souboru ''setcal.c'' (Set Calculator). Vstupní data budou čtena ze souboru, jehož jméno bude zadáno argumentem příkazové řádky. Program provádí operace zadané v souboru po řádcích jedním průchodem. Výsledek zpracování každého řádku program tiskne na standardní výstup (tedy počet řádků na výstupu odpovídá počtu řádků vstupního souboru).

=== Překlad a odevzdání zdrojového souboru ===

Odevzdání: Odevzdejte zdrojový soubor ''setcal.c'' prostřednictvím informačního systému.

Překlad: Program překládejte s následujícími argumenty

<pre>$ gcc -std=c99 -Wall -Wextra -Werror setcal.c -o setcal</pre>

=== Syntax spuštění ===

Program se spouští v následující podobě: (./setcal značí umístění a název programu):

<pre>./setcal FILE</pre>

=== Formát vstupního souboru ===

Textový soubor se skládá ze tří po sobě následujících částí:
- Definice univerza - jeden řádek začínající "U " a pokračující mezerou oddělenými prvky,
- Definice množin a relací - jeden nebo více řádků začínající "S " nebo "R " a pokračující mezerou oddělenými prvky (řádek začínající "S" indikuje definici množiny, "R" slouží pro definici relace),
- Příkazy nad množinami a relacemi - jeden nebo více řádků začínající "C " a pokračující identifikátorem příkazu.

==== Univerzum ====

Prvky univerza <s>mohou být</s><span style="color:red">jsou</span> řetězce obsahující malá a velká písmena anglické abecedy. Délka řetězce je maximálně 30 znaků. Prvky univerza nesmí obsahovat identifikátory příkazů (viz níže) a klíčová slova true a false. Všechny prvky v množinách a relacích musí patřit do univerza. Příklad:
<pre>U Apple Lemon Orange Banana Peach</pre>

==== Množiny ====

Každá množina je definovaná na jednom řádku mezerou oddělenými prvky z univerza. Identifikátorem množiny je číslo řádku, na kterém je množina definovaná (vzhledem k tomu, že na prvním řádku souboru je univerzum, identifikátory množin tedy začínají číslem 2). Identifikátory množin jsou použity v operacích (viz níže). Příklad definice množiny:
<pre>S Apple Banana Peach</pre>

==== Relace ====

Každá relace je definována výčtem dvojic. Dvojice je ohraničená kulatými závorkami, první a druhý prvek dvojice jsou oddělené mezerou. Jednotlivé dvojice jsou oddělené mezerou. Příklad:
<pre>R (Apple Banana) (Apple Peach) (Apple Apple)</pre>


==== Příkazy ====

Každý příkaz je definován na jednom řádku, začíná identifikátorem příkazu a argumenty příkazu jsou oddělené mezerou (od identifikátoru i mezi sebou). Argumenty příkazu jsou číselné identifikátory množin a relací (celá kladná čísla, číslo 1 identifikuje množinu univerza). Příklad:
<pre>C minus 1 2</pre>

=== Příkazy nad množinami ===

Příkaz pracuje nad množinami a jeho výsledkem je buď množina (v tom případě tiskne množinu ve stejném formátu jako se očekává ve vstupním souboru, tj. začíná "S " a pokračuje mezerou oddělenými prvky) nebo je výsledkem pravdivostní hodnota (v tom případě tiskne true nebo false na samostatný řádek) nebo je výsledkem přirozené číslo (které se tiskne na samostatný řádek).

* <code>empty A</code> - tiskne true nebo false podle toho, jestli je množina definovaná na řádku A prázdná nebo neprázdná.
* <code>card A</code> - tiskne počet prvků v množině A (definované na řádku A).
* <code>complement A</code> - tiskne doplněk množiny A.
* <code>union A B</code> - tiskne sjednocení množin A a B.
* <code>intersect A B</code> - tiskne průnik množin A a B.
* <code>minus A B</code> - tiskne rozdíl množin A \ B.
* <code>subseteq A B</code> - tiskne true nebo false podle toho, jestli je množina A podmnožinou množiny B.
* <code>subset A B</code> - tiskne true nebo false, jestli je množina A vlastní podmnožina množiny B.
* <code>equals A B</code> - tiskne true nebo false, jestli jsou množiny rovny.

=== Příkazy nad relacemi ===

Příkaz pracuje nad relacemi a jeho výsledkem je buď pravdivostní hodnota (tiskne true nebo false), nebo množina (tiskne množinu ve formátu jako ve vstupnímu souboru).

* <code>reflexive R</code> - tiskne true nebo false, jestli je relace reflexivní.
* <code>symmetric R</code> - tiskne true nebo false, jestli je relace symetrická.
* <code>antisymmetric R</code> - tiskne true nebo false, jestli je relace antisymetrická.
* <code>transitive R</code> - tiskne true nebo false, jestli je relace tranzitivní.
* <code>function R</code> - tiskne true nebo false, jestli je relace R funkcí.
* <code>domain R</code> - tiskne definiční obor funkce R (lze aplikovat i na relace - první prvky dvojic).
* <code>codomain R</code> - tiskne obor hodnot funkce R (lze aplikovat i na relace - druhé prvky dvojic).
* <code>injective R <span style="color:red">A B</span></code> - tiskne true nebo false, jestli je funkce R injektivní. <span style="color:red">A a B jsou množiny; a&#8712;A, b&#8712;B, (a,b)&#8712;R.</span>
* <code>surjective R <span style="color:red">A B</span></code> - tiskne true nebo false, jestli je funkce R surjektivní. <span style="color:red">A a B jsou množiny; a&#8712;A, b&#8712;B, (a,b)&#8712;R.</span>
* <code>bijective R <span style="color:red">A B</span></code> - tiskne true nebo false, jestli je funkce R bijektivní. <span style="color:red">A a B jsou množiny; a&#8712;A, b&#8712;B, (a,b)&#8712;R.</span>

=== Implementační detaily ===

* Maximální podporovaný počet řádků je 1000.
* Na pořadí prvků v množině a v relaci na výstupu nezáleží.
* Všechny prvky množin a v relacích musí patřit do univerza. Pokud se prvek v množině nebo dvojice v relaci opakuje, jedná se o chybu.

== Příklady použití ==

<pre>$ cat sets.txt
U a b c x y z
S a b c x
S x y z
C intersect 2 3
C minus 2 3</pre>
<pre>$ ./setcal sets.txt
U a b c x y z
S a b c x
S x y z
S x
S a b c</pre>

<pre>$ cat rel.txt
U dad mom girl boy man woman
R (dad boy) (dad girl) (mom boy) (mom girl)
R (dad man) (boy man) (mom woman) (girl woman)
C codomain 2
C function 3</pre>
<pre>$ ./setcal rel.txt
U dad mom girl boy man woman
R (dad boy) (dad girl) (mom boy) (mom girl)
R (dad man) (boy man) (mom woman) (girl woman)
S boy girl
true</pre>
