
#include "hnor20.h"
#include <iostream>

void TestInterp()
{
    double delta_s = 0.0;
    double delta = 0.0;

    INTERP_METHOD m = INTERP_BICUBIC;
	
    C_hnor grid;
    delta = grid.GetFactor(-20.414736, -49.975325, m) - ( -7.64);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = grid.GetFactor(-29.907579, -51.823868, m) - (  5.95);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = grid.GetFactor(-20.194407, -43.618629, m) - ( -5.64);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = grid.GetFactor(-20.895839, -54.868729, m) - (  2.83);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = grid.GetFactor(-25.530668, -51.774829, m) - (  4.06);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = grid.GetFactor(-16.090905, -57.711762, m) - (  8.35);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = grid.GetFactor(-11.735657, -49.138925, m) - (-16.30);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = grid.GetFactor(-8.593642 , -61.863849, m) - (  6.45);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = grid.GetFactor(-9.905970 , -67.738284, m) - ( 24.05);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;	
    delta = grid.GetFactor(-32.540392 , -53.156549, m) - ( 11.17);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = grid.GetFactor(-28.653433 , -55.674432, m) - ( 10.35);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = grid.GetFactor(-27.344112 , -49.007961, m) - ( 1.58);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = grid.GetFactor(-26.688981 , -53.4969219, m) - ( 5.04);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = grid.GetFactor(-18.022189 , -52.922957, m) - ( -3.31);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = grid.GetFactor(-18.356500 , -46.159374, m) - ( -10.39);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = grid.GetFactor(-18.138242 , -40.956673, m) - ( -9.56);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = grid.GetFactor(-20.210832 , -40.837231, m) - ( -5.96);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;	
    delta = grid.GetFactor(-24.027784 , -47.513957, m) - ( -3.46);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = grid.GetFactor(-25.499264 , -49.394001, m) - ( 4.12);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = grid.GetFactor(-13.017753 , -53.024325, m) - ( -8.75);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;
    delta = grid.GetFactor(-21.323203 , -47.127257, m) - ( -5.49);
    delta_s += std::abs(delta);	    std::cout << delta << std::endl;

    std::cout << std::endl << (delta_s / 21.0) << std::endl;
}

int main(int argc, char **argv)
{
    TestInterp();

	return 0;
}
