<h1>STAR Language</h1>

<p>STAR language is a small scripting language that can be used for performing basic arithmetic operations. It supports only two data types: integers and strings (referred to as text). Integers can only take positive values, and when forced to take a negative value, they would be set to zero. Assignments in STAR can have arithmetic expressions with two operands only; complicated arithmetic expressions are not allowed.</p>

<p>The lexical details of the language have already been described in the former project. This document will focus on general syntax and semantics.</p>

<h2>Variables and Declarations</h2>
<p>Variables should be declared before they are used. They can be declared with the keywords int or text. Declaration does not have to be at the beginning of the file but must precede every usage of the variable.</p>

<p>Examples:</p>
<ul>
  <li><code>int myInt.</code></li>
  <li><code>text myString.</code></li>
  <li><code>int a, b, c, d.</code></li>
  <li><code>int count is 5.</code></li>
  <li><code>text errMsg is “An error has just occurred:”.</code></li>
  <li><code>int a is 1, b is 2.</code></li>
</ul>

<h2>Assignments</h2>
<p>Assignments are performed via the “is” keyword.</p>

<p>Examples:</p>
<ul>
  <li><code>myString is “Hello world”.</code></li>
  <li><code>myInt is myInt +1.</code></li>
  <li><code>myInt is 1 – 3. <!-- This will assign value 0 to myInt --></code></li>
</ul>

<h2>Lines of Code</h2>
<p>STAR supports lines of code that are terminated with a period ‘.’.</p>

<p>Example:</p>
<ul>
  <li><code>text myString</code></li>
  <li><code>is “Hello”.</code></li>
</ul>

<h2>Input / Output</h2>
<p>read, write, and newLine commands are used for I/O. All I/O takes place on a command-line console. The write statement of STAR can print constants and variables only (both text or int) and does not accept arithmetic expressions.</p>

<p>Examples:</p>
<ul>
  <li><code>read “prompt:”, varName.</code></li>
  <li><code>read varName.</code></li>
  <li><code>write “The values are”, a ,“  “,  b, “  “,  c.</code></li>
</ul>

<h2>Loops</h2>
<p>There is just a basic for loop in STAR, accomplished through the loop statement. Loops can also be nested.</p>

<p>Example:</p>
<pre><code>int i.
i is 1.
loop 10 times
{
  write i.
  newLine.
  i is i+1.
}
</code></pre>

<h2>Operations</h2>
<p>Text values (both variables and constants) can also be added and subtracted. The + operator between two text values concatenates them. Subtraction removes the first occurrence of the second string from the first string.</p>

<p>Examples:</p>
<ul>
  <li><code>text Str1 is “ like home.” , Str2.</code></li>
  <li><code>Str2 is “There is no place” + Str1.</code></li>
  <li><code>write Str1.</code></li>
  <li>Would print “There is no place like home”.</li>
</ul>
<ul>
  <li><code>text s is “Ohh ice ice baby!”.</code></li>
  <li><code>text s2.</code></li>
  <li><code>s2 is s - “ice”.</code></li>
  <li><code>write s2.</code></li>
  <li>Would print “Ohh ice baby!”.</li>
</ul>

<p>This README file was created using ChatGPT.</p>
