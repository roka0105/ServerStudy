#include "global.h"
#include "reflection.h"

int main()
{
	user_t yeram;
    yeram.index = 100;
    yeram.name = "ÃÖ¿¹¶÷";
    cout<<yeram.GetField("name")->ref<string>(&yeram)<<endl;
    cout << yeram.GetField("index")->ref<int>(&yeram)<<endl;
    for (field_info_map_t<user_t>::fieldInfo_Iterator i = yeram.get_field_begin(); i != yeram.get_field_end(); ++i)
    {
        cout << i->first << endl;
    }
}