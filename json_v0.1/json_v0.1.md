# 启程

## 1. 项目架构

本次学习的`json`一共包含三个文件

1. myjson.h: myjson的头文件，含有对外的类型和API函数声明
2. myjson.cpp：leptjson 的实现文件(implementation file),含有内部的类型声明和函数实现。此文件会编译成库
3. test.cpp：我们使用测试驱动开发(test driven development, TDD)。此文件包含测试程序，需要链接 leptjson 库

## 2. 项目说明

### 2.1 数据定义

由于JSON有六种数据类型

- null: null
- boolean: true/false
- number: 一般浮点数
- string: "..."
- array:  [...]
- object: {...}

因此我们声明一个枚举类型来表示他们,，一般会使用项目的简写作为标识符的前缀。通
常枚举值用全大写（如 JSON_NULL ），而类型及函数则用小写（如  json_type ）

```c++
enum json_type {JSON_NULL, JSON_FALSE, JSON_TRUE, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT};
```

接下来，我们声明 `JSON` 的数据结构。`JSON` 是一个树形结构，我们最终需要实现一个树的数据结构，每
个节点使用`json_value`类表示，我们会称它为一个`JSON`值(`JSON value`)。

```c++
class json_value {
    json_type type
};
```

### 2.2 JSON 语法子集

下面是此单元的 JSON 语法子集，使用 [RFC7159](https://tools.ietf.org/html/rfc7159) 中的 [ABNF](https://tools.ietf.org/html/rfc5234) 表示：

~~~
JSON-text = ws value ws
ws = *(%x20 / %x09 / %x0A / %x0D)
value = null / false / true 
null  = "null"
false = "false"
true  = "true"
~~~

当中 `%xhh` 表示以 16 进制表示的字符，`/` 是多选一，`*` 是零或多个，`()` 用于分组。

那么第一行的意思是，JSON 文本由 3 部分组成，首先是空白（whitespace），接着是一个值，最后是空白。

第二行告诉我们，所谓空白，是由零或多个空格符（space U+0020）、制表符（tab U+0009）、换行符（LF U+000A）、回车符（CR U+000D）所组成。

第三行是说，我们现时的值只可以是 null、false 或 true，它们分别有对应的字面值（literal）。

我们的解析器应能判断输入是否一个合法的 JSON。如果输入的 JSON 不合符这个语法，我们要产生对应的错误码，方便使用者追查问题。

在这个 JSON 语法子集下，我们定义 3 种错误码：

- 若一个 JSON 只含有空白，传回 `JSON_PARSE_EXPECT_VALUE`。
- 若一个值之后，在空白之后还有其他字符，传回 `JSON_PARSE_ROOT_NOT_SINGULAR`。
- 若值不是那三种字面值，传回 `JSON_PARSE_INVALID_VALUE`。

### 2.3 函数声明

目前我们只需要两个API函数。

1. 解析JSON

```c++
int json_parse(json_value* v, const char* json)
```

传入的 JSON 文本是一个 C 字符串（空结尾字符串／null-terminated string），由于我们不应该改动这个输入字符串，所以使用 `const char*` 类型。

```c++
json_value v;
const char json[] = ...;
int ret = json_parse(&v, json);
```

返回值是以下这些枚举值，无错误会返回 `LEPT_PARSE_OK`，其他值在下节解释。

```c++
enum {
    JSON_PARSE_OK = 0,
    JSON_PARSE_EXPECT_VALUE,
    JSON_PARSE_INVALID_VALUE,
    JSON_PARSE_ROOT_NOT_SINGULAR
};
```

2. 访问结果的函数，获取其类型

```c++
json_type json_get_type(const json_value& v)
```

### 2.4 单元测试

一般来说，软件开发是以周期进行的。例如，加入一个功能，再写关于该功能的单元测试。但也有另一种软件开发方法论，称为测试驱动开发`(test-driven development, TDD)`，它的主要循环步骤是：

1. 加入一个测试。
2. 运行所有测试，新的测试应该会失败。
3. 编写实现代码。
4. 运行所有测试，若有测试失败回到3。
5. 重构代码。
6. 回到 1。

`TDD` 是先写测试，再实现功能。好处是实现只会刚好满足测试，而不会写了一些不需要的代码，或是没有被测试的代码。
