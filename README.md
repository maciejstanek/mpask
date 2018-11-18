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

 1. Expand complex addresses, eg. { iso org(3) dod(3) 1 }.
