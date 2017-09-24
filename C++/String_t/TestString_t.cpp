#include "String_t.h"
#include "../../inc/mu_test.h"
#include <stdio.h>
#include <string.h>





UNIT(TestDefaultConstructor)
	
	String_t str_t1;
	
	ASSERT_THAT(0 == str_t1.Length());
	
END_UNIT



UNIT(TestConstructorWithConstCharPointer)
	
	String_t str_t1("100% No bugs here\n");
	
	ASSERT_THAT(18 == str_t1.Length());
	
END_UNIT



UNIT(TestConstructorWithConstStringTRef)
	
	String_t str_t1("100% No bugs here\n");
	String_t& str_t1r = str_t1;
	String_t str_t2(str_t1r);
	
	ASSERT_THAT(18 == str_t2.Length());
	
END_UNIT



UNIT(TestGetString)
	
	String_t str_t1("100% No bugs here\n");
	char* str;
	
	str = (char*)str_t1.GetString();
	
	ASSERT_THAT(0 == strcmp(str, "100% No bugs here\n"));
	
END_UNIT



UNIT(TestSetString)
	
	String_t str_t1;
	str_t1.SetString("Yo Yo Yo\n");
	char* str;
	
	str = (char*)str_t1.GetString();
	ASSERT_THAT(0 == strcmp(str, "Yo Yo Yo\n"));
	
END_UNIT




UNIT(TestCompare)
	
	String_t str_t1("yam");
	String_t str_t2("yam");
	
	ASSERT_THAT(0 == str_t1.Compare(str_t2));
	
	str_t2.SetString("yan");
	
	ASSERT_THAT(1 == str_t1.Compare(str_t2));
	
	str_t2.SetString("yal");
	
	ASSERT_THAT(2 == str_t1.Compare(str_t2));
	
END_UNIT



UNIT(TestEqualsOperator)
	
	String_t str_t1("yam");
	String_t str_t2;
	
	str_t2 = str_t1;
	
	ASSERT_THAT(0 == str_t1.Compare(str_t2));
	
END_UNIT



UNIT(TestConvertToLower)
	
	String_t str_t1("YAMAHA");
	String_t str_t2("yamaha");
	
	str_t1.ConvertToLowerCase();
	
	ASSERT_THAT(0 == str_t1.Compare(str_t2));
	
END_UNIT




UNIT(TestConvertToUpper)
	
	String_t str_t1("yamaha");
	String_t str_t2("YAMAHA");
	
	str_t1.ConvertToUpperCase();
	
	ASSERT_THAT(0 == str_t1.Compare(str_t2));
	
END_UNIT



TEST_SUITE(String_t)

	TEST(TestDefaultConstructor)
	TEST(TestConstructorWithConstCharPointer)
	TEST(TestConstructorWithConstStringTRef)
	TEST(TestGetString)
	TEST(TestSetString)
	TEST(TestCompare)
	TEST(TestEqualsOperator)
	TEST(TestConvertToLower)
	TEST(TestConvertToUpper)
	
	
END_SUITE







