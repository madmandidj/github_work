#ifndef __ACCOUNT_FACTORY_H__
#define __ACCOUNT_FACTORY_H__

#include <string>
using namespace std;
class AccountImpl;


class AccountFactory
{
	public:
		static AccountImpl* Create(const string& _impl, unsigned int _ID, const string& _name, unsigned int _amount)

	protected:
	
	private:
		~AccountFactory();
		AccountFactory();
};


















#endif /* #ifndef __ACCOUNT_FACTORY_H__ */
