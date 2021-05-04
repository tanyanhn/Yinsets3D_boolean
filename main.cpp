#include "src/Totalheader.h"
#include <string>
#include <iostream>

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        std::cout << "Please input ./main.exe arg1, arg2, arg3, arg4, \n"
                  << "arg1 : operator name. \n"
                  << "arg2, arg3 : input filename.\n "
                  << "arg4 : output filename.\n";
        return 1;
    }
    std::string op(argv[1]), input1(argv[2]),
        input2(argv[3]), output(argv[4]);

    YSB::YinSet<YSB::Real> y1(input1), y2(input2), y3;

    if (op == "meet")
        y3 = std::move(y1.meet(y2));
    else if (op == "join")
        y3 = std::move(y1.join(y2));
    else if (op == "complement")
        y3 = std::move(y1.complement());
    else
        std::cout << "operator undefine./n";

    y3.objOutput(output);

    return 0;
}
