#include <WiFi.h>

#include "vector.hpp"


void setup () {
    Serial.begin (115200);
    delay (3000);


    // Examples of vector constructors
    vector<String> v1;                          // empty vector of Strings
    vector<int> v2 ( { 100, 200, 300, 400 } );  // constructor of vector of integers from brace enclosed initializer list
    vector<int> v3 = { 500, 600, 700, 800 };    // constructor of vector of integers and its initialization from brace enclosed initializer list
    vector<int> v4 = v3;                        // copy-constructor


    // Examples of vector assignment
    v2 = { 3, 2, 1 };                           // assignment from brace enclosed initializer list
    v3 = v2;                                    // assignemnt from another vector


    // Example of vector comparison
    if ( v2 == v3 )
        Serial.println ("v2 and v3 are equal");
    else
        Serial.println ("v2 and v3 are different");


    // Examples of inserting new elements to the vector
    v3.push_back (4);                           // insert 4 at the end of the vector
    v3.push_front (5);                          // please note that push_front is not a STL C++ vector member function
    v3.insert (2, 6);                           // insert 6 at position 2


    // Example of searching for an element in the vector
    int position = v3.find (4);
    if (position >= 0)
        Serial.println ("4 found in v3 at position " + String (position));
    else
        Serial.println ("4 not found in v3");


    // Examples of deleting elements from the vector
    v3.erase (position);                        // delete element at selected position
    v3.pop_back ();                             // delete an element at the end of the vector
    v3.pop_front ();                            // please note that pop_front is not a STL C++ vector member function


    // Examples of scanning through the vector elements
    for (int i = 0; i < v2.size (); i++)        // scan vector elements with their position index
        Serial.print (v2 [i]);
    Serial.println ();
    for (auto e: v3)                            // scan vector elements with an iterator
        Serial.print (e);
    Serial.println ();        


    // Sorting vector elements
    v3.sort ();
    for (auto e: v3)
        Serial.print (e);
    Serial.println ();


    // Finding min and max elements of the vector
    auto minElement = min_element (v2);
    if (minElement)                             // check if min element is found (if v3 is not empty)
        Serial.printf ("min element of v2 = %i\n", *minElement);
    auto maxElement = max_element (v2);
    if (maxElement)                             // check if max element is found (if v3 is not empty)
        Serial.printf ("max element of v2 = %i\n", *maxElement);


    // Detecting errors that occured in vector operations
    signed char e = v3.push_back (9);
    
    if (!e) // != OK
        Serial.println ("push_back succeeded");
    else {
        // report error or check flags
        Serial.printf ("insert error: ");
        switch (e) {
            case BAD_ALLOC:       Serial.printf ("BAD_ALLOC\n"); break;
            case OUT_OF_RANGE:    Serial.printf ("OUT_OF_RANGE\n"); break;
            case NOT_FOUND:       Serial.printf ("NOT_FOUND\n"); break;
        }
    }
        

    // Checking if an error has occurred only once after many vector operations
    v1.clearErrorFlags ();  // clear possible error flags from previous operations
    for (int i = 1000; i < 1100; i++)
        v1.push_back ( String (i) );

    e = v1.errorFlags ();
    if (!e) // OK
        Serial.println ("100 push_backs succeeded");
    else {
        Serial.printf ("an error occured at least once in 100 push_backs: ");  // check flags for details
        // or check specific error
        if (e & BAD_ALLOC) Serial.println ("BAD_ALLOC");       
        if (e & OUT_OF_RANGE) Serial.println ("OUT_OF_RANGE");   
        if (e & NOT_FOUND) Serial.println ("NOT_FOUND");
    }


                // capacity and speed test
                v1.clear ();
                v2.clear ();
                v3.clear (); 
                v4.clear ();

                vector<unsigned long> v5;
                #ifdef __USE_PSRAM_FOR_VECTORS__
                    size_t r = ESP.getFreePsram () / sizeof (unsigned long);
                    while (v5.reserve (r)) {
                        Serial.printf ("PSRAM reserve (%lu) failed\n", r);
                        r -= 100;
                    }
                #else
                    size_t r = heap_caps_get_largest_free_block (MALLOC_CAP_DEFAULT) / sizeof (unsigned long);
                    while (v5.reserve (r)) {
                        Serial.printf ("Heap reserve (%lu) failed\n", r);
                        r -= 100;
                    }
                #endif
                unsigned long l;
                unsigned long startMillis = millis ();
                for (l = 1; l <= 1000000; l++) {
                    if (v5.push_back (l))
                        break;
                }
                unsigned long endMillis = millis ();
                Serial.printf ("Free heap: %lu, free PSRAM: %lu\n", ESP.getFreeHeap (), ESP.getFreePsram ());
                // for (auto e: v5) Serial.println (e);
                v5.clear ();
                Serial.printf ("Maximum number of vector<unsigned long> elements in the memory is %lu\n", l); // ESP32-S2: heap: 516094, PSRAM: 516076
                Serial.printf ("Average push_back time = %lf us\n", l, (double) (endMillis - startMillis) * 1000 / l);
                Serial.printf ("Free heap: %lu, free PSRAM: %lu\n", ESP.getFreeHeap (), ESP.getFreePsram ());
}

void loop () {

}
