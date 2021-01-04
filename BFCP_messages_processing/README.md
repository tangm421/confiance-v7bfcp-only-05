# bfcp解析库

根据项目依赖修改的bfcp解析库

## 关于原语（Primitive）定义的变更历史

[draft-sandbakken-xcon-bfcp-udp-01](https://datatracker.ietf.org/doc/html/draft-sandbakken-xcon-bfcp-udp-01)版本新增三个原语

	+-------+-----------------------+------------------+
	| Value | Primitive             | Direction        |
	+-------+-----------------------+------------------+
	|  ...  | ...                   | ...              |
	|   14  | FloorRequestStatusAck | P -> S ; Ch -> S |
	|   15  | ErrorAck              | P -> S ; Ch -> S |
	|   16  | FloorStatusAck        | P -> S ; Ch -> S |
	+-------+-----------------------+------------------+


[draft-sandbakken-xcon-bfcp-udp-02](https://datatracker.ietf.org/doc/html/draft-sandbakken-xcon-bfcp-udp-02)版本再次新增两个原语

	+-------+-----------------------+--------------------+
	| Value | Primitive             | Direction          |
	+-------+-----------------------+--------------------+
	|  ...  | ...                   | ...                |
	|   14  | FloorRequestStatusAck | P -> S ; Ch -> S   |
	|   15  | ErrorAck              | P -> S ; Ch -> S   |
	|   16  | FloorStatusAck        | P -> S ; Ch -> S   |
	|   17  | Goodbye               | P -> S ; Ch -> S ; |
	|       |                       | P <- S ; Ch <- S   |
	|   18  | GoodbyeAck            | P -> S ; Ch -> S ; |
	|       |                       | P <- S ; Ch <- S   |
	+-------+-----------------------+--------------------+

[draft-ietf-bfcpbis-rfc4582bis-04](https://datatracker.ietf.org/doc/html/draft-ietf-bfcpbis-rfc4582bis-04)以及之后的版本去掉了 ErrorAck，导致原 ErrorAck 后续的三个原语定义的值发生变更

	+-------+-----------------------+--------------------+
	| Value | Primitive             | Direction          |
	+-------+-----------------------+--------------------+
	|  ...  | ...                   | ...                |
	|   14  | FloorRequestStatusAck | P -> S ; Ch -> S   |
	|   15  | FloorStatusAck        | P -> S ; Ch -> S   |
	|   16  | Goodbye               | P -> S ; Ch -> S ; |
	|       |                       | P <- S ; Ch <- S   |
	|   17  | GoodbyeAck            | P -> S ; Ch -> S ; |
	|       |                       | P <- S ; Ch <- S   |
	+-------+-----------------------+--------------------+

## 修改依据
由于需要兼容第三方的bfcp标准，目前测试的华为SIP软终端（TE Desktop V6.6.1.815）和宝利通的SIP软终端（Polycom RealPresence Desktop 3.5.1.61139）均以 `16` 作为 `FloorStatusAck` 的值，故而根据[draft-ietf-bfcpbis-rfc4582bis-03](https://datatracker.ietf.org/doc/html/draft-ietf-bfcpbis-rfc4582bis-03)标准草案进行部分内容的适应修改。


<br><br><br><br><br>
-------------------------------------------------------------

---------------------------------
http://confiance.sourceforge.net/
---------------------------------


=====
NOTES
=====

This is a brief guide to the compilation of the BFCP building
and parsing libraries, according to BFCP RFC specifications
(attached), and of the test program that has been made available
to show their use.

This guide has been conceived by installing the applications on
a Linux RedHat Fedora Core 6 or LinuX Ubuntu 6.06 machine, but it
should not give any problem on different distributions.



=====
INDEX
=====

1. Compiling the libraries
2. Testing the libraries



==========================
1. Compiling the libraries
==========================

There are several available targets to compile the code:

	- 'make linux' will compile the testcode and the
	  library, creating an executable file for Linux;
	- 'make win32' will compile the testcode and the
	  library, creating an executable file for Windows;
	- 'make so' will only compile the library, creating
	  a shared object for Linux;
	- 'make dll' will only compile the library, creating
	  a DLL for Windows.

If you want to compile all the available targets, just use:

	make all

Edit the Makefile according to your settings and compiler.



========================
2. Testing the libraries
========================

A command line test code has been made available to test the
libraries. Just play with it according to your needs to learn
how the library works and to make tests on your own.

The test code that is available is testcode.c, a small application
that builds a BFCP message, saves it to a file (mesg.hex), opens
the file again, reads its contents and finally parses the built message,
showing all the attributes. By default the test code will build a
ChairAction message: just change the primitive name and the required
arguments to try to build a message of your choice.
