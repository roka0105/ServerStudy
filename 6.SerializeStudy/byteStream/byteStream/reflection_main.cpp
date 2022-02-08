#include "global.h"
#include "cpp_reflection.h"
#include <algorithm>
int main()
{
	user_t user;
	user.index = 100;
	user.name = "choi";
	assert(user_t::get_field("index")->ref<int>(&user) == user.index);
	assert(user_t::get_field("name")->ref<string>(&user) == user.name);

	for_each(user_t::Get_field_begin(), user_t::Get_field_end(), [&](pair<string, field_t*> pair)//람다식임.
	{
		cout << pair.first.c_str() << endl;
		string type(pair.first.c_str());
		if (!strcmp(pair.first.c_str(), "index"))
		cout<<pair.second->ref<int>(&user)<<endl;
		else cout << pair.second->ref<string>(&user) << endl;
	});
	user.Printf();
}