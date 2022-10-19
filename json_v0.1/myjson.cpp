#include "myjson.h"
#include <assert.h>
#include <cstdlib>
#include <string>

//检查字符
#define CHECK(c, ch) do{ assert(*c.json == (ch)); c.json++;} while (0)

//json语法子集
// JSON-text = ws value ws
// ws(white space) = *(%x20 / %x09 / %x0A / %x0D)
// value = null / false / true (目前的值 只有三种)
// null  = "null"
// false = "false"
// true  = "true"


// 为了减少解析函数之间传递多个参数，
// 我们把这些数据都放进一个json_context结构体
class json_context{
public:
    const char* json;
};

//解析空白字符
static void json_parse_whitespace(json_context &c){
    const char* p = c.json;
    // 为了跳到value的起始位置 
    while(*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r'){
        p++;
    }
    c.json = p;
}
//解析null
static int json_parse_null(json_context &c, json_value &v){
    //检查首字母是不是'n'
    CHECK(c, 'n');
    //解析null
    if(c.json[0] != 'u' || c.json[1] != 'l' || c.json[2] != 'l'){
        //不是null，说明是非法值
        return EXPECT::JSON_PARSE_INVALID_VALUE;
    }
    
    c.json += 3; //指针往前移动
    v.type = json_type::JSON_NULL;
    return EXPECT::JSON_PARSE_OK;
}
//解析value
static int json_parse_value(json_context &c, json_value &v){
    switch (*c.json)
    {
        //说明碰到了null
        case 'n': return json_parse_null(c,v);
        //结束符
        case '\0': return EXPECT::JSON_PARSE_EXPECT_VALUE; 
        default: return EXPECT::JSON_PARSE_INVALID_VALUE;
    }
}
//json解析器
int json_parse(json_value *v, const char* json){
   json_context c;
   assert(v != NULL);
   c.json = json;
   v->type = json_type::JSON_NULL;
   json_parse_whitespace(c);
   return json_parse_value(c, *v);
}
// 获取json的类型
json_type json_get_type(const json_value* v){
    assert(v != NULL);
    return v->type;
}