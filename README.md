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
