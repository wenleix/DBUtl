#include <cstdlib>
#include <cmath>
#include <cstdio>

#include "DBUtl/zipf.h"
#include "DBUtl/Log.h"
#include "DBUtl/RNG.h"

//using namespace DBUtl;

double DBUtl::Zipf::theta;
double DBUtl::Zipf::zetaN;
double DBUtl::Zipf::zetaTwo;
double DBUtl::Zipf::alpha;
double DBUtl::Zipf::eta;
int DBUtl::Zipf::maxRandom;

double DBUtl::Zipf::zeta(int n, double theta) {
    double ans = 0.0;
    for(int i = 1; i <= n; i++) {
        ans += pow(1.0 / i, theta);
    }
    return ans;
}


int DBUtl::Zipf::zipf(int myMaxRandom, double myTheta) {

    DBUTL_ASSERT(myTheta > 0);
    RNG& rng = RNG::get(); //TODO: setseed.

    //Cache values.
    if(maxRandom != myMaxRandom || myTheta != theta) {

        maxRandom = myMaxRandom;
        theta = myTheta;
        zetaN = zeta(maxRandom, theta);
        zetaTwo = zeta(2, theta);
        alpha = 1.0 / (1.0 - theta);
        eta = (1.0 - pow(2.0 / maxRandom, 1.0 - theta)) / (1.0 - zetaTwo / zetaN);
    }

    double u = rng.rand();
    double uz = u * zetaN;

    if(uz < 1.0) {
        return 0;
    }

    if(uz < 1.0 + pow(0.5, theta)) {
        return 1;
    }
    return (int) (maxRandom * pow(eta * u - eta + 1.0, alpha));
}


int DBUtl::zipf(int myMaxRandom, double myTheta) {
    return Zipf::zipf(myMaxRandom, myTheta);
}
