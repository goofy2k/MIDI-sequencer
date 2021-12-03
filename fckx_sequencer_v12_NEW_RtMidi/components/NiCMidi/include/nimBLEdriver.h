/***********************************************
*definitions to make NimBLE driver varaiables globally accessible
*see: https://stackoverflow.com/questions/19929681/c-global-variable-declaration/19929727
*answer by Wolf, but learn more!
*/

extern int globaltestPointer; //extern to make it globally accessible????

class NimBLEGluer {
    
    public:
        //static int testPointer; //static to make it globally accessible????
        int testPointer2 = 123;
    friend void printTestPointer(NimBLEGluer x);
    void setPointer( int inp);
    int getPointer(void ){return testPointer2;
	};
    //constructor
    NimBLEGluer();
}; 

//NimBLEGluer NimBLEDAta;   