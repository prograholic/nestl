# nestl <a target="_blank" href="https://travis-ci.org/prograholic/nestl">![Travis status][badge.Travis]</a> <a target="_blank" href="https://ci.appveyor.com/project/prograholic/nestl">![Appveyor status][badge.Appveyor]</a>


**TODO: translate description from russian to english**

**This implementation is not production ready yet**

Design and implementation of some of stl containers, smart pointers and algorithms without exception support.
This library allows to write common code for environments with exceptions and for environments without exceptions.
Therefore all operations which may perform in any modes are marked as `noexcept`.

Кроме этого, для среды с исключениями библиотека предоставляет расширения для контейнеров и умных указателей.
Эти расширения позволяют обычным для программиста способом инициализировать, наполнять контейнеры

Motivation
==========

Небольшой пример использования:

```
nestl::vector<int> vec;
nestl::default_operation_error err;
vec.push_back_nothrow(err, 10);
if (err)
{
    // handle error somehow
    std::cerr << "error occured" << std::endl;
}
```


Для чего эта библиотека нужна
-----------------------------

Как известно, в C++ исключения - это краеугольный камень, так как исключения позволяют использовать RAII и т.д.
Но, с другой стороны, язык C++ является языком общего назначения,
и кроме всего прочего, используется как язык для системного программирования,
а в системном программирование отсутствие исключений скорее норма, нежели исключение.

Кроме этого, есть и другие области программирования, где использование исключение неоправданно, например, из-за производительности.
Например, в ОС Windows бросание исключения требует перехода в режим ядра, и бросок исключения фактически начинается из ядра.
Поэтому, бросание исключения может быть дорогой операцией.


Почему не подходит стандартная библиотека
-----------------------------------------
Рассмотрим следующий пример:

```
std::vector<int> x;
x.push_back(1);
```

В среде без исключений этот код либо вообще не скомпилируется, либо скомпилируется, но может таить в себе несколько проблем.
Одна из проблем - как узнать, успешно завершилась операция или нет?
Ну, мы можем позвать метод `x.size()` и проверить, что значение равно 1.

А как проверить в случае, если у нас следующая конструкция:
```
std::vector<std::list<int>> m;
m.push_back(get_list_somehow());
```
В этом случае, нам нужно проверить не только вектор m, но еще и лист, который мы добавили, так как его копирование могло тоже сломаться.

Есть еще одна проблема - реализация std::vector, может ожидать, что если аллокатор вернул указатель,
то он валиден (не nullptr) и содержит достаточно места для элементов.
Как поступить аллокатору в случае отсутствия исключений, если он не может выделить память?
Вернуть nullptr? Мы можем сломать std::vector.
Выполнить std::abort? Ну это не совсем user-friendly, кроме того, в некоторые системы, должны уметь обрабатывать такие ошибки.
Или же, например, пользователь ошибся, и ввел некорректное число и т.д.

Итого, из данных примеров видно, что стандарнтая библиотека не может быть использована в среде без исключений полным образом.
Она может быть использована только до возникновения первой ошибки, после этого никаких гарантий нет.


Если переформулировать проблему, то в среде без исключений мы должны предоставить некие аналоги следующих механизмов:
1. exception handling - explicit error codes handling
2. Any constructor which may throw - two-phase initialization (default ctor + `init(args)`)
3. Any assignment operator which may throw - explicit `copy` method (for example `vec.copy_nothrow(err, other_vec);`)




Режимы работы библиотеки
------------------------

Библиотека работает в двух режимах - в режиме, когда есть исключения, и когда исключений нет.
Программист может проверить тот или иной режим явно с помощью макроса NESTL_HAS_EXCEPTIONS.
Значение этого макроса вычисляется в true, если исключения есть, и false если исключений нет.

Кроме этого, есть специальный тип 
`nestl::exception_support_t` - в случае наличия исключений, он эквивалентен типу std::true_type,
и std::false_type в случае, если исключений нет.

Также есть еще один вспомогательный шаблон
`template <typename HasException, typename NoException>
using dispatch = typename ::std::conditional<exception_support_t::value, HasExceptions, NoExceptions>::type
`
Этот шаблон позволяет выбирать ту или иную реализацию в зависимости от наличия исключений.



Общая структура библиотеки
--------------------------

Как и в стандартной библиотеке, в nestl мы разделяем, контейнеры, аллокаторы и алгоритмы.
Кроме того, мы дополнительно отделяем и обработку ошибок, так как у клиента могут существовать свои механизмы обработки ошибок.


Error handling
--------------

Так как в среде без исключений мы не можем бросать и ловить исключения,
то единственным вариантом обработки ошибок становятся коды ошибок (или их аналоги).

Библиотека предоставляет для этого т.н. OperationError - это сущность, которая в себе содержит каким-то образом информацию об ошибке.
Объект этого типа можно проверить на наличие ошибки с помощью `explicit operator bool()`

Библиотека предоствляет т.н. `default_operation_error` - это дефолтная реализация OperationError для каждого режима работы.


Кроме того, программист может сам добавить свою реализацию OperationError для своих нужд.
Для того чтобы это сделать, библиотека требует от OperationError реализацию `operator bool` или аналога.
Этот оператор должен возвращать `true` в случае, если объект содержит ошибку, и `false` в противном случае.

Кроме этого, библиотека требует наличия следующих функций:
`build_length_error` - функция должна создать ошибку, аналог исключения `std::length_error`
`build_bad_alloc` - функция должна создать ошибку, аналог исключения `std::bad_alloc`

Также, для среды с исключениями библиотека требует функций:
`from_exception_ptr` - функция должна создать ошибку из объекта `std::exception_ptr`
`from_exception` - функция должна создать ошибку из произвольного объекта исключения
`throw_exception` - функция должна бросить исключение из данного объекта OperationError


Containers
----------
Библиотека предоставляет набор контейнеров


Type requirements and limitations
---------------------------------
В библиотеке есть несколько ограничений на типы, которые могут использоваться в контейнерах и алгоритмах.
Если тип реализует `move ctor` and `move assignment operator`, то они должны быть noexcept.
Если тип реализует `copy ctor` and `copy assignment operator`, то они должны быть noexcept.

Если тип не может реализовать `copy ctor` and `copy assignment operator`, с указанными требованиями,
то программист может предоставить специализацию nestl::class_operations для своего типа, и реализовать статические методы
`construct` для эмуляции конструктора копирования




How to build
============
Library is header-only. Library requires C++-11 compliant compiler and standard library.


How to use NESTL
-----------------------------------------

How to run tests
----------------
Nestl needs following dependencies for running tests:
* CMake (http://www.cmake.org/), minimal version is 3.0

For configuring building and running nestl test suite one should perform following commands:
```
mkdir build
cd build
cmake
make
make test
```



Compilers supported
===================

Library has been tested on following platforms and compilers:

Toolchain
-----------------------------
i386-linux-gnu G++-4.8.3
x86_64-linux-gnu clang++-3.4
i386-linux-gnu clang++-3.4
i386-linux-gnu clang++-3.5
i386-windows msvc-2013 *
i386-windows msvc-2015
i386-windows msvc-2017


--
* msvc-2013 does not support `noexcept` specifier, therefore we emulate it using `throw()` in nestl implementation