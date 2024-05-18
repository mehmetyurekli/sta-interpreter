<p>STAR language is a small scripting language that can be used for performing basic arithmetical operations. It supports only two data types: integers and strings (called text). Integers can only take positive values and would take the value zero when forced to take a negative value. Assignments in STAR can have arithmetic expressions with two, operands only. Complicated arithmetic expressions are not allowed.</p>

<p>The lexical details of the language have already been described in the former project. This document will focus on general syntax and semantics.</p>

<p>Variables and Declarations:
Variables should be declared before they are used. They can be declared with the simple keywords int or text. Declaration does not have to be at the beginning of the file. It has to precede every usage of the variable though. 
</p>

<p>int myInt.</p>
<p></p>text myString.</p>
<p>int a,b,c,d.</p>
<p>int count is 5.</p>
<p>text errMsg is “An error has just occurred:”.</p>
<p>int a is 1, b is 2.</p>

<p>Assignments:
Assignments are performed via the “is” keyword.
</p>

<p>myString is “Hello world”.</p>
<p>myInt is myInt +1.</p>
<p>myInt is 1 – 3 .  /* This will assign value 0 to myInt*/</p>

<p>Lines of Code:
STAR supports lines of code that are terminated with a period ‘.’. The lines of text are not important.
</p>

<p>text myString</p>
<p>is “Hello”.</p>

<p>Input / Output:
read, write and newLine commands are used for I/O.
All I/O takes place on a command line console. 
The write statement of STAR can print constants and variables only (being both text or int). It does not accept arithmetic expressions.
</p>

<p>read “prompt:”, varName.</p>
<p>read varName.</p>
<p>write “The values are”, a ,“  “,  b, “  “,  c.</p>

<p>Loops:
There is just a basic for loop in STAR. This is accomplished through the loop statement. Also loops can be nested.
</p>
<p>int i.</p>
<p>i is 1.</p>
<p>loop 10 times</p>
<p>{ write i.  newLine.</p>
<p>    i is i+1.</p>
<p>}</p>

<p>Operations:
Text values (both variables and constants) can also be added and subtracted. +  operator between two text values would concatenate these values.
  Subtraction should remove the first occurrence of the second string from the first string.
</p>

<p>text Str1 is “ like home.” , Str2.</p>
<p>Str2 is “There is no place” + Str1.</p>
<p>write Str1.  /* Would print “There is no place like home”.*/</p>

<p>text s is “Ohh ice ice baby!”.</p>
<p>text s2.</p>
<p>s2 is s - “ice”.</p>
<p>write s2.  /* Prints “Ohh  ice baby!”*/</p>



