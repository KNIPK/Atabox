//============================================================================
// Name        : jaworekTests.cpp
// Author      : KNI
// Version     :
// Copyright   : take what you want
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <gst/gst.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	GstElement *element;
	GstElementFactory *factory;
	gst_init(& argc, &argv);

	//skrocona wersja
	//element = gst_element_factory_make("fakesrc", "source");

	factory = gst_element_factory_find("fakesrc");
	if(!factory){
		g_print("Nie udało się znaleźć fabryki");
		return -1;
	}

	element = gst_element_factory_create(factory, "source");
	if(!element){
		g_print("Nie udało się utworzyć elementu");
		return -1;
	}

	gst_object_unref( GST_OBJECT(element));

	return 0;
}
