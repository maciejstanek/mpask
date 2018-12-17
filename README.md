SMI & MIB2
==========

Bazuje na *ASN.1*.

Co znajdziemy w MIBach:
 1. Deklaracje typów
 2. Deklaracje identyfikatorów
 3. Deklaracje obiektów
 4. Importy

Typy
----

<typ> := INTEGER | OCTET STRING | OBJECT IDENTIFIER | NULL

Listy: SEQUENCE (OF) { <element>, <inny element>, ... }

Listy mogą być zagnieżdżone.

Nowe typy danych
----------------

Przykład:
<nazwa> := [<widocznosc> <identyfikator>] <explicity> <typ> <ograniczenia>

Składnia:
 * <widoczność> := UNIVERSAL | APPLICATION | CONTEX-SPECIFIC | PRIVATE
 * <identyfikator> := <int>
 * <explicity> := IMPLICIT | EXPLICIT
 * <znak biały> := spacja | tabulacja | znak nowej linii
 * <ograniczenia> (np "(SIZE(4))")

Importy
-------

IMPORTS <oddzielone przecinkami typy> FROM <nazwa pliku>;

Deklaracje identyfikatów
------------------------

system/interfaces/at/ip OBJECT IDENTIFIER ::= { <nazwa wyżej> <int> }

Parsowanie OID
--------------

mib-2 OBJECT IDENTIFIER ::= { mgmt 1 }
internet OBJECT IDENTIFIER ::= { iso org(3) dod(3) 1 }

Kodowanie BER
-------------

Do stringów. Budowa: identyfikator/tag + długość + zawartość

Do zaliczenia
-------------

Raportować trzy obiekty przez SNMP, np. uptime itp.

Potrzebne rzeczy:
 * plik MIB-II

Efekt:
 * można oglądać drzewko hierarchii (może .dot file?)

Pakiet: net-snmp

Parsowanie obiektów SMI
-----------------------

<nazwa> OBJECT-TYPE
  SYNTAX TimeTicks
  ACCESS read-only
  STATUS mandatory
  ...

TODO
====

 1. ~~Expand complex addresses, eg. { iso org(3) dod(3) 1 }.~~

USAGE - GRAPHS
==============

To make a visualization of a _mib-2_ tree run all the tests and then execute the following:
```
unflatten -l 6 -f $MPASK_ROOT/build/tests/mib.dot > mib_fixed.dot
# xdot mib_fixed.dot # view results
dot -Tpng mib_fixed.dot -o mib_fixed.png
```
Complex address
===============

`internet OBJECT IDENTIFIER ::= { iso org(3) dod(6) 1 }`

Scrambler state
---------------

```
{
  {"root", {<root(-1)>, {<iso(1)>}},
  {"iso", {<iso(1)>, {}},
  {"internet", {<internet(1)>, {}} // Have the complex address
}
```

Scrambler target
----------------

```
{
  {"root", {<root(-1)>, {<iso(1)>}},
  {"iso", {<iso(1)>, {org(3)>}},
  {"org", {<org(3)>, {dod(6)>}},
  {"dod", {<dod(6)>, {internet(1)>},
  {"internet", {<internet(1)>, {}}
}
```

Scrambler target for simple address only
----------------------------------------

```
{
  {"root", {<root(-1)>, {<iso(1)>}},
  {"iso", {<iso(1)>, {internet(1)>}},
  {"internet", {<internet(1)>, {}}
}
```

Conclusions
-----------

 * Create all missing intermediate nodes
   - `org(3)` and `dod(6)` in this case
   - `iso(1)` already exists
 * Skew chain them
   - `iso(1)` has child `org(3)`
   - `org(3)` has child `dod(6)`
   - `dod(6)` has child `internet(1)`

BER
---

Schema:
```
Xxx DEFINITIONS AUTOMATIC TAGS ::= 
BEGIN
  Aaa ::= [1] IMPLICIT INTEGER
  Bbb ::= [1] EXPLICIT INTEGER
  Yyy ::= SEQUENCE
  {
    zzz Aaa
    -- zzz Bbb
  }
END
```

Data:
```
value Yyy ::= {
  zzz 255
}
```

implicit (Aaa):
```
  universal contructed sequence         30
  length 4                              04
    universal primitive zero?           80
    length 2                            02
      value 255                         00 FF
```

explicit (Bbb):
```
  universal contructed sequence         30
  length 6                              06
    context-specific contructed zero?   A0
    length 4                            04
      universal primitive integer       02
      length 2                          02
      value                             00 FF
```

Schema:
```
Xxx DEFINITIONS AUTOMATIC TAGS ::=
BEGIN
  Aaa ::= [APPLICATION 1] INTEGER
  Bbb ::= [APPLICATION 2] IMPLICIT INTEGER
  Ccc ::= [APPLICATION 3] EXPLICIT INTEGER
  Ddd ::= SEQUENCE
  {
    aaa Aaa,
    bbb Bbb,
    ccc Ccc
  }
END
```

Data:
```
value Ddd ::= {
  aaa 5,
  bbb 5,
  ccc 5
}
```

BER:
```
Encoding to the file 'data.ber' using BER encoding rule...
Ddd SEQUENCE: tag = [UNIVERSAL 16] constructed; length = 11
  aaa Aaa INTEGER: tag = [0] primitive; length = 1
    5
  bbb Bbb INTEGER: tag = [1] primitive; length = 1
    5
  ccc : tag = [2] constructed; length = 3
    Ccc INTEGER: tag = [UNIVERSAL 2] primitive; length = 1
      5
Encoded successfully in 13 bytes:
300B8001 05810105 A2030201 05
```

Analysis:
```
> 30                  sequence
  > 0B                length 11
    > 80              context-specific primitive (tag zero)
      > 01            length 1
        > 05          value 5
    > 81              context-specific primitive (tag one)
      > 01            length 1
        > 05          value 5
    > A2              context-specific constructed (tag two)
      > 03            length 3
        > 02          universal primitive integer
          > 01        length 1
            > 05      value 5
```

WHAT I DID - A REPORT
=====================

 * encoding octet string
 * encoding long sequences and strings (_2^1008_)
 * handling `EXPLICIT` and `IMPLICIT`

