#ifndef _DBUTL_ZIPF_H_
#define _DBUTL_ZIPF_H_


namespace DBUtl {

    class Zipf {
    private:
        static double theta;
        static double zetaN;
        static double zetaTwo;
        static double alpha;
        static double eta;
        static int maxRandom;

        static double zeta(int n, double theta);    

    public:
        static int zipf(int myMaxRandom, double myTheta);
    };

    int zipf(int myMaxRandom, double myTheta);
//    {
//        return Zipf::zipf(myMaxRandom, myTheta);
//    }

}

#endif
